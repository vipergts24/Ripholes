/*----------------------------------------------------------------

   Ripholes in Rubbish
   Page Health class header
   Author: Mee Cha

-----------------------------------------------------------------*/

#ifndef __PAGEHEALTH_H__
#define __PAGEHEALTH_H__

#include <nds.h> 

class PageHealth 
{
public:
   PageHealth();
   ~PageHealth();

   // Status checkers
   void update();

   // accessor and mutator functions
   s23 sGetHealth();
   void setHealth(s32 num);
   void subtract(s32 num);
   bool bIsHealthy();


private:
   s32 health;
   bool bHealthy;    // default is true. Used to determine if a page is healthy
   
	Sprite * healthGUI;
	pair<s32,s32> position;

};

#endif