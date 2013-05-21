
//{{BLOCK(winScreen)

//======================================================================
//
//	winScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 97 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 6208 + 2048 = 8768
//
//	Time-stamp: 2011-12-03, 23:01:31
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __WINSCREEN__
#define __WINSCREEN__

#define winScreenTilesLen 6208
extern const unsigned short winScreenTiles[3104];

#define winScreenMapLen 2048
extern const unsigned short winScreenMap[1024];

#define winScreenPalLen 512
extern const unsigned short winScreenPal[256];

#endif // __WINSCREEN__

//}}BLOCK(winScreen)
