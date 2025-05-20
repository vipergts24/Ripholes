/*
   Ripholes in Rubbish
   Player Class Header
*/

#if !defined(_PLAYER_H)
#define _PLAYER_H

#include <vector>
#include "LevelObject.h"
#include "hitBox.h"
#include "Sprite.h"
#include "Sound.h"
#include "Background.h"
#include "Collectible.h"
#include "AIController.h"

using namespace std;

//Extends our base class LevelObject
class Player : public LevelObject
{
public:
	Player(pair<s32,s32> inPos, u8 era, u8 pid);
	~Player();
	virtual void update(bool bScroll, vector<LevelObject*>* objList, int coll);

	// Accessor Functions
	virtual u8 getCurrPage();
	virtual pair<s32,s32> getPos();
	virtual u8 getSpriteID();
	Sprite * getSprite()    {return sprite;}
    Sprite * getMiniSprite(){ return miniSprite;}
	virtual u8 GetTile(s16 x, s16 y);
    virtual u8 uGetPageId() {return pageId;}
	bool getAlive()         {return isAlive;}
	bool getAtGoal()        {return atGoal;}
    bool bGetIsCollected()  {return bPlayerCollected;}
	bool getIsOnPlatform()  {return onPlatform;}

	// mutator functions
	void setCurrPage(u8 uPageId)  {pageId = uPageId;}
	void setFrozen(bool flag)     {isFrozen = flag;}
    void setStartMoving(bool flag){startMoving = flag;}
	void setAlive(bool flag)      {isAlive = flag;}
	void setAtGoal(bool flag)     {atGoal = flag;}
    void handleScrib(vector<LevelObject*> * objList);
    void handleStatic(vector<LevelObject*> * objList);
    void handleCollectible(vector<LevelObject*> * objList);
    void handleChurricane(vector<LevelObject*> * objList);
	void handlePlatform(vector<LevelObject*> * objList);
    void handleSpring(vector<LevelObject*> * objList);
    void handleCat(vector<LevelObject*> * objList);
	void setOnPlatform(bool flag){onPlatform = flag;}

private:

	bool onPlatform;
	bool isJumping;
    bool bPlayerCollected;   // sees if the collectible in the level is collected
	bool isAlive;
	bool atGoal;
	u8 hitCount;
	void addHit();
	void Move(vector<LevelObject*> * objList);
	void Scroll(bool bScroll);
	pair<s32,s32> posTemp;
	void CheckCollisions();
	void checkSpriteCol(vector<LevelObject*>* objList);
	void CheckHealth();
    void CheckGoal();
	bool hitWall;
	bool onSprite;
	bool playerJustFlipped;
    bool startMoving;
	bool freezeControls;
	int freezeCount;
};

#endif  //_PLAYER_H
