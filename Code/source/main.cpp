#include <PA9.h>       // Include for PA_Lib
#include <utility>
#include "../include/Game.h"
#include "../include/StartMenu.h" //Include Start Menu Class
#include "../include/IngameGUI.h"
#include "../include/Player.h"
#include "../include/Level.h"
#include "../include/Sound.h"
#include "../include/Mode.h"

int main(int argc, char ** argv)
{
	Game gaRipholes = Game();
	gaRipholes.titleScreen();

	while(1)
	{
      gaRipholes.update();
		PA_WaitForVBL();
	}
	return 0;
}

