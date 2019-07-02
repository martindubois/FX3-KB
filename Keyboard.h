
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Keyboard.h

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

#pragma once

// Functions
/////////////////////////////////////////////////////////////////////////////

extern void Key          ( uint8_t aModifier, uint8_t aKey );
extern void Key_Char     ( char aC );
extern void Key_Down     ( uint8_t aModifier, uint8_t aKey );
extern void Key_Hibernate();
extern void Key_PowerDown();
extern void Key_Sleep    ();
extern void Key_String   ( const char * aStr );
extern void Key_Up       ( uint8_t aModifier );
extern void Key_Wakeup   ();

extern CyBool_t Keyboard_ClearFeature  ( uint16_t aIndex );
extern void     Keyboard_Define        ( void );
extern CyBool_t Keyboard_ProcessRequest( uint32_t aData0, uint32_t aData1 );
extern void     Keyboard_Reset         ( void );
extern void     Keyboard_SetConf       ( void );

extern void Payload( void );
