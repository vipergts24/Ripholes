#ifndef __HELPERDOOD_H__
#define __HELPERDOOD_H__

#include <utility>
#include <vector>
#include <PA9.h>
#include "AIController.h"
#include "LevelObject.h"
#include "hitBox.h"
#include "Sprite.h"

using namespace std;

class HelperDood : public LevelObject
{
   public:
      HelperDood ( pair<s32,s32> inPos, bool isAnimating, pair<s16,s16> frames, u8 speed,  u8 pid, bool isRip, string name, Character character, vector<string> &vsBehavs );
      HelperDood ( pair<s32,s32> inPos, bool isAnimating, pair<s16,s16> frames, u8 speed,  u8 pid, bool isRip, string name, Character character, AIController &aiCopyMe );
      HelperDood ( HelperDood& hdDood );
      ~HelperDood();

      virtual void update ( bool drawSprites, bool moveObjects, vector<LevelObject*> * objList, int coll );
      virtual void update ( bool drawSprites );

      virtual u8 getCurrPage();
      virtual pair<s32,s32> getPos();
      virtual u8 getSpriteID();
      AIController* aiGetController () { return aiCont;     };
      bool bGetAnimating ()            { return bAnimating; };
      pair<s16,s16> psGetFrames ()     { return psFrames;   };
      u8 uGetAnimSpeed ()              { return uSpeed;     };
      
      virtual void doSpeechEvent(){};
      void moveDood(vector<LevelObject*>* vObjList);
      void doodTileCollisions();
      void doodObjectCollisions(vector<LevelObject*> * objList);

   private:
      AIController* aiCont;
      pair<s16,s16> psFrames;
      u8 uSpeed;
      bool bAnimating;
};

#endif
