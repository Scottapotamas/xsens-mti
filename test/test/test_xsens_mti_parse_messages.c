// These tests cover inbound message IDs
// Tests written from real captured packets from MTi-300

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
    memset( &test_imu, 0, sizeof(test_imu) );
    test_imu.output_cb = &mock_output_function;
    test_imu.event_cb = &mock_event_function;
    init( &test_imu );
}
 
void tearDown(void)
{

}

// TESTS
void test_parse_GoToConfigAck( void )
{   
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x31, 
                                0x00,
                                0xD0 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}

void test_parse_StringOutputType( void )
{   
    // <no data (possible: StringOutputType, 2 bytes)>
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x8F, 
                                0x00,
                                0x72 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}

void test_parse_OutputConfiguration( void )
{   
    //{(PacketCounter, Freq: 65535), (SampleTimeFine, Freq: 65535)}
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0xC1, 
                                0x08,
                                0x10, 0x20, 0xFF, 0xFF, 0x10, 0x60, 0xFF, 0xFF, 
                                0x9C };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");

}

void test_parse_InitMTResults( void )
{   
    //DeviceID: 037003F8
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x03, 
                                0x04,
                                0x03, 0x70, 0x03, 0xF8, 
                                0x8C };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}

void test_parse_Configuration( void )
{
    // DeviceID: 037003F8, 
    // Sample Period: 1152, 
    // OutputSkipFactor: 0, 
    // SyncInMode: 0, 
    // SyncInSkipFactor: 0, 
    // SyncInOffset: 0, (Year: 0, Month: 0, Day: 0), (Hour: 0, Min: 0, Sec: 0, Ms: 0), 
    // reserved: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00, 
    // reserved: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00, 
    // numDevices: 1, 
    // DeviceID: 037003F8, 
    // MTDataLength: 0, 
    // OutputMode: 0, 
    // OutputSettings: 1, 
    // reserved: 00 27 01 08 02 49 05 01
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x0D, 
                                0x76,
                                0x03, 0x70, 0x03, 0xF8, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x70, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x27, 0x01, 0x08, 0x02, 0x49, 0x05, 0x01,
                                0x9B };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");

}


void test_parse_FirmareRev( void )
{   
    //(major: 1, minor: 8, revision: 2, buildnr: 37, svnrev: 70964)
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x13, 
                                0x0B,
                                0x01, 0x08, 0x02, 0x00, 0x00, 0x00, 0x25, 0x00, 0x01, 0x15, 0x34, 
                                0x69 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}

void test_parse_GotoMeasurementAck( void )
{   
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x11, 
                                0x00,
                                0x8C };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_ASSERT_TRUE_MESSAGE(0, "Missing test");
}