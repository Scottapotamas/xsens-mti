#include "unity.h"
#include <string.h>

// MODULE UNDER TEST
#include "xsens_mti.h"
#include "xsens_mti_private.h"

// DEFINITIONS 
 
// PRIVATE TYPES
 
// PRIVATE DATA
interface_t test_imu = { 0 };


// PRIVATE FUNCTIONS

void mock_output_function( uint8_t *buffer, uint16_t size );
void mock_event_function( EventFlags_t event );


void mock_output_function( uint8_t *buffer, uint16_t size )
{
    printf("Output of %d bytes\n", size);
}

void mock_event_function( EventFlags_t event )
{
    printf("Notified of evt: %d\n", event);
}



// SETUP, TEARDOWN
void setUp(void)
{
    test_imu.output_cb = &mock_output_function;
    test_imu.event_cb = &mock_event_function;
    init( &test_imu );
}
 
void tearDown(void)
{

}

// TESTS
void test_init_valid( void )
{   
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}

void test_init_invalid( void )
{   
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");

}

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
        parse( &test_imu, test_packet[i]);
    }

    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
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

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");

}