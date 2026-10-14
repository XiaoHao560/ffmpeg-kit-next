/*
 * Copyright (c) 2026 Taner Sener
 *
 * This file is part of FFmpegKitNext and is licensed under the GNU Lesser
 * General Public License, version 3 or later.
 */

#ifndef FFMPEG_KIT_FLUTTER_WINDOWS_PLUGIN_STATE_H
#define FFMPEG_KIT_FLUTTER_WINDOWS_PLUGIN_STATE_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <utility>

namespace ffmpeg_kit_windows {

// Admission and teardown share this gate. An admitted execution keeps its
// lease until the native call returns, even when that call throws.
class PluginState {
    struct Execution {
        std::function<void()> cancel;
    };

  public:
    class Lease {
      public:
        ~Lease() { owner_.Finish(execution_); }
        Lease(const Lease &) = delete;
        Lease &operator=(const Lease &) = delete;

      private:
        friend class PluginState;
        Lease(PluginState &owner, std::shared_ptr<Execution> execution) : owner_(owner), execution_(std::move(execution)) {}

        PluginState &owner_;
        std::shared_ptr<Execution> execution_;
    };

    std::unique_ptr<Lease> TryStart(std::function<void()> cancel) {
        auto execution = std::make_shared<Execution>(Execution{std::move(cancel)});
        // Allocate the lease before registering it, so allocation failure cannot
        // leave teardown waiting for a worker that never started.
        auto lease = std::unique_ptr<Lease>(new Lease(*this, execution));
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopping_)
                return nullptr;
            executions_.push_back(execution);
        }
        return lease;
    }

    void BeginShutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
    }

    void CancelAndWait() {
        BeginShutdown();
        std::unique_lock<std::mutex> lock(mutex_);
        while (!executions_.empty()) {
            const auto executions = executions_;
            lock.unlock();
            for (const auto &execution : executions) {
                try {
                    execution->cancel();
                } catch (...) {
                    // Keep draining other workers and retry cancellation on the next
                    // pass; a native error must not escape the plugin destructor.
                }
            }
            lock.lock();
            // Native registration can overwrite a cancellation sent before it.
            // Repeat until every admitted native call has returned. No lifecycle
            // lock is held while calling native code or waiting for stream I/O.
            finished_.wait_for(lock, std::chrono::milliseconds(10), [this] { return executions_.empty(); });
        }
    }

  private:
    void Finish(const std::shared_ptr<Execution> &execution) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            executions_.remove(execution);
        }
        finished_.notify_all();
    }

    std::mutex mutex_;
    std::condition_variable finished_;
    bool stopping_ = false;
    std::list<std::shared_ptr<Execution>> executions_;
};

} // namespace ffmpeg_kit_windows

#endif
