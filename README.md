# NESPico

> A NES controller USB interface with a Raspberry Pi Pico.

## Building

These instructions are extremely terse, and Linux-based only. For detailed steps,
instructions for other platforms, and just in general, we recommend you see [Raspberry Pi Pico C/C++ SDK](https://rptl.io/pico-c-sdk)

1. Install CMake (at least version 3.13), and GCC cross compiler
   ```bash
   sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi
   ```
1. Clone this repo and update submodules.
   ```bash
   git clone https://github.com/luxedo/NESPico.git
   cd NESPico
   git submodule update --init --recursive
   ```
1. Setup a CMake build directory.
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
1. Make your target from the build directory you created. The build will
replace the prebuilt binary.
   ```bash
   make
   ```
## PreBuilt

If you don't want to bother and build the binary yourself, then the latest
version is avaliable at [bin/nespico.uf2](bin/nespico.uf2)

## Pinout

| Fun   | NES    | Pico   |
| ----- | ------ | ------ |
| 3v3   | White  | 3V3    |
| GND   | Brown  | Ground |
| Latch | Orange | GPIO 5 |
| Clock | Red    | GPIO 4 |
| Data  | Yellow | GPIO 6 |

# Links

* [Tiny USB](https://github.com/hathach/tinyusb)
* [Linux Gamepad Specification](https://www.kernel.org/doc/html/latest/input/gamepad.html?highlight=gamepad)
* [MC14021B - NES Controller shift register](https://pdf1.alldatasheet.com/datasheet-pdf/view/11945/ONSEMI/MC14021BCP/+018W7UllREMIcPhBztzvlMb+/datasheet.pdf)
* [USB HID Reports Descriptors Tutorial](https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/)
* [Pico HID Gamepad](https://github.com/Drewol/rp2040-gamecon)
* [NES Controller MicroPython Version](https://github.com/printnplay/Pico-MicroPython/blob/main/NES2USB.py)
* [Arduino NES Controller](https://github.com/Grouflon/arduino-nes-controller/blob/master/src/nes/nes.ino)
