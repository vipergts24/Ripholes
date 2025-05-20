#ifndef __COLLECTIBLE_H__
#define __COLLECTIBLE_H__

#include "LevelObject.h"
#include "hitBox.h"
#include "Sprite.h"
#include <utility>
#include <PA9.h>

class Collectible : public LevelObject
{
   public:
      Collectible(pair<s32,s32> inPos, u8 pid, u8 levelId, string name);
      virtual void update(bool bDraw);
		virtual u8 getCurrPage();
		virtual pair<s32,s32> getPos();
		virtual u8 getSpriteID();
		virtual ~Collectible();
      virtual hitBox * getHit(){return hitbox;}
      virtual Sprite * getSprite(){return sprite;};
      void setCollected(bool isCol);
      bool getCollected(){return isCollected;}

   private:
      bool isCollected;
      u8 uLevelId;
};

#endif  //_COLLECTIBLE_H
