


#include "HelperDood.h"

HelperDood::HelperDood(pair<s32,s32> inPos, u8 pid, bool isRippable){
		pos.first = norm_fix(inPos.first);
		pos.second = norm_fix(inPos.second);
		pageId = pid;
		xVel = 0;
		yVel = 0;
		isRippable = isRippable;
		isFrozen = false;
		isHole = false;
		//doodSprite = new Sprite(1, 0, (void*) child_avatar_Sprite, OBJ_SIZE_32X32, 2, pos.first, pos.second);
		hitbox = new hitBox();
		hitbox->setHitBox("scribz");
}
