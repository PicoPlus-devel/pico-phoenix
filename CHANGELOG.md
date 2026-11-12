# CHANGELOG

pico-phoenix: the arcade game Phoenix (Amstar, 1980) for RP2350 boards with HDMI/DVI output.

# General Info

[Binaries for each configuration and PCB design are at the end of this page](#downloads___).

The game ROMs are not included. Copy MAME's `phoenix.zip` to `/roms/arcade/PHOENIX` on the SD card, as it is or unzipped. See the [README](https://github.com/PicoPlus-devel/pico-phoenix#game-data).

# v0.3

- **The DIP switches can now be changed in the settings menu**: coins per credit, lives and the bonus life. Saving a change restarts the game. The choices are kept on the SD card. The readme lists them.
- **Pico Computer 3.** The game now runs on the [Pico Computer 3](https://github.com/UKTailwind/Pico-Computer-3): HDMI, sound through HDMI or the audio jack, USB controllers, and NES or SNES controllers and a Wii Classic controller on the I/O header. The pin assignment may still change. See [Pico Computer 3](https://github.com/PicoPlus-devel/pico-infonesPlus#pico-computer-3) in the pico-infonesPlus readme. Thanks to @javavi.
- **PCB for the Waveshare RP2350-PiZero.** The game now runs on a PCB for the Waveshare RP2350-PiZero, designed by @szuping: HDMI, sound through HDMI or the audio jack, a USB controller on the PIO-USB port, NES and SNES controller ports, and a socket for an ESP32 module that connects Bluetooth controllers through [BlueRetro](https://github.com/darthcloud/blueretro). The PCB design is not yet available. See [PCB with Waveshare RP2350-PiZero](https://github.com/PicoPlus-devel/pico-infonesPlus#pcb-with-waveshare-rp2350-pizero) in the pico-infonesPlus readme.
- **NES and SNES controllers on the controller ports are now read with the same timing as a real console**, on all boards. Thanks to @javavi.
- Under pico-bootLoader, the settings menu entry that goes back to the bootloader is now called **Return to boot loader**.
- Fixed: on boards with HSTX video, with *Display Mode* set to DVI, the picture could randomly drop out, and now and then a single line was shown shifted to the right for one frame.

# v0.2.3

- **No scanlines on boards with PicoDVI output.** With *Tate mode* off they did not display well, so the scanline screen mode is no longer offered on these boards. A saved scanline setting switches to the mode without scanlines.
- **USB keyboard in the menus.** Enter selects and Esc goes back.
- Fixed: on the Adafruit Fruit Jam and other boards with PIO-USB, USB controllers were often not found at start-up.

# v0.2.2

- The release now includes updateAll.exe, which installs the ROM set on the SD card from a Windows computer. The readme explains how to use it.

# v0.2.1

- **Every line of the picture is shown** on boards with HSTX video output, such as the Adafruit Fruit Jam, when *Tate mode* is off. Before, one line in sixteen was left out.

# v0.2

- **Settings are shared with the other arcade games** of this family and stored in `/settings_ARC.dat`. Settings saved by v0.1 are not carried over, so they return to their defaults once.
- The menu colour settings are no longer shown in the settings menu.

# v0.1

Initial release.

- Phoenix with sound and music, on every RP2350 board of this family. PSRAM is not needed.
- Missing ROM files are named on screen.
- **Tate mode** setting: the picture is turned upright for a normal screen, or shown unrotated for a monitor turned on its side, either way round.
- Can be started from pico-bootLoader, where it appears in the Arcade category.

<a name="downloads___"></a>
## Downloads by configuration

Binaries for each configuration are listed below. Only RP2350 boards are supported (Pico 2, Pimoroni Pico Plus 2 and the boards below); there are no binaries for the RP2040 (Pico). Binaries for the Pico 2 also work on the Pico 2 W.

Wiring and assembly are the same as for the NES emulator; the Readme links go to the [pico-infonesPlus README](https://github.com/PicoPlus-devel/pico-infonesPlus#setup).

### Standalone boards

| Board | Binary | Readme |
|:--|:--|:--|
| Adafruit Metro RP2350 | [picoPhoenix_AdafruitMetroRP2350_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitMetroRP2350_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#adafruit-metro-rp2350) |
| Adafruit Fruit Jam | [picoPhoenix_AdafruitFruitJam_arm_piousb.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitFruitJam_arm_piousb.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#adafruit-fruit-jam) |
| Waveshare RP2350-PiZero | [picoPhoenix_WaveShareRP2350PiZero_arm_piousb.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_WaveShareRP2350PiZero_arm_piousb.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#waveshare-rp2040rp2350-pizero-development-board) |
| Olimex RP2040-PICO-PC with a Pico 2 | [picoPhoenix_OlimexPicoPC_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_OlimexPicoPC_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#olimex-rp2040-pico-pc) |
| Pico Computer 3 (PC3) | [picoPhoenix_PicoComputer3_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_PicoComputer3_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#pico-computer-3) |

### Breadboard

| Board | Binary | Readme |
|:--|:--|:--|
| Pico 2 | [picoPhoenix_AdafruitDVISD_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitDVISD_pico2_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#raspberry-pi-pico-or-pico-2-setup-with-adafruit-hardware-and-breadboard) |
| Pimoroni Pico Plus 2 | [picoPhoenix_AdafruitDVISD_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitDVISD_pico2_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#raspberry-pi-pico-or-pico-2-setup-with-adafruit-hardware-and-breadboard) |
| Adafruit Feather RP2350 with HSTX Port and TLV320DAC3100 I2S DAC | [picoPhoenix_AdafruitFeatherRP2350_TLV320DAC3100_arm_piousb.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitFeatherRP2350_TLV320DAC3100_arm_piousb.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-phoenix#supported-hardware) |

### PCB Pico 2 and Pimoroni Pico Plus 2

| Board | Binary | Readme |
|:--|:--|:--|
| Pico 2 | [picoPhoenix_AdafruitDVISD_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitDVISD_pico2_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#pcb-with-raspberry-pi-pico-or-pico-2-and-pimoroni-pico-plus-2) |
| Pimoroni Pico Plus 2 (PCB v2.6 and up, headers required) | [picoPhoenix_AdafruitDVISD_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_AdafruitDVISD_pico2_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#pcb-with-raspberry-pi-pico-or-pico-2-and-pimoroni-pico-plus-2) |

PCB: [pico_nesPCB_v2.6.zip](https://github.com/PicoPlus-devel/pico-infonesPlus/releases/latest/download/pico_nesPCB_v2.6.zip)

### PCB Waveshare RP2350-Zero (PCB required)

| Board | Binary | Readme |
|:--|:--|:--|
| Waveshare RP2350-Zero | [picoPhoenix_WaveShareRP2350ZeroWithPCB_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_WaveShareRP2350ZeroWithPCB_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#pcb-with-waveshare-rp2040rp2350-zero) |

PCB: [Gerber_PicoNES_Mini_PCB_v2.0.zip](https://github.com/PicoPlus-devel/pico-infonesPlus/releases/latest/download/Gerber_PicoNES_Mini_PCB_v2.0.zip)

### PCB Waveshare RP2350-USBA with PCB

[Binary](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_WaveShare2350USBA_arm_piousb.uf2)

PCB: [Gerber_PicoNES_Micro_v1.2.zip](https://github.com/PicoPlus-devel/pico-infonesPlus/releases/latest/download/Gerber_PicoNES_Micro_v1.2.zip)

[Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#pcb-with-waveshare-rp2350-usb-a)

### PCB Waveshare RP2350-PiZero

| Board | Binary | Readme |
|:--|:--|:--|
| Waveshare RP2350-PiZero (*) | [picoPhoenix_WaveShareRP2350PiZeroLiuPCB_arm_piousb.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_WaveShareRP2350PiZeroLiuPCB_arm_piousb.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#pcb-with-waveshare-rp2350-pizero) |

This binary is only for the RP2350-PiZero on this PCB. The PCB design is not yet available.

(*) If you fitted this board with PSRAM and it has a non-Winbond flash chip, apply the [flash_config fix](https://github.com/PicoPlus-devel/pico-infonesPlus#psram-with-a-non-winbond-flash-chip) before flashing the emulator.

### Pimoroni Pico DV

| Board | Binary | Readme |
|:--|:--|:--|
| Pico 2/Pico 2 w | [picoPhoenix_PimoroniDVI_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_PimoroniDVI_pico2_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#raspberry-pi-pico-or-pico-2-setup-for-pimoroni-pico-dv-demo-base) |
| Pimoroni Pico Plus 2 | [picoPhoenix_PimoroniDVI_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_PimoroniDVI_pico2_arm.uf2) | [Readme](https://github.com/PicoPlus-devel/pico-infonesPlus#raspberry-pi-pico-or-pico-2-setup-for-pimoroni-pico-dv-demo-base) |

### SpotPear HDMI

| Board | Binary |
|:--|:--|
| Pico 2/Pico 2 w | [picoPhoenix_SpotpearHDMI_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_SpotpearHDMI_pico2_arm.uf2) |

### Murmulator M1

| Board | Binary |
|:--|:--|
| Pico 2/Pico 2 w | [picoPhoenix_MurmulatorM1_pico2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_MurmulatorM1_pico2_arm.uf2) |

### Murmulator M2

| Board | Binary |
|:--|:--|
| Pico 2/Pico 2 w | [picoPhoenix_MurmulatorM2_arm.uf2](https://github.com/PicoPlus-devel/pico-phoenix/releases/latest/download/picoPhoenix_MurmulatorM2_arm.uf2) |

### pico-bootLoader

Phoenix is also part of the [pico-bootLoader](https://github.com/PicoPlus-devel/pico-bootLoader) SD-card bundle, in the Arcade category. Download the bundle from the pico-bootLoader releases page.
