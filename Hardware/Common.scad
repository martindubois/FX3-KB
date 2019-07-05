
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Hardware/Common.scad

// Constants
/////////////////////////////////////////////////////////////////////////////

SizeX_mm = 82.3;
SizeY_mm = 56  ;

// Modules
/////////////////////////////////////////////////////////////////////////////

module MountHoles()
{
    union()
    {
        translate( [            4.06,            7.06, -1 ] ) cylinder( r = 2, h = 20, $fs = 0.1 );
        translate( [ SizeX_mm - 4.06,            7.06, -1 ] ) cylinder( r = 2, h = 20, $fs = 0.1 );
        translate( [            4.06, SizeY_mm - 7.06, -1 ] ) cylinder( r = 2, h = 20, $fs = 0.1 );
        translate( [ SizeX_mm - 4.06, SizeY_mm - 7.06, -1 ] ) cylinder( r = 2, h = 20, $fs = 0.1 );
    }
}

module Plate( aZ )
{
    cube( [ SizeX_mm, SizeY_mm, aZ ] );
}
