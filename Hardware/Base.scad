
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Hardware/Base.scad

include <Common.scad>

PlateZ_mm = 5;
SizeZ_mm  = 8;

difference()
{
  union()
  {
    Plate( PlateZ_mm );

    translate( [ 0,              0, PlateZ_mm ] )
      cube( [ SizeX_mm, 3.5, SizeZ_mm - PlateZ_mm ] );
    translate( [ 0, SizeY_mm - 3.5, PlateZ_mm ] )
      cube( [ SizeX_mm, 3.5, SizeZ_mm - PlateZ_mm ] );

    translate( [            0,                3, PlateZ_mm ] )
      cube( [        5, 21, SizeZ_mm - PlateZ_mm ] );
    translate( [            0, SizeY_mm - 8 - 3, PlateZ_mm ] )
      cube( [        5,  8, SizeZ_mm - PlateZ_mm ] );
    translate( [ SizeX_mm - 5,                3, PlateZ_mm ] )
      cube( [        5, 19, SizeZ_mm - PlateZ_mm ] );
    translate( [ SizeX_mm - 5, SizeY_mm - 7 - 3, PlateZ_mm ] )
      cube( [        5,  7, SizeZ_mm - PlateZ_mm ] );
  }

  union()
  {
    LessPlastic();
    MountHoles();

    // Headers
    translate( [ 14.5,                3.5, -1 ] )
      cube( [ 53, 7, PlateZ_mm + 2 ] );
    translate( [ 14.5, SizeY_mm - 7 - 3.5, -1 ] )
      cube( [ 53, 7, PlateZ_mm + 2 ] );
  }
}
