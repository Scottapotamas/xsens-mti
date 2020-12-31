# xsens MTi Serial C Library

This C library is intended for use on microcontrollers (though can be used on typical computers fairly easily) for high performance robotics applications.

The parser is hardware agnostic, with a design philosophy around structured data and callback functions when valid packets are decoded.

> Happy to discuss issues & merge reasonable PRs.

## Why?

With the exception of one Arduino library (which didn't work for me), and examples from xsens which are SPI/I2C & specific to MTi-1/3/7 modules, there wasn't anything available (other than ARM SBC options) which would let me interface an external AHRS unit.

At time of writing, there aren't _any pure C_ libraries/examples for communicating over UART/RS232.

The packet layout and packet-in-packet (MData2) format variations mean writing a general handler and decoder is a bit terse, and xsens example code shifts this responsibility to the developer. This library _aims_ to provide a more sane approach, with application code not requiring any buffer handling or IMU specific logic.

## Does it work?

Yes.

MTManager was used with various output configurations to capture real packets alongside decoded reference outputs (see `/docs/mtmanager_packets.md`). These packets back the test-driven development approach of the library, while the [MT Low Level Documentation PDF](https://www.xsens.com/hubfs/Downloads/Manuals/MT_Low-Level_Documentation.pdf) was used as reference material.

Additionally, I've tested the library against a MTi-300 connected to a STM32F4 discovery board via RS232 transciever.

I use this library in production with several MTi-300 units connected to a custom STM32F429 controller, also through a RS232 transciever.

## What's the catch?

Most of the xsens IMU hardware is pretty expensive. So it's unlikely many people will read this...

- No specific effort has been put into ensuring portability of this library to big-endian processors.
- Without other IMU hardware, I'm unable to provide functional/correctness guarantees.
- No inbuilt support for MTi-7/600/700 features such as GNSS or position.
- While the library tries to follow all message identifiers and fields outlined by xsens, some sections aren't covered (but an escape hatch is provided).

# Usage

> TODO

# Running tests

Testing uses the [Ceedling](http://www.throwtheswitch.org/ceedling/) (Ruby/rake) based testing framework with `Unity` and `CMock`.

I don't provide Ceedling's vendor files inside this repo, so first runs need to 're-initalise' the test structure.

1. Use the provided `setup_tests.sh` script to begin.

   - You might need to add execution permission first. Run `chmod +x setup_tests.sh`.
   - Depending on your Ruby/Gem system configuration things may not work first try. Failing that, manually install `ceedling` with `gem install ceedling`.

2. Run the `setup_tests.sh` script. If you don't have ceedling installed, it will prompt to install the ruby gem.

3. Once setup, run `ceedling` or `ceedling test:all`.

## Coverage Analysis

- Run `ceedling gcov:all` to generate the coverage reports.
- Use `ceedling utils:gcov` to generate a pretty HTML report.
  - HTML output is located in the `/build/artifacts/gcov` folder.

You need `gcovr` installed, and on some Linux distros, may also need a `gcovr` runtime dependancy `jinja2`.

When attempting to run coverage analysis on OSX, the output results are slightly different due to subtle differences with the `clang` based toolchain.

I test with a `gcc`+`gcov` environment running on `$LINUX_DISTRO`.

# References & Acknowledgement

- xsens have documented their protocol and the behaviour in the [MT Low Level Documentation PDF](https://www.xsens.com/hubfs/Downloads/Manuals/MT_Low-Level_Documentation.pdf), and there are only a few typos.
- xsens MTManager software provides a packet viewer was used to capture and decode several 'golden' packets used in unit tests.

This library is [Apache2 licenced](LICENSE).
