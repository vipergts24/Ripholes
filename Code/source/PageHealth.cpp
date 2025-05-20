#include "../include/PageHealth.h"

// Constructor
PageHealth::PageHealth(bool bAnimate)
{
   health = 100;
   bHealthy = true;
   bDoAnimate = bAnimate;
   counter = 0;
}

// Destuctor
PageHealth::~PageHealth()
{
}

// draws the current HP of the page
void PageHealth::update(Sprite* sPageHealth, bool bBeginLvl)
{
   // if the health of the page is less than 1
   if (health < 1)
   {
      bHealthy = false;
   }

   // See if the game should animate the page health bar (at level load up) or not
   if (bDoAnimate && !bBeginLvl)
   {
      if (counter < 99)
      {
         counter++;
         sPageHealth->setFrameAnimation ( counter );
      }
      else
      {
         bDoAnimate = false;  // stop animating
      }
   }
   // Determine which sprite to draw for the health meters
   else 
   {
      if ( findRange() > 1 )
      {
         sPageHealth->setFrameAnimation ( findRange() - 1 );
      }
      else
      {
         sPageHealth->setFrameAnimation ( findRange()  );
      }
   }
}

// returns the current health value
u8 PageHealth::sGetHealth()
{
   return health;
}

// Updates the health to the given parameter
void PageHealth::setHealth(u8 num)
{
   health = num;
}

// subtracts health from the page if it can be subtracted
// Usage: passed to page.cpp's decHealth()'s function from rip mechanic in level.cpp
void PageHealth::subtract(u8 num)
{
   if (health > 0) 
   {
      health = health - num;
   }
}

// returns true if the current page is healthy to play
bool PageHealth::bIsHealthy()
{
   return bHealthy;
}

// Returns a number related to health of current page 
//   that will determine what sprite meter to draw in the gui
u8 PageHealth::findRange()
{
   if (health >= 100)
   {
      return 100;
   }
   else if (health < 100 && health >= 90)
   {
      return 90;
   }
   else if (health < 90 && health >= 80)
   {
      return 80;
   }
   else if (health < 80 && health >= 70)
   {
      return 70;
   }
   else if (health < 70 && health >= 60)
   {
      return 60;
   }
   else if (health < 60 && health >= 50)
   {
      return 50;
   }
   else if (health < 50 && health >= 40)
   {
      return 40;
   }
   else if (health < 40 && health >= 30)
   {
      return 30;
   }
   else if (health < 30 && health >= 20)
   {
      return 20;
   }
   else if (health < 20 && health >= 10)
   {
      return 10;
   }
   else if (health < 10 && health > 0)
   {
      return 1;
   }
   else 
   {
      return 0;
   }
   
}
