//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Background files : 
extern const int level_01_mini_map_Info[3]; // BgMode, Width, Height
extern const unsigned short level_01_mini_map_Map[768] __attribute__ ((aligned (4))) ;  // Pal : level_01_mini_map_Pal
extern const unsigned char level_01_mini_map_Tiles[43200] __attribute__ ((aligned (4))) ;  // Pal : level_01_mini_map_Pal
extern PAGfx_struct level_01_mini_map; // background pointer


// Palette files : 
extern const unsigned short level_01_mini_map_Pal[138] __attribute__ ((aligned (4))) ;


#endif

