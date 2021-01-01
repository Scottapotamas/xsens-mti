# xsens MTi Serial C Library

This C library is intended for use on microcontrollers (though can be used on typical computers fairly easily) for high performance robotics applications.

The parser is hardware agnostic, with a design philosophy around structured data and callback functions when valid packets are decoded.

> Happy to discuss issues & merge reasonable PRs.

## Why?

With the exception of one Arduino library (which didn't work for me), and examples from xsens which are SPI/I2C & specific to MTi-1/3/7 modules, there wasn't anything available (other than ARM SBC options) which would let me interface an external AHRS unit to my embedded platform.

At time of writing, I couldn't find any suitable C libraries/examples for communicating over UART/RS232.

The packet layout and packet-in-packet (MData2) format variations mean writing a general handler and decoder is a bit terse, and the xsens C++ example shifts this responsibility to the developer.

This library _aims_ to provide a more sane approach, with application code not requiring any buffer handling or IMU specific logic.

## Does it work?

[Yes, look at this example firmware and GUI](https://github.com/Scottapotamas/xsens-stm32-eui).

MTManager was used with various output configurations to capture real packets alongside decoded reference outputs (see `/docs/mtmanager_packets.md`). These packets back the test-driven development approach of the library, while the [MT Low Level Documentation PDF](https://www.xsens.com/hubfs/Downloads/Manuals/MT_Low-Level_Documentation.pdf) was used as reference material.

I've tested the library against a MTi-300 connected to a STM32F4 discovery board via RS232 transceiver.

I use this library in production with several MTi-300 units connected to a custom STM32F429 controller, also through a RS232 transceiver.

## What's the catch?

Most of the xsens IMU hardware is pretty expensive. So it's unlikely many people will read this...

- No specific effort has been put into ensuring portability of this library to big-endian processors.
- Without other IMU hardware, I'm unable to provide functional/correctness guarantees.
- No inbuilt support for MTi-7/600/700 features such as GNSS or position.
- While the library tries to follow all message identifiers and fields outlined by xsens, some sections aren't covered (but an escape hatch is provided).

# Usage

A minimal Arduino compatible implementation is described in `/example/motion`.

## Basics

- Include `xsens-mti.h` in your application code.

- Create a callback function which will receive motion data after the library has parsed and decoded packets:

  ```c
  void imu_event_cb( EventFlag_t event, EventData_t *mtdata );
  ```

- Create another callback function which is used by the library to write binary data to the IMU:

  ```c
  void imu_write_cb( uint8_t *buffer, uint16_t length );
  ```

- Declare a `interface_t`, and pass it pointers to those two callback functions:

  - At compile time:

    ```c
    interface_t imu_interface = {.event_cb = &imu_event_cb, .output_cb = &imu_write_cb};
    ```

  - At run-time:

    ```c
    interface_t imu_interface;
    imu_interface.event_cb = &imu_event_cb;
    imu_interface.output_cb = &imu_write_cb;
    ```

- Pass inbound serial data to the library as it arrives. The parser uses a statemachine design and is happy accepting a single byte, or many, with no timing requirements.

  - Individual bytes:

    ```c
    xsens_mti_parse( &imu_interface, byte );
    ```

  - Buffer of data:

    ```c
    xsens_mti_parse_buffer( &imu_interface, &rx_buff, rx_len );
    ```

That's it.

## Using motion data

In the application code event callback `imu_event_cb` or similar, catch relevant events generated by the library when one of the `MData2` packets is received.

- `EventFlag_t` signifies what kind of data has been processed.

- `EventData_t` is a structure containing:
  - a type flag (enum) describing what data format is used in the
  - union of possible types ranging from `uint16_t` to a `float[4]`  

This method allows the application code to receive ready-to-use values immediately after they are decoded.

> Note that `*mtdata` points to data on the stack and is not long-lived.

```c
void imu_event_cb( EventFlag_t event, EventData_t *mtdata )
{
    switch( event )
    {
        case XSENS_EVT_PACKET_COUNT:
            // ...
        break;

        case XSENS_EVT_ACCELERATION:
            if( mtdata->type == XSENS_EVT_TYPE_FLOAT3 )
            {
                // Update the PID controller with x, y, z accelerometer 
                // These are 4-byte floats
                controller_update( mtdata->data.f4x3[0], 
                                   mtdata->data.f4x3[1], 
                                   mtdata->data.f4x3[2] );
            }
        break;
            
        case XSENS_EVT_PRESSURE:
        	// ...
        break;
	}
}
```

## Overriding parser functionality

In situations where you want to handle a packet directly, declare your own handler function to process the payload.

This is the previously mentioned 'escape-hatch' you should use if trying to handle unsupported packets.

```c
void imu_icc_command_ack( packet_buffer_t *packet )
{
    // Use the packet fields to post-process as needed:
    //     packet->message_id
    //     packet->length
    //     packet->payload
}
```

During your setup, provide the callback pointer and `MID` to filter for. Use the enum value in `xsens_constants.h` or the raw value:

```
bool ok = xsens_mti_override_id_handler( 0x75, &user_handle_icc_command_ack );
bool ok = xsens_mti_override_id_handler( MT_ICCCOMMANDACK, &imu_icc_command_ack );
```

When the parser has a valid packet (passed CRC check), the callback is fired.

> Note that the `packet_buffer_t` structure being pointed to is contained in the user-space `interface_t` state variable (typically heap allocated).
>
> This data will persist after the callback, but **will be wiped** when another packet is received.

This payload data is directly from the IMU (unprocessed), and you'll likely need to convert fields from their big-endian format to little-endian. `xsens_utility.h` has some helper functions to assist:

```c
uint16_t coalesce_16BE_16LE( uint8_t *source );
uint32_t coalesce_32BE_32LE( uint8_t *source );
float    coalesce_32BE_F32LE( uint8_t *source );
```

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

- xsens have documented their protocol and the behaviour in the [MT Low Level Documentation PDF](https://www.xsens.com/hubfs/Downloads/Manuals/MT_Low-Level_Documentation.pdf).
- xsens MTManager software provides a packet viewer was used to capture and decode several 'golden' packets used in unit tests.

This library is [Apache2 licenced](LICENSE).
