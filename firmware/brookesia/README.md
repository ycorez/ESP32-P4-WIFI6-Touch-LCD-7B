# ESP32-P4-WIFI6-Touch-LCD-7B ESP-Brookesia Firmware

[简体中文](README_ZH.md)

This is the default firmware source for ESP32-P4-WIFI6-Touch-LCD-7B. It keeps
the LCD-X Brookesia launcher feature set: ESP-Hosted Wi-Fi, camera, audio,
music, video, drawing, spectrum analysis, settings, calculator, and Xiaozhi,
and adds 7B-specific RS485 and CAN/TWAI terminal applications.

## Requirements

- ESP-IDF v5.5.5
- ESP32-P4 Rev3.x silicon
- 32 MiB flash and octal PSRAM

## Source layout

- `main/`: firmware entry point and dependency manifest.
- `components/`: applications, board support, and bundled Brookesia source.
- `spiffs/`: music and Xiaozhi audio resources used to build the storage image.
- `sdkconfig.defaults*` and `partitions.csv`: shared settings, silicon profiles,
  and flash layout.

Keep these directories and files together when copying the source project.
`build*/`, `managed_components/`, and `dependencies.lock` are generated locally;
the first build downloads managed dependencies from the component manifests.
Prebuilt factory images are in
[`../../releases/factory_firmware/`](../../releases/factory_firmware/).

## Display and clock contract

The 7B display path is fixed to its own panel parameters and must not use the
LCD-X timing values:

| Panel | Resolution | MIPI-DSI lanes | Lane rate | DPI clock |
| --- | --- | ---: | ---: | ---: |
| EK79007 | 1024 x 600 | 2 | 1000 Mbps/lane | 52 MHz |

The local BSP selects `MIPI_DSI_PHY_PLLREF_CLK_SRC_DEFAULT` for Rev3.x. The
pre-v3 compatibility branch uses the legacy `MIPI_DSI_PHY_CLK_SRC_DEFAULT`, but
the delivered configuration sets `CONFIG_ESP32P4_REV_MIN_300=y` and does not
enable `CONFIG_ESP32P4_SELECTS_REV_LESS_V3`.

GT911 RESET is `GPIO23` and its INT signal is not connected (`GPIO_NUM_NC`).
The BSP probes I2C address `0x5D`, then `0x14`, and reads touch data by polling.

## Board peripherals

| Application | Pins and configuration | Board behavior |
| --- | --- | --- |
| RS485 Terminal | UART1: GPIO27 TX, GPIO26 RX, 115200 8N1 | The on-board automatic-direction transceiver needs no RTS/DE GPIO. Automatic greeting transmission is enabled by default. |
| CAN / TWAI Terminal | GPIO22 TX, GPIO21 RX; 500 kbit/s normal mode | Uses the on-board TJA1051. Automatic transmission is disabled by default; the app reports bus state and requests recovery after BUS_OFF. |

VO4 supplies the complete GPIO39-GPIO48 VDD_IO_5 bank. The board uses
GPIO39-GPIO44 for SDMMC and GPIO45 for active-low card-power control; GPIO46-
GPIO48 remain in the same bank. The local BSP owns and enables that 3.3 V LDO
once. RS485 and CAN use fixed `ESP_3V3` domains and do not depend on VO4.

The board truth table and imported official schematic record are available in
[`../../docs/board.md`](../../docs/board.md),
[`../../docs/sources.md`](../../docs/sources.md), and
[`../../schematic/README.md`](../../schematic/README.md).

## Build

Export ESP-IDF v5.5.5, then run this command from this directory:

```bash
idf.py -B build-v5.5.5-rev3_x -D SDKCONFIG="$PWD/build-v5.5.5-rev3_x/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev3_x" build
```

The application image is
`build-v5.5.5-rev3_x/esp32-p4-lcd-7b-brookesia.bin`.

For a Rev1.3 board, use the separate test profile and build directory:

```bash
idf.py -B build-v5.5.5-rev1_3 -D SDKCONFIG="$PWD/build-v5.5.5-rev1_3/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev1_3" build
idf.py -B build-v5.5.5-rev1_3 -p PORT flash
```

This profile selects pre-v3 silicon, the legacy MIPI PHY reference clock and
200 MHz PSRAM. It must not be flashed to Rev3.x. The touch X/Y mirror flags
are both disabled, rotating touch coordinates by 180 degrees relative to the
previous firmware; display rotation remains unchanged. The maintainer confirmed
the touch correction on Rev1.3. The dated Rev3.x FactoryOnly image below is unchanged.

For the Rev3.2 image with application INFO logs on both UART0 and native USB
Serial/JTAG, use a fresh build directory so the earlier USB-only sdkconfig is
not reused:

```bash
idf.py -B build-v5.5.5-rev3_2-dual-log -D SDKCONFIG="$PWD/build-v5.5.5-rev3_2-dual-log/sdkconfig" -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.rev3_2" build
mkdir -p ../../releases/factory_firmware
(cd build-v5.5.5-rev3_2-dual-log && python -m esptool --chip esp32p4 merge_bin -o ../../../releases/factory_firmware/ESP32-P4-WIFI6-Touch-LCD-7B-FactoryOnly-Rev3.2-260910.bin @flash_args)
idf.py -B build-v5.5.5-rev3_2-dual-log -p PORT -b 115200 monitor
```

ESP-IDF v5.5.5 has no separate minimum-revision 3.2 option, so this profile
uses its supported range 3.1-3.99, which includes Rev3.2. It selects the Rev3.x
MIPI PHY reference clock and 250 MHz PSRAM, and retains the corrected touch
orientation. Write the 16 MiB combined image from offset `0x0` on a 32 MiB
flash board. This Rev3.2 build has not been tested on physical Rev3.2 hardware.
UART0 uses GPIO37 TX / GPIO38 RX, 115200 baud, 8N1 and no flow control.
Use H1, labeled `Type-C UART`, for the CH343P USB-to-UART path, or H2,
labeled `USB1.1 Type-C`, for native USB Serial/JTAG (GPIO25 D+ / GPIO24 D-).
J1 is the separate USB-A/USB OTG connector and is not a console output.
Application INFO/WARN/ERROR output is mirrored to native USB Serial/JTAG;
connect to the matching serial port before resetting the board to capture startup.
The existing memory-status log repeats every five seconds once the launcher is ready.
Bootloader WARN output uses the primary UART0 console. Bootloader INFO exceeds
the existing 24 KiB slot, so WARN preserves the current partition layout.
USB secondary output starts with the application and does not mirror bootloader logs.

## Merge the factory image

After a successful build, create the dated FactoryOnly image:

```bash
mkdir -p ../../releases/factory_firmware
(cd build-v5.5.5-rev3_x && python -m esptool --chip esp32p4 merge_bin -o ../../../releases/factory_firmware/ESP32-P4-WIFI6-Touch-LCD-7B-FactoryOnly-260820.bin @flash_args)
```

The merged image is 16 MiB and is intended to be written from offset `0x0`.
Compilation and image validation do not constitute flashing or hardware runtime
validation. They do not test the panel, touch, RS485 automatic direction or
peer, CAN peer acknowledgement/termination, or physical BUS_OFF recovery.
