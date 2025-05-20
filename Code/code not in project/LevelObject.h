

#if !defined(_LEVELOBJECT_H)
#define _LEVELOBJECT_H
#include <vector>
#include <utility>


typedef struct{
   s8 x, y, flipx;
}pointInfo; 

//a bounding box to contatin the information about where a sprite collides on it's edges
//including which direction the sprite is facing to account for different shapes on the left/right
typedef struct{
   pointinfo left, right, up, down;
   u8 flipped;
}hitBox;

class LevelObject {
public:
	u8 getCurrPage();
	vector<pair<s32,s32>> getPos();
	void update();
	bool playSound();
    static u8 getSprite(){
        int i;
        for(i=0;i<128;i++){
           if(sprite[i]==0){
               sprite[i]=1;
		    	return i;
		    }			
      }
	    return -1;
    }
	// checkCollision()
protected:
	// spriteInfo
	vector<pair<s32,s32>> pos;
	s32 xVel;
	s32 yVel;
	u8 objId;
	u8 pageId;
    static u8 spriteArray[128];
	bool isRippable;
	bool isFrozen;
	bool isHole;
	hitBox hitbox;
};

#endif  //_LEVELOBJECT_H
