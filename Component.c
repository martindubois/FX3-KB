
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Component.h

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

// Function
/////////////////////////////////////////////////////////////////////////////

void Component_AssertFailure( unsigned int aLine )
{
	volatile unsigned int lCount;

	for ( ;; )
	{
		lCount ++;
	}
}
