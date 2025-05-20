
#ifndef __PAUSEMENU_H__
#define __PAUSEMENU_H__

#include "Sprite.h"

class PauseMenu
{
	private:
		Sprite * sMenuRestart;
		Sprite * sMenuWorldMap;
	public:
		PauseMenu();
		~PauseMenu();
		bool getRestart();
		bool getWorldMap();
      void ShowMenu();
      void HideMenu();
};

#endif
