//Ezodus display resolution. ZealOS burns the resolution into the system
//compile at boot (GrGlobals.ZC DefinePrint of the VBE mode); Ezodus does the
//same at HCRT build time, so after changing these rebuild the image:
//	./ezodus -c -f HCRT_BOOTSTRAP.BIN BuildHCRT.ZC
//Both values must be multiples of 8 (8x8 text cells), minimum 640x480.
#define GR_WIDTH	640
#define GR_HEIGHT	480
