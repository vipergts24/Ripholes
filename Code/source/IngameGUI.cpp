#include <PA9.h>
#include "../include/IngameGUI.h"

InGameGUI::InGameGUI(): pageHP(100)
{
   PA_InitSpriteExtPrio(1);//Enable sprite Priority
   inv = new Inventory();
   //finv = new FriendInventory();

   bgInGameGui = new Background ( "top_gui", 1, 3, false );
}   

void InGameGUI::updateInven(u8 currPage)
{

}

Inventory* InGameGUI::getInv()
{
   return inv;
}

FriendInventory* InGameGUI::getFInv()
{
   //return finv;
}

void InGameGUI::updatePageHealth(u8 currPage)
{

} 
void InGameGUI::updateMinimap(u8 currPage)
{

} 
/*
void accessInventory(stylus.press);
{
   invSlideDown();
}
*/ 
void InGameGUI::invSlideDown()
{

} 
void InGameGUI::endPasteMode()
{

} 
void InGameGUI::invSlideUP()
{

} 
void InGameGUI::invEndMode()
{

} 
void InGameGUI::invMoveSprite()
{

} 

void InGameGUI::update(){
   inv->update();
   //finv->update();
}

InGameGUI::~InGameGUI()
{
   delete inv;
   //delete finv;
   delete bgInGameGui;
}
