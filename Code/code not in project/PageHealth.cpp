#include "PageHealth.h"

// Constructor
PageHealth::PageHealth()
{
   health = 100.0;
   bHealthy = true;
   position.first = ;
   position.second = ;
}

// Destuctor
PageHealth::~PageHealth()
{
}

// draws the current HP of the page
void PageHealth::update()
{
   // if the health of the page is less than 0
   if (health < 1)
   {
      bHealthy = false;
   }
}

// returns the current health value
s23 PageHealth::sGetHealth()
{
   return health;
}

// Updates the health to the given parameter
void PageHealth::setHealth(s32 num)
{
   health = num;
}

// subtracts health from the page (used by rip function)
void PageHealth::subtract(s32 num)
{
   health = health - num;
}

// returns true if the current page is healthy to play
bool PageHealth::bIsHealthy()
{
   return bHealty;
}
