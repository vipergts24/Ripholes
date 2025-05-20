/*
   Ripholes in Rubbish
   Level Object Header (no cpp associated since everything is inline)
*/

#ifndef __LEVELOBJECT_H__
#define __LEVELOBJECT_H__

#include <utility>
#include <vector>
#include <PA9.h>
#include "hitBox.h"
#include "Sprite.h"
#include "Character.h"

#define LEVEL_LENGTH 64
#define GRAVITY 68
#define FRICTION 65

class LevelObject 
{
   public:
      //virtual Functions
      virtual u8 getCurrPage(){return pageId;}
      virtual pair<s32,s32> getPos(){return position;}
      virtual void setCurPage ( u8 uPId ) { pageId = uPId; }
      virtual void update ( bool bScroll, bool moveObjects, vector<LevelObject*> * objList, int coll ){}
      virtual void update ( bool drawSprites ){}
      virtual void update ( bool drawSprites, Sprite * magic){}
      virtual void setCM(int newCM){currCM = newCM;};
      virtual u8 getSpriteID(){return sprite->getId();}
      virtual Character getCharType(){return charType;}
      //GetTile returns the Tile Type for collisions.  There can be collisions dealing with only 
	  //specific parts of the object like jumping through a cloud yields no top collision but there
	  //is a bottom collision to stand on it.  Other collisions cause damage and others also are for
	  //AI patrol points
      virtual u8 GetTile(s16 x, s16 y)
      {
         return (my_buffer[0])[ (((y >>3)*(LEVEL_LENGTH)) + (x>>3)) * 2 ];
      }
      virtual void anyTileCollision()
      {
         aCollMap[0] = TileCollision ( hitbox->left,  hitbox->flipped );
         aCollMap[1] = TileCollision ( hitbox->right, hitbox->flipped );
         aCollMap[2] = TileCollision ( hitbox->up,    hitbox->flipped );
         aCollMap[3] = TileCollision ( hitbox->down,  hitbox->flipped );
      }
      virtual void anyObjectCollision(vector<LevelObject*>* vObjList)
      {
         aObjectColl[0] = ObjectCollision ( hitbox->left,  hitbox->flipped, vObjList );
         aObjectColl[1] = ObjectCollision ( hitbox->right, hitbox->flipped, vObjList );
         aObjectColl[2] = ObjectCollision ( hitbox->up,    hitbox->flipped, vObjList );
         aObjectColl[3] = ObjectCollision ( hitbox->down,  hitbox->flipped, vObjList );
      }
      virtual ~LevelObject(){};
      Sprite* getSprite(){return sprite;};

      //Setters
      void setPos(s32 x,s32 y){position.first=x;position.second=y;}
      void setSprite(Sprite* spr) { sprite = spr;}

      //Getters
      bool getIsRippable(){return isRippable;}
      hitBox * getHit(){return hitbox;}
	  virtual s32 getXVel(){return xVel;}
      virtual s32 getYVel(){return yVel;}
      bool isEvent;
      Sprite* sGetSprite() {return sprite;}

	  // Collision Types.  Colors are for the collision map, number is the u8 type
	  // If you looked at our collision map, the map the map would be identical to the level except various
      // colors indicate different properties as follows:
	  // 0: Magenta, empty
	  // 1: Black,   collision
	  // 2: Red,     death
	  // 3: Green,   bottom-only collision
	  // 4: Blue,    patrol turn-point 

	  //Collision Functions
      u8 TileCollision(hitBox::pointInfo point, u8 flipped)
      {
         return (GetTile(fix_norm(position.first)+point.x + (flipped*point.flipx), fix_norm(position.second)+point.y));
      }
     
	  //May want to move from being inline on future interations
      u8 ObjectCollision(hitBox::pointInfo point, u8 flipped,vector<LevelObject*>* vObjList)
      {
         vector<LevelObject*>::iterator loIter;
         s16 collide = 0;

         for(loIter = vObjList->begin(); loIter != vObjList->end(); loIter++)
         {
            if((*loIter)->getCharType() != HOLE || (*loIter)->getCharType() != REVERSEHOLE || (*loIter)->getCharType() != 1)
            {
               if(collide == 0)
               {
			      //Checks Character type from Character.h
                  collide = GetCharacter(fix_norm(position.first)+point.x + (flipped*point.flipx), fix_norm(position.second)+point.y, (*loIter));
               }
               else break;
            }
         }
         return collide;
      }

      //Gets the character Type (Enumerated value in Character.h)
      u8 GetCharacter(s16 x, s16 y, LevelObject * obj)
      {
         s16 x1 = fix_norm(obj->getPos().first)  + obj->getHit()->left.x + (obj->getHit()->flipped * obj->getHit()->left.flipx);
         s16 x2 = fix_norm(obj->getPos().first)  + obj->getHit()->right.x + (obj->getHit()->flipped * obj->getHit()->right.flipx);
         s16 y1 = fix_norm(obj->getPos().second) + obj->getHit()->up.y;
         s16 y2 = fix_norm(obj->getPos().second) + obj->getHit()->down.y;
         if((x1 < x) && (x < x2) && (y1 < y) && (y < y2))
         {
            return obj->getCharType();
         }
         else
         {
            return 0;
         }
      }
      //Checks if the object is touching the ground based off the tile
      virtual u8 TouchingGround()
      {
         return GetTile((fix_norm(position.first))+16, (fix_norm(position.second))+33);
      }
      //Checks if the object is resting on another sprite
      virtual u8 TouchingSpriteGround(vector<LevelObject*>* vObjList)
      {
         hitBox::pointInfo temp = hitbox->down;
         temp.y += 1;
         return ObjectCollision(temp, hitbox->flipped, vObjList);
      }

   protected:
      hitBox * hitbox;
      Sprite * sprite;
	  Sprite * miniSprite;
      pair<s32,s32> position;
      s32 xVel;
      s32 yVel;
      int currCM;
      u8 pageId;
      bool isRippable;
      bool isFrozen;
      bool isHole;
      s16 aCollMap[4];  //Collision Map indicating Collisions: 0 == left, 1 == right, 2 == up, 3 == down
      s16 aObjectColl[4];
      Character charType;
};

#endif  //_LEVELOBJECT_H
