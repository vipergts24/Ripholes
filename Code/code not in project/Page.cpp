/* Ripholes in Rubbish
 * PROGRAMMER: Mee Cha
 * Class for Page
*/

#include "Page.h"

 // Constructor: use when given parameters
Page::Page(u8 uPageNum) {
   uPageID = uPageNum;         // first page of a level is page 0
   sHealth = 100.0;
   //vObjList = new vector<LevelObject>;
}

 Page::~Page() 
 {
	//delete vObjList;
 }

  // Update in Page class will call update on the list of objects it contains
 void Page::update()
 {	
	// Update the background: screen num, background #, filename
	// NOTE: make sure we only load a background if it isn't already loaded (use a bool)
	//PA_EasyBgLoadPtr(0, 0, bgLevel);		

	/*for (loIter = lObjList.begin(); lObjList != loIter.end(); ++loIter ) 
	{
		loIter->update();
	}*/
 }


 // Iterates over list of objects and returns which one was touched.
 //LevelObject Page::loGetTouchedObj() 
 //{
	//for (loIter = lObjList.begin(); lObjList != loIter.end(); ++loIter)
	//{
	//	if (PA_SpriteTouched(loIter) )
	//	{
	//		return *loIter;
	//	}
	//}
 //}

 // LATER: Remove object from list of Page's current objects, given a pointer to it.
 //newItor Page::rmvObj( Vector<LevelObj>::iterator loIter )		
 //{
	////lObjList.erase(loIter);		// does not handle deletion of items
 //}

 // Add an object to the list, happens upon creation and pasting.
 /*void Page::addObj(LevelObject &myObj) 
 {
	lObjList.push_back(myObj);
 }*/

 // Create dood upon initialization of the level.
 void Page::createDood (vector<string> &vString)
 {
	 vector<string>::iterator iter = vString.begin();
	 //bg = &background;			// TODO: get correct background given info from file
	 
	 // if there is a #, tell program to know the rest of the line are behaviors (needs to be written in parser too)

   pair<s32, s32> pDoodPos;
   string myName;

   int typeDood = iDoodType(*iter);
	switch ( typeDood )
	{
		case 0:  // player
		//	// posX, posY, Era, pallette
		//	pair<s32,s32> pos;
  //       pos.first = atoi( (*(++iter)).c_str() );
		//	pos.second = atoi( (*(++iter)).c_str() );

		//	u8 era = atoi( (*(++iter)).c_str() );
		//	u8 palNum = atoi( (*(++iter)).c_str() );

		//	Player * avatar = new Player(pos,era,palNum,uPageID);
		//	//addObj(avatar);		// add the player object to our list of objects on the current page.
		//	break;

		case 1:     // edood xPos yPos SpriteImg palletteId
         pDoodPos.first = atoi( (*(++iter)).c_str() );
         pDoodPos.second = atoi( (*(++iter)).c_str() );
         myName = *(++iter);
         PA_OutputText(1, 1, 2, "edood pos: %d, %d, %s", pDoodPos.first, pDoodPos.second, myName.c_str() );
			break;
		
		case 2:     //hdood
         PA_OutputSimpleText(1, 1, 4, "Hay I r hdood" );
			break;
		
		case 3:     // odood
			break;
		
		case 4:     // platform
			break;
		
		case 5:     // memoir
			break;
		
		case 6:     // door
			break;
		
		case 7:     // hole
         PA_OutputSimpleText(1, 1, 6, "Hay I r hole" );
			break;
		
		default:
			break;
	}
 }

 // Accessor functions
 u8 Page::uGetPageId() const {return uPageID;}
 s32 Page::sGetHealth() const {return sHealth;}

 
 int Page::iDoodType(string s)
{
   
   if (!strcmp(s.c_str(),"player")) return 0;
   
   if (!strcmp(s.c_str(),"edood")) return 1;
   
   if (!strcmp(s.c_str(),"hdood")) return 2;
   
   if (!strcmp(s.c_str(),"odood")) return 3;
   
   if (!strcmp(s.c_str(),"platform")) return 4;
   
   if (!strcmp(s.c_str(),"memoir")) return 5;
   
   if (!strcmp(s.c_str(),"door")) return 6;
   
   if (!strcmp(s.c_str(),"hole")) return 7;
   
   return 8;
}

 //// LATER: Class for Page flip mechanic. Returns a boolean that determines if the flip was successful or not
 //// Flipping
 //bool Page::bFlip(bool bFlipPrev)
 //{
	//if (bFlipPrev)			// check to see if the player wants to flip to a prev Page
	//{
	//	if (currPage != 1)		// continue Page flip if player is not on first Page
	//	{
	//		setCurrPage(uPageID - 1);  // subtract one Page to get new Page, function is in level
	//	}
	//}
	//else if (RSRel == true)
	//{
	//	if (lvl.uGetPageID() != lvl.uGetTotalPages())
	//}
 //}

 //page.bFlip()