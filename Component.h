
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Component.h

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <cyu3error.h>
#include <cyu3types.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define false  ( 0 )
#define true   ( 1 )

// Macros
/////////////////////////////////////////////////////////////////////////////

// ASSERT rather than assert because assert is already defined and use printf
#define ASSERT(C)  if ( ! ( C ) ) { Component_AssertFailure( __LINE__ ); }

// Function
/////////////////////////////////////////////////////////////////////////////

extern void Component_AssertFailure( unsigned int aLine );
