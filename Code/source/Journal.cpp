#include "../include/Journal.h"

Journal::Journal()
{
   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);
   PA_FatFreeSprBuffers();

   bDrawGFX       = true;
   bInJournal     = true;
   vsCollSillou   = new vector<Sprite*>();
   vsCollectibles = new vector<Sprite*>();
   vsArky         = new vector<Sprite*>();
}

Journal::~Journal()
{
   delete bgTop;
   delete bgBot;
   delete sBox;
   delete sMap;

   // clear all sprites in vector of collecticles collected
   vector<Sprite*>::iterator vsIter = vsCollectibles->begin();
   for (; vsIter != vsCollectibles->end(); vsIter++)
   {
      delete (*vsIter);
   }

   delete vsCollectibles;

   // clear all sprites of vector of collectible sillouettes
   vsIter = vsCollSillou->begin();
   for (; vsIter != vsCollSillou->end(); vsIter++)
   {
      delete (*vsIter);
   }
   
   delete vsCollSillou;

   // clear all sprites of media drawn for arky
   vsIter = vsArky->begin();
   for (; vsIter != vsArky->end(); vsIter++)
   {
      delete (*vsIter);
   }

   delete vsArky;

   PA_ResetSpriteSysScreen(0);
   PA_ResetSpriteSysScreen(1);
   PA_FatFreeSprBuffers();
}
 
void Journal::update(vector< pair<u8,bool> >  &vpCollStats)
{
   // draw background on top screen if not done so already
   if ( bDrawGFX )
   {
      bDrawGFX = false; 
      drawGFX();
      drawCollected(vpCollStats);
      drawColSil();
   }
   checkIfInJournal();
}

// initializes graphical assets for the journal 
void Journal::drawGFX()
{
   bgTop = new Background ("journal_bot", 1, 2 , false);
   bgBot = new Background ("journal_bot", 0, 2 , false);
   sBox  = new Sprite (1, "journal_box", "journal", 20<<8, 20<<8, false );
   sMap  = new Sprite (0, "journal_map", "journal", 115<<8, 162<<8, false );
}

// updates bInJournal boolean value that determines if the player is viewing journal or not
void Journal::checkIfInJournal()
{
   if (PA_SpriteTouched ( (*sMap).getId() ) )
   {
      bInJournal = false;
   }
}

// draw the sillouettes of the collectibles
void Journal::drawColSil()
{
   PA_InitText (1,0);
   PA_SetTextCol(1, 0, 0, 0);     //black text color
   PA_OutputText(1,13,4,"Mediums collected:");

   addSillou(*( new Sprite(1,"collectible_crayon_sil", "journal", 110<<8, 55<<8, true )));  // red
   addSillou(*( new Sprite(1,"collectible_crayon_sil", "journal", 152<<8, 55<<8, true )));  // orange
   addSillou(*( new Sprite(1,"collectible_crayon_sil", "journal", 194<<8, 55<<8, true )));  // yellow

   addSillou(*( new Sprite(1,"collectible_crayon_sil", "journal", 120<<8, 97<<8, true )));  // green
   addSillou(*( new Sprite(1,"collectible_crayon_sil", "journal", 162<<8, 97<<8, true )));  // blue
   addSillou(*( new Sprite(1,"collectible_crayon_sil", "journal", 204<<8, 97<<8, true )));  // purple
   
   PA_OutputText(1,1,20,"Collect all the different \nmedia to draw something cool!");
}

// see which collectibles was collected from levels and draw them
void Journal::drawCollected( vector< pair<u8,bool> >  &vpCollStats )
{
   vector< pair<u8,bool> >::iterator vpIter = vpCollStats.begin();

   // if the curr (iter) collectible has been collected
   for (; vpIter != vpCollStats.end(); vpIter++ )
   {
      // check to see if we need to draw collectible on screen

      if ( (*vpIter).first == 3 && (*vpIter).second == true )
      {
         addObj(*(new Sprite(1,"collectible_crayon_red", "journal", 110<<8, 55<<8, true ) ) );
         addArky("red");
      }
      else if ( (*vpIter).first == 4 && (*vpIter).second == true )
      {
         addObj(*(new Sprite(1,"collectible_crayon_orange", "journal", 152<<8, 55<<8, true ) ) );
         addArky("orange");
      }
      else if ( (*vpIter).first == 5 && (*vpIter).second == true )
      {
         addObj(*(new Sprite(1,"collectible_crayon_yellow", "journal", 194<<8, 55<<8, true ) ) );
         addArky("yellow");
      }
      else if ( (*vpIter).first == 6 && (*vpIter).second == true )
      {
         addObj(*(new Sprite(1,"collectible_crayon_green", "journal", 120<<8, 97<<8, true ) ) );
         addArky("green");
      }
      else if ( (*vpIter).first == 7 && (*vpIter).second == true )
      {
         addObj(*(new Sprite(1,"collectible_crayon_blue", "journal", 162<<8, 97<<8, true ) ) );
         addArky("blue");
      }
      else if ( (*vpIter).first == 2 && (*vpIter).second == true )
      {
         addObj(*(new Sprite(1,"collectible_crayon_purple", "journal", 204<<8, 97<<8, true ) ) );
         addArky("purple");
      }
   }
}


void Journal::addSillou(Sprite &myObj) 
{
   vsCollSillou->push_back(&myObj);
}

void Journal::addObj(Sprite &myObj) 
{
   vsCollectibles->push_back(&myObj);
}

void Journal::addArky(string sColor)
{
   // if the color is red
   if ( !strcmp(sColor.c_str(), "red") )
   {
      addArkySprites(*(new Sprite(0,"arky_red1", "journal", 25<<8, 25<<8, true ) ) );
      addArkySprites(*(new Sprite(0,"arky_red2", "journal", 89<<8, 25<<8, true ) ) );
      addArkySprites(*(new Sprite(0,"arky_red3", "journal", 25<<8, 89<<8, true ) ) );
      addArkySprites(*(new Sprite(0,"arky_red4", "journal", 89<<8, 89<<8, true ) ) );
   }
   else if ( !strcmp(sColor.c_str(), "orange") )
   {
      addArkySprites(*(new Sprite(0,"arky_orange1", "journal", 25<<8, 25<<8, true ) ) );
      addArkySprites(*(new Sprite(0,"arky_orange2", "journal", 89<<8, 25<<8, true ) ) );
      addArkySprites(*(new Sprite(0,"arky_orange3", "journal", 25<<8, 89<<8, true ) ) );
      addArkySprites(*(new Sprite(0,"arky_orange4", "journal", 89<<8, 89<<8, true ) ) );
   }
   else if ( !strcmp(sColor.c_str(), "yellow") )
   {
      addArkySprites(*(new Sprite(0,"arky_yellow1", "journal", 25<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_yellow2", "journal", 89<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_yellow3", "journal", 25<<8, 89<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_yellow4", "journal", 89<<8, 89<<8, false ) ) );
   }
   else if ( !strcmp(sColor.c_str(), "green") )
   {
      addArkySprites(*(new Sprite(0,"arky_green1", "journal", 25<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_green2", "journal", 89<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_green3", "journal", 25<<8, 89<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_green4", "journal", 89<<8, 89<<8, false ) ) );
   }
   else if ( !strcmp(sColor.c_str(), "blue") )
   {
      addArkySprites(*(new Sprite(0,"arky_blue1", "journal", 25<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_blue2", "journal", 89<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_blue3", "journal", 25<<8, 89<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_blue4", "journal", 89<<8, 89<<8, false ) ) );
   }
   else if ( !strcmp(sColor.c_str(), "purple") )
   {
      addArkySprites(*(new Sprite(0,"arky_purple1", "journal", 25<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_purple2", "journal", 89<<8, 25<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_purple3", "journal", 25<<8, 89<<8, false ) ) );
      addArkySprites(*(new Sprite(0,"arky_purple4", "journal", 89<<8, 89<<8, false ) ) );
   }
}

void Journal::addArkySprites(Sprite &myObj)
{
   vsArky->push_back(&myObj);
}