#include "../include/Collectible.h"

Collectible::Collectible(pair<s32,s32> inPos, u8 pid, u8 levelId, string name)
{
   position.first = norm_fix(inPos.first);
   position.second = norm_fix(inPos.second);
   pageId = pid;
   uLevelId = levelId;
   xVel = 0;
   yVel = 0;
   isCollected = false;
   isRippable = false;
   isFrozen = false;
   isHole = false;
   sprite = new Sprite(0,name.c_str(),"era0",position.first,position.second, false);
   hitbox = new hitBox ( name );
   charType = COLLECTIBLE;
   PA_SetSpriteExtPrio(0, this->getSpriteID(), 2);
}

void Collectible::update(bool bDraw)
{
   if ( bDraw && !isCollected)
   {
      sprite->moveSprite(position.first,position.second);
   }
   else 
   {
      sprite->hideSprite();
   }

}
void Collectible::setCollected(bool isCol)
{
   isCollected = isCol;
}
u8 Collectible::getCurrPage()
{
   return pageId;
}
pair<s32,s32> Collectible::getPos()
{
   return position;
}
u8 Collectible::getSpriteID()
{
   return sprite->getId();
}
Collectible::~Collectible()
{
   delete this->sprite;
   delete this->hitbox;
}
