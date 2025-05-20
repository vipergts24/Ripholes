/* Ripholes in Rubbish
 * PROGRAMMER: Mee Cha
 * Header file for World Map
*/

#ifndef __WorldMap_H__
#define __WorldMap_H__

#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

class WorldMap {
public:

	// Game class needs to hold a list of available levels and pass the to worldMap to create it.
	WorldMap(vector<u8> availLevels);
	~WorldMap();

	void drawMap();
	vector<LevelObject*>::iterator getTouched();
	
	// accessor functions
	u8 uGetLvl();

private:
	vector<LevelObject*> * vsLevels;		// list of levels available. Journal will be #0.
	vector<LevelObject*>::iterator vsIter;
	u8 uLvl;

	//string wmStatus;	// will determine how to draw the worldmap, given player

}

#endif 