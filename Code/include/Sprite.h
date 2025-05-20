/*
   Ripholes in Rubbish
   Sprite Class Header
*/

#if !defined(_SPRITE_H)
#define _SPRITE_H

#include <utility>
#include <map>
#include <string>
#include <PA9.h>

using namespace std;

#define norm_fix(x)			((x)<<8)
#define fix_norm(x)			((x)>>8)  //xxx truncates instead of rounds

//Static Functions
void clearMap();
void initPalettes();
void initHoleGfx();
void freeHoleGfx();

class Sprite
{

public:
   Sprite(u8 screenNum, string name, string palName, s32 xPos, s32 yPos, bool isAnim);
   ~Sprite();

   // update functions
   void setupSprite(string name);
   void hideSprite();
   void assignId();
   void moveSprite(s32 x, s32 y);
   void animateSprite(s16 firstFrame, s16 lastFrame, u8 speed);
   void setFrameAnimation(s16 frame);
   void hFlip(u8 direction);
   void printSprites();

   // accessor functions
   bool bGetAnimating() { return bIsAnimating;}
   string getName()     { return name;}
   string getPalName()  { return palName;}
   u8 getScreen()       { return screen;}
   u8 getId()           { return spriteId; }
   
   // public variables
   u8 screen;
   static pair<s32,s32> scroll;
   static map<string, u16 > spriteNames;
   static map<string, u8  > spritePals;
   
private:
   static u8 lowerScreen[128];
   static u8 topScreen[128];
   static string lowerScreenPal[16];
   static string topScreenPal[16];
   u8 spriteId;
   //pal and tiles
   u8       spriteShape;
   string   name;
   u8       spriteSize;
   void*    spriteTiles;
   string   palName;
   u8       palNum;
   bool     bIsAnimating;

};

#endif
