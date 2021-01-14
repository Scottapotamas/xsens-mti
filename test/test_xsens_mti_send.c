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