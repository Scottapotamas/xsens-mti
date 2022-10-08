#include "unity.h"
#include <string.h>

// MODULE UNDER TEST
#include "xsens_mti.h"
#include "xsens_mti_private.h"
#include "xsens_constants.h"
#include "xsens_mdata2.h"
#include "xsens_utility.h"


// DEFINITIONS 
 
// PRIVATE TYPES
 
// PRIVATE DATA
xsens_interface_t test_imu = { 0 };


// PRIVATE FUNCTIONS

void mock_output_function( uint8_t *buffer, uint16_t size );
void mock_event_function( XsensEventFlag_t event, XsensEventData_t *data );

#define OUTBOUND_BUFFER_SIZE 2048

uint8_t outbound_cache[OUTBOUND_BUFFER_SIZE] = { 0 };
uint16_t outbound_cache_pos = 0;

void mock_output_function( uint8_t *buffer, uint16_t size )
{
    memcpy( &outbound_cache, buffer, size );
    outbound_cache_pos += size;

    if( outbound_cache_pos > OUTBOUND_BUFFER_SIZE )
    {
        TEST_FAIL_MESSAGE("Test harness cache ran out of room");
    }
}

void mock_event_function( XsensEventFlag_t event, XsensEventData_t *data )
{
    // printf("Notified of evt: %d\n", event);
}

// SETUP, TEARDOWN
void setUp(void)
{
    memset( &outbound_cache, 0, sizeof(outbound_cache) );
    outbound_cache_pos = 0;

    memset( &test_imu, 0, sizeof(test_imu) );
    test_imu.output_cb = &mock_output_function;
    test_imu.event_cb = &mock_event_function;
}
 
void tearDown(void)
{

}

// TESTS
void test_send_configstate( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x30, 
                            0x00,
                            0xD1 };


    xsens_mti_request( &test_imu, MT_GOTOCONFIG );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );          
}

void test_send_measuremenetstate( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x10, 
                            0x00,
                            0xF1 };

    xsens_mti_request( &test_imu, MT_GOTOMEASUREMENT );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}


void test_request_baud( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x18, 
                            0x00,
                            0xE9 };

    xsens_mti_request( &test_imu, MT_REQBAUDRATE );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_send_setbaud_115k2( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x18, 
                            0x01,
                            0x02,
                            0xE6 };

    xsens_mti_set_baudrate( &test_imu, XSENS_BAUD_115200 );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_send_setbaud_230k4( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x18, 
                            0x01,
                            0x01,
                            0xE7 };

    xsens_mti_set_baudrate( &test_imu, XSENS_BAUD_230400 );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
    
}

void test_send_setbaud_921k6( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x18, 
                            0x01,
                            0x80,
                            0x68 };

    xsens_mti_set_baudrate( &test_imu, XSENS_BAUD_921600 );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_send_resetorientation_heading( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0xA4, 
                            0x02,
                            0x00,
                            0x01,
                            0x5A };

    xsens_mti_reset_orientation( &test_imu, XSENS_ORIENTATION_HEADING_RESET );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );                   
}

void test_send_resetorientation_align( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0xA4, 
                            0x02,
                            0x00,
                            0x04,
                            0x57 };

   xsens_mti_reset_orientation( &test_imu, XSENS_ORIENTATION_ALIGNMENT_RESET );
   TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_request_reqdid( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x00, 
                            0x00,
                            0x01 };

    xsens_mti_request( &test_imu, MT_REQDID );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_request_reqproductcode( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x1C, 
                            0x00,
                            0xE5 };

    xsens_mti_request( &test_imu, MT_REQPRODUCTCODE );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_request_hardwareversion( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x1E, 
                            0x00,
                            0xE3 };

    xsens_mti_request( &test_imu, MT_REQHARDWAREVERSION );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_request_reqfirmwareversion( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x12, 
                            0x00,
                            0xEF };

    xsens_mti_request( &test_imu, MT_REQFWREV );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

void test_request_runselftest( void )
{   

    uint8_t expected[] = {  0xFA, 
                            0xFF, 
                            0x24, 
                            0x00,
                            0xDD };

    xsens_mti_request( &test_imu, MT_RUNSELFTEST );
    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}


// From MT manual example, page 66
void test_set_configuration( void )
{   
    // PacketCounter    0x1020
    // SampleTimeFine   0x1060
    // Quaternion       0x2010 @ 100 Hz (0x0064)
    // Acceleration     0x4020 @ 400 Hz (0x0190)
    // Rate of Turn     0x8020 @ 400 Hz (0x0190)
    // Magnetic Field   0xC020 @ 100 Hz (0x0064)
    // Statusword       0xE020
    // LatLon           0x5042 @ 100 Hz (0x0064)
    // Altitude         0x5022 @ 100 Hz (0x0064)
    // Velocity XYZ     0xD012 @ 100 Hz (0x0064)

    // 0xFFFF and 0x0000 are both valid representations for 'max frequency'

    uint8_t expected[] = {  0xFA,
                            0xFF,
                            0xC0,
                            0x28,
                            0x10, 0x20, 0xFF, 0xFF, 0x10, 0x60, 0xFF, 0xFF, 0x20, 0x10, 0x00, 0x64, 0x40, 0x20, 0x01, 0x90, 0x80, 0x20, 0x01, 0x90, 0xC0, 0x20, 0x00, 0x64, 0xE0, 0x20, 0xFF, 0xFF, 0x50, 0x42, 0x00, 0x64, 0x50, 0x22, 0x00, 0x64, 0xD0, 0x12, 0x00, 0x64,
                            0x73 };

    XsensFrequencyConfig_t settings[] = {
        { .id = XDI_PACKET_COUNTER, .frequency = 0xFFFF },
        { .id = XDI_SAMPLE_TIME_FINE, .frequency = 0xFFFF },
        { .id = XDI_QUATERNION, .frequency = 100 },
        { .id = XDI_ACCELERATION, .frequency = 400 },
        { .id = XDI_RATE_OF_TURN, .frequency = 400 },
        { .id = XDI_MAGNETIC_FIELD, .frequency = 100 },
        { .id = XDI_STATUS_WORD, .frequency = 0xFFFF },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_LAT_LON, XSENS_FLOAT_FIXED1632, XSENS_COORD_ENU), .frequency = 100 },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_ALTITUDE_ELLIPSOID, XSENS_FLOAT_FIXED1632, XSENS_COORD_ENU), .frequency = 100 },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_VELOCITY_XYZ, XSENS_FLOAT_FIXED1632, XSENS_COORD_ENU), .frequency = 100 },
    };

    xsens_mti_set_configuration( &test_imu, settings, 10 );

    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

// From sniffed flow with MTi-300 and MTManager software
void test_set_configuration_2( void )
{
    // PacketCounter, Freq: 65535, 
    // SampleTimeFine, Freq: 65535, 
    // Quaternion|Float|ENU, Freq: 400, 
    // Acceleration|Float, Freq: 400, 
    // DeltaV|Float, Freq: 400, 
    // FreeAcceleration|Float, Freq: 400, 
    // RateOfTurn|Float, Freq: 400, 
    // DeltaQ|Float, Freq: 400, 
    // MagneticField|Float, Freq: 100, 
    // Temperature|Float, Freq: 10, 
    // BaroPressure, Freq: 50, 
    // StatusWord, Freq: 65535

    uint8_t expected[] = {  0xFA,
                            0xFF,
                            0xC0,
                            0x30,
                            0x10, 0x20, 0xFF, 0xFF, 0x10, 0x60, 0xFF, 0xFF, 0x20, 0x10, 0x01, 0x90, 0x40, 0x20, 0x01, 0x90, 0x40, 0x10, 0x01, 0x90, 0x40, 0x30, 0x01, 0x90, 0x80, 0x20, 0x01, 0x90, 0x80, 0x30, 0x01, 0x90, 0xC0, 0x20, 0x00, 0x64, 0x08, 0x10, 0x00, 0x0A, 0x30, 0x10, 0x00, 0x32, 0xE0, 0x20, 0xFF, 0xFF,
                            0x99 };

    XsensFrequencyConfig_t settings[] = {
        { .id = XDI_PACKET_COUNTER, .frequency = 0xFFFF },
        { .id = XDI_SAMPLE_TIME_FINE, .frequency = 0xFFFF },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_QUATERNION, XSENS_FLOAT_SINGLE, XSENS_COORD_ENU), .frequency = 400 },
        { .id = XDI_ACCELERATION, .frequency = 400 },
        { .id = XDI_DELTA_V, .frequency = 400 },
        { .id = XDI_FREE_ACCELERATION, .frequency = 400 },
        { .id = XDI_RATE_OF_TURN, .frequency = 400 },
        { .id = XDI_DELTA_Q, .frequency = 400 },
        { .id = XDI_MAGNETIC_FIELD, .frequency = 100 },
        { .id = XDI_TEMPERATURE, .frequency = 10 },
        { .id = XDI_BARO_PRESSURE, .frequency = 50 },
        { .id = XDI_STATUS_WORD, .frequency = 0xFFFF },
    };

    xsens_mti_set_configuration( &test_imu, settings, XSENS_ARR_ELEM(settings) );

    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

// From sniffed flow with MTi-300 and MTManager software
void test_set_configuration_3( void )
{

    // PacketCounter, Freq: 65535, 
    // SampleTimeFine, Freq: 65535, 
    // Quaternion|Float|ENU, Freq: 200, 
    // RateOfTurn|1220, Freq: 100, 
    // MagneticField|1632, Freq: 100, 
    // Temperature|Float, Freq: 1, 
    // BaroPressure|float, Freq: 2, 

    uint8_t expected[] = {  0xFA,
                            0xFF,
                            0xC0,
                            0x1C,
                            0x10, 0x20, 0xFF, 0xFF, 0x20, 0x10, 0x00, 0xC8, 0x80, 0x21, 0x00, 0x64, 0xC0, 0x22, 0x00, 0x64, 0x08, 0x10, 0x00, 0x01, 0x30, 0x10, 0x00, 0x02, 0xE0, 0x20, 0xFF, 0xFF, 
                            0x5B };

    XsensFrequencyConfig_t settings[] = {
        { .id = XDI_PACKET_COUNTER, .frequency = 0xFFFF },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_QUATERNION, XSENS_FLOAT_SINGLE, XSENS_COORD_ENU), .frequency = 200 },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_RATE_OF_TURN, XSENS_FLOAT_FIXED1220, XSENS_COORD_ENU), .frequency = 100 },
        { .id = XSENS_IDENTIFIER_FORMAT(XDI_MAGNETIC_FIELD, XSENS_FLOAT_FIXED1632, XSENS_COORD_ENU), .frequency = 100 },
        { .id = XDI_TEMPERATURE, .frequency = 1 },
        { .id = XDI_BARO_PRESSURE, .frequency = 2 },
        { .id = XDI_STATUS_WORD, .frequency = 0xFFFF },
    };

    xsens_mti_set_configuration( &test_imu, settings, XSENS_ARR_ELEM(settings) );

    TEST_ASSERT_EQUAL_HEX8_ARRAY( &expected, &outbound_cache, sizeof(expected) );
}

// Check configuration function doesn't generate outputs for stupid arguments
void test_set_configuration_empty( void )
{   
    // Lie about count with the null-ptr
    xsens_mti_set_configuration( &test_imu, NULL, 1 );

    // No bytes output
    TEST_ASSERT_EQUAL_INT( 0, outbound_cache_pos);

    // Test valid pointer but invalid count
    XsensFrequencyConfig_t settings[2] = { 0 };
    xsens_mti_set_configuration( &test_imu, settings, 0 );
    TEST_ASSERT_EQUAL_INT( 0, outbound_cache_pos);
}

void test_set_configuration_maximum( void )
{   
    // Generates packet with the maximum number of configuration fields
    //  these are empty/null values, intended to test argument/count handling
    XsensFrequencyConfig_t settings[32] = { 0 };
    xsens_mti_set_configuration( &test_imu, settings, 32 );

    uint8_t packet_overhead_bytes = 5;
    uint8_t payload_bytes = 32 * 4; // 4-bytes per configured field

    TEST_ASSERT_EQUAL_INT( packet_overhead_bytes+payload_bytes, outbound_cache_pos);
}

void test_set_configuration_too_many( void )
{   
    XsensFrequencyConfig_t settings[35] = { 0 };
    xsens_mti_set_configuration( &test_imu, settings, 34 );

    TEST_ASSERT_EQUAL_INT( 0, outbound_cache_pos);
}