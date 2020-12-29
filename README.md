# xsens MTi Serial C Library

This C library is intended for use on microcontrollers (though can be used on typical computers fairly easily) for high performance robotics applications.

The parser is hardware agnostic, with a design philosophy around structured data and callback functions when valid packets are decoded.

With the exception of one Arduino library (which didn't work for me), and the MBED examples from xsens which is SPI/I2C and specific to MTi-1/3/7 modules, there aren't any pure C libraries for communicating over UART/RS232.

> I test (and use in production) this library against MTi-300 units, connected to a STM32F4 discovery board via a RS232 transciever.
>
> Without other IMU models, I'm unable to provide guarantees of functionality or correctness (no position support for 600/700 series, etc).
>
> Happy to discuss issues & merge reasonable PRs.

# Usage

> TODO

# Running tests

MTManager was used with various hardware configurations to capture real packets from the IMU with decoded reference outputs. See `/docs/mtmanager_packets.md`.

Testing uses the [Ceedling](http://www.throwtheswitch.org/ceedling/) (Ruby/rake) based testing framework with `Unity` and `CMock`.

I don't provide Ceedling's vendor files inside this repo, so first runs need to 're-initalise' the test structure.

1. Use the provided `test/setup_tests.sh` script to begin with. 
	- You might need to add execution permission first. From `/test`, run `chmod +x setup_tests.sh`.
	- Depending on your Ruby/Gem system configuration things may not work first try. Failing that, manually install `ceedling` with `gem install ceedling`.

2. Run the `setup_tests.sh` script. If you don't have ceedling installed, it will prompt to install the ruby gem.

3. Once setup, run `ceedling` or `ceedling test:all`.

There are also a series of small scripts for other various analysis checks.

## Coverage Analysis

- Run `ceedling gcov:all` to generate the coverage reports.  
- Use `ceedling utils:gcov` to generate a pretty HTML report.
  - HTML output is located in the `/test/build/artifacts/gcov` folder.

You need `gcovr` installed, and on some Linux distros, may also need a `gcovr` runtime dependancy `jinja2`.

When attempting to run coverage analysis on OSX, the output results are slightly different due to subtle differences with the `clang` based toolchain.

I test with a `gcc`+`gcov` environment running on `$LINUX_DISTRO`.

# References & Acknowledgement

- xsens have documented their protocol and the behaviour well in the [MT Low Level Documentation PDF](https://www.xsens.com/hubfs/Downloads/Manuals/MT_Low-Level_Documentation.pdf).
- xsens MTManager software provides a packet viewer was used to capture and decode several 'golden' packets used in unit tests.

This library is [MIT licensed](LICENSE).