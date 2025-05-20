// palib refs all over the place

#include "../include/FriendInventory.h"

using namespace std;

FriendInventory::FriendInventory(): isOpen(false),
isOpening(false),
isClosing(false),
oneFull(false),
twoFull(false),
threeFull(false),
x(0),    
y(-64),
invScroll(0)
{
   spriteR1C1 = new Sprite(0, "friends_row1col1", "GUI", x<<8, y<<8, true );
   spriteR1C2 = new Sprite(0, "friends_row1col2", "GUI", x<<8, y<<8, true );
   spriteR1C3 = new Sprite(0, "friends_row1col3", "GUI", x<<8, y<<8, true );
   spriteR2C3 = new Sprite(0, "friends_row2col3", "GUI", x+128<<8, y+64<<8, true );

   PA_SetSpriteExtPrio(0, spriteR1C1->getId(), 1);
   PA_SetSpriteExtPrio(0, spriteR1C2->getId(), 1);
   PA_SetSpriteExtPrio(0, spriteR1C3->getId(), 1);
   PA_SetSpriteExtPrio(0, spriteR2C3->getId(), 1);
   PA_SetSpritePrio(0, spriteR1C1->getId(), 1);
   PA_SetSpritePrio(0, spriteR1C2->getId(), 1);
   PA_SetSpritePrio(0, spriteR1C3->getId(), 1);
   PA_SetSpritePrio(0, spriteR2C3->getId(), 1);
   for(u8 i = 0;i < 3;++i) invList[i] = NULL;
}

void FriendInventory::update()
{
   if(Stylus.Newpress && PA_StylusInZone(128,0,192,20))
   {
      if(isOpen)isClosing = true;
      else isOpening = true;
   }
   if(isOpening && y + invScroll <= 0)
   {
      PA_SetSpriteXY(0,spriteR1C1->getId(),x,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C2->getId(),x+64,y+invScroll);
      PA_SetSpriteXY(0,spriteR1C3->getId(),x+128,y+invScroll);
      PA_SetSpriteXY(0,spriteR2C3->getId(),x+128,y+64+invScroll);
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
      PA_SetSpriteXY(0,spriteR2C3->getId(),x+128,y+64+invScroll);	
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

void FriendInventory::startCloseInventory()
{
   //isOpen = false;
   isClosing = true;
}

void FriendInventory::startOpenInventory()
{
   //isOpen = true;
   isOpening = true;
}

void FriendInventory::addItem(HelperDood* objToAdd)
{
   Sprite * spr = new Sprite(0, objToAdd->getSprite()->getName(), objToAdd->getSprite()->getPalName(), 0<<8, 0<<8, objToAdd->getSprite()->bGetAnimating());
   if(!oneFull)
   {
      HelperDood * invItem = new HelperDood(pair<s32,s32>(x,y), false, pair<s16,s16>(0,0), 0, objToAdd->getCurrPage(), true, objToAdd->getSprite()->getName(), objToAdd->getCharType(), *(objToAdd->aiGetController()));
      delete objToAdd->getSprite();
      invItem->setSprite(spr);
      PA_SetSpriteExtPrio(0,invItem->getSpriteID(),0);
      invItem->setPos(0<<8,-64<<8);
      PA_SetSpriteXY(0,invItem->getSprite()->getId(),0,-64);
      invList[0] = invItem;
      oneFull = true;
   }
   else if(!twoFull)
   {
      HelperDood * invItem = new HelperDood(pair<s32,s32>(x,y), false, pair<s16,s16>(0,0), 0, objToAdd->getCurrPage(), true, objToAdd->getSprite()->getName(), objToAdd->getCharType(), *(objToAdd->aiGetController()));
      delete objToAdd->getSprite();
      invItem->setSprite(spr);
      PA_SetSpriteExtPrio(0,invItem->getSpriteID(),0);
      invItem->setPos(64<<8,-64<<8);
      PA_SetSpriteXY(0,invItem->getSprite()->getId(),64,-64);
      invList[1] = invItem;
      twoFull = true;

   }

   else if(!threeFull)
   {
      HelperDood * invItem = new HelperDood(pair<s32,s32>(x,y), false, pair<s16,s16>(0,0), 0, objToAdd->getCurrPage(), true, objToAdd->getSprite()->getName(), objToAdd->getCharType(), *(objToAdd->aiGetController()));
      delete objToAdd->getSprite();
      invItem->setSprite(spr);
      PA_SetSpriteExtPrio(0,invItem->getSpriteID(),0);
      invItem->setPos(128<<8,-64<<8);
      PA_SetSpriteXY(0,invItem->getSprite()->getId(),128,-64);
      invList[2] = invItem;
      threeFull = true;

   }

}

void FriendInventory::removeItem (LevelObject * objToRemove )
{
   for(u8 i = 0;i < 3;++i){
      if(invList[i] && invList[i]->getSpriteID() == objToRemove->getSpriteID()){
         delete invList[i];
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

bool FriendInventory::isFull()
{
   if(oneFull && twoFull && threeFull)
      return true;
   else 
      return false;
}

LevelObject * FriendInventory::loGetTouchedObj() 
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

FriendInventory::~FriendInventory()
{
	delete spriteR1C1;
	delete spriteR1C2;
	delete spriteR1C3;
	delete spriteR2C3;
   for(u8 i = 0; i < 3; ++i)
   {
      if(invList[i])
      {
         delete invList[i];
      }
   }
}
