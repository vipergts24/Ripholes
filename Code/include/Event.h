#ifndef __EVENT_H__
#define __EVENT_H__

#include <PA9.h>
#include "LevelObject.h"
#include "hitBox.h"
#include "Background.h"

using namespace std;

class Event : public LevelObject
{

public:

   Event(pair<s32,s32> inPos, u8 pid, s16 sFrame, s16 eFrame, string name, Character type, string EventBgName);
   Event(string name, Character type);
   ~Event();

   bool isEvent;
   bool getInSpeech(){return bInSpeech;};

   virtual void update(bool drawSprites);
   virtual u8 getCurrPage();
   virtual pair<s32,s32> getPos();
   virtual u8 getSpriteID();
   void clearBG();
   void drawBG ();

   // speech event occurs on touch of speech box in level.cpp
   void doSpeech();
   void setSpeech(bool speech);
   void nextFrame(Sprite * text);

   u8 getFrameCount(){return FrameCount;}
   u8 getEndFrame(){return EndFrame;}

private:
   Background* bgBeginLvl;
   bool bInSpeech;
   s16 StartFrame;
   s16 FrameCount;
   s16 EndFrame;
   u8 uBoxFrame;
   string EventBgName;

};

#endif
