
//{{BLOCK(startScreen)

//======================================================================
//
//	startScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 271 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 17344 + 2048 = 19904
//
//	Time-stamp: 2011-12-03, 22:53:34
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __STARTSCREEN__
#define __STARTSCREEN__

#define startScreenTilesLen 17344
extern const unsigned short startScreenTiles[8672];

#define startScreenMapLen 2048
extern const unsigned short startScreenMap[1024];

#define startScreenPalLen 512
extern const unsigned short startScreenPal[256];

#endif // __STARTSCREEN__

//}}BLOCK(startScreen)
