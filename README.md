# CanoKey on nRF52840

**WARNING: THIS CANOKEY PORT IS IN VERY EARLY STAGE AND UNDER ACTIVE DEVELOPMENT. THE CODE IS NOT TESTED, IT MAY OR MAY NOT WORK, PLEASE USE AT YOUR OWN RISK.**

**警告：本项目仍处于非常早期的开发阶段。该代码未经过严格测试，并且可能完全无法工作，请自行承担使用风险。**

CanoKey is an open-source USB security token, providing the following functions:

- OpenPGP Card V3.4 (RSA, ECDSA, ED25519 supported)
- PIV Card
- TOTP / HOTP (RFC6238 / RFC4226)
- U2F
- FIDO2 (WebAuthn)

It works on modern Linux/Windows/macOS operating systems without additional driver required. The NFC function of canokey-core is not currently supported.

**THERE IS ABSOLUTELY NO SECURITY ASSURANCE OR WARRANTY USING THE NRF52 VERSION.**

**ANYONE WITH PHYSICAL ACCESS CAN RETRIEVE ANY DATA (INCLUDING SECRET KEYS) FROM THE KEY.**

**IT IS ALSO PRONE TO SIDE-CHANNEL ATTACKS.**

**YOU MUST USE IT AT YOUR OWN RISK.**

**A SECURE VERSION CAN BE FOUND AT https://canokeys.org**

## Hardware

This CanoKey-nRF52 implementation is based on Nordic nRF52840 MCU, features 1MB flash and 256KB RAM. We support the following boards:

### NRF52840 Dongle and variants

The nRF52840 Dongle is a small, low-cost USB dongle designed by Nordic Semiconductor, with a small USB Key form factor. It's often referred to as PCA10056 in the nRF52840 SDK. There exists multiple compatible boards in the market, including:

- [nRF52840 Dongle](https://www.nordicsemi.com/Software-and-tools/Development-Kits/nRF52840-Dongle)
- [makerdiary nRF52840 Dongle](https://makerdiary.com/products/nrf52840-mdk-usb-dongle)
- [makerdiary nRF52840 MDK USB Dongle w/ Case](https://makerdiary.com/collections/frontpage/products/nrf52840-mdk-usb-dongle-w-case)
- [Ebyte E104-BT5040UA](https://www.ebyte.com/en/product-view-news.html?id=1267)

Your can use any of them, by selecting the corresponding board in the `CMakeLists.txt` file:

```cmake
set(CANOKEY_BOARD OPT_BOARD_NRF52840_DONGLE)
```

### Makerdiary nRF52840 M.2 Developer Kit

nRF52840 M.2 Developer Kit is a versatile IoT prototyping platform, features a M.2 form factor core module, plus a fully-integrated DAPLink debugger with Power Profiling. This is a good choice for developers who wish to debug the firmware.

```cmake
set(CANOKEY_BOARD OPT_BOARD_MAKERDIARY_M2_DOCK)
```

### Other boards

You can easily port CanoKey to other boards, by defining the board name macro in `config/board_selection.h`, and implement the board pinout config in `config/boards/<vendor>/<board_name>.h`. Also, you can copy offical nRF52840 SDK board config files from `nrf_sdk/nRF5_SDK_17.1.0_ddde560/components/boards` directory into this project.


## Build and test

### Build and flash bootloader

This project uses [Adafruit_nRF52_Bootloader](https://github.com/adafruit/Adafruit_nRF52_Bootloader), which is a [UF2](https://github.com/Microsoft/uf2) compliant bootloader for nRF5x devices. 

If your board ships with this bootloader (such as the Feitian OpenSK USB Dongle), you can skip the following steps. Otherwise, you need to build and flash the bootloader first.

1. Build the bootloader, see the [Adafruit document](https://github.com/adafruit/Adafruit_nRF52_Bootloader) for this. We don't use SoftDevice in this project, so unfortunately you can't use the prebuilt binaries in their release page.

Notice: the board name for bootloader is slightly different from this project, do not mistaken them.

2. Flash the bootloader, in `Adafruit_nRF52_Bootloader` directory, run (maybe you need to change `FLASHER` to `nrfjprog` depends on environment):

```shell
# Flash MBR (Master Boot Record)
$ make BOARD=feather_nrf52840_express FLASHER=pyocd flash-mbr

# Flash bootloader
$ make BOARD=feather_nrf52840_express FLASHER=pyocd flash
```

### Build the Firmware

Prerequisites:

- CMake >= 3.18
- GNU ARM Embedded Toolchain, downloaded from [ARM](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- git (used to generate embedding version string)
- python3 (used to generate UF2 format DFU file)

Build steps:

```shell
# clone this repo and all the submodules
# in the top-level folder
$ git submodule update --init

$ mkdir build
$ cd build
$ cmake -DCROSS_COMPILE=<path-to-toolchain>/bin/arm-none-eabi- \
    -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release ..

$ make canokey_flash.uf2
```

### Flash the firmware

After building the firmware, you should get a `canokey_flash.uf2` file in the build directory. Enter DFU mode for your device, and it should appear as a USB drive. Copy the `canokey_flash.uf2` file to the USB drive, and the device will reboot and run the new firmware.

### Initialize and Test

Prerequisites:

- Linux OS with pcscd, pcsc_scan and scriptor installed

Connect the CanoKey to PC, an USB CCID device should show up. The `pcsc_scan` command should be able to detect a smart card:

```shell
$ pcsc_scan
Using reader plug'n play mechanism
Scanning present readers...
0: Canokeys Canokey [OpenPGP PIV OATH] (123456) 00 00
 
Fri Aug 19 21:19:54 2022
 Reader 0: Canokeys Canokey [OpenPGP PIV OATH] (123456) 00 00
  Event number: 0
  Card state: Card inserted, 
  ATR: 3B F7 11 00 00 81 31 FE 65 43 61 6E 6F 6B 65 79 99

ATR: 3B F7 11 00 00 81 31 FE 65 43 61 6E 6F 6B 65 79 99
+ TS = 3B --> Direct Convention
+ T0 = F7, Y(1): 1111, K: 7 (historical bytes)
  TA(1) = 11 --> Fi=372, Di=1, 372 cycles/ETU
    10752 bits/s at 4 MHz, fMax for Fi = 5 MHz => 13440 bits/s
  TB(1) = 00 --> VPP is not electrically connected
  TC(1) = 00 --> Extra guard time: 0
  TD(1) = 81 --> Y(i+1) = 1000, Protocol T = 1 
-----
  TD(2) = 31 --> Y(i+1) = 0011, Protocol T = 1 
-----
  TA(3) = FE --> IFSC: 254
  TB(3) = 65 --> Block Waiting Integer: 6 - Character Waiting Integer: 5
+ Historical bytes: 43 61 6E 6F 6B 65 79
  Category indicator byte: 43 (proprietary format)
+ TCK = 99 (correct checksum)

Possibly identified card (using /usr/share/pcsc/smartcard_list.txt):
3B F7 11 00 00 81 31 FE 65 43 61 6E 6F 6B 65 79 99
        Canokey (Other)
        http://canokeys.org/
```

Then, initialize the CanoKey by running `device-config-init.sh`. This script will login as admin with default PIN `123456`, set device serial number to current Unix timestamp, then write an attestation certificate used by FIDO. Refer to [admin doc](https://doc.canokeys.org/development/protocols/admin/) if you want to customize these steps.

```shell
$ ./device-config-init.sh 'Canokeys Canokey [OpenPGP PIV OATH] (00000000) 00 00'
Reader name: Canokeys Canokey [OpenPGP PIV OATH] (00000000) 00 00
Using given card reader: Canokeys Canokey [OpenPGP PIV OATH] (00000000) 00 00
Using T=1 protocol
Reading commands from STDIN
> 00 A4 04 00 05 F0 00 00 00 00 
< 90 00 : Normal processing.
> 00 20 00 00 06 31 32 33 34 35 36 
< 90 00 : Normal processing.
> 00 30 00 00 04 61 eb fc 55
< 90 00 : Normal processing.
> 00 01 00 00 20 cc d3 ee 4d ea 2a 3c ca d8 ce 3d 6a 47 f5 45 9c 80 79 7a d9 15 d6 f4 62 8b 5d 36 ef f2 76 d6 87
< 90 00 : Normal processing.
> 00 02 00 00 00 02 7b 30 82 02 77 30 82 01 5f a0 03 02 01 02 02 01 0d 30 0d 06 09 2a 86 48 86 f7 0d 01 01 0b 05 00 30 31 31 2f 30 2d 06 03 55 04 03 0c 26 43 61 6e 6f 4b 65 79 73 20 46 49 44 4f 20 41 74 74 65 73 74 61 74 69 6f 6e 20 52 6f 6f 74 20 43 41 20 4e 6f 2e 31 30 1e 17 0d 32 30 30 37 31 35 30 31 35 35 30 30 5a 17 0d 33 30 30 34 31 34 30 31 35 35 30 30 5a 30 66 31 20 30 1e 06 03 55 04 03 0c 17 43 61 6e 6f 4b 65 79 20 53 65 72 69 61 6c 20 30 30 31 31 34 35 31 34 31 22 30 20 06 03 55 04 0b 0c 19 41 75 74 68 65 6e 74 69 63 61 74 6f 72 20 41 74 74 65 73 74 61 74 69 6f 6e 31 11 30 0f 06 03 55 04 0a 0c 08 43 61 6e 6f 4b 65 79 73 31 0b 30 09 06 03 55 04 06 13 02 43 4e 30 59 30 13 06 07 2a 86 48 ce 3d 02 01 06 08 2a 86 48 ce 3d 03 01 07 03 42 00 04 06 5c 77 8f 90 f3 5b 30 fc 64 c0 ff db 6a ea 64 bb c7 bd c5 63 89 01 60 96 c2 6d ac 83 cf 54 63 47 07 d0 57 72 e2 55 06 4a 55 c4 00 c7 d3 67 32 4a b6 26 82 e3 58 22 06 1e b9 9a 52 2c 97 54 99 a3 30 30 2e 30 09 06 03 55 1d 13 04 02 30 00 30 21 06 0b 2b 06 01 04 01 82 e5 1c 01 01 04 04 12 00 00 24 4e b2 9e e0 90 4e 49 81 fe 1f 20 f8 d3 b8 f4 30 0d 06 09 2a 86 48 86 f7 0d 01 01 0b 05 00 03 82 01 01 00 40 e1 20 47 e0 53 70 85 8c 1b db 55 db a6 8b 1e 4c a3 9a c6 e4 54 b5 d9 e9 35 65 04 7a c8 0a 3e 9a 9f 61 79 ec 86 d4 e5 87 20 a3 4b 1c 60 21 98 71 a4 6d c4 a4 5a 22 bd f4 aa c4 0a c4 b1 c3 5d ad 4c 1f 52 a0 ec 22 0c 53 38 54 57 55 2b 83 a6 71 9a ad 1d 03 1e a6 30 87 f7 17 d1 53 86 96 88 17 6d 14 4e 9e d5 b9 f2 50 38 5a 86 c6 75 50 fa 42 f9 1d ec 3d 03 35 13 d4 fc 20 fc 44 e4 86 cd a2 21 99 a6 1b 42 23 fe 56 36 6b 2c ed 45 39 fc 47 32 bb 25 92 08 fb 0f e6 c3 2f 14 3c 87 af f5 11 36 3a fc 5a 62 19 dd b3 b6 e4 b7 88 e3 7f 31 b6 a3 8a 24 79 10 1b 16 e0 ec 87 23 0c 48 b4 33 2a 9b 8c 78 fd 1e 91 fe 45 e6 eb 32 22 eb 91 72 0d e5 f2 1f 52 52 bf e7 5a 61 7b f7 15 c4 4b 01 48 8b 40 35 4e 39 8c 80 5c a7 99 df c6 4c 27 75 43 cd 1f 96 8d a1 f2 2c 9e a5 d1 ea 87 41 64 02
< 90 00 : Normal processing.
```

After initialization, you are free to use CanoKey with applications, such as:

- GPG, e.g. `gpg --card-status`
- SSH with pkcs11, e.g. `ssh -I /usr/lib/x86_64-linux-gnu/opensc-pkcs11.so user@host`
- piv-tool
- Websites with U2F enabled, e.g., https://github.com/settings/security
- Websites with WebAuthn enabled, e.g., https://webauthn.me/
- CanoKey Web Console: https://console.canokeys.org/

## Documentation

Check out our official [documentation](https://doc.canokeys.org).

## License

All software, unless otherwise noted, is licensed under Apache 2.0.

Unless you explicitly state, otherwise, any contribution intentionally submitted for inclusion in the work by you, as defined in the Apache-2.0 license without any additional terms or conditions.

All hardware, unless otherwise noted, is dual licensed under CERN and CC-BY-SA. You may use CanoKey hardware under the terms of either the CERN 1.2 license or CC-BY-SA 4.0 license.

All documentation, unless otherwise noted, is licensed under CC-BY-SA. You may use CanoKey documentation under the terms of the CC-BY-SA 4.0 license
