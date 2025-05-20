// Ripholes in Rubbish
// Game class source file

#include <PA9.h>
#include <cstdlib>
#include <cstring>
#include "../include/Game.h"

using namespace std;

//Initializes all the required library calls such as PA Library, Fat loading system, etc
Game::Game () 
{
   PA_Init ();    // PA Init...
   PA_InitVBL ();	// VBL Init...
   PA_InitText(0,3);
   PA_InitText(1,0);

   // Init AS_Lib for mp3
   PA_VBLFunctionInit ( AS_SoundVBL );
   AS_Init ( AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH );
   AS_SetDefaultSettings ( AS_PCM_8BIT, 48000, AS_SURROUND );

   PA_SetScreenSpace ( 0 );

   // Filesystem (EFS and FAT) init
   EFS_Init( EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL );
   PA_FatInitAllBuffers();
   PA_FatSetBasePath("resources"); // See FAT loading example for folder structure
   PA_SetBrightness(0, -31); // all black
   PA_SetBrightness(1, -31); // all black	

   mCurMode = INIT;
   uLvlId = 0;
   bStartLvl = false;
   bLevelEnd = false;
   bRestartLevel = false;
   bWorldMap = true;
   bFirstMap = true;
   bWinLoseSpriteDrawn = false;
   bJournalNotMade = true;
   bFirstJournalUpdate = true;
   bFirstLevelUpdate = true;
   soMusic = new Sound ();
   svSaveData = new Save ();

   ColList =  new vector<Collectible*>();
   initCollStats();

   vuAvaiLvls.clear();
   initPalettes();//All palettes initialized here
   string sLvlName;
}

Game::~Game ()
{
	delete soMusic;
	delete svSaveData;
	delete ColList;
}

// Go to the title screen
void Game::titleScreen ()
{
   StartMenu* smMenu = new StartMenu (); //Create StartMenu
   this->mCurMode = TITLE;
   smMenu->initStartMenu ( *soMusic );

   uTypeGame = smMenu->getGameType();
   initLevelList();
   delete smMenu;

   mCurMode = MAP;

   // make the list of collectibles
   initCollStats();
}
//Sets the fade and creates the level based off the name and ID
void Game::playLevel ( string sLevName )
{
   PA_SetBrightness(0, -31); // all black
   PA_SetBrightness(1, -31); // all black	
   soMusic->stopMusic();
   
   mCurMode = LEVEL;
   leCurLvl = new Level ( sLevName, uLvlId );
   bFirstLevelUpdate = true;
   return;
}

// Load saved settings and progress from file
void Game::loadOpts ()
{
   return;
}

// Load default settings with no levels past 1 available
void Game::defOpts ()
{
   return;
}
//Game Update is ran every frame and handles all updates down the hierarchy
//The Major modes are Level, Journal where art pieces are collected and viewed
//and WorldMap (where level selection occurs)
void Game::update ()
{

   if ( bStartLvl )
   {
      sLvlName = "level";
      sLvlName.append ( 1, uLvlId + '0' );
      sLvlName.append ( ".txt" );
      bStartLvl = false;
      this->playLevel ( sLvlName.c_str() );
      return;
   }

   if ( bWorldMap ) initWorldMap ();    


   // Possible modes:
   // INIT, TITLE, OPTIONS, MAP, LEVEL, JOURNAL
   // Level class ONLY: LEVEL_PASTE, LEVEL_RIP, LEVEL_FLIP
   switch ( mCurMode )
   {

   case MAP://Creates the worldmap where level selection occurs
      doWorldMap();
      break;

   case LEVEL://Calls update on the level, handles the music, and win/lose status
      leCurLvl->update (ColList, bLevelEnd, bRestartLevel);

      if ( bFirstLevelUpdate )
      {
         if(uLvlId == 1 || uLvlId == 2 || uLvlId == 3 )
         {
            soMusic->playMusic ( soMusic->uMusicVolume, "theme" );
         }
         else
         {
            soMusic->playMusic ( soMusic->uMusicVolume, "mountain" );
         }

         bFirstLevelUpdate = false;
         
      }


      if (bIsCollected == false) bIsCollected = leCurLvl->pGetPlayer()->bGetIsCollected();

      if( bLevelEnd )
      {
         bWorldMap = true;
         bLevelEnd = false;
         updateColl();
			//bool winCondition = leCurLvl->pGetPlayer()->getAtGoal();
         soMusic->stopMusic();
         delete leCurLvl;
         bFirstLevelUpdate = true;

         // check to see if the player won the level or lost it
			if( leCurLvl->pGetPlayer()->getAtGoal() )
         {
            createWinLoseScreen("win");
            updateLevelList();
         }
			else 
         {
				createWinLoseScreen("lose");
         }

         bWinLoseSpriteDrawn = false;
         break;
      }

      if( bRestartLevel )
      {
         soMusic->stopMusic();
         delete leCurLvl;
         bFirstLevelUpdate = true;
         bRestartLevel = false;
         bStartLvl = true;
         createWinLoseScreen("lose");
         bWinLoseSpriteDrawn = false;
      }

      break;

   case JOURNAL://Initializes Journal mode and goes to the journal screen
      if (bJournalNotMade) // only make journal if it needs to be
      {
         bJournalNotMade = false; 
         initJournal();
      }
      doJournal();
      break;

   default:
      break;
   }
   soMusic->controlSound ();
}
//Returns the selected level
Level* Game::leGetCurLvl ()
{
   return leCurLvl;
}
//Sets the game mode
void Game::setMode(Mode mode)
{
   mCurMode = mode;
}
//Initializes Journal Screen
void Game::initJournal()
{
   PA_SetBrightness ( 0, -31 );
   PA_SetBrightness ( 1, -31 );

   mCurMode = JOURNAL;
   jNew = new Journal();
}
//Main function for updating the journal
void Game::doJournal()
{
   // only update journal if it needs to be updated (ie: player is in journal)
   if ( jNew->bGetInJournal() == true )
   {
      jNew->update( vpCollStats );

      if ( bFirstJournalUpdate ) 
      {
         PA_SetBrightness ( 0, 0 );
         PA_SetBrightness ( 1, 0 );
         bFirstJournalUpdate = false;
      }
   }
   // else the journal needs to be closed
   else
   {
      bFirstMap = true;

      delete jNew;
      bJournalNotMade      = true;
      bWorldMap            = true;
      bFirstJournalUpdate  = true;
      mCurMode             = MAP;
      initWorldMap();
   }
}

// Enter the world map for level selection
void Game::initWorldMap ()
{
   PA_SetBrightness(0, -31); // all black
   PA_SetBrightness(1, -31); // all black	
   PA_InitText ( 1, 0 );
   wmMap = new WorldMap ( vuAvaiLvls );

   // If this is the first map, music was paused when the start menu or journal died
   // otherwise it was stopped by a level, so play anew
   if ( bFirstMap )
   {
      bFirstMap = false;
   }
   else
   {
      soMusic->playMusic ( soMusic->uMusicVolume, "wmap" );
   }

   mCurMode = MAP;
   bWorldMap = false;
}
//Main function for updating the World Map
void Game::doWorldMap()
{
   wmMap->update (ColList);

   // see if the player clicked anything in the map
   if ( wmMap->bGetChosen() )
   {
      uLvlId = wmMap->uGetLvl ();

      // if player wanted to load the journal
      if ( uLvlId == 0 ) 
      {
         initJournal();
         bJournalNotMade = false;
      }
      // else she wanted to load a level
      else
      {
         bStartLvl = true;
         PA_SetBrightness(0, -31); // all black
         PA_SetBrightness(1, -31); // all black	
      }
      
      bIsCollected = false;
      delete wmMap;
   }
}

//Initializes the collectibles
void Game::initCollStats()
{
   vpCollStats = vector< pair<u8,bool> >();

   // only 10 different collectibles
   for (u8 uLevel = 1; uLevel < 10; uLevel++)
   {
      vpCollStats.push_back( make_pair(uLevel,false) );
   }
}
//Updates the collectibles as they are collected in each level.
void Game::updateColl()
{
   // if the collectible for the curr level got collected
   if ( bIsCollected)
   {
      vector< pair<u8,bool> >::iterator iterStats = vpCollStats.begin();

      //iteratre through the list of coll/bool stats
      for (; iterStats != vpCollStats.end(); iterStats++)
      {
         // find the correct entry in the vector
         if ( leCurLvl->getLevelId() == (*iterStats).first )
         {
            (*iterStats).second = true;      // set it's stat to true (ie: collected)
         }
      }
   }
}

// winlose screen, will be drawn according to what type is passed to it (win/lose strings)
void Game::createWinLoseScreen(string sType )
{
   bool bstyFlag = true;
   Background* bgWinLose;
   Background* bgBottom;
   
   // collected background
	bgBottom = new Background("winscreen_screen1",0,1,false);

   if (strcmp( sType.c_str(), "lose") )
   {
	   bgWinLose = new Background("winscreen_screen0",1,1,false);
   }
   else
   {
	   bgWinLose = new Background("losescreen_screen0",1,1,false);
   }

   //Wait for stylus input before proceeding to worldmap
	while(bstyFlag){
      // if the item for the current level is collected
      if ( bIsCollected && bWinLoseSpriteDrawn == false)
      {
         bWinLoseSpriteDrawn = true;
         drawWinLoseCollected();
      }

      // close the losing menu
      if (Pad.Newpress.Anykey || Stylus.Newpress) 
		{
			bstyFlag = false;

         // if a collectible was collected, delete it's sprite for the win/lose screen
         if ( bIsCollected ) delete sWinLoseColl;

			delete bgWinLose;
         delete bgBottom;
		}
	}
}
//Initializes the level list.  Currently two levels are available for a new game and a saved game
// has all levels available for selection by default
void Game::initLevelList()
{
   if(uTypeGame == NEW_GAME)
   {
      vuAvaiLvls.push_back ( 0 );
      vuAvaiLvls.push_back ( 1 );
      //vuAvaiLvls.push_back ( 2 );
      //vuAvaiLvls.push_back ( 3 );
      //vuAvaiLvls.push_back ( 4 );
      //vuAvaiLvls.push_back ( 5 );
   }else
   {
      vuAvaiLvls.push_back ( 0 );
      vuAvaiLvls.push_back ( 1 );
      vuAvaiLvls.push_back ( 2 );
      vuAvaiLvls.push_back ( 3 );
      vuAvaiLvls.push_back ( 4 );
      vuAvaiLvls.push_back ( 5 );
      vuAvaiLvls.push_back ( 6 );
      vuAvaiLvls.push_back ( 7 );
   }
}

//Updates the available level list
void Game::updateLevelList()
{
   if (uLvlId < 8)
      vuAvaiLvls.push_back(++uLvlId);
}

// checks to see if we should draw any of the collectible media in the win/lose screens
void Game::drawWinLoseCollected()
{
   if ( uLvlId == 3 ) 
   {
      sWinLoseColl = new Sprite(0,"collectible_crayon_red", "journal", 123<<8, 90<<8, false);
   }
   else if ( uLvlId == 4 ) 
   {
      sWinLoseColl = new Sprite(0,"collectible_crayon_orange", "journal", 123<<8, 90<<8, false);
   }
   else if ( uLvlId == 5 ) 
   {
      sWinLoseColl = new Sprite(0,"collectible_crayon_yellow", "journal", 123<<8, 90<<8, false);
   }
   else if ( uLvlId == 6 ) 
   {
      sWinLoseColl = new Sprite(0,"collectible_crayon_green", "journal", 123<<8, 90<<8, false);
   }
   else if ( uLvlId == 7 ) 
   {
      sWinLoseColl = new Sprite(0,"collectible_crayon_blue", "journal", 123<<8, 90<<8, false);
   }
   else if ( uLvlId == 2 ) 
   {
      sWinLoseColl = new Sprite(0,"collectible_crayon_purple", "journal", 123<<8, 90<<8, false);
   }
}

void Game::loadingScreen ()
{

}
