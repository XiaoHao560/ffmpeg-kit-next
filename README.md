# ffmpeg-kit-next

[中文版](README_zh.md)

Automatically build the latest Android AAR for [ffmpeg-kit-next](https://github.com/arthenica/ffmpeg-kit-next) using GitHub Actions.

> ⚠️ **GPL License Notice**  
> This project builds the **`full-gpl`** variant. The generated AAR and its binaries include GPL-licensed components. When using, modifying, or distributing this build artifact, you must comply with the GPL and all applicable third-party licenses. If your project is not compatible with the GPL, please use an LGPL variant or build a compliant version yourself.  
> This notice is not legal advice. For specific obligations, please refer to the upstream repository and [FFmpeg Legal](https://ffmpeg.org/legal.html).

The ffmpeg-kit-next built by this project is mainly intended for [EzConvert](https://github.com/XiaoHao560/EzConvert), so only the following ABIs are included by default:

- `arm64-v8a`
- `armeabi-v7a-neon`

If you need other architectures, please refer to the [build script](.github/workflows/build-android-aar.yml) in this project, modify it as needed, and run GitHub Actions.

## Build

- Periodically fetches and builds the latest ffmpeg-kit-next via GitHub Actions.
- Build variant: `full-gpl`

## Related Projects

- [EzConvert](https://github.com/XiaoHao560/EzConvert): The main project this build artifact is intended for.
- [ffmpeg-kit-next](https://github.com/arthenica/ffmpeg-kit-next): The upstream source repository.

## Custom Build

1. Fork this repository.
2. Edit [.github/workflows/build-android-aar.yml](.github/workflows/build-android-aar.yml) and adjust the target architecture/platform.
3. Go to the repository's **Actions** page, select the corresponding workflow, and run it.
4. After the build completes, download the AAR artifact from the Actions run.

## Notes

- By default, only `arm64-v8a` and `armeabi-v7a-neon` for the Android platform are built.
- Other architectures are not enabled by default; you need to modify the workflow yourself.
- This project builds the `full-gpl` variant, which is subject to GPL requirements. If you distribute an app integrated with this AAR, make sure you satisfy GPL obligations such as providing the corresponding source code.

## License

- The licenses of upstream [ffmpeg-kit-next](https://github.com/arthenica/ffmpeg-kit-next) and [FFmpeg](https://ffmpeg.org/legal.html) depend on the selected build variant.
- This repository builds the `full-gpl` variant by default, which includes GPL-licensed components.
- Please confirm your project's GPL compatibility before use.