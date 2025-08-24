# stk500v1_isp

Generic AVR ISP implementation

## Overview

This project is generic ISP (in-system programmer) implementation for AVR microcontrollers.

_Generic_ means hardware-independent; you can install it to any (requirements-fulfilled) MCUs.  
There is sample implementation for Raspberry Pi Pico at `src/app/main.c` .

## Specification

- Communication protocol with PC: STK500 v1 (in details, see [AVR061](https://www.microchip.com/content/dam/mchp/documents/OTH/ApplicationNotes/ApplicationNotes/doc2525.pdf).)
- Supported MCU requirements:
  - **<= 128KB** flash capacity (currently ATmega2560 (used on Arduino MEGA 2560) is not supported)
  - Serial Programming via SPI supported (Fuse `DWEN`, `RSTDISBL` is unprogrammed)

## Development

Thank you for your contribution!
For more information about development, see [DEVELOPMENT.md](DEVELOPMENT.md).

## License

This repository is published under [MIT License](LICENSE).
