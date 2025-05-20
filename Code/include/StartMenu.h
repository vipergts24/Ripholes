#ifndef __STARTMENU_H__
#define __STARTMENU_H__

#include <string>
#include <PA9.h>  
#include "Background.h"
#include "Mode.h"
#include "Sound.h"
#include "Sprite.h"

class StartMenu
{
   public:
      StartMenu();
      ~StartMenu();

      void update();
      void initStartMenu ( Sound &soMusic );
      void menuNewGame();
      void menuLoadGame();
      void menuContCredit(string sType);
      
      void drawTitle();

      // accessors
      u8 getGameType();
      bool checkPress();

   private:
      bool bUpdate;
      Sprite* sTitle1;
      Sprite* sTitle2;
      Sprite* sTitle3;
      Sprite* sTitle4;
      Sprite* sLoad;
      Sprite* sOptions;
      Sprite* sNew;
      Background * bgTop;
      Background * bgBottom;
      Mode mMenuMode;
};   

#endif
