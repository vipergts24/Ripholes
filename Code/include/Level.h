/* 
   Ripholes in Rubbish Level Class Header

   The Level class encapsulates everything in a given level from the loading screen, pause menus
   Victory/Lose screen, GUI for the top screen, the player, and pages in the level.  The page class
   shows more details of such things as the characters (doodz) on screen, backgrounds and more.

   The level class also contains the core mechanics of our game: ripping out objects, pasting objects,
   flipping to pages beneath the avatar, and bursting through pages above the avatar.

   In addition, the level class has an update function that calls just about every update function for each class
   that can be seen further on in the code.
*/

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <cstdio>
#include <string>
#include <PA9.h>
#include "efs_lib.h"

#include "Collectible.h"
#include "HelperDood.h"
#include "Hole.h"
#include "IngameGUI.h"
#include "Mode.h"
#include "Page.h"
#include "PauseMenu.h"
#include "Player.h"
#include "Sound.h"
#include "Sprite.h"

using namespace std;

class Level  
{
private:
   u8       uBeginLvl;
   u8       uLevelNum;
   u8       animPHcounter;  // used to initially animate page health meter upon load up
   string   sLevelString;
   bool     bFirstLoad;     // used to see if the level is in its first state of loading
   bool     bStartAnimation;
   bool     bEventOpen;
   bool     bBeginLvl;

   vector<Page*>*          vpPageList;
   vector<Page*>::iterator itCurrPage;
   vector<Sprite*>*        vsPHgui;
   InGameGUI*              guGUI;
   Sprite*                 sPageHealth;
   Sprite*                 sPageNums;
   Sprite*                 sPageTotal;
   Sprite*                 sPageAva;
   Sprite*                 sRipMagic;
   Player*                 avatar;
   Event*                  eBeginLvl;
   pair <s32, s32>         psAvaPos;
   Mode                    mLevelMode;
   Background*             bgLoadTop;
   Background*             bgLoadBot;

   void makeLoadScreen ();
   void killLoadScreen ();
   void loadTopScreenSprites ();
   Level ();                              // Don't use the default constructor
public:
   Level ( string sInputFile, u8 uLvlNum );
   ~Level ();
   PauseMenu * pMenu;

   // updator functions
   void update (vector<Collectible*>* ColList, bool &bLevelEnd, bool &bRestartLevel);
   void drawPageID ( );
   void endLevel();
   void restartLevel();

   // Mechanic related functions
   bool checkRip ();
   void checkPaste ();
   void burst ();
   bool checkBurst();
   bool checkTile(s16 x, s16 y);
   void flip ( bool bFlipPrev );
   bool bCheckPos ( Player* pAva, LevelObject* loHole);
   void checkSpeech(Event* eventObj);
   void doEventPopup();
   void pause();

   // accessor functions
   u8 iGetTotalPgs ();
   InGameGUI* igGetGUI ();
   vector<Page *>::iterator itGetCurPg ();
   Player* pGetPlayer() {return avatar;}
   u8 getLevelId() {return uLevelNum;}
};

#endif
