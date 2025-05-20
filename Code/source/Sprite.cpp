/*
  Ripholes in Rubbish 
  Sprite Class Source

  All Sprites are loaded in dynamically through a file system with various attributes
  such as name, type, position, animation information, and even specific AI Controllers.
  This is why a lot of the attributes require a string because they are mapping what is
  found in the data file to an actual file array of pixels for the Sprite
*/

#include "../include/Sprite.h"


#define ColorMode256 1
//Private static members of the Sprite class.  Lower and Upper screen memory can only hold 128 sprites
//and have separate array containers for them and palettes
u8 Sprite::lowerScreen[128];
u8 Sprite::topScreen[128];
string Sprite::lowerScreenPal[16];
string Sprite::topScreenPal[16];
//Public static members
pair<s32,s32> Sprite::scroll;
map<string, u16 > Sprite::spriteNames;//Maps Sprite by name
map<string, u8  > Sprite::spritePals; //Maps Sprite by Palette name

//Sprite Constructor takes in a screen number, name, palette name that it's assigned to, position,
//and if its an animation sprite.
Sprite::Sprite(u8 screenNum, string name, string palName, s32 xPos, s32 yPos, bool isAnim){
   this->name = name;
   this->palName = palName;
   screen = screenNum;
   assignId();//Assigns an ID based off available location in memory
   setupSprite(name);//Assigns a size and shape for memory allocation based off the name from a file
   bIsAnimating = isAnim;
   
   if(spriteNames[name] && !bIsAnimating)
   {  //Library Graphics call that takes in screen number for VRAM bank, size and shape, palette among other attributes
      PA_CreateSpriteFromGfx(screen, spriteId, spriteNames[name], spriteShape, spriteSize, ColorMode256, spritePals[palName], xPos>>8, yPos>>8);
   }
   else
   {
      PA_FatLoadSprite(spriteId, name.c_str() );//Loads sprite into RAM using Fat File System
      u16 x = PA_CreateGfx(screen, (void*)sprite_gfx[spriteId], spriteShape, spriteSize, ColorMode256);
      spriteNames[name] = x;
      PA_CreateSpriteFromGfx(screen, spriteId, x, spriteShape, spriteSize, ColorMode256, spritePals[palName], xPos>>8, yPos>>8);
      if (!bIsAnimating) PA_FatFreeSprite(spriteId);
   }
}

//Each Sprite needs to be assigned an ID upon creation
void Sprite::assignId(){
   //Assigns appropriate ID to blank space from 0 to 118.  Slots 119-127 are reserved
   //for 4 holes, 4 reverse holes, and 1 magic (ripping animation)
   u8 i;
   if(screen == 0)
   {
      for(i=0;i<119;i++){
         if(lowerScreen[i]==0){
            Sprite::lowerScreen[i]=1;
            spriteId = i;
            break;
         }
      }
   }
   else
   {
      for(i=0;i<119;i++){
         if(topScreen[i]==0){
            Sprite::topScreen[i]=1;
            spriteId = i;
            break;
         }
      }
   }
}
//Basic move Sprite functions
void Sprite::moveSprite(s32 x, s32 y){
   //No scrolling on the top screen since it is just a UI, so the sprites can simply be placed
   //bit shifted to the right and on the screen
   if(screen ==1){
      PA_SetSpriteXY(screen, spriteId, x>>8, y>>8);
   }
   else {//Since it is scrollable on the bottom screen
      if (((x>>8 > (scroll.first>>8)-(PA_GetSpriteLx(screen, spriteId)-1)) && (x>>8 < (scroll.first>>8) + 256)) &&
         ((y>>8 > (scroll.second>>8)-(PA_GetSpriteLy(screen, spriteId)-1)) && (y>>8 < (scroll.second>>8) + 192))) {
            PA_SetSpriteXY(screen, spriteId, (x - scroll.first)>>8, (y - scroll.second)>>8); // Otherwise, move it to the desired position
      }
      else {
         PA_SetSpriteX(screen, spriteId, 256); //If the sprite is out of bounds, move it offscreen        
      }
   }
}
//Hiding Sprites are moved off screen but still in VRAM for when they come into view or are shown
void Sprite::hideSprite(){
   PA_SetSpriteX(screen, spriteId, 256);
}
//Starts the animation of a sprite based off a first and last frame index and speed
void Sprite::animateSprite(s16 firstFrame, s16 lastFrame, u8 speed){
   PA_StartSpriteAnim(screen, spriteId, firstFrame, lastFrame, speed);
}
//Sets the specific Frame Animation
void Sprite::setFrameAnimation(s16 frame){
   PA_SetSpriteAnim(screen, spriteId, frame);
}
//Sets the horizontal sprite flip direction
void Sprite::hFlip(u8 direction){
   PA_SetSpriteHflip(screen, spriteId, direction);
}
//Clears all the sprites out of the map
void clearMap()
{
   Sprite::spriteNames.clear();
}
//Initiates all of the Sprite Palettes based off of each type
void initPalettes()
{
   PA_FatEasyLoadSpritePal(0,0, "GUI");
   PA_FatEasyLoadSpritePal(1,0, "GUI");
   Sprite::spritePals["GUI"] = 0;

   PA_FatEasyLoadSpritePal(0,1, "childAvatar");
   PA_FatEasyLoadSpritePal(1,1, "childAvatar");
   Sprite::spritePals["childAvatar"] = 1;

   PA_FatEasyLoadSpritePal(0,2, "era0");
   PA_FatEasyLoadSpritePal(1,2, "era0");
   Sprite::spritePals["era0"] = 2;

   PA_FatEasyLoadSpritePal(0,3, "worldMap");
   PA_FatEasyLoadSpritePal(1,3, "worldMap");
   Sprite::spritePals["worldMap"] = 3;

   PA_FatEasyLoadSpritePal(0,4, "journal");
   PA_FatEasyLoadSpritePal(1,4, "journal");
   Sprite::spritePals["journal"] = 4;
}
//Initializes graphics for rip magic animation, holes, and reverse holes when a level starts
void initHoleGfx()
{

   PA_FatLoadSprite(119, "riphole_magic");
   u16 x = PA_CreateGfx(0, (void*)sprite_gfx[119], 0, 3, ColorMode256);
   Sprite::spriteNames["riphole_magic"] = x;

   PA_FatLoadSprite(120, "riphole_marquee_hole1" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[120], 0, 3, ColorMode256);
   Sprite::spriteNames["riphole_marquee_hole1"] = x;

   PA_FatLoadSprite(121, "riphole_marquee_hole2" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[121], 0, 3, ColorMode256);
   Sprite::spriteNames["riphole_marquee_hole2"] = x;

   PA_FatLoadSprite(122, "riphole_marquee_hole3" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[122], 0, 3, ColorMode256);
   Sprite::spriteNames["riphole_marquee_hole3"] = x;

   PA_FatLoadSprite(123, "riphole_marquee_hole4" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[123], 0, 3, ColorMode256);
   Sprite::spriteNames["riphole_marquee_hole4"] = x;

   PA_FatLoadSprite(124, "hole1" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[124], 0, 3, ColorMode256);
   Sprite::spriteNames["hole1"] = x;

   PA_FatLoadSprite(125, "hole2" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[125], 0, 3, ColorMode256);
   Sprite::spriteNames["hole2"] = x;

   PA_FatLoadSprite(126, "hole3" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[126], 0, 3, ColorMode256);
   Sprite::spriteNames["hole3"] = x;

   PA_FatLoadSprite(127, "hole4" );
   x = PA_CreateGfx(0, (void*)sprite_gfx[127], 0, 3, ColorMode256);
   Sprite::spriteNames["hole4"] = x;


}
//Frees all the graphics associated with magic animation, holes, and reverse holes
void freeHoleGfx()
{
   PA_FatFreeSprite(119);
   PA_FatFreeSprite(120);
   PA_FatFreeSprite(121);
   PA_FatFreeSprite(122);
   PA_FatFreeSprite(123);
   PA_FatFreeSprite(124);
   PA_FatFreeSprite(125);
   PA_FatFreeSprite(126);
   PA_FatFreeSprite(127);
}
//Destructor for Sprites.  Sets the ID back to zero so another created sprite  can take its ID
//Frees up the memory if it is the only sprite of its type
Sprite::~Sprite(){
   if(screen == 0 ){
      lowerScreen[this->spriteId]=0;
   }
   else {
      topScreen[this->spriteId] = 0;
   }
   PA_StopSpriteAnim(0,this->spriteId);
   PA_DeleteSprite(screen,spriteId);

   if ( bIsAnimating && name != "riphole_magic")
   {
      PA_FatFreeSprite(spriteId);
   }
}

//sets the sprite Sizes based off the name.  All size macros are really just integers for the library
//If the size were a bit larger, a more efficient implementation would be to use a map instead of 
//if/else statements
void Sprite::setupSprite(string name)
{
   spriteShape = 0;

   if(name == "childAvatar")
   {
      spriteSize = 2;
   }
   else if(name =="scrib"  || name =="bat"    || 
           name =="rabbit" || name =="spider" ||
           name =="spring" )
   {
      spriteSize = 2;
   }
   else if(name =="top_pagenums" || name =="event_speech")
   {
      spriteSize = 1;
   }
   else if(name == "pMenuRestart" || name == "pMenuWorldMap")
   {
      spriteShape = 1;
      spriteSize = 3;
   }
   else if(name =="journal_map" || name == "worldmap_box" || name == "save" )
   {
      spriteSize = 2;
   }
   else if(name.substr(0,11) == "collectible")
   {
      spriteSize = 2;
   }
   else if(name =="avatar_mini_head")
   {
      spriteSize = 1;
   }
   else if(name.substr(0,2) == "wm")
   {
      spriteSize = 2;
   }
   else
   {
      spriteSize = 3;
   }
}



