
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Hardware/Base.scad

include <Common.scad>

PlateZ_mm =  7;
SizeZ_mm  = 13;

difference()
{
  union()
  {
    Plate( PlateZ_mm );

    translate( [ 0,             0, PlateZ_mm ] )
      cube( [ SizeX_mm, 3, SizeZ_mm - PlateZ_mm ] );
    translate( [ 0, SizeY_mm -  3, PlateZ_mm ] )
      cube( [ SizeX_mm, 3, SizeZ_mm - PlateZ_mm ] );

    translate( [            0,                 3, PlateZ_mm ] )
      cube( [        5,  17, SizeZ_mm - PlateZ_mm ] );
    translate( [            0, SizeY_mm -  9 - 3, PlateZ_mm ] )
      cube( [        5,  9, SizeZ_mm - PlateZ_mm ] );
    translate( [ SizeX_mm - 4,                 3, PlateZ_mm ] )
      cube( [        4,  6, SizeZ_mm - PlateZ_mm ] );
    translate( [ SizeX_mm - 5, SizeY_mm - 12 - 3, PlateZ_mm ] )
      cube( [        5, 12, SizeZ_mm - PlateZ_mm ] );

    translate( [ 0, SizeY_mm - 11 - 9 - 3, PlateZ_mm ] )
      cube( [ 3, 11, SizeZ_mm - PlateZ_mm - 3 ] );

    translate( [ SizeX_mm - 3, SizeY_mm - 32 - 12 - 3, PlateZ_mm ] )
      cube( [ 3, 32, SizeZ_mm - PlateZ_mm - 3 ] );

    translate( [ 40, 32, PlateZ_mm ] )
      cube( [ 12, 9, SizeZ_mm - PlateZ_mm - 2 ] );
  }

	 union()
  {
    LessPlastic();
    MountHoles();

    // USB 3.0 connector
    translate( [ -1, 20, -1 ] )
      cube( [ 21, 13, PlateZ_mm + 2 ] );

    // SRAM_ENABLE jumber j5
    translate( [ SizeX_mm - 1 - 3, 10, 4 ] )
      cube( [ 3, 6, SizeZ_mm ] );

    // PMODE jumber j4
    translate( [ 43, 35, -1 ] )
      cube( [  6,  3, SizeZ_mm ] );

    translate( [ 43, 33, -1 ] )
      cube( [  6,  7, 4 ] );

    // VBUS jumper j2
    translate( [ 14, 35, -1 ] )
      cube( [  6,  3, SizeZ_mm ] );

    // Jumber j1
    translate( [ -1, 11, -1 ] )
      cube( [ 4, 6, SizeZ_mm + 2 ] );
  }
}
