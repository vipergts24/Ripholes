/*  Ripholes in Rubbish Stylus Input Wrapper Library
 *  
 *  Written to standardize PALib functions as well as
 *  allow for macro functions to be written using them.
 */

#ifndef __WSTYLUS_H__
#define __WSTYLUS_H__

#include <PA9.h>       // Include for PA_Lib

/* PAlib Defines:
 *  #define 	PA_MoveSprite(sprite)   PA_MoveSpriteEx(PA_Screen, sprite, PA_GetSpriteLx(0, sprite), PA_GetSpriteLy(0, sprite))
 *  	Move a sprite according to the stylus's position. The sprite will be 'hooked' if the stylus passes over it, and then they'll be linked together. Returns 1 if the sprite is moved. You can also get information from PA_MovedSprite.Moving (1 if you are moving a sprite), .Sprite (sprite moved), .X (X position of the center of the sprite), .Y (Y position of the center of the sprite), .Vx (horizontal speed ! useful if you want to make the sprite continue to move when you release the stylus...), and .Vy
 *  #define 	PA_StylusInZone(x1, y1, x2, y2)   ((Stylus.X>=x1)&&(Stylus.Y>=y1)&&(Stylus.X<x2)&&(Stylus.Y<y2))
 * 	Check if the stylus is in a given zone... Returns 1 if yes, 0 if not
 *
 * Functions:
 *  void 	PA_UpdatePad (void)
 *  	Update the Keypad, use it once per frame (in the VBL for example). You can then retrieve the held down keys with Pad.Held.A (or Up, Down...), Newly pressed keys with Pad.Newpress.R, and the just released keys with Pad.Released.Up...
 *  void 	PA_UpdateStylus (void)
 * 	Update the Stylus position. You can then check if the stylus is current in use (Stylus.Held), newly pressed (Stylus.Newpress), or released (Stylus.Released), and get it's position (Stylus.X, Stylus.Y).
 *  u8 	PA_MoveSpritePix (u8 sprite)
 * 	Move a sprite according to the stylus's position, only if you touch a sprite's pixel. This is similar to PA_MoveSprite, but slightly slower and requires PA_InitSpriteDraw(screen, sprite) before. The sprite will be 'hooked' if the stylus passes over it, and then they'll be linked together. Returns 1 if the sprite is moved. You can also get information from PA_MovedSprite.Moving (1 if you are moving a sprite), .Sprite (sprite moved), .X (X position of the top left corner of the sprite), .Y (Y position of the top left corner of the sprite), .Vx (horizontal speed ! useful if you want to make the sprite continue to move when you release the stylus...), and .Vy
 *  u8 	PA_MoveSpriteEx (u8 screen, u8 sprite, u8 lx, u8 ly)
 * 	Move a sprite according to the stylus's position. See PA_MoveSprite for more details... The difference is that here you chose the sprite dimension (lx and ly), which is useful if the sprite is smaller than the DS standard sizes... (for example 20x20...). This will also limit the 'hooking' distance
 *  u8 	PA_MoveSpriteDistance (u8 sprite, u8 distance)
 * 	Move a sprite according to the stylus's position. See PA_MoveSprite for more details... The difference is that here you chose the hooking distance in pixels
 *  u8 	PA_SpriteStylusOverEx (u8 sprite, u8 lx, u8 ly)
 * 	Check if the stylus position is over a given sprite (stylus pressed or not)
 *  u8 	PA_SpriteTouchedEx (u8 sprite, u8 lx, u8 ly)
 * 	Check if a given sprite is touched. Returns 1 if touched... You can chose the width and height around the sprite
 *  u8 	PA_SpriteTouched (u8 sprite)
 * 	Check if a given sprite is touched. Returns 1 if touched...
 *  u8 	PA_SpriteStylusOver (u8 sprite)
 * 	Check if the stylus position is over a given sprite (stylus pressed or not) 
 */

/* Defined in PA_Keys.h:
 *
 * typedef struct {
 *   u8 Held, Released, Newpress, Newpress0;
 *   s16 X, Y, altX, altY, Pressure, Vx, Vy, oldVx, oldVy, Downtime, Uptime, DblClick;
 * } PA_Stylus;
 */

// Hardware accessors, These are either 0 or 1:
bool bStylusNewpress ()
{
   u8 x = Stylus.Newpress;

   if ( x <= 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

bool bStylusHeld ()
{
   u8 x = Stylus.Held;

   if ( x <= 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

bool bStylusReleased ()
{
   u8 x = Stylus.Released;

   if ( x <= 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

// These are X: {0, 255} and Y: {0, 192}:
s16 sStylusX ()
{
   return Stylus.X;
}

s16 sStylusY ()
{
   return Stylus.Y;
}

// Stylus has a few special attributes as well:
bool bStylusDblClick () // Behaves just like Newpress, Held, and Released
{
   u8 x = Stylus.DblClick;

   if ( x <= 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

s16 sStylusDowntime () // Divide by 60 to get this in seconds
{
   return Stylus.Downtime;
}

s16 sStylusUptime () // Divide by 60 to get this in seconds
{
   return Stylus.Uptime;
}

#endif
