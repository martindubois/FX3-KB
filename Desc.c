
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Desc.c

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3usbconst.h>

#include "HID.h"

#include "Desc.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define CONFIG_INTERFACE  (  1 )
#define CONFIG_SIZE_byte  ( 34 )

#define BOS_DSCR_TYPE          ( 15 )
#define DEVICE_CAPB_DSCR_TYPE  ( 16 )
#define SS_USB_CAPB_TYPE       (  3 )
#define USB2_EXTN_CAPB_TYPE    (  2 )

const uint8_t Desc_BOS[] =
{
    5, // <--------------------- 1
    BOS_DSCR_TYPE,            // 2
    22, 0x00, // <============== 4   Length of this descriptor and all sub descriptors
    0x02,                     // 5   Number of device capability descriptors

    7, // <-------------------- 1  6
    DEVICE_CAPB_DSCR_TYPE,  //  2  7
    USB2_EXTN_CAPB_TYPE,    //  3  8
    0x00, 0x00, 0x00, 0x00, //  7 12

    10, // <------------------- 1 13
    DEVICE_CAPB_DSCR_TYPE,  //  2 14
    SS_USB_CAPB_TYPE,       //  3 15
    0x00,                   //  4 16
    0x0E, 0x00,             //  6 18
    0x03,                   //  7 19
    0x00,                   //  8 20
    0x00, 0x00,             // 10 22
};

// TODO  Desc  Define a constant for the HID descriptor type

const uint8_t Desc_Config_FS[ CONFIG_SIZE_byte ] =
{
    9, // <------------------------------------------ 1
    CY_U3P_USB_CONFIG_DESCR,                      //  2
    CONFIG_SIZE_byte, 0x00, // <===================== 4
    CONFIG_INTERFACE,                             //  5
    0x01,                                         //  6   Configuration number
    0x00,                                         //  7   COnfiguration string index
    0xa0,                                         //  8
    0x32,                                         //  9   Max power consumption of device (in 2mA unit) : 100mA

        9, // <------------------------------------- 1 10
        CY_U3P_USB_INTRFC_DESCR,                  // 2 11
        0x00,                                     // 3 12 Interface number
        0x00,                                     // 4 13 Alternate setting number
        0x01,                                     // 5 14 Number of end points
        0x03,                                     // 6 15 Interface class
        0x01,                                     // 7 16 Interface sub class
        0x01,                                     // 8 17 Interface protocol code
        0x00,                                     // 9 18 Interface descriptor string index

            9, // <--------------------------------- 1 19
            0x21,                                 // 2 20
            0x11, 0x01,                           // 4 22
            0x00,                                 // 5 23
            0x01,                                 // 6 24
            0x22,                                 // 7 25
            DESC_REPORT_KEYBOARD_SIZE_byte, 0x00, // 9 27

            7, // <--------------------------------- 1 28
            CY_U3P_USB_ENDPNT_DESCR,              // 2 29
            0x81,                                 // 3 30
            0x03,                                 // 4 31
            0x08, 0x00,                           // 6 33
            0x0a,                                 // 7 34
};

const uint8_t Desc_Config_HS_SS[ CONFIG_SIZE_byte ] =
{
    9, // <------------------------------------------ 1
    CY_U3P_USB_CONFIG_DESCR,                      //  2
    CONFIG_SIZE_byte, 0x00, // <===================== 4
    CONFIG_INTERFACE,                             //  5
    0x01,                                         //  6   Configuration number
    0x00,                                         //  7   COnfiguration string index
    0xa0,                                         //  8
    0xFA,                                         //  9   Max power consumption of device (in 2mA unit) : 500mA

        9, // <------------------------------------- 1 10
        CY_U3P_USB_INTRFC_DESCR,                  // 2 11
        0x00,                                     // 3 12 Interface number
        0x00,                                     // 4 13 Alternate setting number
        0x01,                                     // 5 14 Number of end points
        0x03,                                     // 6 15 Interface class
        0x01,                                     // 7 16 Interface sub class
        0x01,                                     // 8 17 Interface protocol code
        0x00,                                     // 9 18 Interface descriptor string index

            9, // <--------------------------------- 1 19
            0x21,                                 // 2 20
            0x11, 0x01,                           // 4 22
            0x00,                                 // 5 23
            0x01,                                 // 6 24
            0x22,                                 // 7 25
            DESC_REPORT_KEYBOARD_SIZE_byte, 0x00, // 9 27

            7, // <--------------------------------- 1 28
            CY_U3P_USB_ENDPNT_DESCR,              // 2 29
            0x81,                                 // 3 30
            0x03,                                 // 4 31
            0x08, 0x00,                           // 6 33
            0x0a,                                 // 7 34
};

const uint8_t Desc_Device_HS[] =
{
    18,    // <-------------------- 1
    CY_U3P_USB_DEVICE_DESCR,    //  2
    0x00, 0x02,                 //  4 USB 2.0 - Microsoft indicate 1.1 (0x10 0x01)
    0x00,                       //  5
    0x00,                       //  6
    0x00,                       //  7
    0x40,                       //  8 Max. packet size on EP0 = 64 bytes - Microsoft indicate 8 bytes
    0xB4, 0x04,                 // 10 Vendor ID
    0x21, 0x00,                 // 12 Product ID
    0x00, 0x00,                 // 14 Device release number
    0x01,                       // 15 Manufacture string index
    0x02,                       // 16 Product string index
    0x00,                       // 17 Serial number string index
    0x01,                       // 18 Number of configurations
};

const uint8_t Desc_Device_SS[] =
{
    19, // <-------------------- 1
    CY_U3P_USB_DEVICE_DESCR, //  2
    0x00, 0x03,              //  4 USB 3.0
    0x00,                    //  5
    0x00,                    //  6
    0x00,                    //  7
    0x09,                    //  8 Max. packet size on EP0 = 1024 bytes
    0xB4, 0x04,              // 10 Vendor ID
    0x21, 0x00,              // 12 Product ID
    0x00, 0x00,              // 14 Device release number
    0x01,                    // 15 Manufacture string index
    0x02,                    // 16 Product string index
    0x00,                    // 17 Serial number string index
    0x01,                    // 18 Number of configurations
};

const uint8_t Desc_Device_Qual[] =
{
    10, // <--------------------- 1
    CY_U3P_USB_DEVQUAL_DESCR, //  2
    0x00, 0x02,               //  4
    0x00,                     //  5
    0x00,                     //  6
    0x00,                     //  7
    0x40,                     //  8
    0x01,                     //  9
    0x00,                     // 10
};

const uint8_t Desc_LangId[] =
{
    4, // <-------------------- 1
    CY_U3P_USB_STRING_DESCR, // 2
    0x09, 0x04               // 4
};

const uint8_t Desc_Report_Keyboard[ DESC_REPORT_KEYBOARD_SIZE_byte ] __attribute__ ( ( aligned ( 32 ) ) ) =
{
    HID_TAG_USAGE_PAGE, HID_UP_GENERIC_DESKTOP,   //   2 Keyboard
    HID_TAG_USAGE     , 0x06                  ,   //   4              Keyboard
    HID_TAG_COLLECTION, 0x01                  ,   //   6              Application
        HID_TAG_REPORT_ID, DESC_REPORT_KEY    ,   //   8 Byte 0

        HID_TAG_USAGE_PAGE     , HID_UP_KEYBOARD, //  10
        HID_TAG_USAGE_MINIMUM  , 0xe0           , //  12              Keyboard Left Control
        HID_TAG_USAGE_MAXIMUM  , 0xe7           , //  14              Keyboard Right GUI
        HID_TAG_LOGICAL_MINIMUM, 0x00           , //  16
        HID_TAG_LOGICAL_MAXIMUM, 0x01           , //  18
            HID_TAG_REPORT_COUNT   , 0x08,        //  20
            HID_TAG_REPORT_SIZE_bit, 0x01,        //  22
            HID_TAG_INPUT          , 0x02,        //  24 Byte 1       Data, Var, Abs, NWrp, Lin, Pref, NNul, Bit

            HID_TAG_REPORT_COUNT   , 0x08,        //  26
            HID_TAG_REPORT_SIZE_bit, 0x01,        //  28
            HID_TAG_INPUT          , 0x01,        //  30 Byte 2       Cnst, Ary, Abs

        HID_TAG_USAGE_PAGE   , HID_UP_LED,        //  32
        HID_TAG_USAGE_MINIMUM, 0x01      ,        //  34              Num Lock
        HID_TAG_USAGE_MAXIMUM, 0x03      ,        //  36              Scroll Lock
            HID_TAG_REPORT_COUNT   , 0x03,        //  38
            HID_TAG_REPORT_SIZE_bit, 0x01,        //  40
            HID_TAG_OUTPUT         , 0x02,        //  42              Data, Var, Abs, NWrp, Lin, Pref, NNul, NVol, Bit

            HID_TAG_REPORT_COUNT   , 0x01,        //  44
            HID_TAG_REPORT_SIZE_bit, 0x05,        //  46
            HID_TAG_OUTPUT         , 0x01,        //  48              Cnst, Ary, Abs, NWrp, Lin, Pref, NNul, NVol, Bit

        HID_TAG_USAGE_PAGE     , HID_UP_KEYBOARD, //  50
        HID_TAG_USAGE_MINIMUM  , 0x00           , //  52              undefined
        0x2a, 0xff, 0x00,                         //  55              Usage Maximum
        HID_TAG_LOGICAL_MINIMUM, 0x00           , //  57
        0x26, 0xff, 0x00,                         //  60              Logical Maximum (255)
            HID_TAG_REPORT_COUNT   , 0x05,        //  62
            HID_TAG_REPORT_SIZE_bit, 0x08,        //  64
            HID_TAG_INPUT          , 0x00,        //  66 Byte 3 to 7  Data, Ary, Abs
    HID_TAG_END,                                  //  67

    HID_TAG_USAGE_PAGE, HID_UP_GENERIC_DESKTOP,   //  69 Control
    HID_TAG_USAGE     , 0x80                  ,   //  71              System Control
    HID_TAG_COLLECTION, 0x01                  ,   //  73              Application
        HID_TAG_REPORT_ID, DESC_REPORT_CONTROL,   //  75 Byte 0

        HID_TAG_REPORT_COUNT   , 0x08,            //  77
        HID_TAG_REPORT_SIZE_bit, 0x01,            //  79
        HID_TAG_USAGE          , 0x81,            //  81              System Power Down
        HID_TAG_USAGE          , 0x82,            //  83              System Sleep
        HID_TAG_USAGE          , 0x83,            //  85              System Wakeup
        HID_TAG_USAGE          , 0x84,            //  87              System Display Dual
        HID_TAG_USAGE          , 0x8e,            //  89              System Cold Restart
        HID_TAG_USAGE          , 0x8f,            //  91              System Warm Restart
        HID_TAG_USAGE          , 0xa7,            //  93              System Speaker Mute
        HID_TAG_USAGE          , 0xa8,            //  95              System Hybernate
        HID_TAG_LOGICAL_MINIMUM, 0x00,            //  97
        HID_TAG_LOGICAL_MAXIMUM, 0x01,            //  99
        HID_TAG_INPUT          , 0x02,            // 101 Byte 1       Data, Var, Abs

        HID_TAG_REPORT_COUNT, 0x30,               // 103
        HID_TAG_INPUT       , 0x03,               // 105 Byte 2 to 7  Cnst, Var, Abs

    HID_TAG_END,                                  // 106
};

const uint8_t Desc_String_Manufacturer[] =
{
    8, // <-------------------- 1
    CY_U3P_USB_STRING_DESCR, // 2
    'K', 0x00,               // 4
    'M', 0x00,               // 6
    'S', 0x00,               // 8
};

const uint8_t Desc_String_Product[] =
{
    14,    // <-------------------- 1
    CY_U3P_USB_STRING_DESCR,    //  2
    'F', 0x00,                  //  4
    'X', 0x00,                  //  6
    '3', 0x00,                  //  8
    '-', 0x00,                  // 10
    'K', 0x00,                  // 12
    'B', 0x00,                  // 14
};
