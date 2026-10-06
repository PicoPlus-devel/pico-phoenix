# pico-phoenix

An emulator of the arcade game Phoenix (Amstar, 1980) for RP2350 boards with DVI/HDMI output.

# Changelog

## v0.1

Initial release.

- Phoenix with sound and music, on every RP2350 board of this family. PSRAM is not needed.
- The game ROMs are not included. Copy MAME's `phoenix.zip` to `/roms/arcade/PHOENIX` on the SD card, as it is or unzipped. Missing files are named on screen.
- **Tate mode** setting: the picture is turned upright for a normal screen, or shown unrotated for a monitor turned on its side, either way round.
- Can be started from pico-bootLoader, where it appears in the Arcade category.
