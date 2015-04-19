
//{{BLOCK(pauseScreen)

//======================================================================
//
//	pauseScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 86 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5504 + 2048 = 8064
//
//	Time-stamp: 2011-12-03, 23:01:19
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __PAUSESCREEN__
#define __PAUSESCREEN__

#define pauseScreenTilesLen 5504
extern const unsigned short pauseScreenTiles[2752];

#define pauseScreenMapLen 2048
extern const unsigned short pauseScreenMap[1024];

#define pauseScreenPalLen 512
extern const unsigned short pauseScreenPal[256];

#endif // __PAUSESCREEN__

//}}BLOCK(pauseScreen)
