/*  Ripholes in Rubbish Input Wrapper Library
 *  
 *  Written to standardize PALib functions as well as
 *  allow for macro functions to be written using them.
 */

#include <PA9.h>       // Include for PA_Lib


class Graphic
{
   private:
      u16 uGfxNum;
      u8  uScreenNum;
      u8  uObjShape;
      u8  uObjSize;
      u8  uColorMode;

   public:
      Graphic ( u8 screen, void *obj_data, u8 obj_shape, u8 obj_size, u8 color_mode );

      u16 uGetGfxNum ();
}

class Sprite 
{
   private:
      u8 uId;

   public:
      Sprite ( u8 uSpriteId );
}

/* bDragSprite causes a sprite to follow the stylus on the lower screen.
 * 
 * RETURNS true if the sprite was moved and false if it was not.
 */
bool bDragSprite ( u8 uSpriteId )
{
   return PA_MoveSprite ( uSpriteId );
}