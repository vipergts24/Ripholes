#include "../include/Event.h"

// give it a position, page id, and type of event to create
Event::Event(pair<s32,s32> inPos, u8 pid, s16 sFrame, s16 eFrame, string name, Character type, string eventBgName)
{
   position.first = norm_fix(inPos.first);
   position.second = norm_fix(inPos.second);
   pageId = pid;
   isEvent = true;
   isRippable = false;
   bInSpeech = false;
   charType = type;
   StartFrame = sFrame;
   FrameCount = 0;
   EndFrame = eFrame;
   uBoxFrame = 0;
   EventBgName = eventBgName;

   hitbox = new hitBox("Event");

   // if it's not a background, make a sprite
   sprite = new Sprite(0, name.c_str(), "era0", position.first, position.second,true);
   sprite->hideSprite();


   PA_SetSpritePrio(0, this->getSpriteID(), 3);

   //sprite = new Sprite(0, "Event", "GUI", position.first, position.second);


}

// constructor for making background event
Event::Event(string name, Character type)
{
   isEvent = true;
   isRippable = false;
   bInSpeech = false;
   charType = type;  
   EventBgName = name;
}

Event::~Event(){
   if (charType == EVENT_SPEECH)
   {
      delete this->sprite;
      delete this->hitbox;
   }
   if (charType == BEGINLVL || charType == EVENTPOPUP)
   {
      delete bgBeginLvl;
   }
}

void Event::update(bool drawSprites)
{
   // only draw the sprite on-screen if the player is viewing the same page it lives on
   if (drawSprites)
   {
      sprite->moveSprite(position.first,position.second);
   }
   // draw the sprite off screen if it is on a page the player isn't looking at
   else 
   {
      sprite->hideSprite();
   }
}


u8 Event::getCurrPage(){
   return pageId;
}
pair<s32,s32> Event::getPos(){
   return position;
}

u8 Event::getSpriteID(){
   return sprite->getId();
}

// runs through the speech mode
void Event::doSpeech()
{
   bgBeginLvl = new Background(EventBgName,1,0,false);
}

void Event::setSpeech(bool speech)
{
   bInSpeech = speech;
}

void Event::nextFrame(Sprite * text)
{
   if(FrameCount != EndFrame)
   {
      text->setFrameAnimation(++FrameCount);
   }
   else
   {
      sprite->setFrameAnimation(0);
      text->hideSprite();
      bInSpeech = false;
   }
}

void Event::clearBG()
{
   delete bgBeginLvl;
}

void Event::drawBG ()
{
   bgBeginLvl =  new Background (EventBgName, 1, 0, false);
}
