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


void mock_output_function( uint8_t *buffer, uint16_t size )
{
    printf("Output of %d bytes\n", size);
}

void mock_event_function( XsensEventFlag_t event, XsensEventData_t *data )
{
    printf("Notified of evt: %d\n", event);
}

// SETUP, TEARDOWN
void setUp(void)
{
    memset( &test_imu, 0, sizeof(test_imu) );
    test_imu.output_cb = &mock_output_function;
    test_imu.event_cb = &mock_event_function;
}
 
void tearDown(void)
{

}

// TESTS
void test_parse_basic( void )
{   
    // Simulate the IMU sending a GoToConfigAck packet
        // Preamble 0xFA
        // Address 0xFF
        // MID - 0x31
        // Size is 0x00
        // CRC is 0xD0

    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x31, 
                                0x00, 
                                0xD0 };

    for( uint16_t i = 0; i < sizeof(test_packet); i++ )
    {
        xsens_mti_parse( &test_imu, test_packet[i]);
    }

    TEST_IGNORE();
}

void test_parse_buffer( void )
{   
    // Check that the buffer helper function behaves the same
    // as the basic test above
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x31, 
                                0x00, 
                                0xD0 };

    xsens_mti_parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();

}

/*
// Quick function to calculate the correct CRC for a message
// As some golden sample payloads didn't have one recorded...
void test_calculate_crc( void )
{
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x31, 
                                0x00,
                             };

    uint8_t checksum = 0;

    for (int i = 1; i < sizeof(test_packet); i++)
    {
	    checksum -= test_packet[i];
    }

    printf("Output CRC should be 0x%X\n", (checksum & 0xff));
}
*/