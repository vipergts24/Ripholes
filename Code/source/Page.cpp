/* Ripholes in Rubbish
* PROGRAMMER: Mee Cha
* Class for Page
*/

#include "../include/Page.h"

// Constructor: use when given parameters
Page::Page(u8 uPageNum, string sBgName, u8 uLvlId) {
   uPageID = uPageNum;         // first page of a level is page 0
   uLevelId = uLvlId;
   vObjList = new vector<LevelObject*>();
   bgLoaded = false;
   sBg = sBgName;
   if(uPageNum == 1)
   {
      myHealth = new PageHealth(true);
   }
   else
   {
      myHealth = new PageHealth(false);
   }
   bDrawTrans = false;
   sAvaTrans = new Sprite(0, "childAvatar", "childAvatar", 0, 0, true );
   sAvaTrans->hideSprite();
   PA_SetSpriteMode(0, sAvaTrans->getId(), 1);  //turns on alpha blending for sprite
   PA_EnableSpecialFx(0, SFX_ALPHA, 0, SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_BD | SFX_OBJ);
}

Page::~Page() 
{

   for(loIter = vObjList->begin(); loIter != vObjList->end(); loIter++)
   {
      delete (*loIter);
   }
	vObjList->clear();
   delete vObjList;
   delete sAvaTrans;
   delete myHealth;
}

void Page::deleteBg()
{
   delete bgPage;
}

/* Update in Page class will call update on the list of objects it contains
* Parameter: bool drawSprites
* Purpose: 
*  - TRUE if the page being updated needs to draw its sprites ON SCREEN.
*  - FALSE if the sprites of the current page being updated need to be drawn OFF SCREEN.
*/            
void Page::update ( bool bDrawSprites, bool bDrawBg, bool moveObjects, pair<s32,s32> psAvaPos, bool bIsCollected, Sprite * magic )
{	
   // Make sure we only load a background if it isn't already loaded
   if( !bgLoaded && bDrawBg ) 
   {
      bgPage   = new Background ( sBg.c_str(), 0, 3, true );
      bgLoaded = true;
      PA_EasyBgScrollXY(0, // Screen
         3, // Background number
         fix_norm(Sprite::scroll.first), // X scroll
         fix_norm(Sprite::scroll.second)); // Y scroll
   }

   // draw transparent avatar offscreen if she doesnt need to be drawn
   if (!bDrawTrans)
   {
      sAvaTrans->hideSprite();
   }else
   {
      sAvaTrans->moveSprite(psAvaPos.first, psAvaPos.second);
   }
   //PA_OutputText(1,0,1, "%d", bgPage->uGetColl());
   // draw sprites on screen, if need be
   for (loIter = vObjList->begin(); loIter != vObjList->end(); ++loIter ) 
   { 
      if((*loIter)->getCharType() == EVENT_SPEECH)
      {
         (*loIter)->update(bDrawSprites);
      }
      else if((*loIter)->getCharType() == HOLE || (*loIter)->getCharType() == REVERSEHOLE)
      {
         (*loIter)->update(bDrawSprites, magic);
      }
      else
      {
         (*loIter)->update ( bDrawSprites, moveObjects, vObjList, bgPage->uGetColl() );
      }
      
      // if the item is a collectible and has been collected, move it off-screen
      if ((*loIter)->getCharType() == COLLECTIBLE && bIsCollected )
      {
         (*loIter)->update(false);
      }
      
   }
}

// Iterates over list of objects and returns which one was touched.
vector<LevelObject*>::iterator Page::loGetTouchedObj() 
{
   for (loIter = vObjList->begin(); loIter != vObjList->end(); ++loIter)
   {
      if ( PA_SpriteTouched( (*loIter)->getSpriteID() ) )
      {
         return loIter;
      }
   }
   return vObjList->end();
}

vector<LevelObject*>::iterator Page::getObj(u8 spriteId)
{
   for (loIter = vObjList->begin(); loIter != vObjList->end(); ++loIter)
   {
      if( (*loIter)->getSpriteID()==spriteId)
      {
         return loIter;
      }
   }
   return vObjList->end();
}


// LATER: Remove object from list of Page's current objects, given a pointer to it.
void Page::rmvObj( vector<LevelObject*>::iterator loIter )		
{
   vObjList->erase(loIter);		// does not handle deletion of items
}

// Add an object to the list, happens upon creation and pasting.
void Page::addObj(LevelObject &myObj) 
{
   vObjList->push_back(&myObj);
}

// Create dood upon initialization of the level.
void Page::createDood ( vector<string> &vString )
{
   if ( vString.size() < 3 ) return;

   vector<string>::iterator iter = vString.begin();

   string myName;
   string EventBgName;
   u8 type;
   pair<s32,s32> posi;
   pair<s16, s16> frames;
   bool animate   = false;
   bool bIsRip    = true; 
   u8 speed = 0;
   int typeDood = iDoodType(*iter);
   int i = 1;

   switch ( typeDood )
   {
      case 1:     //dood
         posi.first  = atoi ( vString.at(i++).c_str() );
         posi.second = atoi ( vString.at(i++).c_str() );
         myName      = vString.at(i++);
         type        = atoi ( vString.at(i++).c_str() );
         if( (i + 4) < (int)vString.size() )
         {
            frames.first  = atoi ( vString.at(i++).c_str() );
            frames.second = atoi ( vString.at(i++).c_str() );
            speed         = atoi ( vString.at(i++).c_str() );

            if ( ( frames.second != 0 ) && ( speed != 0 ) )
            {
               animate = true;
            }

            // if 
            if ( strcmp(vString.at(i++).c_str(),"rippable") )
               bIsRip = false;


            // Construct string vector for behaviors LULz
            iter += 9;
            vector<string> vsBehavs = vector<string> ( iter, vString.end() );

            addObj(*(new HelperDood(posi, animate, frames, speed, uPageID, bIsRip, myName, (Character)type, vsBehavs )));
            
         }
         break;

      case 2:     // speech box
         posi.first = atoi( (*(++iter)).c_str() );
         posi.second = atoi( (*(++iter)).c_str() );
         myName = *(++iter);
         type = atoi( (*(++iter)).c_str() );
         if( iter != vString.end() )
         {
            frames.first = atoi( (*(++iter)).c_str() );
            frames.second = atoi( (*(++iter)).c_str() );
         }
         EventBgName = *(++iter);
         addObj(*(new Event(posi, uPageID, frames.first, frames.second, myName, (Character)type, EventBgName)));

         break;
      default:
         break;
   }
}


int Page::iDoodType(string s)
{

   if (!strcmp(s.c_str(),"dood")) return 1;

   if (!strcmp(s.c_str(),"speech")) return 2;

   return 8;
}

s32 Page::sgetObjX(LevelObject* obj)
{
   return PA_GetSpriteX(0, obj->getSprite()->getId());
}
s32 Page::sgetObjY(LevelObject* obj)
{
   return PA_GetSpriteY(0, obj->getSprite()->getId());
}
s32 Page::sgetObjLength(LevelObject* obj){
   return PA_GetSpriteLx(0, obj->getSprite()->getId());
}
s32 Page::sgetObjHeight(LevelObject* obj){
   return PA_GetSpriteLy(0, obj->getSprite()->getId());
}

void Page::flipBg ()
{
	bgLoaded = false;
	delete bgPage;
}

// upon successful rip, decrease the health of the current page by variable num
void Page::decHealth(u8 num)
{
   myHealth->subtract(num);
}

void Page::checkDrawTrans(bool bSetTrue, Player* ava )
{
   if (bSetTrue)
   {
      bDrawTrans = true;
      sAvaTrans->moveSprite( ava->getPos().first, ava->getPos().second );
   }
   else
   {
      bDrawTrans = false;
      sAvaTrans->hideSprite();
   }
}

void Page::setDrawTransFalse ()
{
      bDrawTrans = false;
}

void Page::hideAllSprites()
{
	for(loIter = vObjList->begin(); loIter != vObjList->end(); ++loIter)
	{
		(*loIter)->getSprite()->hideSprite();
	}
}

int Page::uGetColl()
{
   
   return bgPage->getSlotNum();


}
