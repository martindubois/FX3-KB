
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Desc.h

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#define DESC_CONTROL_POWER_DOWN    ( 0x01 )
#define DESC_CONTROL_SLEEP         ( 0x02 )
#define DESC_CONTROL_WAKEUP        ( 0x04 )
#define DESC_CONTROL_DISPLAY_DUAL  ( 0x08 )
#define DESC_CONTROL_COLD_RESTART  ( 0x10 )
#define DESC_CONTROL_WARM_RESTART  ( 0x20 )
#define DESC_CONTROL_SPEAKER_MUTE  ( 0x40 )
#define DESC_CONTROL_HIBERNATE     ( 0x80 )

#define DESC_REPORT_CONTROL  ( 0x02 )
#define DESC_REPORT_KEY      ( 0x01 )

#define DESC_REPORT_KEYBOARD_SIZE_byte ( 106 )

extern const uint8_t Desc_BOS[];

extern const uint8_t Desc_Config_FS   [];
extern const uint8_t Desc_Config_HS_SS[];

extern const uint8_t Desc_Device_HS[];
extern const uint8_t Desc_Device_SS[];

extern const uint8_t Desc_Device_Qual[];

extern const uint8_t Desc_LangId[];

extern const uint8_t Desc_Report_Keyboard[];

extern const uint8_t Desc_String_Manufacturer[];
extern const uint8_t Desc_String_Product     [];
