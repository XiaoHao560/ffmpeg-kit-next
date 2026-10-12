# ffmpeg-kit-next

使用 GitHub Actions 自动构建最新版 [ffmpeg-kit-next](https://github.com/arthenica/ffmpeg-kit-next) 的 Android AAR。

> ⚠️ **GPL 许可提醒**  
> 本项目构建的是 **`full-gpl`** 变体。生成的 AAR 及其二进制包含 GPL 许可组件，因此使用、修改或分发该构建产物时，必须遵守 GPL 及所包含第三方组件的许可证要求。若你的项目不兼容 GPL，请改用 LGPL 变体或自行构建合规版本。  
> 此提醒不构成法律建议，具体义务请查阅上游仓库及 [FFmpeg Legal](https://ffmpeg.org/legal.html)。

本项目构建的 ffmpeg-kit-next 主要服务于 [EzConvert](https://github.com/XiaoHao560/EzConvert)，因此默认仅包含以下 ABI：

- `arm64-v8a`
- `armeabi-v7a-neon`

如需其他架构，请参考项目中的 [构建脚本](.github/workflows/build-android-aar.yml)，自行修改并运行 GitHub Actions。

## 构建

- 定时通过 GitHub Actions 自动拉取并构建最新 ffmpeg-kit-next
- 构建变体：`full-gpl`

## 相关项目

- [EzConvert](https://github.com/XiaoHao560/EzConvert)：本构建产物主要服务的项目
- [ffmpeg-kit-next](https://github.com/arthenica/ffmpeg-kit-next)：上游源仓库

## 自定义构建

1. Fork 本仓库
2. 编辑 [.github/workflows/build-android-aar.yml](.github/workflows/build-android-aar.yml)，调整目标架构/平台
3. 进入仓库的 **Actions** 页面，选择对应 workflow 并运行
4. 构建完成后，在 Actions 运行记录中下载 AAR 产物

## 注意事项

- 默认仅构建供安卓平台使用的 `arm64-v8a` 和 `armeabi-v7a-neon`
- 其他架构未默认启用，需要自行修改 workflow
- 本项目构建的是 `full-gpl` 变体，受 GPL 许可证约束；分发集成该 AAR 的应用时，请确保满足 GPL 的相应义务，例如提供对应源代码等

## 许可证

- 上游 [ffmpeg-kit-next](https://github.com/arthenica/ffmpeg-kit-next) 与 [FFmpeg](https://ffmpeg.org/legal.html) 的许可证取决于所选构建变体
- 本仓库默认构建 `full-gpl` 变体，包含 GPL 许可组件
- 使用前请自行确认你的项目与 GPL 的兼容性