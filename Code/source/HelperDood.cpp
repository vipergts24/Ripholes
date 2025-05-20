#include "../include/HelperDood.h"
#include "../include/Sprite.h"

HelperDood::HelperDood ( pair<s32,s32> inPos, bool isAnimating, pair<s16,s16> frames, u8 speed,  u8 pid, bool isRip, string name, Character character, vector<string> &vsBehavs )
{
   position.first = norm_fix(inPos.first);
   position.second = norm_fix(inPos.second);
   pageId = pid;
   xVel = 0;
   yVel = 0;
   isRippable = isRip;
   isFrozen = false;
   isHole = false;
   charType = character;
   bAnimating = isAnimating;
   psFrames.first  = frames.first;
   psFrames.second = frames.second;
   uSpeed = speed;

   if(name.substr(0,11) == "collectible")
   {
      sprite = new Sprite( 0, name.c_str(), "journal", position.first, position.second, isAnimating);
   }
   else
   {
      sprite = new Sprite( 0, name.c_str(), "era0", position.first, position.second, isAnimating);
   }
   sprite->hideSprite();

   if( isAnimating )
   {
      sprite->animateSprite ( frames.first, frames.second, speed);
   }

   hitbox = new hitBox ( name.c_str() );
   PA_SetSpritePrio    ( 0, this->getSpriteID(), 2 );

   aiCont = new AIController ( vsBehavs );
}

HelperDood::HelperDood ( pair<s32,s32> inPos, bool isAnimating, pair<s16,s16> frames, u8 speed,  u8 pid, bool isRip, string name, Character character, AIController &aiCopyMe )
{
   position.first = norm_fix(inPos.first);
   position.second = norm_fix(inPos.second);
   pageId = pid;
   xVel = 0;
   yVel = 0;
   isRippable = isRip;
   isFrozen = false;
   isHole = false;
   sprite = new Sprite( 0, name.c_str(), "era0", position.first, position.second, isAnimating );
   charType = character;
   bAnimating = isAnimating;
   psFrames.first  = frames.first;
   psFrames.second = frames.second;
   uSpeed = speed;

   if( isAnimating )
   {
      sprite->animateSprite ( frames.first, frames.second, speed);
   }

   hitbox = new hitBox ( name.c_str() );
   PA_SetSpriteExtPrio ( 0, this->getSpriteID(), 2 );

   aiCont = new AIController ( aiCopyMe );
}

HelperDood::HelperDood ( HelperDood& hdDood )
{
   // HelperDood members
   this->aiCont      = new AIController ( *(hdDood.aiCont) );

   // LevelObject members
   this->hitbox = new hitBox ( *(hdDood.hitbox) );
   this->sprite = new Sprite ( hdDood.getSprite()->getScreen(),
                               hdDood.getSprite()->getName(),
                               hdDood.getSprite()->getPalName(),
                               256, 192, hdDood.getSprite()->bGetAnimating() );
   this->position.first  = hdDood.position.first;
   this->position.second = hdDood.position.second;
   this->xVel            = hdDood.xVel;
   this->yVel            = hdDood.yVel;
   this->currCM          = hdDood.currCM;
   this->pageId          = hdDood.pageId;
   this->isRippable      = hdDood.isRippable;
   this->isFrozen        = hdDood.isFrozen;
   this->isHole          = hdDood.isHole;
   this->aCollMap[0]     = hdDood.aCollMap[0];
   this->aCollMap[1]     = hdDood.aCollMap[1];
   this->aCollMap[2]     = hdDood.aCollMap[2];
   this->aCollMap[3]     = hdDood.aCollMap[3];
   this->aObjectColl[0]  = hdDood.aObjectColl[0];
   this->aObjectColl[1]  = hdDood.aObjectColl[1];
   this->aObjectColl[2]  = hdDood.aObjectColl[2];
   this->aObjectColl[3]  = hdDood.aObjectColl[3];
   this->charType        = hdDood.charType;
}

void HelperDood::update (bool drawSprites, bool moveObjects, vector<LevelObject*> * objList, int coll)
{
   pair<s32,s32> pNewPos; // Used to update the dood's position
   currCM = coll;   // Make sure collision map is current
   

   // Check collisions and update position if the dood isn't static
   
   //PA_OutputText(1,0,19,"%d %d %d %d", (my_buffer[currCM])[0], (my_buffer[currCM])[2], (my_buffer[currCM])[4], (my_buffer[currCM])[6]);
   if ( !(aiCont->bStatic()) && drawSprites )
   {
      //PA_OutputText(1,0,2, "%d", currCM);
      //PA_OutputText(1,0,18,"%d %d %d %d", (my_buffer[currCM])[0], (my_buffer[currCM])[2], (my_buffer[currCM])[4], (my_buffer[currCM])[6]);

      moveDood ( objList );
   }

   // only draw the sprite on-screen if the player is viewing the same page it lives on
   if ( drawSprites )
   {
      sprite->moveSprite( position.first, position.second );
   }
   // else dont draw the sprites because the player is not on the right page to see them
   else 
   {
      sprite->hideSprite();
   }
}

// used for collectible
void HelperDood::update (bool drawSprites )
{
   if ( drawSprites )
   {
      sprite->moveSprite( position.first, position.second );
   }
   // else dont draw the sprites because the player is not on the right page to see them
   else 
   {
      sprite->hideSprite();
   }
}

u8 HelperDood::getCurrPage()
{
   return pageId;
}

pair<s32,s32> HelperDood::getPos()
{
   return position;
}

u8 HelperDood::getSpriteID()
{
   return sprite->getId();
}

HelperDood::~HelperDood()
{
   delete sprite;
   delete hitbox;
	delete aiCont;
}

void HelperDood::moveDood(vector<LevelObject*>* vObjList)
{
   
   pair<s32,s32> pNewVel = make_pair ( (s32)0, (s32)0 );

   
   

   // Initial collision test
   
   
   
   anyTileCollision ();
   
   anyObjectCollision ( vObjList );
   


   // Set velocities for processing
   xVel = 0;
   if ( aCollMap[COLL_BOTTOM] == COLL_BLK ) yVel = 0;
   if ( aCollMap[COLL_BOTTOM] == COLL_GRN ) yVel = 0;

   // Check with the AI behaviors and adjust position
   pair<s32,s32> pCurVel = make_pair ( xVel, yVel );
   
   pNewVel = aiCont->pUpdate ( pCurVel, aCollMap, aObjectColl );
   doodTileCollisions ();
   doodObjectCollisions(vObjList);
   
   xVel += pNewVel.first;
   yVel += pNewVel.second;

   // Adjust velocities
   if( !TouchingGround() || !TouchingSpriteGround ( vObjList ) )
   {
      yVel += GRAVITY;
   }

   // Fix position
   position.first  += xVel;
   position.second += yVel;

   // Flip sprite based on movement direction
   if ( xVel >= 0 && charType != PLATFORM)
   {
      getSprite()->hFlip(0);
      hitbox->flipped = 1;
   }
   else
   {
      getSprite()->hFlip(1);
      hitbox->flipped = 0;
   }

   // Adjust position based on collision map
   //while ( aCollMap[0] == 1 || aCollMap[1] == 1 || aCollMap[2] == 1 || aCollMap[3] == 1 )
   //{
      
   //}

   //while ( aObjectColl[0] == STATIC || aObjectColl[1] == STATIC || aObjectColl[2] == STATIC || aObjectColl[3] == STATIC )
   //{
      
   //}
   
      
}

void HelperDood::doodTileCollisions()
{
   anyTileCollision();
   while(aCollMap[0] == 1) // Collision on the left of the sprite...
   { 
      position.first+= norm_fix(1); // Move by 1 pixel...
      anyTileCollision();
   }

   while(aCollMap[1] == 1) // Collision on the right of the sprite...
   { 
      position.first-= norm_fix(1); // Move by 1 pixel...
      anyTileCollision();
   }

   while(aCollMap[2] == 1) // Collision with the something on top
   {
      position.second+=norm_fix(1);
      yVel=0;
      anyTileCollision();
   } 

   while(aCollMap[3] == 1) // Collision on the bottom of the sprite...
   {
      position.second -= norm_fix(1); // Move by 1/2 pixel...
      anyTileCollision();
   }

   // Fix y-vel
   if( (TouchingGround()==1 || TouchingGround()==3) && yVel>0)
   {
      yVel = 0;
   }
}

void HelperDood::doodObjectCollisions(vector<LevelObject*> * objList)
{
   anyObjectCollision ( objList );
   while(aObjectColl[0] == STATIC) // Collision on the left of the sprite...
   { 
      position.first+= norm_fix(1); // Move by 1 pixel...
      anyObjectCollision ( objList );
   }

   while(aObjectColl[1] == STATIC) // Collision on the right of the sprite...
   { 
      position.first-= norm_fix(1); // Move by 1 pixel...
      anyObjectCollision ( objList );
   }

   while(aObjectColl[2] == STATIC) // Collision with the something on top
   {
      position.second+=norm_fix(1);
      yVel=0;
      anyObjectCollision ( objList );
   } 

   while(aObjectColl[3] == STATIC) // Collision on the bottom of the sprite...
   {
      position.second -= norm_fix(1); // Move by 1/2 pixel...
      anyObjectCollision ( objList );
   }

   // Fix y-vel
   if( (TouchingSpriteGround(objList)== STATIC || TouchingSpriteGround(objList)== PLATFORM) && yVel>0)
   {
      yVel = 0;
   }

}
