#ifndef __WorldMap_H__
#define __WorldMap_H__

#include <vector>
#include <string>
#include <cstdlib>
#include <utility>
#include <PA9.h>
#include "LevelObject.h"
#include "Background.h"
#include "Collectible.h"
#include "Sprite.h"

using namespace std;

class WorldMap {
public:

	// Game class needs to hold a list of available levels and pass the to worldMap to create it.
	WorldMap(vector<u8> availLevels);
	~WorldMap();

	void update(vector<Collectible*>* ColList);
	vector<Sprite*>::iterator getTouched();

	void initLvlList();
	
	// accessor and mutator functions
	u8 uGetLvl();		// gets the level that needs to be drawn. Used by Game Class
	bool bGetChosen();
	void addLevel(Sprite* mySprite, u8 uNum);

		
private:
   
	bool  bChosen;		// used by Game class to switch modes
	u8    uLvl;			// the level ID that the player clicked on.
	u8    uPlayLvl;			
	u8    mode;			// if player clicked on a level (uLvl), open up a window making sure they want to play it

	vector<u8>           uAvailvl;			// list of level IDs available. Journal will be #0.
	vector<u8>::iterator vuAvailIter;

   //vector<u8> vuCollected; // vector if items collected so far by player.
	vector< pair<Sprite*, u8> >*           vsLevels;		// list of level sprites available. Journal will be #0.
	vector< pair<Sprite*, u8> >::iterator  vsIter;

	Sprite * sBox;

   Sprite * spDesktop1;
   Sprite * spDesktop2;

   Background * bgWorldMap;
   Background * bgWorldTop;


};

#endif 
