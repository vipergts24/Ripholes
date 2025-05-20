#include "../include/Background.h"
#include <nds.h>
#include <string.h>
#include <stdio.h>

#include <level1page1.h>

#define LEVEL_PAGE_BITMAP(l,p) (u16*)level##l##page##p##Bitmap
#define LEVEL_PAGE_PAL(l,p) (u16*)level##l##page##p##Pal

Background::Background (u8 layer, u8 level, u8 page, u16 width, u16 height)
{
   bgNum = bgInit(layer, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
   DC_FlushAll();
   dmaCopyHalfWords(3, level1page1Bitmap, bgGetGfxPtr(bgNum), 256*256);
   dmaCopyHalfWords(3, level1page1Pal, BG_PALETTE, 512);
}

Background::~Background ()
{
}

void Background::scroll(u16 x, u16 y)
{
   bgSetScroll(bgNum, x, y);
}

void Background::update ()
{
   bgUpdate();
}
