# ESP-Brookesia 出厂固件

[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.5.5-E7352C?logo=espressif)](https://github.com/espressif/esp-idf/releases/tag/v5.5.5)
[![Target](https://img.shields.io/badge/target-ESP32--P4-1F6FEB)](https://www.espressif.com/zh-hans/products/socs/esp32-p4)
[![LVGL](https://img.shields.io/badge/LVGL-9.5.0-2AA9E0?logo=lvgl)](https://lvgl.io/)

[English](README.md) | 简体中文

本目录包含 **Waveshare ESP32-P4-WIFI6-Touch-LCD-7B** 的 ESP-Brookesia 固件源码，
支持显示、触摸、音频、摄像头、SD 卡和 ESP32-C6 Hosted Wi-Fi，并提供 7B 专用的
RS485 与 CAN/TWAI 终端应用。

## 平台配置

| 项目 | 配置 |
| --- | --- |
| 主控 | ESP32-P4 ，32 MiB Flash，Octal PSRAM（250 MHz） |
| 显示 / 触摸 | 7 英寸 EK79007 1024 x 600 MIPI-DSI 面板，GT911 电容触摸 |
| 开发框架 | ESP-IDF v5.5.5，`main/idf_component.yml` 要求 `>=5.5,<6.0` |
| UI | LVGL 9.5.0，本地 ESP-Brookesia core 0.6.0-beta2 |
| 板级支持 | 本地 BSP `esp32_p4_wifi6_touch_lcd_7b` 3.0.0 与 `bsp_extra` |
| LVGL 适配 | `espressif/esp_lvgl_adapter` 0.6.3 |
| 无线网络 | ESP32-C6，通过 4-bit SDIO 运行 ESP-Hosted 1.4.7 / `esp_wifi_remote` 0.14.5 |
| 语音助手 | `espressif/esp_xiaozhi` 0.1.1、ESP-SR 2.4.7、xiaozhi-fonts 1.6.0 |

显示、触摸、ES8311、ES7210、SD 卡、摄像头和背光由本地 BSP 与 `bsp_extra` 提供。
芯片版本配置见 [ESP32-P4 芯片版本配置](../../docs/ESP32P4_REVISION_CONFIG_ZH.md)，
固件镜像与 CI 产物的区别见 [固件与 CI 包](../../docs/firmware_ZH.md)。

## 应用列表

<table>
  <tr>
    <td align="center"><img src="components/draw/assets/img_app_drawpanel.png" width="64" alt="画板图标"><br><b>DrawPanel</b></td>
    <td align="center"><img src="components/brookesia_app_squareline_demo/assets/esp_brookesia_app_icon_launcher_squareline_112_112.png" width="64" alt="Squareline 图标"><br><b>Squareline</b></td>
    <td align="center"><img src="components/brookesia_app_calculator/assets/img_app_calculator.png" width="64" alt="计算器图标"><br><b>Calculator</b></td>
    <td align="center"><img src="components/SpecAnalyzer/assets/img_app_specanalyzer.png" width="64" alt="频谱图标"><br><b>SpecAnalyzer</b></td>
    <td align="center"><img src="components/MusicPlayer/assets/img_app_musicplayer.png" width="64" alt="音乐播放器图标"><br><b>MusicPlayer</b></td>
    <td align="center"><img src="components/Camera/assets/img_app_camera.png" width="64" alt="摄像头图标"><br><b>Camera</b></td>
  </tr>
  <tr>
    <td align="center"><img src="components/VideoPlayer/assets/img_app_vedioplayer.png" width="64" alt="视频播放器图标"><br><b>VideoPlayer</b></td>
    <td align="center"><img src="components/HardwareApps/assets/img_app_rs485.png" width="64" alt="RS485 图标"><br><b>RS485</b></td>
    <td align="center"><img src="components/HardwareApps/assets/img_app_can.png" width="64" alt="CAN 图标"><br><b>CAN</b></td>
    <td align="center"><img src="components/Settings/assets/img_app_settings.png" width="64" alt="设置图标"><br><b>Settings</b></td>
    <td align="center"><img src="components/XiaozhiApp/assets/img_app_xiaozhi.png" width="64" alt="AIChats 图标"><br><b>AIChats</b></td>
  </tr>
</table>

| 应用 | 功能 | 运行依赖 |
| --- | --- | --- |
| DrawPanel | 固定画布区域，边缘绘制不会拖动整个应用 | 显示和触摸 |
| Squareline | Registry 安装的 Squareline Studio UI 演示与应用模板 | 显示和触摸 |
| Calculator | 适合触摸屏操作的 Brookesia 计算器 | 显示和触摸 |
| SpecAnalyzer | MIC1/MIC2 麦克风的 FFT 频谱 | ES7210 TDM 输入 |
| MusicPlayer | 播放 `/spiffs/music` 中的本地 MP3，带频谱可视化 | SPIFFS `storage`、ES8311 输出 |
| Camera | OV5647 MIPI CSI 预览，PPA 旋转 90 度并缩放到横屏 | OV5647、`esp_video` |
| VideoPlayer | SD 卡全屏 AVI/MJPEG 播放，音频可用时同步播放 | SD 卡、JPEG/PPA、ES8311 输出 |
| RS485 | UART1 RS485 终端，默认自动发送问候语 | 板载自动方向收发器 |
| CAN | 500 kbit/s TWAI 终端，显示总线状态并在 BUS_OFF 后请求恢复 | 板载 TJA1051 |
| Settings | WLAN 扫描/连接、声音、屏幕亮度和设备信息 | ESP32-C6 Hosted Wi-Fi 和板级服务 |
| AIChats | 网络检查、小智激活、WakeNet 唤醒、AEC/VAD、Opus 上下行、服务端文字、表情与 TTS | Wi-Fi、`model`/`storage` 镜像、ES8311、ES7210 |

## 构建

### 环境要求

需要 ESP-IDF v5.5.5、`esp32p4` 工具链，以及访问 ESP Component Registry 的网络连接。
烧录时请使用支持数据传输的 USB 线。以下命令均在本目录执行。

`build*/`、`managed_components/` 和 `dependencies.lock` 由本地构建生成；首次编译会
根据组件依赖清单下载托管依赖。复制源码工程时，应完整保留 `main/`、`components/`、
`spiffs/`、`sdkconfig.defaults*` 和 `partitions.csv`。

### 芯片版本配置

不同芯片版本使用独立的 `sdkconfig.defaults.*` 和构建目录，生成的镜像不能混用：

| Profile | 适用硬件 | 关键差异 |
| --- | --- | --- |
| `rev3_x` | ESP32-P4 Rev3.x（交付配置） | Rev3.x MIPI PHY 参考时钟、250 MHz PSRAM |
| `rev3_2` | Rev3.2，UART0 与 USB Serial/JTAG 双路日志 | 版本范围 3.1-3.99，尚未在 Rev3.2 实机上测试 |
| `rev1_3` | Rev1.3 测试板 | pre-v3 芯片、旧版 MIPI PHY 时钟、200 MHz PSRAM，不能烧录到 Rev3.x |

Rev3.x：

```sh
idf.py -B build-v5.5.5-rev3_x -D SDKCONFIG="$PWD/build-v5.5.5-rev3_x/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev3_x" build
```

Rev1.3：

```sh
idf.py -B build-v5.5.5-rev1_3 -D SDKCONFIG="$PWD/build-v5.5.5-rev1_3/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev1_3" build
```

Rev3.2 双路日志：

```sh
idf.py -B build-v5.5.5-rev3_2-dual-log -D SDKCONFIG="$PWD/build-v5.5.5-rev3_2-dual-log/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev3_2" build
```

Rev1.3 配置的触摸 X/Y 镜像标志均关闭，触摸坐标相对上一版旋转 180 度，显示旋转保持
不变；维护者已在 Rev1.3 实机上确认该修正。

### 首次完整烧录与分区变更

首次安装需要写入 bootloader、分区表、应用、ESP-SR 模型和 SPIFFS 资源（以 Rev3.x 为例）：

```sh
idf.py -B build-v5.5.5-rev3_x -p PORT flash monitor
```

修改分区偏移或大小、从旧分区布局升级，或者怀疑模型和文件系统镜像残留时，
先擦除 Flash 再完整烧录：

```sh
idf.py -B build-v5.5.5-rev3_x -p PORT erase-flash flash monitor
```

分区、语音模型和 `spiffs/` 资源均未变化时，可以只烧录应用：

```sh
idf.py -B build-v5.5.5-rev3_x -p PORT app-flash monitor
```

`app-flash` 不会更新 `srmodels.bin`、`storage.bin` 或分区表。
修改 `partitions.csv`、WakeNet/模型 Kconfig、xiaozhi-fonts、内置音乐或提示音后，
必须重新执行完整 `flash`。

### 合并出厂镜像

编译成功后可生成带日期的 FactoryOnly 镜像：

```sh
mkdir -p ../../releases/factory_firmware
(cd build-v5.5.5-rev3_x && python -m esptool --chip esp32p4 merge_bin -o ../../../releases/factory_firmware/ESP32-P4-WIFI6-Touch-LCD-7B-FactoryOnly-260820.bin @flash_args)
```

合并镜像大小为 16 MiB，从偏移 `0x0` 写入。编译和镜像校验不能替代烧录或硬件运行
验证。

## AIChats / 小智

AIChats 使用 `esp_xiaozhi` 处理协议，本工程负责 UI、激活、音频和生命周期适配。

首次使用：

1. 在 Settings 中配置 WLAN。
2. 打开 AIChats；设备未绑定账号时会显示并播报激活码。
3. 在小智控制面板添加设备并输入激活码。
4. 页面进入待命状态后，说出唤醒词（默认是“你好小智”），或点击表情开始对话。

交互说明：

- 待命时点击表情开始一轮对话；等待激活时点击激活码会重新播报。对话进行中点击不会打断，
  播报过程中可用唤醒词打断。
- 关闭 AIChats 时会断开与服务端的连接，重新打开后自动重连；暂停（切到其他应用）时保持连接。

如果日志出现 `Speech model partition 'model' is missing` 或
`Speech model image is empty or unreadable`，请重新构建并完整烧录，确保分区表和
`srmodels.bin` 已写入设备。

本地 UI 语言和传输方式可在以下菜单中设置：

```text
idf.py menuconfig
  Xiaozhi Application
    Local UI language            English / Simplified Chinese
    Preferred Xiaozhi transport  WebSocket（默认）/ Auto / MQTT+UDP
```

UI 语言选项只控制固件内置文字，不会翻译服务端返回的对话、TTS 或激活消息。本地字符串位于
`components/XiaozhiApp/XiaozhiLocalization.*`。

板级音频输出使用 ES8311，输入使用 ES7210。语音模式下 I2S 总线为 24 kHz、16-bit，
ES7210 输出 4 槽 TDM，顺序为 `[MIC1, MIC3（回声参考）, MIC2, MIC4]`。AIChats 取 MIC1、
MIC2 和回声参考，重采样到 16 kHz 后送入 AFE 的 `MMR` 布局，启用 AEC 消除播报回声，
再编码为 16 kHz 单声道 Opus 上行；服务端下行 Opus 直接解码为 24 kHz 输出。

## 板级硬件说明

### 显示与触摸

7B 显示链路固定使用自身的屏幕参数，不得套用 LCD-X 的时序：

| 控制器 | 分辨率 | MIPI-DSI lane 数 | 单 lane 速率 | DPI 时钟 |
| --- | --- | ---: | ---: | ---: |
| EK79007 | 1024 x 600 | 2 | 1000 Mbps/lane | 52 MHz |

本地 BSP 为 Rev3.x 选择 `MIPI_DSI_PHY_PLLREF_CLK_SRC_DEFAULT`。pre-v3 兼容分支使用旧版
`MIPI_DSI_PHY_CLK_SRC_DEFAULT`，但交付配置固定启用 `CONFIG_ESP32P4_REV_MIN_300=y`，
且不启用 `CONFIG_ESP32P4_SELECTS_REV_LESS_V3`。

GT911 的 RESET 为 `GPIO23`，INT 未连接（`GPIO_NUM_NC`）。BSP 先探测 I2C 地址 `0x5D`，
再探测 `0x14`，并通过轮询读取触摸数据。

### RS485 与 CAN

| 应用 | 引脚与配置 | 开发板行为 |
| --- | --- | --- |
| RS485 | UART1：GPIO27 TX、GPIO26 RX、115200 8N1 | 板载自动方向收发器无需 RTS/DE GPIO；默认开启自动发送问候语。 |
| CAN | GPIO22 TX、GPIO21 RX；500 kbit/s normal mode | 使用板载 TJA1051；默认关闭自动发送，应用显示总线状态并在 BUS_OFF 后请求恢复。 |

### 电源域

VO4 为整个 GPIO39-GPIO48 VDD_IO_5 域供电。开发板使用 GPIO39-GPIO44 连接 SDMMC、
GPIO45 进行低有效卡电源控制；GPIO46-GPIO48 仍属于同一电源域。本地 BSP 持有并只启用
一次该 3.3 V LDO。RS485 与 CAN 使用固定 `ESP_3V3` 域，不依赖 VO4。

### 串口与日志

UART0 使用 GPIO37 TX / GPIO38 RX，115200 波特率、8N1、无流控。H1 标注为 `Type-C UART`，
通过 CH343P 转接 UART；H2 标注为 `USB1.1 Type-C`，对应原生 USB Serial/JTAG
（GPIO25 D+ / GPIO24 D-）。J1 是独立的 USB-A/USB OTG 接口，不输出控制台日志。

`rev3_2` 配置将应用 INFO/WARN/ERROR 日志同时转发到原生 USB Serial/JTAG；先打开对应串口，
再复位开发板以捕获启动日志。bootloader 使用 WARN 级别以保留现有 24 KiB 空间，
USB 副控制台从应用阶段开始输出，不转发 bootloader 日志。

## 摄像头与媒体

Camera 使用 OV5647 MIPI RAW8 800 x 640、50 FPS binning 模式，与 LCD-7C 相同；800 x 1280
模式偏色明显。PPA 将横屏画面放大铺满屏幕；由于缩放系数按 1/16 步进量化，固件选取能让输出
正好铺满 1024 x 600 屏幕的系数和裁剪区域，画面上下会被裁掉一部分。若改用竖屏传感器模式，
画面会改为旋转 90 度显示。

VideoPlayer 只扫描 `/sdcard/avi`，`.avi` 扩展名不区分大小写。SD 卡根目录中的 AVI 和 MP4
不会被枚举。播放器通过 `avi_player` 读取 AVI，使用 JPEG 解码 MJPEG，再由 PPA 缩放后写入
LCD 帧缓冲；音频初始化失败时会继续静音播放画面。

MusicPlayer 读取 `/spiffs/music` 中的 MP3，工程自带五首曲目。修改曲目后必须重新生成并烧录
`storage.bin`，只执行 `app-flash` 不会生效。

## 开发说明

- MCP 工具入口是 `components/XiaozhiApp/XiaozhiDeveloperTools.cpp` 中的
  `registerXiaozhiDeveloperTools()`，默认不注册任何工具。`esp_xiaozhi` 0.1.1 的 WebSocket
  任务只有 4 KB 栈，MCP 消息就在该任务中处理；注册工具前需要先解决栈空间问题，否则
  `tools/list` 序列化会导致栈溢出。
- 新应用继承 `esp_brookesia::systems::phone::App`，实现 `run()` 和 `back()`，再按资源所有权覆盖其他生命周期函数。
- LVGL 对象只能在 LVGL 线程或短时间持有显示锁时操作；关闭应用时应释放其 timer、task、queue、codec session、SD handle 和 LVGL object。
- `bsp_extra` 的音频读写使用独立的输入锁和输出锁，codec 重新配置时才同时持有两者；
  全双工应用（如 AIChats）可以同时采集和播放。
- VideoPlayer 播放时启用 adapter 的 Dummy Draw 并暂停 LVGL，退出后停止 `avi_player`，再恢复 LVGL 绘制。

## 工程结构

| 路径 | 职责 |
| --- | --- |
| `CMakeLists.txt` | ESP-IDF 工程入口 |
| `main/main.cpp` | 初始化 Phone、安装应用并处理 Wi-Fi 状态事件 |
| `main/CMakeLists.txt` | 组装 `spiffs/` 与小智字体，生成 `storage` 分区镜像 |
| `components/brookesia_core/` | 本地 Brookesia core 实现 |
| `components/esp32_p4_wifi6_touch_lcd_7b/` | 本地 7B BSP（显示、触摸、SD 卡、电源域） |
| `components/bsp_extra/` | 音频 codec、Wi-Fi 服务和板级外设扩展 |
| `components/{brookesia_app_calculator, brookesia_app_squareline_demo, draw, Camera, XiaozhiApp, VideoPlayer, MusicPlayer, SpecAnalyzer, Settings, HardwareApps}/` | 启动器中的计算器、Squareline 演示、绘图、相机、小智、视频、音乐、频谱、设置以及 RS485/CAN 应用 |
| `spiffs/` | 生成 `storage` 分区镜像的音乐和小智提示音 |
| `partitions.csv` | Flash 分区布局 |
| `sdkconfig.defaults*` | 工程默认配置与芯片版本 profile |

## 注意事项

- LVGL 锁只用于 UI 操作；网络、SD 扫描、音频初始化、NVS 和模型加载应在锁外执行。
- 模型、字体、提示音、音乐和分区变化需要完整 `flash`。
- 多个音频应用共享同一物理总线，切换应用时需要停止或移交 codec session。
- Host 组件必须与 ESP32-C6 中的协处理器固件兼容；`esp_wifi_remote` 与 `esp_hosted` 已固定版本，升级前需确认协处理器固件。
- 编译和镜像校验不会测试屏幕、触摸、RS485 自动方向或对端、CAN 对端确认/终端电阻或实体 BUS_OFF 恢复。

`main/main.cpp` 中的 `EXAMPLE_SHOW_MEM_INFO = true`，因此会定期统计 SRAM/PSRAM。
详细统计日志由 `EXAMPLE_SHOW_MEM_INFO_LOG` 单独控制。

反馈问题时，请附上板卡和芯片版本、ESP-IDF 版本、完整复现步骤和日志中的第一个实际错误。

本地 Brookesia core 使用
[Apache-2.0 许可证](components/brookesia_core/license.txt)，该许可证只适用于对应组件。
`components/XiaozhiApp/third_party/xiaozhi_esp32/` 中的代码遵循其目录内的 MIT 许可证。
