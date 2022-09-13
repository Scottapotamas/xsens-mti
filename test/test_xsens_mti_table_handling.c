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
void mock_handler_a( xsens_packet_buffer_t *packet )
{

}

void mock_handler_b( xsens_packet_buffer_t *packet )
{

}

void mock_handler_c( xsens_packet_buffer_t *packet )
{

}

// PRIVATE DATA
#define MOCK_TABLE_COUNT 8

message_handler_ref_t mock_handler_table[ ] = {
    { .id = 1, .handler_fn = NULL },
    { .id = 2, .handler_fn = NULL },
    { .id = 3, .handler_fn = &mock_handler_a },
    { .id = 4, .handler_fn = &mock_handler_b },
    { .id = 5, .handler_fn = NULL },
    { .id = 100, .handler_fn = &mock_handler_c },
    { .id = 20, .handler_fn = NULL },
    { .id = 6, .handler_fn = NULL },
};


// SETUP, TEARDOWN
void setUp(void)
{

}
 
void tearDown(void)
{

}

// TESTS

void test_table_length_elements( void )
{
    // This approach is used in xsens_mti.c when calling xsens_mti_find_handler_entry
    // TODO: consider making it into a macro to test what's actually run?
    uint8_t table_length = sizeof( mock_handler_table ) / sizeof( message_handler_ref_t );
    TEST_ASSERT_EQUAL_INT( MOCK_TABLE_COUNT, table_length );
}

// Attempting to find a missing ID should return null 
void test_find_handler_invalid_id( void ) 
{
    message_handler_ref_t* table_entry = 0;

    table_entry = xsens_mti_find_handler_entry( 11, mock_handler_table, MOCK_TABLE_COUNT );

    TEST_ASSERT_EQUAL_PTR( NULL, table_entry );
}

// Finding an ID in the table with placeholder (NULL) should still return the entry
void test_find_handler_id_with_null( void ) 
{
    message_handler_ref_t* table_entry = 0;

    table_entry = xsens_mti_find_handler_entry( 2, mock_handler_table, MOCK_TABLE_COUNT );

    TEST_ASSERT_EQUAL_PTR( &mock_handler_table[1], table_entry );
    TEST_ASSERT_NULL( table_entry->handler_fn );
}

// Finding an ID in the table with pointer should point to the right place
void test_find_handler( void ) 
{
    message_handler_ref_t* table_entry = 0;

    table_entry = xsens_mti_find_handler_entry( 3, mock_handler_table, MOCK_TABLE_COUNT );
    TEST_ASSERT_EQUAL_PTR( &mock_handler_table[2], table_entry );
    TEST_ASSERT_EQUAL_PTR( &mock_handler_a, table_entry->handler_fn );
}
