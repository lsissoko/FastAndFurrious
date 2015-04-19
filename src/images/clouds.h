
//{{BLOCK(clouds)

//======================================================================
//
//	clouds, 512x256@8, 
//	+ palette 256 entries, not compressed
//	+ 47 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x32 
//	Total size: 512 + 3008 + 4096 = 7616
//
//	Time-stamp: 2010-12-06, 03:13:32
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __CLOUDS__
#define __CLOUDS__

#define cloudsTilesLen 3008
extern const unsigned short cloudsTiles[1504];

#define cloudsMapLen 4096
extern const unsigned short cloudsMap[2048];

#define cloudsPalLen 512
extern const unsigned short cloudsPal[256];

#endif // __CLOUDS__

//}}BLOCK(clouds)
