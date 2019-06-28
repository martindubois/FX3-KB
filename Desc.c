
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Desc.c

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3usbconst.h>

#include "Desc.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#ifdef _CONSUMER_CONTROL_
	#define CONFIG_INTERFACE  (    2 )
	#define CONFIG_SIZE_byte  ( 0x3b )
#else
	#define CONFIG_INTERFACE  (    1 )
	#define CONFIG_SIZE_byte  ( 0x22 )
#endif

#define BOS_DSCR_TYPE          ( 15 )
#define DEVICE_CAPB_DSCR_TYPE  ( 16 )
#define SS_USB_CAPB_TYPE       (  3 )
#define USB2_EXTN_CAPB_TYPE    (  2 )

const uint8_t Desc_BOS[] =
{
	0x05,
	BOS_DSCR_TYPE,
	0x16, 0x00, // Length of this descriptor and all sub descriptors
	0x02,       // Number of device capability descriptors

	0x07,
	DEVICE_CAPB_DSCR_TYPE,
	USB2_EXTN_CAPB_TYPE,
	0x00, 0x00, 0x00, 0x00,

	0x0A,
	DEVICE_CAPB_DSCR_TYPE,
	SS_USB_CAPB_TYPE,
	0x00,
	0x0E, 0x00,
	0x03,
	0x00,
	0x00, 0x00,
};

const uint8_t Desc_Config_FS[ CONFIG_SIZE_byte ] =
{
	0x09,
	CY_U3P_USB_CONFIG_DESCR,
	CONFIG_SIZE_byte, 0x00,
	CONFIG_INTERFACE,
	0x01, // Configuration number
	0x00, // COnfiguration string index
	0xa0,
	0x32, // Max power consumption of device (in 2mA unit) : 100mA

        0x09,
        CY_U3P_USB_INTRFC_DESCR,
        0x00, // Interface number
        0x00, // Alternate setting number
        0x01, // Number of end points
        0x03, // Interface class
        0x01, // Interface sub class
        0x01, // Interface protocol code
        0x00, // Interface descriptor string index

			0x09,
			0x21,
			0x11, 0x01,
			0x00,
			0x01,
			0x22,
			DESC_REPORT_KEYBOARD_SIZE_byte, 0x00,

			0x07,
			CY_U3P_USB_ENDPNT_DESCR,
            0x81,
            0x03,
            0x08, 0x00,
            0x0a,

	#ifdef _CONSUMER_CONTROL_
        0x09,
        CY_U3P_USB_INTRFC_DESCR,
        0x01,
        0x00,
        0x01,
        0x03,
        0x00,
        0x00,
        0x00,

        	0x09,
        	0x21,
        	0x11, 0x01,
        	0x00,
        	0x01,
        	0x22,
        	DESC_REPORT_CONSUMER_CONTROL_SIZE_byte, 0x00,

        	0x07,
        	CY_U3P_USB_ENDPNT_DESCR,
        	0x82,
        	0x03,
        	0x08, 0x00,
        	0x0a,
	#endif
};

const uint8_t Desc_Config_HS[ CONFIG_SIZE_byte ] =
{
    0x09,
    CY_U3P_USB_CONFIG_DESCR,
	CONFIG_SIZE_byte, 0x00,
	CONFIG_INTERFACE,
    0x01,       // Configuration number
    0x00,       // COnfiguration string index
    0xa0,
    0xFA,       // Max power consumption of device (in 2mA unit) : 500mA

        0x09,
        CY_U3P_USB_INTRFC_DESCR,
        0x00, // Interface number
        0x00, // Alternate setting number
        0x01, // Number of end points
        0x03, // Interface class
        0x01, // Interface sub class
        0x01, // Interface protocol code
        0x00, // Interface descriptor string index

			0x09,
			0x21,
			0x11, 0x01,
			0x00,
			0x01,
			0x22,
			DESC_REPORT_KEYBOARD_SIZE_byte, 0x00,

			0x07,
			CY_U3P_USB_ENDPNT_DESCR,
            0x81,
            0x03,
            0x08, 0x00,
            0x0a,

	#ifdef _CONSUMER_CONTROL_
        0x09,
        CY_U3P_USB_INTRFC_DESCR,
        0x01,
        0x00,
        0x01,
        0x03,
        0x00,
        0x00,
        0x00,

        	0x09,
        	0x21,
        	0x11, 0x01,
        	0x00,
        	0x01,
        	0x22,
        	DESC_REPORT_CONSUMER_CONTROL_SIZE_byte, 0x00,

        	0x07,
        	CY_U3P_USB_ENDPNT_DESCR,
        	0x82,
        	0x03,
        	0x08, 0x00,
        	0x0a,
	#endif
};

const uint8_t Desc_Config_SS[ CONFIG_SIZE_byte ] =
{
	0x09,
	CY_U3P_USB_CONFIG_DESCR,
	CONFIG_SIZE_byte, 0x00,
	CONFIG_INTERFACE,
	0x01,       // Configuration number
	0x00,       // COnfiguration string index
	0xa0,
	0xFA,       // Max power consumption of device (in 2mA unit) : 500mA

		0x09,
		CY_U3P_USB_INTRFC_DESCR,
		0x00, // Interface number
		0x00, // Alternate setting number
		0x01, // Number of end points
		0x03, // Interface class
		0x01, // Interface sub class
		0x01, // Interface protocol code
		0x00, // Interface descriptor string index

			0x09,
			0x21,
			0x11, 0x01,
			0x00,
			0x01,
			0x22,
			DESC_REPORT_KEYBOARD_SIZE_byte, 0x00,

			0x07,
			CY_U3P_USB_ENDPNT_DESCR,
			0x81,
			0x03,
			0x08, 0x00,
			0x0a,

	#ifdef _CONSUMER_CONTROL_
		0x09,
		CY_U3P_USB_INTRFC_DESCR,
		0x01,
		0x00,
		0x01,
		0x03,
		0x00,
		0x00,
		0x00,

			0x09,
			0x21,
			0x11, 0x01,
			0x00,
			0x01,
			0x22,
			DESC_REPORT_CONSUMER_CONTROL_SIZE_byte, 0x00,

			0x07,
			CY_U3P_USB_ENDPNT_DESCR,
			0x82,
			0x03,
			0x08, 0x00,
			0x0a,
	#endif
};

const uint8_t Desc_Device_HS[] =
{
	0x12,
	CY_U3P_USB_DEVICE_DESCR,
	0x00, 0x02, // USB 2.0 - Microsoft indicate 1.1 (0x10 0x01)
	0x00,
	0x00,
	0x00,
	0x40,       // Max. packet size on EP0 = 64 bytes - Microsoft indicate 8 bytes
	0xB4, 0x04, // Vendor ID
	0x21, 0x00, // Product ID
	0x00, 0x00, // Device release number
	0x01,       // Manufacture string index
	0x02,       // Product string index
	0x00,       // Serial number string index
	0x01,       // Number of configurations
};

const uint8_t Desc_Device_SS[] =
{
	0x12,
	CY_U3P_USB_DEVICE_DESCR,
	0x00, 0x03, // USB 3.0
	0x00,
	0x00,
	0x00,
	0x09,       // Max. packet size on EP0 = 1024 bytes
	0xB4, 0x04, // Vendor ID
	0x21, 0x00, // Product ID
	0x00, 0x00, // Device release number
	0x01,       // Manufacture string index
	0x02,       // Product string index
	0x00,       // Serial number string index
	0x01,       // Number of configurations
};

const uint8_t Desc_Device_Qual[] =
{
	0x0A,
	CY_U3P_USB_DEVQUAL_DESCR,
	0x00, 0x02,
	0x00,
	0x00,
	0x00,
	0x40,
	0x01,
	0x00,
};

const uint8_t Desc_LangId[] =
{
	0x04,
	CY_U3P_USB_STRING_DESCR,
	0x09, 0x04
};

#ifdef _CONSUMER_CONTROL_

	const uint8_t Desc_Report_Consumer_Control[ DESC_REPORT_CONSUMER_CONTROL_SIZE_byte ] __attribute__ ( ( aligned ( 32 ) ) ) =
	{
		0x05, 0x0c, // Usage Page (Consumer Devices)
		0x09, 0x01, // Usage (Consumer Control)
		0xa1, 0x0a, // Collection (Application)
			0x85, 0x01,                   // Report ID (1)
			0x05, 0x0c,                   // Usage Page (Consumer Devices)
			0x19, 0x00,                   // Usage Minimum (Undefined)
			0x2a, 0xff, 0x03,             // Usage Maximum
			0x95, 0x01,                   // Report Count (1)
			0x75, 0x10,                   // Report Size (16)
			0x15, 0x00,                   // Logical Minimum (0)
			0x27, 0xff, 0x03, 0x00, 0x00, // Logical Maximum (1023)
			0x81, 0x00,                   // Input (Data, Ary, Abs)
			0x05, 0x07,                   // Usage Page (Keyboard/Keypad)
			0x19, 0x00,                   // Usage Minimum (Undefined)
			0x29, 0xff,                   // Usage Maximum
			0x75, 0x08,                   // Report Size (8)
			0x26, 0xff, 0x00,             // Logicial Maximum (255)
			0x81, 0x00,                   // Input (Data, Ary, Abs)
			0x81, 0x01,                   // Input (Cnst, Ary, Abs)
			0x06, 0x00, 0xff,             // Usage Page (Vendor-Defined 1)
			0x0a, 0x03, 0xfe,             // Usage (Vendor-Defined 65027)
			0x0a, 0x04, 0xfe,             // Usage (Vendor-Defined 65028)
			0x95, 0x02,                   // Report Count (2)
			0x75, 0x01,                   // Report Size (1)
			0x25, 0x01,                   // Logical Maximum (1)
			0x81, 0x02,                   // Input (Data, Var, Abs, NWrp, Lin, Pref, NNul, Bit)
			0x0a, 0x05, 0xff,             // Usage (Vendor-Defined 65285)
			0x95, 0x01,                   // Report Count (1)
			0x75, 0x05,                   // Report Size (5)
			0x25, 0x1f,                   // Logical Maximum (31)
			0x81, 0x02,                   // Input (Data, Var, Abs, NWrp, Lin, Pref, NNul, Bit)
			0x75, 0x05,                   // Report Size (5)
			0x81, 0x01,                   // Input (Cnst, Ary, Abs)
			0x0a, 0x02, 0xff,             // Usage (Vendor-Defined 65282)
			0x26, 0xff, 0x00,             // Logical Maximum (255)
			0x75, 0x08,                   // Report Size (8)
			0x81, 0x02,                   // Input (Data, Var, Abs, NWrp, Lin, Pref, NNul, Bit)
		0x0c,       // End Collection
		0x05, 0x01, // Usage Page (Generic Desktop)
		0x09, 0x08, // Usage (System Control)
		0x0a, 0x01, // Collection
			0x85, 0x03,       // Report ID (3)
			0x19, 0x00,       // Usage Minimum (Undefined)
			0x29, 0xff,       // Usage Maximum
			0x15, 0x00,       // Logicial Minimum (0)
			0x26, 0xff, 0x00, // Logical Maximum (255)
			0x81, 0x00,       // Input (Data, Ary, Abs)
		0xc0, // End Collection
	};

#endif

const uint8_t Desc_Report_Keyboard[ DESC_REPORT_KEYBOARD_SIZE_byte ] __attribute__ ( ( aligned ( 32 ) ) ) =
{
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
    	0x05, 0x07,       // Usage Page (Keyboard/Keypad)
    	0x19, 0xe0,       // Usage Minimum (Keyboard Left Control)
    	0x29, 0xe7,       // Usage Maximum (Keyboard Right GUI)
    	0x15, 0x00,       // Logical Minimum (0)
    	0x25, 0x01,       // Logical Maximum (1)
    	0x95, 0x08,       // Report Count (8)
    	0x75, 0x01,       // Report Size (1)
    	0x81, 0x02,       // Input (Data, Var, Abs, NWrp, Lin, Pref, NNul, Bit)
    	0x95, 0x08,       // Report Count (8)
    	0x75, 0x01,       // Report Size (1)
    	0x81, 0x01,       // Input (Cnst, Ary, Abs)
    	0x05, 0x08,       // Usage Page (LEDs)
    	0x19, 0x01,       // Usage Minimum (Num Lock)
    	0x29, 0x03,       // Usage Maximum (Scroll Lock)
    	0x95, 0x03,       // Report Count (3)
    	0x75, 0x01,       // Report Size (1)
    	0x91, 0x02,       // Output (Data, Var, Abs, NWrp, Lin, Pref, NNul, NVol, Bit)
    	0x95, 0x01,       // Report Count (1)
    	0x75, 0x05, 	  // Report Size (5)
    	0x91, 0x01, 	  // Output (Cnst, Ary, Abs, NWrp, Lin, Pref, NNul, NVol, Bit)
    	0x05, 0x07,		  // Usage Page (Keyboard/Keypad)
    	0x19, 0x00, 	  // Usage Minimum (Undefined)
    	0x2a, 0xff, 0x00, // Usage Maximum
    	0x15, 0x00,       // Logical Minimum (0)
    	0x26, 0xff, 0x00, // Logical Maximum (255)
    	0x95, 0x06,       // Report Count (6)
    	0x75, 0x08, 	  // Report Size (8)
    	0x81, 0x00,       // Input (Data, Ary, Abs)
    0xC0, // End Collection
};

const unsigned int Desc_Report_Keyboard_Size_byte = sizeof( Desc_Report_Keyboard );

const uint8_t Desc_String_Manufacturer[] =
{
	0x08,
	CY_U3P_USB_STRING_DESCR,
	'K', 0x00,
	'M', 0x00,
	'S', 0x00,
};

const uint8_t Desc_String_Product[] =
{
	0x0e,
	CY_U3P_USB_STRING_DESCR,
	'F', 0x00,
	'X', 0x00,
	'3', 0x00,
	'-', 0x00,
	'K', 0x00,
	'B', 0x00,
};
