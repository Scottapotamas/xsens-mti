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

// PRIVATE DATA
xsens_interface_t if_ground_truth = { 0 };

xsens_interface_t if_manual = { 
    .state = 0,
    .packet = {0},
    .payload_pos = 0,
    .crc = 0,
    .event_cb = &mock_event_function,
    .output_cb = &mock_output_function, 
};

xsens_interface_t if_rx_macro = XSENS_INTERFACE_RX( &mock_event_function );
xsens_interface_t if_rx_tx_macro = XSENS_INTERFACE_RX_TX( &mock_event_function, &mock_output_function );

// SETUP, TEARDOWN
void setUp(void)
{
    memset( &if_ground_truth, 0, sizeof(if_ground_truth) );
}
 
void tearDown(void)
{

}

// TESTS

// Check a compile-time and run-time structure setup match
void test_manual_macro( void )
{   
    if_ground_truth.event_cb = &mock_event_function;
    if_ground_truth.output_cb = &mock_output_function;

    TEST_ASSERT_EQUAL_INT8_ARRAY( &if_manual, &if_ground_truth, sizeof(xsens_interface_t));
}

// Check read-only macro matches
void test_rx_macro( void )
{   
    if_ground_truth.event_cb = &mock_event_function;

    TEST_ASSERT_EQUAL_INT8_ARRAY( &if_rx_macro, &if_ground_truth, sizeof(xsens_interface_t));
}

// Check read-write macro matches
void test_rx_tx_macro( void )
{
    if_ground_truth.event_cb = &mock_event_function;
    if_ground_truth.output_cb = &mock_output_function;

    TEST_ASSERT_EQUAL_INT8_ARRAY( &if_rx_tx_macro, &if_ground_truth, sizeof(xsens_interface_t));
}