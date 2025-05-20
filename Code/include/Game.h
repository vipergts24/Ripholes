// Ripholes in Rubbish
// Game class header

#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <PA9.h>
#include "efs_lib.h"
#include "Collectible.h"
#include "Journal.h"
#include "Level.h"
#include "Mode.h"
#include "Save.h"
#include "Sound.h"
#include "StartMenu.h"
#include "WorldMap.h"

using namespace std;

class Game
{
   private:

      u8          uLvlId;
      u8          uTypeGame;        // used to see if user starts a new or loads game
      bool        bStartLvl;
      bool        bLevelEnd;
      bool        bRestartLevel;
      bool        bWorldMap;
      bool        bFirstMap;
      bool        bJournalNotMade;
      bool        bFirstJournalUpdate;
      bool        bIsCollected;     // sees if a collectible was collected
      bool        bWinLoseSpriteDrawn;
      bool        bFirstLevelUpdate;
      string      sLvlName;
      Sprite *    sWinLoseColl;
      vector<u8>              vuAvaiLvls;
      // u8 = level id, bool determines if collectible for that level has been collected
      vector< pair<u8,bool> >   vpCollStats;   
      vector<Collectible*>*   ColList;
      pair<s32,s32>           psGameStats;
      Journal*    jNew; 
      Level*      leCurLvl;
      Mode        mCurMode;
      Sound*      soMusic;
      WorldMap*   wmMap;
      Save*       svSaveData;

   public:

      Game ();
      ~Game ();

      void titleScreen ();
      void loadOpts ();
      void defOpts ();
      void playLevel ( string sLevName );
      void setMode(Mode mode);
      void updateColl();
      void drawWinLoseCollected();
      void createWinLoseScreen(string sType);
      void updateLevelList();
      void loadingScreen ();

      // object initializers
      void initJournal();
      void initLevelList();
      void initWorldMap ();
      void initCollStats();

      // update functions
      void update ();
      void doJournal ();
      void doWorldMap ();

      Level* leGetCurLvl ();
};

#endif
