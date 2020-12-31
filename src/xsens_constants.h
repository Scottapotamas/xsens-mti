#ifndef XSENS_CONSTANTS_H
#define XSENS_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

// Library Related Abstractions

// The basic concept of a valid decoded packet
typedef struct 
{
    uint8_t message_id;
    uint16_t length;
    uint8_t payload[2048];
} packet_buffer_t;


// Event flags sent to application-level code
typedef enum  {
    XSENS_EVT_PACKET_COUNT = 0,
    XSENS_EVT_TIME_FINE,
    XSENS_EVT_TIME_COARSE,

    XSENS_EVT_QUATERNION,
    XSENS_EVT_DELTA_Q,

    XSENS_EVT_ACCELERATION,
    XSENS_EVT_FREE_ACCELERATION,
    XSENS_EVT_DELTA_V,

    XSENS_EVT_RATE_OF_TURN,
    XSENS_EVT_MAGNETIC,
    XSENS_EVT_PRESSURE,
    XSENS_EVT_TEMPERATURE,

    XSENS_EVT_STATUS_WORD,
} EventFlag_t;

typedef enum {
    XSENS_EVT_TYPE_NONE = 0,
    XSENS_EVT_TYPE_U16,
    XSENS_EVT_TYPE_U32,
    XSENS_EVT_TYPE_FLOAT,
    XSENS_EVT_TYPE_FLOAT3,
    XSENS_EVT_TYPE_FLOAT4,
} EventDataType_t;

typedef struct {
    EventDataType_t type;
    union {
        uint16_t u2;      
        uint32_t u4;
        float    f4;
        float    f4x3[3];
        float    f4x4[4];
    } data;  
} EventData_t;

// Userspace callback to notify application layer code of a relevant event
// These events are accompanied by output-ready decoded data
typedef void (*callback_event_t)(EventFlag_t, EventData_t*);

// Callback to userspace serial write function
// Uses args for a uint8_t buffer of bytes, with a uint16_t size value
typedef void (*callback_data_out_t)(uint8_t*, uint16_t);

// Specific Values and Structures for xsens handling

#define PREAMBLE_BYTE 0xFA
#define ADDRESS_BYTE 0xFF

#define LENGTH_EXTENDED_MODE 0xFF
#define LENGTH_NONE 0x00

enum {
    ERROR_PERIOD_INVALID = 0x03,
    ERROR_MESSAGE_INVALID = 0x04,
    ERROR_TIMER_OVERFLOW = 0x30,
    ERROR_BAUDRATE = 0x20,
    ERROR_PARAMETER_INVALID = 0x21,
    ERROR_DEVICE = 0x28,
} ERRORCODE;

// Message Identifiers for MTi->HOST
enum {
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

// Message Identifiers for Host->MTi
enum {
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

enum  {
    MODE_WAKEUP = 0,
    MODE_CONFIG,
    MODE_MEASUREMENT,
} DEVICE_MODE;

// MData2 Field Identifiers
enum  {
    XDI_UTC_TIME = 0x1010,
    XDI_PACKET_COUNTER = 0x1020,
    XDI_SAMPLE_TIME_FINE = 0x1060,
    XDI_SAMPLE_TIME_COARSE = 0x1070,

    XDI_TEMPERATURE = 0x0810,

    XDI_QUATERNION = 0x2010,
    XDI_ROTATION_MATRIX = 0x2020,
    XDI_EULER_ANGLES = 0x2030,

    XDI_BARO_PRESSURE = 0x3010,

    XDI_DELTA_V = 0x4010,
    XDI_ACCELERATION = 0x4020,
    XDI_FREE_ACCELERATION = 0x4030,
    XDI_ACCELERATION_HIGH_RATE = 0x4040,

    XDI_ALTITUDE_ELLIPSOID = 0x5020,
    XDI_POSITION_ECEF = 0x5030,
    XDI_LAT_LON = 0x5040,
    XDI_GNSS_PVT_DATA = 0x7010,
    XDI_GNSS_SAT_INFO = 0x7020,
    XDI_GNSS_PVT_PULSE = 0x7030,

    XDI_RATE_OF_TURN = 0x8020,
    XDI_DELTA_Q = 0x8030,
    XDI_RATE_OF_TURN_HIGH_RATE = 0x8040,

    XDI_RAW_ACC_GYRO_MAG_TEMP = 0xA010,
    XDI_RAW_GYRO_TEMP = 0xA020,

    XDI_MAGNETIC_FIELD = 0xC020,
    
    XDI_VELOCITY_XYZ = 0xD010,

    XDI_STATUS_BYTE = 0xE010,
    XDI_STATUS_WORD = 0xE020,
    XDI_DEVICE_ID = 0xE080,
    XDI_LOCATION_ID = 0xE090,

} XDA_TYPE_IDENTIFIER;

// 32-bit status structure
typedef struct {
    unsigned int self_test: 1;
    unsigned int filter_valid: 1;
    unsigned int gnss_fix: 1;
    unsigned int no_rotation_status: 2;
    unsigned int representitive_motion: 1;
    unsigned int clock_bias_estimation: 1;
    unsigned int reserved_0: 1;
    unsigned int clip_acc_x: 1;
    unsigned int clip_acc_y: 1;
    unsigned int clip_acc_z: 1;
    unsigned int clip_gyro_x: 1;
    unsigned int clip_gyro_y: 1;
    unsigned int clip_gyro_z: 1;
    unsigned int clip_mag_x: 1;
    unsigned int clip_mag_y: 1;
    unsigned int clip_mag_z: 1;
    unsigned int reserved_1: 2;
    unsigned int clipping: 1;
    unsigned int reserved_2: 1;
    unsigned int sync_in_mark: 1;
    unsigned int sync_out_mark: 1;
    unsigned int filter_mode: 3;
    unsigned int has_gnss_pulse: 1;
    unsigned int rtk_status: 2;
    unsigned int reserved_3: 3;
} xdi_status32_t;

union XDI_STATUS32_UNION {
    uint32_t word;
    xdi_status32_t bitfield;
};

#ifdef __cplusplus
}
#endif

#endif 
//end XSENS_CONSTANTS_H