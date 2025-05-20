/*
   Ripholes in Rubbish
   Player Class Source

*/
#include "../include/Player.h"


using namespace std;

#define norm_fix(x)			((x)<<8)
#define fix_norm(x)			((x)>>8)  //xxx truncates instead of rounds
#define fix_mult(x,y)		(((x)*(y))>>8)
#define ratio_fix(x,y)	((256*(x))/(y))
#define fixed_fraction(x)			((x)&&0xff)
#define CAMERA_TURN_AROUND 2
#define MAX_SPEED 500
#define MIN_SPEED 100
#define ACCEL 80
#define LEVELLENGTHPIXELS 512
#define FREEZE_TIME 60

//Player Constructor.  Position, era, and ID are read from the data file
Player::Player(pair<s32,s32> inPos, u8 era, u8 pid)
{
	position.first = norm_fix(inPos.first);
	position.second = norm_fix(inPos.second);
	pageId      = pid;
	hitWall     = false;
	xVel        = 0;
	yVel        = 0;
	hitCount    = 0;
	freezeCount = 0;
	isRippable  = false;
	isFrozen    = false;
	isHole      = false;
	isJumping   = false;
	isAlive     = true;
	atGoal      = false;
	onSprite    = false;
    startMoving = false;
	freezeControls = false;
	onPlatform  = false;
    bPlayerCollected = false;  // player has not collected collectible media yet
	//Creates a hitbox class to keep track of a rectangle around the player for collision detection
	hitbox      = new hitBox("player");
    charType    = PLAYER;
    
	//First Iteration had only one era (child), future iterations will include era for preteen, teen, and college phases
	if(era == 0)
	{
	  sprite     = new Sprite(0, "childAvatar", "childAvatar", position.first, position.second, true );
      sprite->hideSprite();
	  miniSprite = new Sprite(1, "avatar_mini_head", "childAvatar", position.first/4, position.second/4, false );
      miniSprite->hideSprite();
	}
    //Sets the sprite priority to the top so no other sprites will cover the player
    PA_SetSpritePrio (0, sprite->getId(), 1); 
    PA_SetSpritePrio (0, miniSprite->getId(), 1);

	sprite->scroll.first  = 0;
	sprite->scroll.second = 0;

   Scroll(true);
}
//Player Destructor
Player::~Player()
{
	delete hitbox;
	delete sprite;
	delete miniSprite;
}
//Increments how many times the player is hit successively
void Player::addHit() 
{
	hitCount += 1;
}

//Handles avatar Movement.  Future iteration should put this into a few functions
//as opposed to one large function
void Player::Move(vector<LevelObject*> * objList)
{
	//if the avatar presses right set the sprite animation to go to the right
	//set the flip to 0 since it's the default direction
	if((Pad.Newpress.Right || Pad.Newpress.Left) && (TouchingGround() || TouchingSpriteGround(objList)))
	{
		sprite->animateSprite(6, 8, 6);
	}
    //Handles Animation and hitbox if the right button is pressed
	//Hitbox is not necessarily a square so when the avatar sprite flips directions the hitbox must flip too
	if(Pad.Newpress.Right) 
	{
		sprite->animateSprite(6, 8, 6);
		sprite->hFlip(0);
		u8 tempFlip = hitbox->flipped;
		hitbox->flipped = 0;
		if(tempFlip != 0) playerJustFlipped = true;
	}
	//does the exactly the same but towards the left
	else if(Pad.Newpress.Left) 
	{
		sprite->animateSprite(6, 8, 6);
		sprite->hFlip(1);
		u8 tempFlip = hitbox->flipped;
		hitbox->flipped = 1;
		if(tempFlip != 1) playerJustFlipped = true;
	}

    //Starts Jumping if A is pressed and the avatar is not in the air
	if ((Pad.Newpress.A) && (TouchingGround() || TouchingSpriteGround(objList)))
	{  // If pressed A and not in the air
		yVel = -1400; // Start jumping
		isJumping = true;
		playJump();//Playes Jump Sound Effect
	}

	//This code gives variable sized jumps depending upon how long the player pressed down the A button
	if(isJumping && !Pad.Held.A && yVel <-400)
	{
		yVel =-400;
	}		

	// Moving Code
	if(!freezeControls)//If controls aren't frozen
	{
		if(Pad.Newpress.Right && xVel == 0) xVel = MIN_SPEED;

		if(Pad.Newpress.Left && xVel == 0) xVel = -MIN_SPEED;

		if((!(TouchingGround() || TouchingSpriteGround(objList))) && (Pad.Held.Right) && xVel < 512) xVel = 512;

		if((!(TouchingGround() || TouchingSpriteGround(objList))) && (Pad.Held.Left) && xVel > -512) xVel = -512;
	}
    //If the avatar is on a platform, it should move with the platform
	if(TouchingSpriteGround(objList)==PLATFORM || TouchingSpriteGround(objList)==FLUFFS)
	{
		vector<LevelObject*>::iterator itor = objList->begin();
		vector<LevelObject*>::iterator end  = objList->end();
		for(;itor!=end;++itor)
		{
			if((*itor)->getCharType()==PLATFORM || (*itor)->getCharType()==FLUFFS)
			{
				xVel = (*itor)->getXVel();
				//Since the avatar gets the velocity of the object it is on, the friction
				//should be cancelled or else the avatar will slide off
				if((*itor)->getXVel() >= 0)
					xVel += FRICTION;
				else xVel -= FRICTION;
			}
		}
	}
    //If the avatar is moving to the right, handle acceleration and friction
	if ((Pad.Held.Right) && ((TouchingGround() || TouchingSpriteGround(objList))) && xVel < MAX_SPEED)
	{
		xVel += ACCEL - FRICTION;
		if(TouchingSpriteGround(objList)==PLATFORM || TouchingSpriteGround(objList)==FLUFFS)
		{
			vector<LevelObject*>::iterator itor = objList->begin();
			vector<LevelObject*>::iterator end  = objList->end();
			for(;itor!=end;++itor)
			{
				if((*itor)->getCharType()==PLATFORM || (*itor)->getCharType()==FLUFFS)
				{
					if((*itor)->getXVel() >= 0)
						xVel = xVel*2 + (*itor)->getXVel();
					else xVel = xVel*-2 + (*itor)->getXVel();
				}
			}
		}
	}
	else if((!Pad.Held.Right) && ((TouchingGround() || TouchingSpriteGround(objList))) && xVel > 0)
	{
		xVel -= FRICTION;
		if(xVel < 0) xVel = 0;
	}
	else if ((Pad.Held.Right) && ((TouchingGround() || TouchingSpriteGround(objList))) && xVel < 0)
	{
		xVel += ACCEL + FRICTION;
		if(TouchingSpriteGround(objList)==PLATFORM || TouchingSpriteGround(objList)==FLUFFS)
		{
			vector<LevelObject*>::iterator itor = objList->begin();
			vector<LevelObject*>::iterator end  = objList->end();
			for(;itor!=end;++itor)
			{
				if((*itor)->getCharType()==PLATFORM || (*itor)->getCharType()==FLUFFS)
				{
					if((*itor)->getXVel() >= 0)
						xVel = xVel*2 + (*itor)->getXVel();
					else xVel = xVel*-2 + (*itor)->getXVel();
				}
			}
		}
	}
	//If the avatar is moving to the left, handle acceleration and friction
	if ((Pad.Held.Left) && ((TouchingGround() || TouchingSpriteGround(objList))) && xVel > -MAX_SPEED)
	{
		xVel -= ACCEL - FRICTION;
		if(TouchingSpriteGround(objList)==PLATFORM || TouchingSpriteGround(objList)==FLUFFS)
		{
			vector<LevelObject*>::iterator itor = objList->begin();
			vector<LevelObject*>::iterator end  = objList->end();
			for(;itor!=end;++itor)
			{
				if((*itor)->getCharType()==PLATFORM || (*itor)->getCharType()==FLUFFS)
				{
					if((*itor)->getXVel() >= 0)
						//xVel -= (*itor)->getXVel();
						xVel = xVel*-2 - (*itor)->getXVel();
					else xVel = xVel*2 + (*itor)->getXVel();
				}
			}
		}
	}
	else if ((Pad.Held.Left) && ((TouchingGround() || TouchingSpriteGround(objList))) && xVel > 0)
	{
		xVel -= ACCEL + FRICTION;
		if(TouchingSpriteGround(objList)==PLATFORM || TouchingSpriteGround(objList)==FLUFFS)
		{
			vector<LevelObject*>::iterator itor = objList->begin();
			vector<LevelObject*>::iterator end  = objList->end();
			for(;itor!=end;++itor)
			{
				if((*itor)->getCharType()==PLATFORM || TouchingSpriteGround(objList)==FLUFFS)
				{
					if((*itor)->getXVel() >= 0)
						//xVel -= (*itor)->getXVel();
						xVel = xVel*-2 - (*itor)->getXVel();
					else xVel = xVel*2 + (*itor)->getXVel();
				}
			}
		}
	}
	else if((!Pad.Held.Left) && ((TouchingGround() || TouchingSpriteGround(objList))) && xVel < 0)
	{
		xVel += FRICTION;
		if(xVel > 0) xVel = 0;
	}
    //If colliding
	if(hitWall == true) xVel = 0;

	// Add gravity
	if(!TouchingGround() || !TouchingSpriteGround(objList)){
		yVel += GRAVITY;
	}

	//check collisions
	checkSpriteCol(objList);
	CheckCollisions();
	posTemp.first = position.first;
	posTemp.second = position.second;

	position.first += xVel;
	position.second += yVel;
    //Sets the animation if falling
	if (!TouchingGround() && !TouchingSpriteGround(objList))  
		sprite->setFrameAnimation(12); // Not on the ground

	else if(!((Pad.Held.Left)||(Pad.Held.Right))) 
		sprite->setFrameAnimation(0);// Image if not in the air and not walking
}

//so that the world scrolls/follows the character.
// parameters: use TRUE to scroll with the avatar as the center.
//    else, focus on the avatar's position ONLY
void Player::Scroll(bool bScroll)
{
	/* This is currently hardcoded to a level which is 1024 pixels long so it tests the extreme boundries of the
	*  world to see if it needs to scroll I.E if it's less than 160 pixels from x = 0 it doesn't scroll to the left anymore
	*  and if it's within 128 pixels of x=1024 it doesn't scroll to the right anymore.  The player must leave a rectangle area
	*  on screen before the camera begins to scroll with the player similarly to games like Yoshi's Island which we thought would
	*  be a nicer implementation than simply having the player be in the middle of the screen. Depending on which way you the player
	*  is facing, more of the screen becomes visible.
	*/

	if (bScroll)
	{
		if(playerJustFlipped)
		{
			if(!((((position.first - Sprite::scroll.first))>>8) > 80 && (((position.first - Sprite::scroll.first))>>8) < 176))
			{
				playerJustFlipped = false;

			}
		}
		else
		{

			if(hitbox->flipped == 1 && ((((position.first-Sprite::scroll.first)>>8 < 171)) || (((position.first-Sprite::scroll.first)>>8 > 85))) )  Sprite::scroll.first -= norm_fix(CAMERA_TURN_AROUND) - xVel;
			else if(hitbox->flipped == 0 && ((((position.first-Sprite::scroll.first)>>8 < 171)) || (((position.first-Sprite::scroll.first)>>8 > 85))) ) Sprite::scroll.first += norm_fix(CAMERA_TURN_AROUND) + xVel;
			if (((fix_norm(position.first - Sprite::scroll.first)) > 171) && (((position.first - Sprite::scroll.first))>>8) > 128)         
			{            
				Sprite::scroll.first = position.first - norm_fix(171);		         
			}	      
			else if (((fix_norm(position.first - Sprite::scroll.first)) < 85) && (((position.first-Sprite::scroll.first))>>8) < 128)         
			{           

				Sprite::scroll.first = position.first - norm_fix(85);		         
			}    
		}

		if (Sprite::scroll.first <= 0)
		{
			Sprite::scroll.first = 0;	
		}

		else if (Sprite::scroll.first >= norm_fix(LEVELLENGTHPIXELS - 256))      
		{         
			Sprite::scroll.first = norm_fix(LEVELLENGTHPIXELS - 256);	      
		}

		if (((fix_norm(position.second-Sprite::scroll.second)) > 96) && (fix_norm(position.second) < LEVELLENGTHPIXELS -48))
		{ 
			Sprite::scroll.second = position.second - norm_fix(96);		
		}
		//this checks the middle cases of if the player has gotten within 64 pixels of either edge of the screen
		else if ((((position.second - Sprite::scroll.second)>>8) < 96) && ((position.second>>8) > 48))
		{
			Sprite::scroll.second = position.second - norm_fix(96);		
		}

		//move player
		sprite->moveSprite(position.first, position.second);
	}
	else
	{
		sprite->hideSprite();
		isFrozen = true;
	}

}


//return the tile on the collision map at the given point x,y
u8 Player::GetTile(s16 x, s16 y)
{
	if (y < fix_norm(Sprite::scroll.second)) return 1; //collide with top
	
	return (my_buffer[0])[ (((y >>3)*(LEVEL_LENGTH)) + (x>>3)) * 2 ];
}

//Checks collision map collisions and modifies player location
void Player::CheckCollisions()
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

   if(aCollMap[3] == 2) isAlive = false; // 2 (Red) = Dead

   if(aCollMap[0] == 1) hitWall = true; // 1 (Terrain) Collision
	
   if(aCollMap[1] == 1) hitWall = true;
   else hitWall = false;
   //Handles Ground Collision
   if( (TouchingGround()==1 || TouchingGround()==3) && yVel>0)
   {
	   yVel = 0;
	   isJumping = 0;
   }
}
//Checks if the Player collides with any of the sprites
void Player::checkSpriteCol(vector<LevelObject*> * objList)
{
   anyObjectCollision(objList);
   CheckGoal();
   //  Handles actions for all different kinds of Enemies and things.
   //  To take one out just comment the one line you want out.
   //  To add a new interaction create a function called handle(xxx) and do your shit.
   handleScrib(objList); 
   handleStatic(objList);
   handleCollectible(objList);
   handlePlatform(objList);
   handleSpring(objList);
   handleCat(objList);
}

//Handles Collision with Static Objects (Doods)
void Player::handleStatic(vector<LevelObject*> * objList)
{
	while(aObjectColl[0] == STATICDOOD) // Collision on the left of the sprite...
	{ 
		position.first+= norm_fix(1); // Move by 1 pixel...
      anyObjectCollision(objList);
      
	}

	while(aObjectColl[1] == STATICDOOD) // Collision on the right of the sprite...
	{ 
		position.first-= norm_fix(1); // Move by 1 pixel...
      anyObjectCollision(objList);

	}

	while(aObjectColl[2] == STATICDOOD) // Collision with the something on top
	{
		position.second+=norm_fix(1);
		yVel=0;
      anyObjectCollision(objList);

	} 
   
   if( (TouchingSpriteGround(objList)==STATICDOOD) && yVel>0)
	{
		yVel = 0;
		isJumping = 0;
	}

	while(aObjectColl[3] == STATICDOOD) // Collision on the bottom of the sprite...
	{
		position.second -= norm_fix(1); // Move by 1/2 pixel...
      anyObjectCollision(objList);
	}
}
//Handles Collision with our AI enemy dood scrib!
//If he touches you, you get hurt, and he knocks you back and freezes the controls for a moment
//If he touches you successively, it will kill kill the player
void Player::handleScrib(vector<LevelObject*> * objList)
{
	while(aObjectColl[COLL_LEFT] == SCRIB)
	{
		xVel = 1000;
		yVel = -700;
		position.first += norm_fix(1);
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
	}
	while(aObjectColl[COLL_RIGHT] == SCRIB)
	{
		xVel = -1000;
		yVel = -700;
		position.first -= norm_fix(1);
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
	}
	while(aObjectColl[COLL_TOP] == SCRIB)
	{
		yVel = 1400;
      if(xVel >0)
         xVel = 300;
      else
         xVel = -300;
		position.second += norm_fix(1);
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
	}
	while(aObjectColl[COLL_BOTTOM] == SCRIB)
	{
		yVel = -1400;
      if(xVel >0)
         xVel = 300;
      else
         xVel = -300;
		position.second -= norm_fix(1);
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
	}
	if(freezeCount < 300)
	{
		freezeCount++;
	}
	else
	{
		freezeControls = false;
	}
	
}

//Handles Collision with a future enemy dood the churricane
void Player::handleChurricane(vector<LevelObject*> * objList)
{


}

//Handles collision with collectible objects
void Player::handleCollectible(vector<LevelObject*> * objList)
{
   anyObjectCollision(objList);

   if (!bPlayerCollected) 
   {
      if (aObjectColl[0] == COLLECTIBLE || aObjectColl[1] == COLLECTIBLE || aObjectColl[2] == COLLECTIBLE || aObjectColl[3] == COLLECTIBLE )
      {
         bPlayerCollected = true;
         playCollected();//Play Sound Effect
      }
   }
}
//Handles collisions with platforms
void Player::handlePlatform(vector<LevelObject*> * objList)
{
	while(aObjectColl[0] == PLATFORM || aObjectColl[0] == FLUFFS) // Collision on the left of the sprite...
	{ 
		position.first+= norm_fix(1); // Move by 1 pixel...
      anyObjectCollision(objList);
      
	}

	while(aObjectColl[1] == PLATFORM || aObjectColl[1] == FLUFFS) // Collision on the right of the sprite...
	{ 
		position.first-= norm_fix(1); // Move by 1 pixel...
      anyObjectCollision(objList);

	}

	while(aObjectColl[2] == PLATFORM || aObjectColl[2] == FLUFFS) // Collision with the something on top
	{
		position.second+=norm_fix(1);
		yVel=0;
      anyObjectCollision(objList);

	} 

	if( (TouchingSpriteGround(objList)==PLATFORM || TouchingSpriteGround(objList)==FLUFFS) && yVel>0)
	{
		yVel = 0;
		isJumping = 0;
	}

	while(aObjectColl[3] == PLATFORM || aObjectColl[3] == FLUFFS) // Collision on the bottom of the sprite...
	{
		position.second -= norm_fix(1); // Move by 1/2 pixel...
		this->setOnPlatform(true);
      anyObjectCollision(objList);
	}
}
//Handles collisions with a spring.  A spring will increase your jump marginally or greatly depending
//on how long you hold A.
void Player::handleSpring(vector<LevelObject*> * objList)
{
   while(aObjectColl[2] == SPRING)
   {
      position.second += norm_fix(1);
      yVel = 0;
      anyObjectCollision(objList);
   }
   
   if(aObjectColl[3] == SPRING)
   {

      yVel = 0; //because if you jump on the spring, you have a positive yVel
      xVel = 0; //so that when you walk onto the spring you dont bounce forward
      if (Pad.Held.A)
      {
         yVel -= 2000;
         isJumping = true;
      }
      else
      {
         yVel -= 1100;
         isJumping = true;
      }
      isJumping = false;
      playSpring();//Play Spring Sound Effect
      anyObjectCollision(objList);//Handles Co
   }

}
//Handles Collision with the Cat dood
void Player::handleCat(vector<LevelObject*> *objList)
{
   bool hissPlayed = false;
   int itor = 0;
   if(aObjectColl[0] == CAT)
	{
		xVel = 0;
		yVel = 0;
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
        if(!hissPlayed) hissPlayed = true;
      
        for(;itor < 600; ++itor) position.first += rand() % 4 + 1;
	}
	if(aObjectColl[1] == CAT)
	{
		xVel = 0;
		yVel = 0;
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
        if(!hissPlayed) hissPlayed = true;

        for(;itor < 600; ++itor) position.first += rand() % 4 + 1;

	}
	if(aObjectColl[2] == CAT)
	{
        xVel = 0;
        yVel = 0;
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
      if(!hissPlayed) hissPlayed = true;
      for(;itor < 600; ++itor) position.first += rand() % 4 + 1;
	}
	if(aObjectColl[3] == CAT)
	{
		yVel = 0;
      xVel = 0;
		position.second -= norm_fix(1);
		anyObjectCollision(objList);
		freezeControls = true;
		freezeCount = 0;
      if(!hissPlayed) hissPlayed = true;
      for(;itor < 600; itor++) position.first += rand() % 4 + 1;
	}
	if(freezeControls && freezeCount < 600)
	{
		freezeCount++;
	}
	else
	{
		freezeControls = false;
        hissPlayed = false;
	}
	
}

//Update function for the player
void Player::update(bool bScroll, vector<LevelObject*> * objList, int coll)
{
	
	if(isAlive && startMoving)
	{
		
		if(hitCount == 5)//If hit successively 5 times, the avatar dies
		{
			isAlive = false;
		}
		else
		{
			currCM = coll;
			CheckHealth();
			if(bScroll)
			{

				Move(objList);
				//Move the mini avatar on the minimap
				PA_SetSpriteX(1, miniSprite->getId(), ((position.first/4)>>8) + 16);
				PA_SetSpriteY(1, miniSprite->getId(), ((position.second/4)>>8) + 16);
			}
			else
			{
				miniSprite->hideSprite();
			}
			Scroll(bScroll);
		}
	}
}

u8 Player::getSpriteID()
{
	return sprite->getId();
}

u8 Player::getCurrPage()
{
	return pageId;
}

pair<s32,s32> Player::getPos()
{
	return position;
}


void Player::CheckHealth()
{
	if(hitCount == 5)
	{
		isAlive = false;
	}
}
//If the player reaches the goal of the level, level complete!
void Player::CheckGoal()
{
   for(u8 i = 0; i < 4; i++)
   {
      if(aObjectColl[i] == GOAL)
      {
         atGoal = true;
      }
   }
}
