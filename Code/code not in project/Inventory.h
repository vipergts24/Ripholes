// 0 palib refs

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <PA9.h>
#include "../include/Sprite.h"
#include "../include/LevelObject.h"
#include "../include/HelperDood.h"
#include <utility>
#include <vector>
using namespace std;

class Inventory
{  
	public:

		Inventory();
		void update();
		//void setSprite(Sprite);
		//Sprite getSprite();
		void startCloseInventory();
		void startOpenInventory();
		void addItem(LevelObject* objToAdd);
		bool isFull();
		void removeItem();
      vector<LevelObject*> * getInvList(){return invList;}
	private:
		vector<LevelObject*> * invList;
		bool isOpen;
		bool isOpening;
		bool isClosing;
		bool oneFull;
		bool twoFull;
		bool threeFull;
		s32 x;    
		s32 y;
		s32 invScroll;
		Sprite * spriteR1C1;
		Sprite * spriteR1C2;
		Sprite * spriteR1C3;
		Sprite * spriteR1C4;
		Sprite * spriteR2C4;

}; 

#endif
