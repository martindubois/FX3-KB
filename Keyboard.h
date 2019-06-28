
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Keyboard.h

#pragma once

// Functions
/////////////////////////////////////////////////////////////////////////////

extern CyBool_t Keyboard_ClearFeature  ( uint16_t aIndex );
extern void     Keyboard_Define        ( void );
extern CyBool_t Keyboard_ProcessRequest( uint32_t aData0, uint32_t aData1 );
extern void     Keyboard_Reset         ( void );
extern void     Keyboard_SetConf       ( void );
