
//{{BLOCK(loseScreen)

//======================================================================
//
//	loseScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 88 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5632 + 2048 = 8192
//
//	Time-stamp: 2011-12-03, 23:01:52
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __LOSESCREEN__
#define __LOSESCREEN__

#define loseScreenTilesLen 5632
extern const unsigned short loseScreenTiles[2816];

#define loseScreenMapLen 2048
extern const unsigned short loseScreenMap[1024];

#define loseScreenPalLen 512
extern const unsigned short loseScreenPal[256];

#endif // __LOSESCREEN__

//}}BLOCK(loseScreen)
