
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
      Plate( 6 );

      {
        translate( [ -1, 20, -1 ] ) 
          cube( [ 21, 13, 8 ] );
        translate( [ 42, 34, -1 ] )
          cube( [  6,  5, 8 ] );
      }
    }

    translate( [ 0,             0, 6 ] )
      cube( [ SizeX_mm, 3, 12 ] );
    translate( [ 0, SizeY_mm -  3, 6 ] )
      cube( [ SizeX_mm, 3, 12 ] );

    translate( [            0,             3, 6 ] )
      cube( [        5, 7, 12 ] );
    translate( [            0, SizeY_mm - 10, 6 ] )
      cube( [        5, 7, 12 ] );
    translate( [ SizeX_mm - 5,             3, 6 ] )
      cube( [        5, 7, 12 ] );
    translate( [ SizeX_mm - 5, SizeY_mm - 10, 6 ] )
      cube( [        5, 7, 12 ] );
  }

  MountHoles();
}
