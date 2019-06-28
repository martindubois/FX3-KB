
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Desc.h

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#ifdef _CONSUMER_CONTROL_
	#define DESC_REPORT_CONSUMER_CONTROL_SIZE_byte ( 106 )
#endif

#define DESC_REPORT_KEYBOARD_SIZE_byte ( 65 )

extern const uint8_t Desc_BOS[];

extern const uint8_t Desc_Config_FS[];
extern const uint8_t Desc_Config_HS[];
extern const uint8_t Desc_Config_SS[];

extern const uint8_t Desc_Device_HS[];
extern const uint8_t Desc_Device_SS[];

extern const uint8_t Desc_Device_Qual[];

extern const uint8_t Desc_LangId[];

#ifdef _CONSUMER_CONTROL_
	extern const uint8_t Desc_Report_Consumer_Control[];
#endif

extern const uint8_t Desc_Report_Keyboard[];

extern const uint8_t Desc_String_Manufacturer[];
extern const uint8_t Desc_String_Product     [];
