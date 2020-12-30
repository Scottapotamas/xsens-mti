#include "unity.h"
#include <string.h>

// MODULE UNDER TEST
#include "xsens_mti.h"
#include "xsens_mti_private.h"

// DEFINITIONS 
 
// PRIVATE TYPES
 
// PRIVATE DATA


// PRIVATE FUNCTIONS


// SETUP, TEARDOWN
void setUp(void)
{

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
        parse(test_packet[i]);
    }

    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}

// void test_parse_buffer( void )
// {   
//     // Check that the buffer helper function behaves the same
//     // as the basic test above
//     uint8_t test_packet[] = {   0xFA, 
//                                 0xFF, 
//                                 0x31, 
//                                 0x00, 
//                                 0xD0 };

//     parse_buffer(&test_packet, sizeof(test_packet));
    
//     TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");

// }