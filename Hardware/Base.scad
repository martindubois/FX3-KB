
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Hardware/Base.scad

include <Common.scad>

difference()
{
  union()
  {
    difference()
    {
      Plate( 4 );
      {
        translate( [ 15,                3, -1 ] )
          cube( [ 51, 7, 6 ] );
        translate( [ 15, SizeY_mm - 7 - 3, -1 ] )
          cube( [ 51, 7, 6 ] );
      }
    }

    translate( [ 0,             0, 4 ] )
      cube( [ SizeX_mm, 3, 4 ] );
    translate( [ 0, SizeY_mm -  3, 4 ] )
      cube( [ SizeX_mm, 3, 4 ] );

    translate( [            0,                3, 4 ] )
      cube( [        5, 21, 4 ] );
    translate( [            0, SizeY_mm - 8 - 3, 4 ] )
      cube( [        5,  8, 4 ] );
    translate( [ SizeX_mm - 5,                3, 4 ] )
      cube( [        5, 19, 4 ] );
    translate( [ SizeX_mm - 5, SizeY_mm - 7 - 3, 4 ] )
      cube( [        5,  7, 4 ] );
  }

  MountHoles();
}
