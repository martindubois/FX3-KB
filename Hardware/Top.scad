
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Hardware/Base.scad

include <Common.scad>

difference()
{
  union()
  {
    Plate( 6 );

    translate( [ 0,             0, 6 ] )
      cube( [ SizeX_mm, 3, 12 ] );
    translate( [ 0, SizeY_mm -  3, 6 ] )
      cube( [ SizeX_mm, 3, 12 ] );

    translate( [            0,                 3, 6 ] )
      cube( [        5,  8, 12 ] );
    translate( [            0, SizeY_mm -  9 - 3, 6 ] )
      cube( [        5,  9, 12 ] );
    translate( [ SizeX_mm - 4,                 3, 6 ] )
      cube( [        4,  6, 12 ] );
    translate( [ SizeX_mm - 5, SizeY_mm - 12 - 3, 6 ] )
      cube( [        5, 12, 12 ] );

    translate( [ 0, SizeY_mm - 11 - 9 - 3, 6 ] )
			  cube( [ 3, 11, 5 ] );

    translate( [ SizeX_mm - 3, SizeY_mm - 24 - 12 - 3, 6 ] )
      cube( [ 3, 24, 8 ] );

    translate( [ 39, 32, 6 ] )
      cube( [ 12, 9, 5 ] );
  }

	 union()
  {
    MountHoles();

    // USB 3.0 connector
    translate( [ -1, 20, -1 ] )
      cube( [ 21, 13, 8 ] );

    // PMODE jumber
    translate( [ 42, 35, -1 ] )
      cube( [  6,  3, 13 ] );

    translate( [ 42, 33, -1 ] )
      cube( [  6,  7, 4 ] );
  }
}
