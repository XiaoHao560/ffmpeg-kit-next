/* Copyright (c) 2026 Taner Sener. LGPL version 3 or later. */
#ifndef FFMPEG_KIT_FLUTTER_LINUX_PLUGIN_STATE_H
#define FFMPEG_KIT_FLUTTER_LINUX_PLUGIN_STATE_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <utility>

namespace ffmpeg_kit_linux {

// One engine owns this gate. Native work holds the state, never the plugin
// GObject, so retaining a worker cannot delay channel-detachment notification.
class PluginState : public std::enable_shared_from_this<PluginState> {
    struct Execution {
        std::function<void()> cancel;
    };

  public:
    class Lease {
      public:
        ~Lease() { owner_->Finish(execution_); }

      private:
        friend class PluginState;
        Lease(std::shared_ptr<PluginState> owner, std::shared_ptr<Execution> execution) : owner_(std::move(owner)), execution_(std::move(execution)) {}
        std::shared_ptr<PluginState> owner_;
        std::shared_ptr<Execution> execution_;
    };

    bool IsStopping() const { return stopping_.load(); }
    bool BeginShutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        return !stopping_.exchange(true);
    }
    std::shared_ptr<Lease> TryStart(std::function<void()> cancel) {
        auto execution = std::make_shared<Execution>(Execution{std::move(cancel)});
        auto lease = std::shared_ptr<Lease>(new Lease(shared_from_this(), execution));
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopping_)
            return nullptr;
        executions_.push_back(execution);
        return lease;
    }
    void CancelAndWait() {
        BeginShutdown();
        std::unique_lock<std::mutex> lock(mutex_);
        while (!executions_.empty()) {
            const auto snapshot = executions_;
            lock.unlock();
            for (const auto &execution : snapshot) {
                try {
                    if (execution->cancel)
                        execution->cancel();
                } catch (...) {
                }
            }
            lock.lock();
            // Cancellation before native registration can be overwritten. Retry
            // until every active native call returns; never hold this lock in native.
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
    std::atomic<bool> stopping_{false};
    std::mutex mutex_;
    std::condition_variable finished_;
    std::list<std::shared_ptr<Execution>> executions_;
};
} // namespace ffmpeg_kit_linux
#endif
