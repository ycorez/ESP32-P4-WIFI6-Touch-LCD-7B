# ESP32-P4-WIFI6-Touch-LCD-7B ESP-Brookesia 固件

[English](README.md)

这是 ESP32-P4-WIFI6-Touch-LCD-7B 的默认固件源码。它保留 LCD-X Brookesia
桌面的功能集合：ESP-Hosted Wi-Fi、摄像头、音频、音乐、视频、绘图、频谱分析、
设置、计算器和小智，并新增 7B 专用的 RS485 与 CAN/TWAI 终端应用。

## 环境要求

- ESP-IDF v5.5.5
- ESP32-P4 Rev3.x 硅片
- 32 MiB Flash 与 Octal PSRAM

## 源码结构

- `main/`：固件入口和依赖清单。
- `components/`：应用、板级支持和随附的 Brookesia 源码。
- `spiffs/`：生成存储镜像所需的音乐和小智音频资源。
- `sdkconfig.defaults*` 与 `partitions.csv`：公共设置、芯片版本配置和 Flash 分区。

复制源码工程时，应完整保留上述目录和文件。`build*/`、`managed_components/`
和 `dependencies.lock` 由本地构建生成；首次编译会根据组件依赖清单下载托管依赖。
预编译出厂镜像位于
[`../../releases/factory_firmware/`](../../releases/factory_firmware/)。

## 屏幕与时钟约束

7B 显示链路固定使用自身的屏幕参数，不得套用 LCD-X 的时序：

| 控制器 | 分辨率 | MIPI-DSI lane 数 | 单 lane 速率 | DPI 时钟 |
| --- | --- | ---: | ---: | ---: |
| EK79007 | 1024 x 600 | 2 | 1000 Mbps/lane | 52 MHz |

本地 BSP 为 Rev3.x 选择 `MIPI_DSI_PHY_PLLREF_CLK_SRC_DEFAULT`。pre-v3 兼容
分支使用旧版 `MIPI_DSI_PHY_CLK_SRC_DEFAULT`，但交付配置固定启用
`CONFIG_ESP32P4_REV_MIN_300=y`，且不启用
`CONFIG_ESP32P4_SELECTS_REV_LESS_V3`。

GT911 的 RESET 为 `GPIO23`，INT 未连接（`GPIO_NUM_NC`）。BSP 先探测 I2C 地址
`0x5D`，再探测 `0x14`，并通过轮询读取触摸数据。

## 板载外设

| 应用 | 引脚与配置 | 开发板行为 |
| --- | --- | --- |
| RS485 Terminal | UART1：GPIO27 TX、GPIO26 RX、115200 8N1 | 板载自动方向收发器无需 RTS/DE GPIO；默认开启自动发送问候语。 |
| CAN / TWAI Terminal | GPIO22 TX、GPIO21 RX；500 kbit/s normal mode | 使用板载 TJA1051；默认关闭自动发送，应用显示总线状态并在 BUS_OFF 后请求恢复。 |

VO4 为整个 GPIO39-GPIO48 VDD_IO_5 域供电。开发板使用 GPIO39-GPIO44 连接
SDMMC、GPIO45 进行低有效卡电源控制；GPIO46-GPIO48 仍属于同一电源域。本地 BSP
持有并只启用一次该 3.3 V LDO。RS485 与 CAN 使用固定 `ESP_3V3` 域，不依赖 VO4。

开发板真值表与导入的官方原理图记录见
[`../../docs/board_ZH.md`](../../docs/board_ZH.md)、
[`../../docs/sources_ZH.md`](../../docs/sources_ZH.md) 和
[`../../schematic/README_ZH.md`](../../schematic/README_ZH.md)。

## 编译

导出 ESP-IDF v5.5.5 环境后，在本目录运行：

```bash
idf.py -B build-v5.5.5-rev3_x -D SDKCONFIG="$PWD/build-v5.5.5-rev3_x/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev3_x" build
```

应用镜像为
`build-v5.5.5-rev3_x/esp32-p4-lcd-7b-brookesia.bin`。

Rev1.3 开发板使用独立的测试配置和构建目录：

```bash
idf.py -B build-v5.5.5-rev1_3 -D SDKCONFIG="$PWD/build-v5.5.5-rev1_3/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev1_3" build
idf.py -B build-v5.5.5-rev1_3 -p PORT flash
```

该配置选择 pre-v3 芯片、旧版 MIPI PHY 参考时钟与 200 MHz PSRAM，不能烧录
到 Rev3.x。触摸 X/Y 镜像标志均关闭，使触摸坐标相对上一版旋转 180 度；
显示旋转保持不变。维护者已在 Rev1.3 实机上确认触摸修正。下方带日期的
Rev3.x FactoryOnly 镜像保持原样。

Rev3.2 镜像同时通过 UART0 和原生 USB Serial/JTAG 输出应用 INFO 日志。
请使用新的构建目录，避免复用此前 USB 单路输出的 sdkconfig：

```bash
idf.py -B build-v5.5.5-rev3_2-dual-log -D SDKCONFIG="$PWD/build-v5.5.5-rev3_2-dual-log/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev3_2" build
mkdir -p ../../releases/factory_firmware
(cd build-v5.5.5-rev3_2-dual-log && python -m esptool --chip esp32p4 merge_bin -o ../../../releases/factory_firmware/ESP32-P4-WIFI6-Touch-LCD-7B-FactoryOnly-Rev3.2-260910.bin @flash_args)
idf.py -B build-v5.5.5-rev3_2-dual-log -p PORT -b 115200 monitor
```

ESP-IDF v5.5.5 没有独立的最低版本 3.2 选项，因此该配置使用包含 Rev3.2 的
3.1-3.99 支持范围，选择 Rev3.x MIPI PHY 参考时钟与 250 MHz PSRAM，并保留
修正后的触摸方向。16 MiB 合并镜像应在 32 MiB Flash 开发板上从偏移 `0x0`
写入。此 Rev3.2 构建尚未在 Rev3.2 实体硬件上测试。
UART0 使用 GPIO37 TX / GPIO38 RX，115200 波特率、8N1、无流控。
H1 标注为 `Type-C UART`，通过 CH343P 转接 UART；H2 标注为 `USB1.1 Type-C`，
对应原生 USB Serial/JTAG（GPIO25 D+ / GPIO24 D-）。J1 是独立的 USB-A/USB OTG
接口，不输出控制台日志。
应用 INFO/WARN/ERROR 日志同时转发到原生 USB Serial/JTAG；先打开对应串口，
再复位开发板以捕获启动日志。桌面就绪后，现有内存状态日志每五秒输出一次。
bootloader WARN 日志通过主控制台 UART0 输出。bootloader 的 INFO 日志会超出
现有 24 KiB 空间，因此使用 WARN 保留当前分区布局。USB 副控制台从应用阶段
开始输出，不转发 bootloader 日志。

## 合并出厂固件

编译成功后生成带日期的 FactoryOnly 镜像：

```bash
mkdir -p ../../releases/factory_firmware
(cd build-v5.5.5-rev3_x && python -m esptool --chip esp32p4 merge_bin -o ../../../releases/factory_firmware/ESP32-P4-WIFI6-Touch-LCD-7B-FactoryOnly-260820.bin @flash_args)
```

合并镜像大小为 16 MiB，烧录起始偏移为 `0x0`。编译和镜像校验不能替代烧录
或硬件运行验证；它们不会测试屏幕、触摸、RS485 自动方向或对端、CAN 对端确认/终端
电阻或实体 BUS_OFF 恢复。
