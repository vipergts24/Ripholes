#ifndef __HOLE_H__
#define __HOLE_H__

#include "LevelObject.h"
#include "hitBox.h"
#include "Sprite.h"
#include <PA9.h>

using namespace std;

class Hole : public LevelObject
{

	public:

		Hole(bool downHole, pair<s32,s32> inPos, u8 pid, u8 random);

		virtual void update(bool drawSprites, Sprite * magic);
		virtual u8 getCurrPage();
		virtual pair<s32,s32> getPos();
		virtual u8 getSpriteID();
      Sprite* getMagic() {return magic;};
      void eraseMagic() {delete magic;};
		~Hole();
   private:
      Sprite * magic;
      bool isDoingMagic;
};

#endif
