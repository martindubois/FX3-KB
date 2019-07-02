
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Component.h

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

// Function
/////////////////////////////////////////////////////////////////////////////

// aLine  The line of the assertion
//
// TODO  Component  Output the line number on a led or IO
void Component_AssertFailure( unsigned int aLine )
{
	volatile unsigned int lCount;

	for ( ;; )
	{
		lCount ++;
	}
}
