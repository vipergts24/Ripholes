#include "../include/WorldMap.h"

// constructor
WorldMap::WorldMap(vector<u8> availLevels)
{
   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);
   PA_FatFreeSprBuffers();

	uAvailvl = availLevels;		// game class will pass world map which levels are available to draw
	vsLevels = new vector< pair<Sprite*, u8> >();
	initLvlList();
	bgWorldMap = new Background ( "worldmap", 0, 3, false );
   bgWorldTop = new Background ( "worldmap_top", 1, 3, false);
   sBox       = new Sprite ( 0, "worldmap_box", "worldMap", 110<<8, 20<<8, true );
   spDesktop1 = new Sprite ( 1, "desktop1",     "GUI",       70<<8,  0<<8, true );
   spDesktop2 = new Sprite ( 1, "desktop2",     "GUI",      134<<8,  0<<8, true );
	bChosen = false;
   uLvl = 0;
   PA_SetBrightness(0, 0); // normal visibility
   PA_SetBrightness(1, 0); // normal visibility

   spDesktop1->animateSprite ( 0, 2, 6 );
   spDesktop2->animateSprite ( 0, 2, 6 );
}

//destructor
WorldMap::~WorldMap()
{
	for (vsIter = vsLevels->begin(); vsIter != vsLevels->end(); ++vsIter)
	{
		delete (*vsIter).first;
	}
   vsLevels->clear();
	delete vsLevels;
	delete bgWorldMap;
   delete bgWorldTop;
   delete sBox;
   delete spDesktop1;
   delete spDesktop2;

   PA_FatFreeSprBuffers();
   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);
}

// update calls to see if the player touched a level
void WorldMap::update(vector<Collectible*>* ColList)
{  
   for (vsIter = vsLevels->begin(); vsIter != vsLevels->end(); ++vsIter)
   {
      if ( PA_SpriteTouched( (*vsIter).first->getId() ) )
      {
         uPlayLvl = (*vsIter).second;
         //PA_OutputText(1,0,5,"Level Selected: %d", uPlayLvl);
         bChosen = true;
      }
   }

   if ( PA_SpriteTouched( sBox->getId() ) )
   {
      uPlayLvl = 0;
      bChosen = true;
   }
}

// Initializes the list of level sprites to draw given the level IDs passed from game class
void WorldMap::initLvlList() 
{
	for ( vuAvailIter = uAvailvl.begin(); vuAvailIter != uAvailvl.end(); ++vuAvailIter )
	{
		switch ( (*vuAvailIter) )
		{
      // row 1
		case 1: 		
         addLevel ((new Sprite(0, "wm_Level07", "worldMap", 35<<8, 65<<8, true )), (*vuAvailIter) );
			break;

		case 2:	   
         addLevel ( (new Sprite(0, "wm_Level08", "worldMap", 75<<8, 65<<8, true )), (*vuAvailIter));
			break;

      case 3:	  
         addLevel ( (new Sprite(0, "wm_Level09", "worldMap", 115<<8, 65<<8, true )),(*vuAvailIter));
			break;

      case 4:	   
         addLevel ( (new Sprite(0, "wm_Level01", "worldMap", 155<<8, 65<<8, true )),(*vuAvailIter));
			break;

      // row 2
      case 5:	   
         addLevel ( (new Sprite(0, "wm_Level03", "worldMap", 35<<8, 105<<8, true )),(*vuAvailIter));
			break;

      case 6:	   
         addLevel ( (new Sprite(0, "wm_Level04", "worldMap", 75<<8, 105<<8, true )),(*vuAvailIter));
			break;

      case 7:	   
         addLevel ( (new Sprite(0, "wm_Level05", "worldMap", 115<<8, 105<<8, true )),(*vuAvailIter));
			break;
         
   //   case 8:	  
   //      addLevel ( (new Sprite(0, "wm_Level02", "worldMap", 155<<8, 105<<8, true )),(*vuAvailIter));
			//break;
   //      
   //   case 9:	   
   //      addLevel ( (new Sprite(0, "wm_Level03", "worldMap", 195<<8, 105<<8, true )),(*vuAvailIter));
			//break;

		default:
			break;
		}
	}
}

void WorldMap::addLevel(Sprite* mySprite, u8 uNum)
{
	vsLevels->push_back( make_pair(mySprite,uNum) );
}

// returns the level to be drawn in the game class.
u8 WorldMap::uGetLvl()
{
	return uPlayLvl;
}

bool WorldMap::bGetChosen()
{
	return bChosen;
}

