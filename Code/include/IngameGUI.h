// 0 palib refs

#ifndef __INGAMEGUI_H__
#define __INGAMEGUI_H__
#include <PA9.h> 
//#include "Mini_Map.h"
#include "Inventory.h"
#include "../include/Background.h"
#include "FriendInventory.h"

class InGameGUI
{
public:
   InGameGUI();
   ~InGameGUI();
   void updateInven(u8);
   void updatePageHealth(u8);
   void updateMinimap(u8);
   //void accessInventory(stylus.press);
   void invSlideDown();
   void endPasteMode();
   void invSlideUP();
   void invEndMode();
   void invMoveSprite();
   void update();
   Inventory* getInv();
   FriendInventory* getFInv();


private:
   Inventory * inv; //Change later
   FriendInventory * finv;
      //Mini_Map minimap;
      //Inventory * inv;
   u8 pageHP;
   Background * bgInGameGui;
   Background * bgMiniMap;
};

#endif
