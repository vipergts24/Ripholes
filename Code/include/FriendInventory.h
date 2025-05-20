// Ripholes in Rubbish Inventory class header

#ifndef __FRIENDINVENTORY_H__
#define __FRIENDINVENTORY_H__

#include <PA9.h>
#include <utility>
#include <vector>
#include "../include/HelperDood.h"
#include "../include/LevelObject.h"
#include "../include/Mode.h"
#include "../include/Sprite.h"

using namespace std;

class FriendInventory
{  
	public:

		FriendInventory();
      ~FriendInventory();
		void update();
		//void setSprite(Sprite);
		//Sprite getSprite();

      // check status of inventory
		void startCloseInventory();
		void startOpenInventory();
		bool isFull();

      // accessors
      bool bIsOpen(){return isOpen;}
      bool bisClosing(){return isClosing;}
      bool bisOpening(){return isOpening;}
      LevelObject** getInvList() {return invList;}
      LevelObject * loGetTouchedObj();
      // mutators
		void addItem(HelperDood* objToAdd);
      void removeItem (LevelObject * objToRemove );

	private:
		//vector<pair<LevelObject*, u8> >* invList;
      LevelObject * invList[3];
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
		Sprite * spriteR2C3;

}; 

#endif
