
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     HID.h

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#define HID_LEFT_CTRL    ( 0x01 )
#define HID_LEFT_SHIFT   ( 0x02 )
#define HID_LEFT_ALT     ( 0x04 )
#define HID_LEFT_GUI     ( 0x08 )
#define HID_RIGHT_CTRL   ( 0x10 )
#define HID_RIGHT_SHIFT  ( 0x20 )
#define HID_RIGHT_ALT    ( 0x40 )
#define HID_RIGHT_GUI    ( 0x80 )

// ===== Key ================================================================

#define HID_KEY_LETTER(L)  ( 0x04 + ( L ) - 'A' )

#define HID_KEY_DIGIT(D)  ( 0x1e + ( D ) - '1' )

#define HID_KEY_0             ( 0x27 )
#define HID_KEY_ENTER         ( 0x28 )
#define HID_KEY_ESCAPE        ( 0x29 )
#define HID_KEY_BACKSPACE     ( 0x2a )
#define HID_KEY_TAB           ( 0x2b )
#define HID_KEY_SPACE         ( 0x2c )
#define HID_KEY_DASH          ( 0x2d )
#define HID_KEY_EQUAL         ( 0x2e )
#define HID_KEY_BRACKET_OPEN  ( 0x2f )
#define HID_KEY_BRACKET_CLOSE ( 0x30 )
#define HID_KEY_BACKSLASH     ( 0x31 )
#define HID_KEY_TILD          ( 0x32 )
#define HID_KEY_DOUBLE_DOT    ( 0x33 )
#define HID_KEY_QUOTE         ( 0x34 )

#define HID_KEY_COLON         ( 0x36 )
#define HID_KEY_DOT           ( 0x37 )
#define HID_KEY_SLASH         ( 0x38 )
#define HID_KEY_CAPS_LOCK     ( 0x39 )

#define HID_KEY_F1_F12(N)  ( 0x40 + ( N ) - 1 )

#define HID_KEY_PRINT_SCREEN  ( 0x46 )
#define HID_KEY_SCROLL_LOCK   ( 0x47 )
#define HID_KEY_PAUSE         ( 0x48 )
#define HID_KEY_INSERT        ( 0x49 )
#define HID_KEY_HOME          ( 0x4a )
#define HID_KEY_PAGE_UP       ( 0x4b )
#define HID_KEY_DELETE        ( 0x4c )
#define HID_KEY_END           ( 0x4d )
#define HID_KEY_PAGE_DOWN     ( 0x4e )
#define HID_KEY_RIGHT_ARROW   ( 0x4f )
#define HID_KEY_LEFT_ARROW    ( 0x50 )
#define HID_KEY_DOWN_ARROW    ( 0x51 )
#define HID_KEY_UP_ARROR      ( 0x52 )
#define HID_KEY_NUM_LOCK      ( 0x53 )
#define HID_KEY_DIV           ( 0x54 )
#define HID_KEY_MUL           ( 0x55 )
#define HID_KEY_SUB           ( 0x56 )
#define HID_KEY_ADD           ( 0x57 )
#define HID_KEY_ENTER_PAD     ( 0x58 )

#define HID_KEY_DIGIT_PAD(D)  ( 0x59 + ( D ) - '1' )

#define HID_KEY_0_PAD    ( 0x62 )
#define HID_KEY_DOT_PAD  ( 0x63 )

#define HID_KEY_EQUAL_PAD  ( 0x67 )

#define HID_KEY_F13_F24(N)  ( 0x68 + ( N ) - 13 )

// ===== Request ============================================================

#define HID_REQUEST_SET_REPORT  ( 0x09 )
#define HID_REQUEST_SET_IDLE    ( 0x0a )
