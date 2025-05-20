// palib refs all over the place

#include "../include/Inventory.h"
#include "gfx/inv_gfx.cpp"
#include "gfx/inv_gfx.h"

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
   //Vector <pair <Sprite,bool> > spriteSlots;
	//PA_DualLoadSpritePal(10, // Palette number
	//					(void*)inv_Pal);	// Palette name
   PA_LoadSpritePal(0,10,(void*)inv_Pal);
	//spriteR1C1 = new Sprite(2, 1, (void*) inventory_row1col1_Sprite, OBJ_SIZE_64X64, 10, 0<<8, 128<<8);
	//spriteR1C2 = new Sprite(2, 1, (void*) inventory_row1col2_Sprite, OBJ_SIZE_64X64, 10, 64<<8, 128<<8);
	//spriteR1C3 = new Sprite(2, 1, (void*) inventory_row1col3_Sprite, OBJ_SIZE_64X64, 10, 128<<8, 128<<8);
	//spriteR1C4 = new Sprite(2, 1, (void*) inventory_row1col4_Sprite, OBJ_SIZE_64X64, 10, 192<<8, 128<<8);
	//spriteR2C4 = new Sprite(2, 0, (void*) inventory_row2col4_Sprite, OBJ_SIZE_64X64, 10, 192<<8, 192<<8);
   spriteR1C1 = new Sprite(1, 0, (void*) inventory_row1col1_Sprite, OBJ_SIZE_64X64, 10, x<<8, y<<8);
	spriteR1C2 = new Sprite(1, 0, (void*) inventory_row1col2_Sprite, OBJ_SIZE_64X64, 10, x<<8, y<<8);
	spriteR1C3 = new Sprite(1, 0, (void*) inventory_row1col3_Sprite, OBJ_SIZE_64X64, 10, x<<8, y<<8);
	spriteR1C4 = new Sprite(1, 0, (void*) inventory_row1col4_Sprite, OBJ_SIZE_64X64, 10, x<<8, y<<8);
	spriteR2C4 = new Sprite(1, 0, (void*) inventory_row2col4_Sprite, OBJ_SIZE_64X64, 10, x+192<<8, y+64<<8);
   
   PA_InitSpriteExtPrio(1);//Enable sprite Priority
   PA_SetSpriteExtPrio(0, spriteR1C1->getId(), 1);
   PA_SetSpriteExtPrio(0, spriteR1C2->getId(), 1);
   PA_SetSpriteExtPrio(0, spriteR1C3->getId(), 1);
   PA_SetSpriteExtPrio(0, spriteR1C4->getId(), 1);
   invList = new vector<LevelObject* >;
}
void Inventory::update()
{
	if(Stylus.Newpress && PA_SpriteStylusOver(spriteR2C4->getId()))
		{
			if(isOpen)isOpen = false;
			else isOpen = true;
		}
	if(isOpen && y + invScroll <= 0)
		{
			//PA_DualSetSpriteXY(spriteR1C1->getId(), // sprite
			//					   x, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR1C2->getId(), // sprite
			//					   x+64, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR1C3->getId(), // sprite
			//					   x+128, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR1C4->getId(), // sprite
			//					   x+192, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR2C4->getId(), // sprite
			//					   x+192, // x position
			//					   y+64+invScroll); // y...
         PA_SetSpriteXY(0,spriteR1C1->getId(),x,y+invScroll);
			PA_SetSpriteXY(0,spriteR1C2->getId(),x+64,y+invScroll);
         PA_SetSpriteXY(0,spriteR1C3->getId(),x+128,y+invScroll);
         PA_SetSpriteXY(0,spriteR1C4->getId(),x+192,y+invScroll);
         PA_SetSpriteXY(0,spriteR2C4->getId(),x+192,y+64+invScroll);
         vector<LevelObject *>::iterator itor = invList->begin();
         vector<LevelObject *>::iterator end = invList->end();
         for(;itor!=end;++itor){
            //(*itor)->getSprite()->moveSprite(x<<8,(y+invScroll)<<8);
            PA_SetSpriteXY(0, (*itor)->getSprite()->getId(), x, y+invScroll);
         }
			invScroll += 5;
		}
	else if(!isOpen && y + invScroll >= -64){
		
			//PA_DualSetSpriteXY(spriteR1C1->getId(), // sprite
			//					   x, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR1C2->getId(), // sprite
			//					   x+64, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR1C3->getId(), // sprite
			//					   x+128, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR1C4->getId(), // sprite
			//					   x+192, // x position
			//					   y+invScroll); // y...
			//PA_DualSetSpriteXY(spriteR2C4->getId(), // sprite
			//			 x+192, // x position
			//			 y+64+invScroll); // y...
         PA_SetSpriteXY(0,spriteR1C1->getId(),x,y+invScroll);
			PA_SetSpriteXY(0,spriteR1C2->getId(),x+64,y+invScroll);
         PA_SetSpriteXY(0,spriteR1C3->getId(),x+128,y+invScroll);
         PA_SetSpriteXY(0,spriteR1C4->getId(),x+192,y+invScroll);
         PA_SetSpriteXY(0,spriteR2C4->getId(),x+192,y+64+invScroll);	
         vector<LevelObject *>::iterator itor = invList->begin();
         vector<LevelObject *>::iterator end = invList->end();
         for(;itor!=end;++itor){
            //(*itor)->getSprite()->moveSprite(x<<8,(y+invScroll)<<8);
            PA_SetSpriteXY(0, (*itor)->getSprite()->getId(), x, y+invScroll);
         }
			invScroll -= 5;
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

}
void Inventory::startOpenInventory()
{
   
}
void Inventory::addItem(LevelObject* objToAdd)
{
   Sprite* spr = new Sprite(1, 0, objToAdd->getSprite()->name, objToAdd->getSprite()->shape, objToAdd->getSprite()->size, objToAdd->getSprite()->pallette, 0<<8, 0<<8);
   if(!oneFull)
	{
      HelperDood * invItem = new HelperDood(pair<s32,s32>(x,y),objToAdd->getCurrPage(),true,"garageleft");
		objToAdd->getSprite()->deleteSprite(0);
      invItem->setSprite(spr);
      PA_SetSpriteExtPrio(0,invItem->getSpriteID(),0);
      invItem->setPos(x<<8,y<<8);
      PA_SetSpriteXY(0,invItem->getSprite()->getId(),x,y);
      invList->push_back(invItem);
      //objToAdd->setSprite(spr);
      //PA_SetSpriteExtPrio(0, objToAdd->getSpriteID(), 0);
		//objToAdd->setPos(x<<8,y<<8);
      //objToAdd->getSprite()->moveSprite(100<<8,100<<8);
      PA_SetSpriteXY(0, objToAdd->getSprite()->getId(), x, y);
      invList->push_back(objToAdd);
      oneFull = true;
	}
	else if(!twoFull)
	{
      //objToAdd->getSprite()->deleteSprite(0);
		//objToAdd->setSprite(spr);
      PA_SetSpriteExtPrio(0, objToAdd->getSpriteID(), 0);
		objToAdd->setPos(74<<8,128<<8);
      twoFull = true;
		//objToAdd->getSprite()->moveSprite(64<<8, 128<<8);

	}

	else if(!threeFull)
	{
		//objToAdd->setSprite(spr);
		objToAdd->setPos(192,128);
		objToAdd->getSprite()->moveSprite(192<<8, 128<<8);

	}

}
/*
void Inventory::addItem(LevelObject* objToAdd)
{   
	Sprite* spr = new Sprite(1, 1, objToAdd->getSprite()->name, objToAdd->getSprite()->shape, objToAdd->getSprite()->size, objToAdd->getSprite()->pallette, 0<<8, 128<<8);
	if(!oneFull)
	{
		objToAdd->getSprite()->deleteSprite(0);
		objToAdd->setSprite(spr);
      PA_SetSpriteExtPrio(1, objToAdd->getSpriteID(), 0);
		objToAdd->setPos(0<<8,128<<8);
      oneFull = true;
	}
	else if(!twoFull)
	{
      objToAdd->getSprite()->deleteSprite(0);
		objToAdd->setSprite(spr);
      PA_SetSpriteExtPrio(1, objToAdd->getSpriteID(), 1);
		objToAdd->setPos(74<<8,128<<8);
      twoFull = true;
		//objToAdd->getSprite()->moveSprite(64<<8, 128<<8);

	}

	else if(!threeFull)
	{
		objToAdd->setSprite(spr);
		objToAdd->setPos(192,128);
		objToAdd->getSprite()->moveSprite(192<<8, 128<<8);

	}
   
}
*/
void Inventory::removeItem()
{
   
}

bool Inventory::isFull()
{
	if(oneFull && twoFull && threeFull)
		return true;
	else 
		return false;
}
