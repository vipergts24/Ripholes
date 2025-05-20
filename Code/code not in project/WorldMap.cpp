/* Ripholes in Rubbish
 * PROGRAMMER: Mee Cha
 * Class for World Map
*/

#include "WorldMap.h"

// constructor
WorldMap::WorldMap(vector<u8> availLevels)
{
	// game class will pass world map with levels are available to draw
	copy(availLevels.begin(), availLevels.end(), vsLevels.begin());
}

/destructor
WorldMap::~WorldMap()
{
	delete vsLevels;
}

// returns the level to be drawn in the game class.
u8 WorldMap::uGetLvl()
{
	return uLvl;
}

// draws all of the sprites for each level available and journal
void WorldMap::drawMap()
{
	for (vsIter = vsLevels.begin(); vsIter != vsLevels.end(); ++vsIter)
	{
		(*vsIter)->update();
	}
}


vector<LevelObject*>::iterator WorldMap::getTouched()
{
	for (vsIter = vsLevels.begin(); vsIter != vsLevels.end(); ++vsIter)
	{
		if ( PA_SpriteTouched( (*loIter)->getSpriteID() ) )
		{
			return loIter;
		}
	}
}