/* 
   Ripholes in Rubbish Level Class Source

   The Level class encapsulates everything in a given level from the loading screen, pause menus
   Victory/Lose screen, GUI for the top screen, the player, and pages in the level.  The page class
   shows more detail of such things as the characters (doodz) on screen, backgrounds and more.

   The level class also contains the core mechanics of our game: ripping out objects, pasting objects,
   flipping to pages beneath the avatar, and bursting through pages above the avatar.

   In addition, the level class has an update function that calls just about every update function for each class
   that can be seen further on in the code.
*/

#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include "../include/Level.h"
#include "../include/Sound.h"
using namespace std;


Level::Level () // Don't use the default constructor
{
}
//The Level constructor reads in an input file, parses it, and creates the 
//required attributes such as pages
//For example, here is the data file for our first level:
//# Mark indicates a comment and is ignored by the parsing.

/*The first thing that is created is page 1, with the appropriate binary file tutorial_ripFlip_page01.bin
  The second thing that is created is a tutorial event for the InGameGUI explaining the tutorial.
  The third thing that is created is the player with x,y positions and the era (even though we only completed the child era)
  The fourth-sixth lines are event speech objects and are created deeper in the object oriented code
  depending on their specific types.  
*/

/*
# (LEVEL 1) Tutorial - Rip and Flip mechanics

#############################################

# PAGE 1

page tutorial_ripFlip_page01 
beginLvl beginLvl_tutorial 
player    35    190   0 
speech 75 215 event_speech 10 0 3 speech_tutorial_ripFlip_01 
speech 400 215 event_speech 10 0 3 speech_tutorial_ripFlip_02 
speech 450 215 event_speech 10 0 3 speech_tutorial_ripFlip_03 

#############################################

# PAGE 2

page tutorial_ripFlip_page02 
dood 64 46 wind 0 0 14 10 not_rippable static 0 
dood 200 10 wind 0 0 14 10 not_rippable static 0 
dood 100 200 door 7 0 0 0 rippable static 0 
speech 350 215 event_speech 10 0 3 speech_tutorial_ripFlip_04  
*/

Level::Level ( string sInputFile , u8 uLvlNum) //Reads in an input file, parses it
{  
   PA_SetTextTileCol(1,1);
   PA_FatFreeSprBuffers();
   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);

   makeLoadScreen (); // Create the loading screens
   PA_SetBrightness ( 0, 0 );
   PA_SetBrightness ( 1, 0 );

   // Construct Members
   uLevelNum     = uLvlNum;
   sLevelString  = sInputFile;
   vpPageList    = new vector<Page *>();
   guGUI         = new InGameGUI();
   mLevelMode    = BEGIN_LVL;
   bFirstLoad    = true;
   bEventOpen    = false;
   animPHcounter = 0; 
   bBeginLvl     = true;
   uBeginLvl     = 0;
   pMenu = new PauseMenu();

   // File Input
   char     sInputLine [150];
   char*    csTemp;
   FILE *   fsLevelFile;
   string   sFirstToken;
   vector<string> vsLineTokens;

   fsLevelFile = fopen ( sInputFile.c_str(), "r" );
   if ( fsLevelFile == NULL ) 
   {
      PA_OutputSimpleText(1, 1, 2, "File not found" );
   }

   // Scan characters from the file into a string
   while ( !feof ( fsLevelFile ) )
   {
      fgets ( sInputLine, 150, fsLevelFile ); // Take in the next line
      
      // Ignore # lines as comments
      if ( sInputLine[0] == '#' ) continue;

      // Tokenize the line
      csTemp = strtok ( sInputLine, " \t" );
      while ( csTemp != NULL )
      {
         vsLineTokens.push_back ( string ( csTemp ) );
         csTemp = strtok ( NULL, " \t" );
      }

      sFirstToken = *(vsLineTokens.begin());

      // If the first token is beginLvl
      if ( strcmp ( sFirstToken.c_str(), "beginLvl" ) == 0) 
      {  
         eBeginLvl = new Event (vsLineTokens[1], (Character)BEGINLVL);
      }
      // If the first token is Page
      else if ( strcmp ( sFirstToken.c_str(), "page" ) == 0) 
      {
         Page * pgNewPage = new Page( vpPageList->size() + 1, vsLineTokens[1], uLevelNum ); // Create a new page, and add it
         vpPageList->push_back ( pgNewPage );
      }
      // if the first token is the player
      else if ( strcmp ( sFirstToken.c_str(), "player" ) == 0) 
      {
         // posX, posY, Era, pallette, (pid)
         pair<s32, s32> posi;
         u8 era;

         posi.first = atoi( vsLineTokens[1].c_str() );
         posi.second = atoi( vsLineTokens[2].c_str() );

         era = atoi( vsLineTokens[3].c_str() );

         avatar = new Player(posi,era, vpPageList->size() );  // create player
      } 
      // it has to be some type of dood
      else
      {
         vpPageList->back()->createDood ( vsLineTokens );
      }

      vsLineTokens.clear ();
   }
   itCurrPage = vpPageList->begin(); // Iterator points to the beginning
   fclose ( fsLevelFile ); // Close the input file

   initHoleGfx(); //At the end of the level construction, initialize graphics for holes and magic animation
}
//Clears all available memory from the level so you can go to the world map for level selection
Level::~Level ()
{
   PA_FatFreeSprBuffers();
   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);
   (*itCurrPage)->deleteBg();
   delete avatar;
   delete sPageHealth;
   delete sPageNums;
   delete sPageTotal;
   delete sPageAva;
   for( itCurrPage = vpPageList->begin(); itCurrPage != vpPageList->end(); itCurrPage++)
   {
      delete (*itCurrPage);
   }
   vpPageList->clear();
   delete vpPageList;

   delete guGUI;
   delete pMenu;
   clearMap();
   freeHoleGfx();
}
//Level Update function calls update on everything contained in the level
//including the pages, GUI, pause menu, player
void Level::update (vector<Collectible*>* ColList, bool &bLevelEnd, bool &bRestartLevel)
{
   if(bBeginLvl)//Hides all sprites on the screen while loading
   {
      avatar->getSprite()->hideSprite();
      (*itCurrPage)->hideAllSprites();
   }

   if ( Pad.Newpress.Start )     // see if the player paused
       pause();
   
   if (mLevelMode == PAUSE)      // if the player DID pause
   {
		if(pMenu->getRestart())
		{
			pause();
			avatar->setAlive(false);
		}
		else if(pMenu->getWorldMap())
		{
			pause();
			bLevelEnd = true;
		}
		else return;
   }
   //Set the game mode to beginning the level
   if ( ( bBeginLvl && uBeginLvl > 20 ) || bEventOpen)
   {
      mLevelMode = BEGIN_LVL;
   }
   //If the avatar is considered dead set to restart
   if(!avatar->getAlive())
   {
      bRestartLevel = true;
   }
   //If the avatar is at the  goal, set the level to end victoriously
   if(avatar->getAtGoal())
   {
      bLevelEnd = true;
   }
   
   (*itCurrPage)->phGetPH()->update(sPageHealth,bBeginLvl);// update the pagehealth graphic of current page. 

   // Update the HUD at top screen to reflect the page the avatar is currently viewing
   sPageAva->setFrameAnimation( (*avatar).uGetPageId() - 1 );

   drawPageID();// draw page ID number on top screen

   // if we get a false returned, that means the page has died.
   if ( (*itCurrPage)->phGetPH()->bIsHealthy() == false )
   {
      bRestartLevel = true;  
   }
   //Checks if it is the transparent version of the avatar (looking at a page the avatar is not on)
   (*itCurrPage)->checkDrawTrans(false,avatar); 

   //Handles Update with Stylus input
   if(Stylus.Newpress && !bBeginLvl && !bEventOpen)
   {
	  //Sets appropriate paste mode
      if( (igGetGUI()->getInv()->bIsOpen() && !igGetGUI()->getInv()->bisClosing()))
      {
         mLevelMode = LEVEL_PASTE;
      }
      // can only be in rip mode if avatar pageID is equal to curr pageID
      else if ( (*itCurrPage)->uGetPageId() == avatar->getCurrPage() )
      {
         if(PA_StylusInZone(192,0,256,20))//Inventory box never moves so is hardcoded to upper right
         {
            igGetGUI()->update(); //Calls update for the InGameGUI which includes Inventory etc
         }
         else
         {
            mLevelMode = LEVEL_RIP;//sets the mode to rip based off Stylus
         }
      }
      // if the avatar is on the page beneath the current page
      else if ( (*itCurrPage)->uGetPageId() == avatar->getCurrPage() - 1)
      {
         mLevelMode = LEVEL_BURST; //Sets the mode to burst
      }
   }

   // checks to see if the player tried to page flip
   if (! (Pad.Newpress.L && Pad.Newpress.R ) )
   {
      if (Pad.Newpress.L /*&& (pGetPlayer()->TouchingGround() || pGetPlayer()->getYVel() == 0)*/)
      {
         mLevelMode = LEVEL_FLIP_PREV;
      }
      else if (Pad.Newpress.R /*&& (pGetPlayer()->TouchingGround() || pGetPlayer()->getYVel() == 0)*/)
      {
         mLevelMode = LEVEL_FLIP_NEXT;
      }
   }

   // After input detection, see which mode we are currently playing in
   switch ( mLevelMode )
   {
   case BEGIN_LVL:
   case EVENT_POPUP:
      doEventPopup();//Stylus touched an event box so create an EventPopup
      break;
   case PAUSE:
	   break;
   case LEVEL:
      {
         if (bBeginLvl) uBeginLvl++;   // draw the level once, and then draw upper begin lvl bg

         vector<Page*>::iterator itAllPages = vpPageList->begin();

         // if this is the first time the level is loading (from world map)
         if (!bFirstLoad)
         {

            // Go through the list of pages and make sure the appropriate page gets its sprites updated
            for ( ; itAllPages != vpPageList->end(); ++itAllPages )
            {
               // if the iterator page pointer is the same page as the player's page
               if ( (*itAllPages)->uGetPageId() == (*itCurrPage)->uGetPageId() )
               {
                  // if the current page is the same as the avatar's, control movements of the avatar
                  if ( avatar->getCurrPage() == (*itAllPages)->uGetPageId())
                  {
                     if(!bBeginLvl)
                        (*itAllPages)->update(true, true, true, avatar->getPos(), avatar->bGetIsCollected(), sRipMagic );
                     PA_EasyBgScrollXY(0, // Screen
			            3, // Background number
			            fix_norm(Sprite::scroll.first), // X scroll
                     fix_norm(Sprite::scroll.second - avatar->getYVel())); // Y scroll
                     avatar->update(true,(*itCurrPage)->vgetObjList(), (*itCurrPage)->uGetColl());
                  }
                  //if the current page is one above the avatar's draw the transparent avatar
                  else if( (*itCurrPage)->uGetPageId() == avatar->getCurrPage() - 1)
                  {
                     if(!bBeginLvl)
                        (*itAllPages)->update(true, true, true, avatar->getPos(), avatar->bGetIsCollected(), sRipMagic );
                     PA_EasyBgScrollXY(0, // Screen
			            3, // Background number
			            fix_norm(Sprite::scroll.first), // X scroll
			            fix_norm(Sprite::scroll.second - avatar->getYVel())); // Y scroll
                     avatar->update(false,(*itCurrPage)->vgetObjList(), (*itCurrPage )->uGetColl());
                     (*itCurrPage)->checkDrawTrans(true,avatar);
                  }
                  // else the current page is not usable by any mechanics
                  else
                  {
                     if(!bBeginLvl)
                        (*itAllPages)->update(true, true, true, avatar->getPos(), avatar->bGetIsCollected(), sRipMagic  );
                     PA_EasyBgScrollXY(0, // Screen
			            3, // Background number
			            fix_norm(Sprite::scroll.first), // X scroll
                     fix_norm(Sprite::scroll.second - avatar->getYVel())); // Y scroll
                     avatar->update(false,(*itCurrPage)->vgetObjList(), (*itCurrPage)->uGetColl());
                  }
               }
               // otherwise, make sure all other sprites are drawn off the screen
               else
               {
                  (*itAllPages)->update(false, false, false, avatar->getPos(), avatar->bGetIsCollected(), sRipMagic  );      
               }
            }

            igGetGUI()->update();//Update the GUI and all elements on the top screen
            
         }

         // else, draw the appropriate pages and sprites
         else 
         {
            killLoadScreen ();
            loadTopScreenSprites ();
            eBeginLvl->drawBG();
            // get the page the avatar is on and update it.
            for ( ; itAllPages != vpPageList->end(); ++itAllPages )
            {
               // if the avatar's page ID is equal to the current iterator's page id
               if ((*avatar).getCurrPage() == (*itAllPages)->uGetPageId() )
               {
                  itCurrPage = itAllPages;
				  //Runs update on the select page which calls update for all objects among others
                  (*itCurrPage)->update(false,true, false, avatar->getPos(), avatar->bGetIsCollected(), sRipMagic );
               }
               else
               {
				   //Runs update on the select page which calls update for all objects among others
                  (*itCurrPage)->update(false,false, false, avatar->getPos(), avatar->bGetIsCollected(), sRipMagic );

               }
            }
            bFirstLoad = false;//Toggle off first load condition
         }
         break;
      }
   case LEVEL_RIP:
      igGetGUI()->update();
      checkRip();//This handles our complete Rip mechanic
      break;
   case LEVEL_PASTE:
	   //Closes the inventory when in paste mode so you can see and paste under the inventory
	   if(guGUI->getInv()->bIsOpen() || guGUI->getInv()->bisOpening())
	   {
		  guGUI->getInv()->startCloseInventory();
	   }
	   
      checkPaste();//This handles our complete Paste mechanic
      break;
   case LEVEL_FLIP_PREV:
      flip(true);//Handles page flipping to a previous page
      break;
   case LEVEL_FLIP_NEXT:
      flip(false);//Handles page flipping to the next page
      break;
   case LEVEL_BURST:
      burst();//Handles our page burst mechanic (avatar bursts through a page above it)
      break;
   default:
      break;
   }
	mLevelMode = LEVEL; //Sets the mode back to level for the next iteration
}

//This is our rip mechanic, handles ripping a whole out of the paper by dragging the stylus a small
//distance from the origin of contact.  If white space is ripped, then a hole is generated with a 
// reverse hole on the page below it so you can see a silhouette of a hole when not on that page for easy
// page traversal
bool Level::checkRip ()
{
   u64 prevDist = 0;
   s32 originX,originY;
   //If not a legal rip (not ripping on the correct page, avatar is not on the ground, or trying to rip on terrain that is not rippable)
   if ( !(avatar->getCurrPage() == (*itCurrPage)->uGetPageId() ) || !(pGetPlayer()->TouchingGround() || pGetPlayer()->TouchingSpriteGround((*itCurrPage)->vgetObjList())) || avatar->GetTile(Stylus.X+(Sprite::scroll.first>>8),Stylus.Y+(Sprite::scroll.second>>8)) != 0)
   {
      playError();//Play Error Sound Effect
      return false;
   }
   //Select the object that is touched by the stylus
   vector<LevelObject*>::iterator objToBeRipped = (*itCurrPage)->loGetTouchedObj();

   if ( Stylus.Newpress && (pGetPlayer()->TouchingGround() || pGetPlayer()->TouchingSpriteGround((*itCurrPage)->vgetObjList()))  && (*itCurrPage)->vgetObjList()->end() != objToBeRipped )
   {
      // continue if the object touched can be ripped and the inverntory isn't full
      if( (*objToBeRipped)->getIsRippable() && !guGUI->getInv()->isFull())
      {
         originX = Stylus.X;
         originY = Stylus.Y;
         playCrumple();//Play page crumple sound effect as the stylus is being pressed

         while (1)
         {
		    //Check the bit shifted distance if it is larger than 10 pixels
            if ( PA_Distance ( originX, originY, Stylus.X, Stylus.Y ) >>8 >= 10 )
            {
               sRipMagic->animateSprite(0,6,15);
               u8 random = rand() % 4;
			   //Creates a randomly generated hole
               Hole * hole = new Hole(true, (*objToBeRipped)->getPos(),(*objToBeRipped)->getCurrPage(), random );
               //Only adds a reverse hole if not on last page
			   if( ((*itCurrPage)->uGetPageId() + 1) <= (u8)vpPageList->size() )
               {
                  Hole * rhole = new Hole(false, (*objToBeRipped)->getPos(),(*objToBeRipped)->getCurrPage()+1, random);
                  (*(itCurrPage+1))->addObj(*rhole);
               }
			   //Adds item to inventory and opens the inventory
               guGUI->getInv()->addItem ( (HelperDood*)*objToBeRipped );
               guGUI->getInv()->startOpenInventory();
               playRip ();//Plays Rip Sound Effect
			   //Removes object from object List since the GUI will handle it
               (*itCurrPage)->rmvObj(objToBeRipped);
               (*itCurrPage)->addObj(*hole);
               (*itCurrPage)->decHealth(10);  // decrease health of the page
               return true;
            }
            else if ( Stylus.Released )
            {	
               break;
            }
			//Increates animation frame based off the tension of the rip (distance goes up)
            if ( PA_Distance ( originX, originY, Stylus.X, Stylus.Y ) > prevDist )
            {
               //inc animation frame
               prevDist = PA_Distance(originX,originY,Stylus.X,Stylus.Y);
            }
			//Decreases animation frame as the tension is released (distance goes down)
            else if ( PA_Distance(originX,originY,Stylus.X,Stylus.Y) < prevDist )
            {
               //dec animation frame
               prevDist = PA_Distance(originX,originY,Stylus.X,Stylus.Y);
            }
            PA_WaitForVBL();//Wait for next frame
         }
      }else
      {
		 //If the object is an event speech box, change the mode and open the event
         if((*objToBeRipped)->getCharType() == EVENT_SPEECH)
         {
            avatar->setStartMoving(false);
            eBeginLvl = (Event*)(*objToBeRipped);
            checkSpeech(eBeginLvl);
            mLevelMode = EVENT_POPUP;
            bEventOpen = true;
         }
      }
   }
   //Check if ripping white Space (no object touched)
   else 
   {
      if(PA_StylusInZone(192,0,256,20) || PA_SpriteTouched((*itCurrPage)->sGetTextBox()->getId()))
      {
         (*itCurrPage)->sGetTextBox()->hideSprite();
         (*itCurrPage)->sGetTextBox()->setFrameAnimation(100);
         return false;
      } 

      s32 destX = Stylus.X + ((Sprite::scroll.first)>>8) - (64)/2;
      s32 destY = Stylus.Y + ((Sprite::scroll.second)>>8) - (64)/2;
      originX = Stylus.X;
      originY = Stylus.Y;
      playCrumple();//Play Crumple Sound Effect

      while (1)
      {
	     //Same as above, but no object placed in the inventory
         if ( PA_Distance ( originX, originY, Stylus.X, Stylus.Y ) >>8 >= 10 )
         {
            sRipMagic->animateSprite(0,6,15);
            u8 random = rand() % 4;
            Hole * hole = new Hole(true, pair<s32,s32>(destX<<8,destY<<8),(*itCurrPage)->uGetPageId(), random);
            (*itCurrPage)->addObj(*hole);
           if(((*itCurrPage)->uGetPageId() + 1) <= (u8)vpPageList->size())
            {
               Hole * rhole = new Hole(false, pair<s32,s32>(destX<<8,destY<<8),(*itCurrPage)->uGetPageId() + 1, random);
               (*(itCurrPage+1))->addObj(*rhole);
            }
            playRip ();
            (*itCurrPage)->decHealth(10);  // decrease health of the page
            return true;
         }
         else if ( Stylus.Released )
         {	
            break;
         }
         if ( PA_Distance ( originX, originY, Stylus.X, Stylus.Y ) > prevDist )
         {
            //inc animation frame
            prevDist = PA_Distance(originX,originY,Stylus.X,Stylus.Y);
         }
         else if ( PA_Distance(originX,originY,Stylus.X,Stylus.Y) < prevDist )
         {
            //dec animation frame
            prevDist = PA_Distance(originX,originY,Stylus.X,Stylus.Y);
         }
         PA_WaitForVBL();
      }
   }
   return true;
}

// Return true if something was pasted correctly
void Level::checkPaste ()
{
   s32 destX=0,destY=0;
   HelperDood* objToBePasted;

   // player can only paste if avatar is on the same page as player
   if ( pGetPlayer()->uGetPageId() == (*itCurrPage)->uGetPageId() )
   {
	  //Can only paste if the player is standing
      if(Stylus.Newpress && (pGetPlayer()->TouchingGround() || pGetPlayer()->TouchingSpriteGround((*itCurrPage)->vgetObjList())))
      {
         while(1)
         {
            objToBePasted = (HelperDood*)(igGetGUI()->getInv()->loGetTouchedObj());//Gets the Touched Object
            if(objToBePasted && objToBePasted->getIsRippable() )
            {   
               while(1)//Handles the stylus dragging an object out of the inventory
               {
                  igGetGUI()->getInv()->update();//Update the inventory before moving
                  PA_MoveSpriteEx(0,objToBePasted->getSprite()->getId(),64,64);//Move the sprite every frame
                  if ( Stylus.Released )
                  {
					 //If not a valid place to paste, error
                     if(avatar->GetTile(Stylus.X+(Sprite::scroll.first>>8),Stylus.Y+(Sprite::scroll.second>>8)) != 0)
                     {
                        PA_SetSpriteXY(0,objToBePasted->getSprite()->getId(),256,0);
                        playError();//Play Error Sound Effect
                        return ;
                     }
					 //If Placing back in the inventory (dragging the object onto the inventory tab)
                     if(PA_StylusInZone(192,0,256,20))
                     {
                        PA_SetSpriteXY(0,objToBePasted->getSprite()->getId(),256,0);
                        return ;
                     }
					 //Otherwise, move the sprite with the stylus until the stylus is released
					 //and paste at that location
                     destX = Stylus.X +((objToBePasted->getSprite()->scroll.first)>>8) - (PA_GetSpriteLx(0, objToBePasted->getSprite()->getId())/2);
                     destY = Stylus.Y + ((objToBePasted->getSprite()->scroll.second)>>8) - (PA_GetSpriteLy(0, objToBePasted->getSprite()->getId())/2);
                     objToBePasted->setPos ( destX<<8, destY<<8 );
                     objToBePasted->setCurPage ( (*itCurrPage)->uGetPageId() );
                     PA_SetSpritePrio(0,objToBePasted->getSpriteID(),2);
                     (*itCurrPage)->addObj(*objToBePasted);
                     guGUI->getInv()->removeItem(objToBePasted);
                     playPaste();
                     return ;
                  }
                  PA_WaitForVBL(); //wait for next frame
               }
            }
            else return;
         }
         PA_WaitForVBL();//wait for next frame
      }
   }
}

// Parameters: bool bFlipPrev
// send TRUE if flipping to the previous page
// send FALSE if flipping to the next page
void Level::flip ( bool bFlipPrev )
{
   pair<s32,s32> pAvaPos = avatar->getPos();
   bool bOnHole = false;

   // save the current location of the avatar on it's page

   /*---Player wants to flip to the prev page---------------------------------------------------*/

   // Try to flip direction previous
   if (bFlipPrev)			
   {
      // continue flip prev if player is not on first Page
      if ( (*itCurrPage)->uGetPageId() > 1 )		
      {
         (*itCurrPage)->flipBg();

         --itCurrPage;     // decrement curr page iterator to focus on the page prev of the one we are working from    

         // Following code block: move the avatar if possible

         // if the curr page is healthy
         if ( (*itCurrPage)->phGetPH()->bIsHealthy() )
         {
            vector<LevelObject*>* vloObjList = (*itCurrPage)->vgetObjList();
            vector<LevelObject*>::iterator itObj = vloObjList->begin();
            
            // iterate through the list of holes to see if the player can move through it
            for (; itObj != vloObjList->end(); ++itObj)
            {
               if((*itObj)->getCharType() == HOLE)
               {
                  //PA_OutputText(1, 1, 12, "Hole: %d ", (*itHoles)->getSpriteID() );
                  bOnHole = bCheckPos ( avatar, (*itObj) ); // returns true if player is behind a hole 

                  // if the player is behind a hole, move the avatar
                  if ( bOnHole && avatar->getCurrPage() == (*itCurrPage)->uGetPageId()+1)
                  {
                     // change avatar's page to the new page if she can be moved
                     avatar->setCurrPage((*(itCurrPage))->uGetPageId() );
                     break;
                  } // end avatar movement to new page
               }
            }  // end hole iteration for collision
         } // end if-cond for healthy test

         // the page is NOT healthy, therefore the currPage has to be flipped to the page before it.
         else 
         {
            flip(true);
         }
      }

      // cannot flip to prev page because avatar is ON the first page
      else
      {
         playError();      //cue that the player cannot flip in that direction
      }
   }

   /*---Player wants to flip to the next page------------------------------------------------------*/

   else 
   {
      //continue if player is not on the last page
      if ((*itCurrPage)->uGetPageId() != vpPageList->size() ) 
      {
         (*itCurrPage)->flipBg();

         // check to see if the player is viewing the same page the avater is on
         if (avatar->getCurrPage() == (*itCurrPage)->uGetPageId() )
         {
            // Following code block: move the avatar if possible

            vector<LevelObject*>* vloObjList = (*itCurrPage)->vgetObjList();
            vector<LevelObject*>::iterator itObj = vloObjList->begin();

            // if there are no holes in the list of holes
            if ( vloObjList->size() < 1 )
            {
               ++itCurrPage;     // flip to the next page without the avatar
            }
            // else, check the hole collision and see if it is possible to move avatar
            else
            {

               // check to see if the next page is healthy enough to flip to
               if ( (*(itCurrPage+1))->phGetPH()->bIsHealthy() )
               {

                  // iterate through the list of holes on curr page to see if the player can move through it
                  for (; itObj != vloObjList->end(); ++itObj)
                  {
                     if((*itObj)->getCharType() == HOLE)
                     {
                        bOnHole = bCheckPos ( avatar, (*itObj) );     // returns true if player is on hole

                        // if the player is on top of a hole, move the avatar
                        if ( bOnHole )
                        {
                           ++itCurrPage;        // change avatar's page to the new page if she can be moved
                           avatar->setCurrPage((*itCurrPage)->uGetPageId());
                           break;
                        }
                     }
                  } // end for loop

                  // flip to the next page without the avatar
                  if (!bOnHole)
                  {
                     ++itCurrPage;    
                  }
               } // end health condition 

               // the page is NOT healthy, therefore the player has to be flipped to the page after it.
               else
               {
                  flip(false);
               }
            }
         }
         // else, don't attempt to move the avatar and just update the iterator
         else 
         {
            ++itCurrPage;
         }
      }

      // cannot flip to next page because avatar is ON the last page
      else  
      {
         playError(); // cue that the player cannot flip in that direction
      }
   }
   // end page flip
}

// returns true if the avatar is able to traverse page
bool Level::bCheckPos ( Player* pAva, LevelObject* loHole)
{
   pair<s32,s32> pOriginalAva = pAva->getPos();
   pair<s32,s32> pOriginalHole = loHole->getPos();
   pair<s32,s32> posAva;
   pair<s32,s32> posHole;

   posAva.first = fix_norm(pAva->getPos().first)+2;
   posAva.second = fix_norm(pAva->getPos().second)+2;

   posHole.first = fix_norm(loHole->getPos().first) + 16;
   posHole.second = fix_norm(loHole->getPos().second) + 16;

   pair<s32,s32> posAvaMax;
   posAvaMax.first = fix_norm(pAva->getPos().first)+24;
   posAvaMax.second = fix_norm(pAva->getPos().second)+24;

   pair<s32,s32> posHoleMax;
   posHoleMax.first = fix_norm(loHole->getPos().first) + 40;
   posHoleMax.second = fix_norm(loHole->getPos().second) + 40;

   if ( ( posAva.first > posHole.first ) && ( posAva.first < posHoleMax.first ) )
   {
      if ( ( posAva.second > posHole.second ) && ( posAva.second < posHoleMax.second ) )
      {
         return true;
      }

      if ( ( posAvaMax.second > posHole.second ) && ( posAvaMax.second < posHoleMax.second ) )
      {
         return true;
      }
   }

   if ( ( posAvaMax.first > posHole.first ) && ( posAvaMax.first < posHoleMax.first ) )
   {
      if ( ( posAva.second > posHole.second ) && ( posAva.second < posHoleMax.second ) )
      {
         return true;
      }

      if ( ( posAvaMax.second > posHole.second ) && ( posAvaMax.second < posHoleMax.second ) )
      {
         return true;
      }
   }

   return false;
}

// mechanic to burst through pages: ie, flip + move avatar to prev page
void Level::burst()
{   
   // decrements page iterator
   if ( checkBurst())
   {
      avatar->setCurrPage( (*itCurrPage)->uGetPageId() );  // moves avatar to desired page
      u8 random = rand() % 4;
      Hole * hole = new Hole(true, pair<s32,s32>(((avatar->getPos().first>>8)-20)<<8,((avatar->getPos().second>>8)-20)<<8),avatar->getCurrPage(), random);
      Hole * rhole =  new Hole(false, pair<s32,s32>(((avatar->getPos().first>>8)-20)<<8,((avatar->getPos().second>>8)-20)<<8), avatar->getCurrPage()+1, random);
      (*itCurrPage)->addObj(*hole);
      (*(itCurrPage+1))->addObj(*rhole);
      playRip();
   } 
}

// used by burst mechanic. returns true if avatar can burst through to prev page
bool Level::checkBurst()
{
   //(*itCurrPage)->checkDrawTrans(true, avatar );
   u8 x, y;
   x = (avatar->getPos().first - Sprite::scroll.first)>>8;
   y = (avatar->getPos().second - Sprite::scroll.second)>>8;
   if(PA_StylusInZone(x-5, y-5, x+32, y+32) && checkTile((avatar->getPos().first)>>8,(avatar->getPos().second)>>8))
   {
      // burst success
      (*itCurrPage)->setDrawTransFalse();
      return true;
   }

   return false;

}
//Sets the speech animation and creates the event
void Level::checkSpeech(Event* eventObj)
{
   eventObj->sGetSprite()->setFrameAnimation(3);
   eventObj->doSpeech();
}

//Checks the Tile to see if it is occupied
bool Level::checkTile(s16 x, s16 y)
{
   u8 temp = avatar->GetTile(x,y);
   if( temp == 1 || temp == 2 || temp ==3 )
   {
      return false;
   }
   return true;
}

//Returns the total pages in the level
u8 Level::iGetTotalPgs ()
{
   return this->vpPageList->size();
}

//Returns an iterator to the page
vector<Page*>::iterator Level::itGetCurPg ()
{
   return this->itCurrPage;
}
//Returns a pointer to the GUI
InGameGUI* Level::igGetGUI ()
{
   return this->guGUI;
}

// determine which page ID sprite to draw on top screen
void Level::drawPageID()
{
   if ( (*itCurrPage)->uGetPageId() > 0 )
      sPageNums->setFrameAnimation ( (*itCurrPage)->uGetPageId() - 1 );
   else
      sPageNums->setFrameAnimation ( (*itCurrPage)->uGetPageId() );
}

//Starts the event popup
void Level::doEventPopup()
{
   // close the beginning lvl bg if any key is pressed
   if ((Pad.Held.Anykey || Stylus.Newpress))
   {
      mLevelMode = LEVEL;
      sPageAva->moveSprite(79<<8, 160<<8);
      sPageTotal->moveSprite(56<<8, 144<<8);
      sPageNums->moveSprite(112<<8, 176<<8);
      sPageHealth->moveSprite(193<<8, 48<<8);
      avatar->setStartMoving(true);
      if(bBeginLvl)
      {
         delete eBeginLvl;
         bBeginLvl = false;
      }
      else
      {
         bEventOpen = false;
         eBeginLvl->clearBG();
         eBeginLvl->getSprite()->setFrameAnimation(1);
      }
   }
   else
   {
      avatar->setStartMoving(false);
      avatar->getMiniSprite()->hideSprite();
      sPageAva->hideSprite();
      sPageTotal->hideSprite();
      sPageNums->hideSprite();
      sPageHealth->hideSprite();
   }
}
//Freezes frame, hides all sprites, changes the mode, and shows the pause menu
void Level::pause()
{
	if(mLevelMode != PAUSE)
	{
		mLevelMode = PAUSE;
		avatar->getSprite()->setFrameAnimation(0);
		pMenu->ShowMenu();
		playSelected();
		(*itCurrPage)->hideAllSprites();
		avatar->getSprite()->hideSprite();
	}
	else 
	{
		mLevelMode = LEVEL;
		playSelected();
      pMenu->HideMenu();
	}
}
//Creates Loading Background
void Level::makeLoadScreen ()
{
   bgLoadBot = new Background ( "loadscreen_screen0", 0, 0, false );
   bgLoadTop = new Background ( "loadscreen_screen1", 1, 0, false );
}

//Destroys Loading Background
void Level::killLoadScreen ()
{
   delete bgLoadTop;
   delete bgLoadBot;
   PA_WaitForVBL();
   
}

void Level::loadTopScreenSprites ()
{
   // set page health sprites
   sPageHealth = new Sprite(1, "health_meter", "GUI", 193<<8, 48<<8, true );  
   sPageHealth->hideSprite();

   // set page number sprites
   sPageNums = new Sprite(1, "top_pagenums", "GUI", 112<<8, 176<<8, true );  
   sPageNums->hideSprite();
   
   // set sprite for total number of pages for level
   sPageTotal = new Sprite(1, "top_pagenums", "GUI", 56<<8, 144<<8, true );  
   sPageTotal->setFrameAnimation( vpPageList->size() - 1 );
   sPageTotal->hideSprite();

   // set sprite for avatar page number
   sPageAva  = new Sprite(1, "top_pagenums", "GUI", 79<<8, 160<<8, true );
   sPageAva->hideSprite();

   sRipMagic = new Sprite(0,"riphole_magic", "GUI", 255, 191, true);
   sRipMagic->hideSprite();
}
