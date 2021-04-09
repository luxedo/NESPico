# NESPico

> Um controlão maneiro de nintendinho, só que USB

Quer jogar nintendinho mas não sabe onde foi parar a nostalgia?
ACHÔ ERRADO OTÁRIO!
Compra um RPi Pico no FilipeFlop, faz aquela gambiarra e show de bola!!

Leia também em: [Língua Raiz](README-pt-BR.md), [English (Lingua nutella)](README.md)

## Biudando

Facil fácil:

1. Instala CMake (versão mínima 3.13), e GCC cross compiler
   ```bash
   sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi
   ```
1. Clona o repo e atualiza dois submodulos
   ```bash
   git clone https://github.com/luxedo/NESPico.git
   cd NESPico
   git submodule update --init  # Thou shalt not be --recursive
   cd pico-sdk
   git submodule update --init
   cd ..
   ```
1. Cria o diretório pra biudar
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
1. Chama o bom e velho `make`zão. O binário em `/bin/` vai ser
   substituído pela sua build
   ```bash
   make
   ```

## PreBuilt

Se estiver com preguiça só pega o arquivo pré compilado [bin/nespico.uf2](bin/nespico.uf2)
e bota no seu `PiPico`.

## Pinout

NES <=> RPi Pico

| Fun   | NES      | Pico   |
| ----- | -------- | ------ |
| 3v3   | Branco   | 3V3    |
| GND   | Marrom   | Ground |
| Latch | Laranja  | GPIO 5 |
| Clock | Vermelho | GPIO 4 |
| Data  | Amarelo  | GPIO 6 |

## Links

- [Tiny USB](https://github.com/hathach/tinyusb)
- [Linux Gamepad Specification](https://www.kernel.org/doc/html/latest/input/gamepad.html?highlight=gamepad)
- [MC14021B - NES Controller shift register](https://pdf1.alldatasheet.com/datasheet-pdf/view/11945/ONSEMI/MC14021BCP/+018W7UllREMIcPhBztzvlMb+/datasheet.pdf)
- [USB HID Reports Descriptors Tutorial](https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/)
- [Pico HID Gamepad](https://github.com/Drewol/rp2040-gamecon)
- [NES Controller MicroPython Version](https://github.com/printnplay/Pico-MicroPython/blob/main/NES2USB.py)
- [Arduino NES Controller](https://github.com/Grouflon/arduino-nes-controller/blob/master/src/nes/nes.ino)

## Licença

> This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
>
> This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
>
> You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.
