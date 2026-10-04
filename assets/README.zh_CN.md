<p align="right">
  <strong>简体中文</strong> · <a href="README.md">English</a>
</p>

# 资源目录（Assets）

本目录集中存放可复用的资源（字库、图片、音乐等），按资源类型分子目录管理。每个资源放在其类型对应的子目录，并记录放置路径、命名方式、集成方式与来源/许可。二进制资源（字体、图片、音频）不属于纯 markdown 文档，请勿与文档混放。涉及版权/授权的资源需注明来源与许可。

## 字库（fonts）

可复用的字库文件与生成的字库源码放在 `fonts/`。

- 命名要能反映字族、字重、字级与格式。
- 记录来源、许可、字符范围、转换命令与目标放置路径。
- 添加字库前评估 Flash 与内部 RAM 影响；ESP32-C3 无 PSRAM。
- 不提交许可不允许分发的字库。

崩溃计数使用 148 个字符的子集，12 px 与 16 px，1 bit，未压缩的 LVGL 格式：

| 文件 | 用途 |
| --- | --- |
| `fonts/NotoSansSC-Meltdown.ttf` | Noto Sans SC 的 SIL Open Font License 子集。见 `fonts/OFL-NotoSansSC.txt`。 |
| `fonts/Rajdhani-Bold.ttf` | SIL Open Font License 的 ASCII U+0020–U+007E。见 `fonts/OFL-Rajdhani.txt`。不打包 macOS 字体。 |
| `fonts/meltdown-glyphs.txt` | 与 Rajdhani 合并的汉字清单。 |
| `fonts/meltdown_font_12.c`、`fonts/meltdown_font_16.c` | 由 `main/` 编译的生成源码。符号为 `meltdown_font_12`、`meltdown_font_16`。 |

字符清单变化后用 `lv_font_conv` 1.5.3 重新生成。`tests/test_meltdown_assets.py` 会在任一尺寸缺少界面字符串中的字符时失败。清单不含破折号和全角标点；界面使用 ASCII 的 `-`、`,` 和 `+`。

## 图片（images）

可复用的源图与生成的显示资产放在 `images/`。

| 文件 | 尺寸与格式 | 用途与来源 |
| --- | --- | --- |
| [`images/home.jpg`](images/home.jpg) | 3840 × 2160，JPEG | 嵌入中英文项目 README 的产品主图，突出 AI Passport 产品形象与开放、人人可创作的理念。 |
| [`images/readme-hardware-specs.png`](images/readme-hardware-specs.png) | 2172 × 724，PNG RGBA | 保留为可选技术参考图，不再用于首页主视觉。于 2026-09-17 使用内置图像生成工具为本仓库生成；已根据文档中的硬件能力契约核对图中的六项标签与参数。 |
| [`images/logo-wordmark.png`](images/logo-wordmark.png) | 1648 × 336，PNG RGBA | 从仓库原始 `images/logo.png` 中精确裁切并去除背景的黑色字标；用于中英文项目 README 的浅色主题。 |
| [`images/logo-wordmark-dark.png`](images/logo-wordmark-dark.png) | 1648 × 336，PNG RGBA | 提取字标的白色版本；README 使用 `<picture>` 在 GitHub 深色主题下显示。 |

- 使用描述性命名，并记录尺寸、像素格式、转换步骤与目标路径。
- 优先采用适合 240 × 320 RGB565 显示的格式，并纳入 Flash 与内部 RAM 考量。
- 许可允许时保留可编辑源文件，并记录来源与许可。
- 图片中不得包含设备二维码秘密、凭证或个人数据。

## 音乐与音效（music）

可复用的音乐与音效源码放在 `music/`。

- 记录来源、许可、采样率、位深、声道、转换命令与目标路径。
- 与当前 BSP 音频路径匹配时优先采用 16 kHz、16 位单声道 PCM。
- 嵌入音频前评估 Flash 与内部 RAM 成本；长录音应流式或分块。
- 无再分发许可不提交媒体文件。

崩溃计数的片段是放在 flash 里的 16 kHz、16 位、单声道 PCM。播放走 `bsp_audio_write`，音量 70。这是体积和响度的折中，尚未在扬声器上测量。

| 文件 | 长度 | 用途 |
| --- | --- | --- |
| `music/meltdown_c3_pcm.c` | 35200 字节，1.10 秒 | 可信归档后的第 1–5 次，以及日期未确认时的每一次。符号 `meltdown_c3_pcm`。 |
| `music/meltdown_egg_pcm.c` | 28800 字节，0.90 秒 | 第 6 次及以后。符号 `meltdown_egg_pcm`。 |

`meltdown_c3_pcm.c` 是本项目制作的壳体破裂声。`meltdown_egg_pcm.c` 是两段 Freesound 录音的裁剪混音。发布包含它的固件时保留下列署名：

1. zimbot 的 DenseCrunch.wav，Freesound 244485，[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)。混音做了裁剪、轻微调速、均衡和叠加。
2. Anthousai 的 egg - crack 01.wav，Freesound 336613，[CC0](https://creativecommons.org/publicdomain/zero/1.0/)。混音使用了滤波后的片段。

可编辑母版和被否定的草稿留在仓库外的 `plans/meltdown-counter/audio/`。静音、翻页、校时和恢复历史都不播放这两段声音。新的一次按下会换掉正在播放的声音。

## 原生网孔界面资源

`dots/meltdown_dots.c/.h` 由 `node tools/gen_meltdown_dots.cjs` 生成，包含 A8 灰度文字/图标蒙版和 24×24 RGB565 网孔纹理。中文源字体是 LVGL 附带的 Source Han Sans SC Normal，SIL OFL，见 `fonts/OFL-SourceHanSansSC.txt`。Avenir Next Condensed 和 Menlo 只作为本机 macOS 栅格化参考，不分发字体文件。小字保留灰度笔画，大数字使用整数 3px 点阵周期；设备不运行字体栅格器。旧 1bpp 字库保留为历史资源，当前界面使用图像蒙版。
