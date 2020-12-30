#ifndef XENS_MTI_H
#define XENS_MTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

typedef enum  {
    EVT_BLAH = 0,
} EventFlags_t;

typedef void (*callback_event_t)(EventFlags_t);

typedef void (*callback_data_out_t)(uint8_t*, uint16_t);

typedef enum {
    PARSER_PREAMBLE = 0,
    PARSER_ADDRESS,
    PARSER_MESSAGE_ID,
    PARSER_LENGTH,
    PARSER_LENGTH_EXTENDED_B1,
    PARSER_LENGTH_EXTENDED_B2,
    PARSER_PAYLOAD,
    PARSER_CRC,
} parser_state_t;

typedef struct 
{
    parser_state_t state;

    uint8_t message_id;
    uint16_t length;
    uint8_t payload[2048];
    uint16_t payload_pos;
    uint8_t crc;
} packet_buffer_t;

typedef struct
{
    packet_buffer_t state;
    callback_event_t event_cb;
    callback_data_out_t output_cb;
} interface_t;



void init( interface_t *interface );

void parse_buffer( interface_t *interface, uint8_t *buffer, uint16_t size );
void parse( interface_t *interface, uint8_t byte );
void reset_parser( interface_t *interface );



#define PREAMBLE_BYTE 0xFA
#define ADDRESS_BYTE 0xFF

#define LENGTH_EXTENDED_MODE 0xFF
#define LENGTH_NONE 0x00

enum  {
    WAKEUP = 0x3E,
    GOTOCONFIGACK = 0x31,
    GOTOMEASUREMENTACK = 0x11,
    RESETACK = 0x41,
    DEVICEID = 0x01,
    PRODUCTCODE = 0x1D,
    HARDWAREVERSION = 0x1F, 
    FIRMWAREREV = 0x13, 
    SELFTESTRESULTS = 0x25, 
    ERROR = 0x42, 
    WARNING = 0x43, 
    CONFIGURATION = 0x0D, 
    MTDATA2 = 0x36, 
    REQLATLONALTACK = 0x6F, 
    SETLATLONALTACK = 0x6F, 
    REQFILTERPROFILEACK = 0x65, 
    REQGNSSPLATFORMACK = 0x77, 
    REQGNSSLEVERARMACK = 0x69, 
    SETGNSSLEVERARMACK = 0x69, 
    UTCTIME = 0x61, 
    ICCCOMMANDACK = 0x75, 
    SETINITIALHEADINGACK = 0xD7, 
    FORWARDGNSSDATAACK = 0xE3, 
} MESSAGE_ID_INBOUND;

enum  {
    WAKEUPACK = 0x3F,
    GOTOCONFIG = 0x30,
    GOTOMEASUREMENT = 0x10,
    RESET = 0x40,
    REQDID = 0x00,
    REQPRODUCTCODE = 0x1C,
    REQHARDWAREVERSION = 0x1E,
    REQFWREV = 0x12,
    RUNSELFTEST = 0x24,
    REQBAUDRATE = 0x18,
    SETBAUDRATE = 0x18,
    REQOPTIONFLAGS = 0x48,
    SETOPTIONFLAGS = 0x48,
    REQLOCATIONID = 0x84,
    SETLOCATIONID = 0x84,
    RESTOREFACTORYDEF = 0x0E,
    REQTRANSMITDELAY = 0xDC,
    SETTRANSMITDELAY = 0xDC,
    REQCONFIGURATION = 0x0C,
    REQSTRINGOUTPUTTYPE = 0x8E,
    SETSTRINGOUTPUTTYPE = 0x8E,
    REQPERIOD = 0x04,
    SETPERIOD = 0x04,
    REQOUTPUTSKIPFACTOR = 0xD4,
    SETOUTPUTSKIPFACTOR = 0xD4,
    REQALIGNMENTROTATION = 0xEC,
    SETALIGNMENTROTATION = 0xEC,
    REQEXTOUTPUTMODE = 0x86,
    SETEXTOUTPUTMODE = 0x86,
    REQSETPORTCONFIG = 0x8C,
    REQSETCANOUTPUTCONFIG = 0xE8,
    REQSETCANCONFIG = 0xE6,
    REQDATA = 0x34,
    REQLATLONALT = 0x6E,
    SETLATLONALT = 0x6E,
    REQAVAILABLEFILTERPROFILES = 0x62,
    REQFILTERPROFILE = 0x64,
    SETFILTERPROFILE = 0x64,
    REQGNSSPLATFORM = 0x76,
    SETGNSSPLATFORM = 0x76,
    SETGNSSLEVERARM = 0x68,
    RESETORIENTATION = 0xA4,
    SETNOROTATION = 0x22,
    REQUTCTIME = 0x60,
    SETUTCTIME = 0x60,
    ADJUSTUTCTIME = 0xA8,
    ICCCOMMAND = 0x74,
    SETINITIALHEADING = 0xD6,
    FORWARDGNSSDATA = 0xE2,
} MESSAGE_ID_OUTBOUND;

typedef struct
{
    uint8_t value;
    uint8_t id;
    uint8_t length;
    uint8_t direction;
} mid_helper_t;


enum  {
    MODE_WAKEUP = 0,
    MODE_CONFIG,
    MODE_MEASUREMENT,
} DEVICE_MODE;

#ifdef __cplusplus
}
#endif

#endif //end XENS_MTI_H