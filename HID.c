
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     HID.h

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <stddef.h>

#include "HID.h"

// Function
/////////////////////////////////////////////////////////////////////////////

uint8_t HID_KeyFromChar( char aC, uint8_t * aModifier )
{
	ASSERT(    0 <  aC        );
	ASSERT( 0x80 >  aC        );
	ASSERT( NULL != aModifier );

	( * aModifier ) = 0;

	if ( ( '1' <= aC ) && ( '9' >= aC ) ) {                                            return HID_KEY_DIGIT ( aC             ); }
	if ( ( 'a' <= aC ) && ( 'z' >= aC ) ) {                                            return HID_KEY_LETTER( aC + 'A' - 'a' ); }
	if ( ( 'A' <= aC ) && ( 'Z' >= aC ) ) { ( * aModifier ) = HID_MODIFIER_LEFT_SHIFT; return HID_KEY_LETTER( aC             ); }

	switch ( aC )
	{
	case '\n' :
	case '\r' :                                             return HID_KEY_ENTER        ;

	case ' '  :                                             return HID_KEY_SPACE        ;
	case '!'  : ( * aModifier ) = HID_MODIFIER_LEFT_SHIFT ; return HID_KEY_DIGIT( '1' ) ;
	case '\"' : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_QUOTE        ;
	case '#'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '3' ) ;
	case '$'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '4' ) ;
	case '%'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '5' ) ;
	case '&'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '7' ) ;
	case '\'' :                                             return HID_KEY_QUOTE        ;
	case '('  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '9' ) ;
	case ')'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_0            ;
	case '*'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '8' ) ;
	case '+'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_EQUAL        ;
	case ','  :                                             return HID_KEY_COLON        ;
	case '-'  :                                             return HID_KEY_DASH         ;
	case '.'  :                                             return HID_KEY_DOT          ;
	case '/'  :                                             return HID_KEY_SLASH        ;
	case '0'  :                                             return HID_KEY_0            ;
	case ':'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DOUBLE_DOT   ;
	case ';'  :                                             return HID_KEY_DOUBLE_DOT   ;
	case '<'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_COLON        ;
	case '='  :                                             return HID_KEY_EQUAL        ;
	case '>'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DOT          ;
	case '?'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_SLASH        ;
	case '@'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '2' ) ;
	case '['  :                                             return HID_KEY_BRACKET_OPEN ;
	case '\\' :                                             return HID_KEY_BACKSLASH    ;
	case ']'  :                                             return HID_KEY_BRACKET_CLOSE;
	case '^'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DIGIT( '6' ) ;
	case '_'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_DASH         ;
	case '`'  :                                             return HID_KEY_TILD         ;
	case '{'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_BRACKET_OPEN ;
	case '|'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_BACKSLASH    ;
	case '}'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_BRACKET_CLOSE;
	case '~'  : ( * aModifier ) = HID_MODIFIER_RIGHT_SHIFT; return HID_KEY_TILD         ;

	default : ASSERT( false );
	}

	return 0;
}
