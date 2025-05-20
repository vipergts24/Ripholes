/*----------------------------------------------------------------

   Ripholes in Rubbish
   Page Health class header
   Author: Mee Cha

-----------------------------------------------------------------*/

#ifndef __PAGEHEALTH_H__
#define __PAGEHEALTH_H__

#include <PA9.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 
#include "Sprite.h"

class PageHealth 
{
public:
   PageHealth(bool bAnimate);
   ~PageHealth();

   // Status checkers.
   void update(Sprite* sPageHealth, bool bBeginLvl);    // returns false if the 
   u8 findRange();   // finds health range to draw sprite in GUI

   // accessor functions
   u8 sGetHealth();
   bool bIsHealthy(); // returns false if the page is in poor condition

   // mutator functions
   void setHealth(u8 num);
   void subtract(u8 num);

private:
   u8 health;
   bool bHealthy;    // default is true. Used to determine if a page is healthy
   bool bDoAnimate;
   u8 counter;

};

#endif
