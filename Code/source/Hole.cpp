#include "../include/Hole.h"

Hole::Hole(bool downHole, pair<s32,s32> inPos, u8 pid, u8 random){
   position.first = inPos.first;
   position.second = inPos.second;
   pageId = pid;
   isHole = true;
   isRippable = false;
   hitbox = new hitBox("hole");
   string name;
   if(downHole)
   {

      switch(random )
      {
      case 0:
         name = "hole1";
         break;
      case 1:
         name = "hole2";
         break;
      case 2:
         name = "hole3";
         break;
      case 3:
         name = "hole4";
         break;
      }
      isDoingMagic = true;
      sprite = new Sprite(0, name, "GUI", position.first, position.second,false);
      charType = HOLE;
   }
   else
   {
      switch(random )
      {
      case 0:
         name = "riphole_marquee_hole1";
         break;
      case 1:
         name = "riphole_marquee_hole2";
         break;
      case 2:
         name = "riphole_marquee_hole4";
         break;
      case 3:
         name = "riphole_marquee_hole3";
         break;
      }
      sprite = new Sprite(0, name, "era0", position.first, position.second,false);
      sprite->animateSprite(0,1,10);
      isDoingMagic = false;
      charType = REVERSEHOLE;
   }
   PA_SetSpritePrio(0, this->getSpriteID(), 3);
}

void Hole::update(bool drawSprites, Sprite * magic)
{
   if(isDoingMagic && PA_GetSpriteAnimFrame(0, magic->getId()) == 6 )
   {
      isDoingMagic = false;
      magic->hideSprite();
   }
   else if(isDoingMagic)
   {
      sprite->hideSprite();
      magic->moveSprite(position.first, position.second);
   }

   else if (drawSprites)
   {
      sprite->moveSprite(position.first,position.second);
   }
   // draw the sprite off screen if it is on a page the player isn't looking at
   else 
   {
      sprite->hideSprite();
   }
}
u8 Hole::getCurrPage(){
   return pageId;
}
pair<s32,s32> Hole::getPos(){
   return position;
}

u8 Hole::getSpriteID(){
   return sprite->getId();
}
Hole::~Hole()
{
   delete sprite;
   delete hitbox;
}
