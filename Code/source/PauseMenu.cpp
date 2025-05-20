
#include "PauseMenu.h"
#include "../include/Sprite.h"

PauseMenu::PauseMenu()
{
	sMenuRestart = new Sprite(0, "pMenuRestart", "era0", 96<<8, 90<<8, false ); 
   sMenuRestart->hideSprite();
	sMenuWorldMap = new Sprite(0, "pMenuWorldMap", "era0", 96<<8, 154<<8, false );
   sMenuWorldMap->hideSprite();
}

PauseMenu::~PauseMenu()
{
	delete sMenuRestart;
	delete sMenuWorldMap;
}

void PauseMenu::ShowMenu()
{
   sMenuRestart->moveSprite( Sprite::scroll.first + (96<<8) , Sprite::scroll.second + (90<<8));
   sMenuWorldMap->moveSprite(Sprite::scroll.first + (96<<8), Sprite::scroll.second + (154<<8));
}

void PauseMenu::HideMenu()
{
   sMenuRestart->hideSprite();
   sMenuWorldMap->hideSprite();
}


bool PauseMenu::getRestart()
{
	return PA_SpriteTouched(sMenuRestart->getId());

}
		

bool PauseMenu::getWorldMap()
{
	return PA_SpriteTouched(sMenuWorldMap->getId());

}
