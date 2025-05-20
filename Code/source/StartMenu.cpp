// StartMenu class source

#include "../include/StartMenu.h"

StartMenu::StartMenu()
{
   PA_Init8bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
   PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	

   // initialize mode
   mMenuMode = TITLE;

   // used to determine when to stop updating
   bUpdate = true;

   // initialize the title graphics on top screen
   Sprite * garbage = new Sprite(1, "startmenu_title_quad1", "GUI", 74<<8, 48<<8, true );
   sTitle1 = new Sprite(1, "startmenu_title_quad1", "GUI", 74<<8, 48<<8, true );
   sTitle2 = new Sprite(1, "startmenu_title_quad2", "GUI", 138<<8, 48<<8, true );
   sTitle3 = new Sprite(1, "startmenu_title_quad3", "GUI", 74<<8, 112<<8, true );
   sTitle4 = new Sprite(1, "startmenu_title_quad4", "GUI", 138<<8, 112<<8, true );

   // initialize the buttons on bottom screen for menu
   sNew = new Sprite(0, "startmenu_newgame", "GUI", 60<<8, 19<<8, true );
   sLoad = new Sprite(0, "startmenu_loadgame", "GUI", 150<<8, 19<<8, true );
   sOptions = new Sprite(0, "startmenu_controls_credits", "GUI", 115<<8, 70<<8, true );

   bgTop    = new Background ( "startmenu_screen1", 1, 0, false );
   bgBottom = new Background ( "startmenu_screen0", 0, 1, false );

   delete garbage;
}   

StartMenu::~StartMenu()
{
   delete sTitle1;
   delete sTitle2;
   delete sTitle3;
   delete sTitle4;
   delete sNew;
   delete sLoad;
   delete sOptions;
   delete bgTop;
   delete bgBottom;

   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);
   PA_ResetBgSysScreen(0);
   PA_ResetBgSysScreen(1);

   // Now that it's all black, clean all that and you're ready to go !
   PA_ResetBgSys();
}

void StartMenu::initStartMenu ( Sound &soMusic )
{     
   PA_SetBrightness(0, -31); // all black
   PA_SetBrightness(1, -31); // all black	

   s8 i = 0;         // used for fading

   // Play music after loading bgs and sprites
   soMusic.playMusic ( soMusic.uMusicVolume, "wmap" );

   // Transition to normal visible background
   for(i = -31; i <= 0; i++)
   {
      PA_SetBrightness(0, i); 
      PA_SetBrightness(1, i); 
      PA_WaitForVBL();		   
   }   

   while (bUpdate)
   {
      update();
   }

   // Transition to black
   for(i = 0; i > -32; i--)
   {
      PA_SetBrightness(0, i); 
      PA_SetBrightness(1, i); 
      PA_WaitForVBL();		   
   }
}   

// Update the state of the menu screen depending on the mode the user is in
void StartMenu::update()
{  
   switch ( mMenuMode )
   {
   case TITLE:
      {
         sNew->moveSprite(60<<8, 19<<8);
         sLoad->moveSprite(150<<8, 19<<8);
         sOptions->moveSprite(115<<8, 70<<8);
         drawTitle();
         break;
      }

   case NEW_GAME:
      menuNewGame();
      break;

   case LOAD_GAME:
      menuLoadGame();
      break;

   case CONTROLS:
      menuContCredit("controls");
      break;

   case CREDITS:
      menuContCredit("credits");
      break;

   default:
      mMenuMode = TITLE;
      break;
   } 
}

// Check to see if the player pressed any of the menu buttons
// Returns TRUE if a button was pressed by player and changes game mode
bool StartMenu::checkPress()
{
   if ( Stylus.Newpress )
   {
      // if the NEW GAME button was touched
      if ( PA_SpriteTouched( sNew->getId() ) )
      {
         mMenuMode = NEW_GAME;
         return true;
      }

      // if the LOAD GAME button was touched
      if ( PA_SpriteTouched( sLoad->getId() ) )
      {
         mMenuMode = LOAD_GAME;
         return true;
      }

      // if the CONTROLS button was touched
      if ( PA_StylusInZone(118,81,176,103) == 1)
      {
         mMenuMode = CONTROLS;
         return true;
      }

      // if the CREDITS button was touched
      if ( PA_StylusInZone(118,108,176,130) == 1)
      {
         mMenuMode = CREDITS;
         return true;
      }
   }

   // Special case to escape from options
   if ( mMenuMode == CONTROLS || mMenuMode == CREDITS )
   {
      if ( Pad.Newpress.Anykey || Stylus.Newpress ) return true;
   }

   // nothing was touched
   return false;
}

void StartMenu::menuNewGame()
{
   bUpdate = false;
   //PA_OutputText (1,0,0,"New Game");
}

void StartMenu::menuLoadGame()
{
   /** TODO: ADD IN RULES TO OPEN SAVED DATA AND PLAY WITH IT **/
   //PA_OutputText (1,0,0,"Load Game");
   bUpdate = false;
}

void StartMenu::menuContCredit(string sType)
{
   // move the menu buttons off-screen
   sNew->hideSprite();
   sLoad->hideSprite();
   sOptions->hideSprite();

   // draw the appropriate background
   Background * bgOptions;
   if ( !strcmp(sType.c_str(),"controls") )
      bgOptions = new Background ( "options_controls", 0, 0, false );
   else
      bgOptions = new Background ( "startmenu_credits", 0, 0, false );

   while (true)
   {
      drawTitle();
      if (Pad.Newpress.Anykey || Stylus.Newpress )
      {
         delete bgOptions;
         mMenuMode = TITLE;
         PA_WaitForVBL();
         break;
      }
   }
}

void StartMenu::drawTitle()
{
   u8 uTFrame = 0;    // title frame incre
   u8 uTcounter = 0;  // pace counter for top screen
   bool bAnimSlow = false;

   u8 uBFrame = 0;   // button frame incre
   u8 uBcounter = 0;  

   // Loop through this code until player chooses what to do
   while( !checkPress() )
   {  
      // animate title screen normally
      if ( !bAnimSlow )
      {
         // if this is true, this means the title screen is done drawing
         if (uTFrame >= 103) 
         {
            bAnimSlow = true;
            uTFrame = 100;
            uTcounter = 0;
         }

         if (uTcounter % 4 == 0)
         {
            sTitle1->setFrameAnimation( uTFrame );
            sTitle2->setFrameAnimation( uTFrame );
            sTitle3->setFrameAnimation( uTFrame );
            sTitle4->setFrameAnimation( uTFrame );
            uTcounter = 0;
            uTFrame++;
         }
      }
      // else animate title screen slowly with the last couple of frames
      else
      {         
         if (uTFrame >= 103) 
         {
            uTFrame = 100;
         }

         if (uTcounter % 4 == 0)
         {
            sTitle1->setFrameAnimation( uTFrame );
            sTitle2->setFrameAnimation( uTFrame );
            sTitle3->setFrameAnimation( uTFrame );
            sTitle4->setFrameAnimation( uTFrame );
            uTcounter = 0;
            uTFrame++;
         }
      }

      uTcounter++;

      if (uBFrame >= 3) 
         uBFrame = 0;

      // animate buttons on bottom screen
      if (uBcounter % 5 == 0 ) 
      {
         sNew->setFrameAnimation( uBFrame );
         sLoad->setFrameAnimation( uBFrame );
         sOptions->setFrameAnimation( uBFrame );
         uBcounter = 0;
         uBFrame++;
      }

      uBcounter++;

      PA_WaitForVBL();
   }	
}

u8 StartMenu::getGameType()
{
   return mMenuMode;
}
