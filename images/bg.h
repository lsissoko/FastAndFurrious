
//{{BLOCK(bg)

//======================================================================
//
//	bg, 512x256@8, 
//	+ palette 256 entries, not compressed
//	+ 38 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x32 
//	Total size: 512 + 2432 + 4096 = 7040
//
//	Time-stamp: 2010-12-06, 03:17:13
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __BG__
#define __BG__

#define bgTilesLen 2432
extern const unsigned short bgTiles[1216];

#define bgMapLen 4096
extern const unsigned short bgMap[2048];

#define bgPalLen 512
extern const unsigned short bgPal[256];

#endif // __BG__

//}}BLOCK(bg)
