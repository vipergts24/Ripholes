// palib refs all over the place

#include "../include/Inventory.h"

using namespace std;

Inventory::Inventory(): isOpen(false),
isOpening(false),
isClosing(false),
oneFull(false),
twoFull(false),
threeFull(false),
x(0),    
y(-64),
invScroll(0)
{
   garbage = new Sprite(0, "inventory_row1col1", "GUI", x<<8, y<<8, true );
   spriteR1C1 = new Sprite(0, "inventory_row1col1", "GUI", x<<8, y<<8, true );
   spriteR1C2 = new Sprite(0, "inventory_row1col2", "GUI", x<<8, y<<8, true );
   spriteR1C3 = new Sprite(0, "inventory_row1col3", "GUI", x<<8, y<<8, true );
   spriteR1C4 = new Sprite(0, "inventory_row1col4", "GUI", x<<8, y<<8, true );
   spriteR2C4 = new Sprite(0, "inventory_row2col4", "GUI", x+192<<8, y+64<<8, true );


   PA_SetSpritePrio(0, spriteR1C1->getId(), 1);
   PA_SetSpritePrio(0, spriteR1C2->getId(), 1);
   PA_SetSpritePrio(0, spriteR1C3->getId(), 1);
   PA_SetSpritePrio(0, spriteR1C4->getId(), 1);
   PA_SetSpritePrio(0, spriteR2C4->getId(), 1);
   for(u8 i = 0;i < 3;++i) invList[i] = NULL;
}

void Inventory::update()
{
   if(Stylus.Newpress && PA_StylusInZone(192,0,256,20))
   {
      if(isOpen)isClosing = true;
      else isOpening = true;
   }
   if(isOpening && y + invScroll <= 0)
   {
      PA_SetSpriteXY(0,spriteR1C1->getId(),x,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C2->getId(),x+64,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C3->getId(),x+128,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C4->getId(),x+192,y+invScroll);
      PA_SetSpriteXY(0,spriteR2C4->getId(),x+192,y+64+invScroll);
      u8 initPadding = 16;
      for(u8 i = 0;i < 3;++i){
         if(invList[i])
            PA_SetSpriteXY(0, invList[i]->getSprite()->getId(),x + initPadding + (i * 80),y + invScroll);
      }
      if( (y + invScroll) >= 0){
         isOpen = true;
         isOpening = false;
      }
      invScroll += 4;
   }
   else if(isClosing && y + invScroll >= -64){

      PA_SetSpriteXY(0,spriteR1C1->getId(),x,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C2->getId(),x+64,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C3->getId(),x+128,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C4->getId(),x+192,y+invScroll);
      PA_SetSpriteXY(0,spriteR2C4->getId(),x+192,y+64+invScroll);	
      u8 initPadding = 16;
      for(u8 i = 0;i < 3;++i){
         if(invList[i])
            PA_SetSpriteXY(0, invList[i]->getSprite()->getId(),x + initPadding + (i * 80),y + invScroll);
      }
      if( (y + invScroll) <= -64){
         isOpen = false;
         isClosing = false;
      }
      invScroll -= 4;
   }

}   
/*
void Inventory::setSprite(sprite)
{

}
sprite Inventory::getSprite()
{

}
*/
void Inventory::startCloseInventory()
{
   //isOpen = false;
   isClosing = true;
}
void Inventory::startOpenInventory()
{
   //isOpen = true;
   isOpening = true;
}
void Inventory::addItem(HelperDood* objToAdd)
{
   //Sprite * spr = new Sprite(0, objToAdd->getSprite()->getName(), objToAdd->getSprite()->getPalName(), 0<<8, 0<<8, objToAdd->getSprite()->bGetAnimating() );

   if ( !oneFull || !twoFull || !threeFull )
   {
      //HelperDood * invItem = new HelperDood(pair<s32,s32>(x,y), objToAdd->bGetAnimating(), objToAdd->psGetFrames(), objToAdd->uGetAnimSpeed(), objToAdd->getCurrPage(), true, objToAdd->getSprite()->getName(), objToAdd->getCharType(), *(objToAdd->aiGetController()));
      //invItem->setSprite(spr);
      //PA_SetSpriteExtPrio(0,invItem->getSpriteID(),0);

      if(!oneFull)
      {
         objToAdd->setPos(0<<8,-64<<8);
         PA_SetSpriteXY(0,objToAdd->getSprite()->getId(),0,-64);
         invList[0] = objToAdd;
         oneFull = true;
      }
      else if(!twoFull)
      {
         objToAdd->setPos(64<<8,-64<<8);
         PA_SetSpriteXY(0,objToAdd->getSprite()->getId(),64,-64);
         invList[1] = objToAdd;
         twoFull = true;

      }
      else if(!threeFull)
      {
         objToAdd->setPos(128<<8,-64<<8);
         PA_SetSpriteXY(0,objToAdd->getSprite()->getId(),128,-64);
         invList[2] = objToAdd;
         threeFull = true;
      }

      PA_SetSpritePrio ( 0, objToAdd->getSprite()->getId(), 0 );
   }
}

void Inventory::removeItem (LevelObject * objToRemove )
{
   for(u8 i = 0;i < 3;++i)
   {
      if(invList[i] && invList[i]->getSpriteID() == objToRemove->getSpriteID())
      {
         invList[i] = NULL;
         if(i == 0){
            oneFull = false;
         }
         else if(i == 1){
            twoFull = false;
         }
         else if(i == 2){
            threeFull = false;
         }
         break;
      }
   }
}

bool Inventory::isFull()
{
   if(oneFull && twoFull && threeFull)
      return true;
   else 
      return false;
}

LevelObject * Inventory::loGetTouchedObj() 
 {
   u8 number = 1;
   for(u8 i = 0;i < 3;++i){
      if(invList[i]){
         if( PA_SpriteTouched( invList[i]->getSpriteID() ) ){
            return invList[i];
         }
      }
      number++;
   }
   return NULL;
 }

Inventory::~Inventory()
{
   delete garbage;
   delete spriteR1C1;
   delete spriteR1C2;
   delete spriteR1C3;
   delete spriteR1C4;
   delete spriteR2C4;
   for(u8 i = 0; i < 3; ++i)
   {
      if(invList[i])
      {
         delete invList[i];
      }
   }
}
