#ifndef __JOURNAL_H__
#define __JOURNAL_H__

#include <vector>
#include <cstdio>
#include <string>
#include <PA9.h>
#include "Sprite.h"
#include "Background.h"
#include "Collectible.h"


class Journal
{
private:
   bool        bDrawGFX;
   bool        bInJournal;

   Background* bgTop;
   Background* bgBot;
   Sprite*     sBox;
   Sprite*     sNext;
   Sprite*     sPrev;
   Sprite*     sMap;

   vector<Sprite*>* vsCollSillou;
   vector<Sprite*>* vsCollectibles;
   vector<Sprite*>* vsArky;

public:
   Journal();
   ~Journal();

   void update(vector< pair<u8,bool> >   &vpCollStats);
   void drawGFX();                        // initialization of drawing sprites
   void drawColSil();                     // draw sillouettes once
   void checkIfInJournal();
   void drawCollected( vector< pair<u8,bool> >  &vpCollStats );
   void addSillou(Sprite &myObj);         // add sillouttes to vector
   void addObj(Sprite &myObj);            // add media to media list
   void addArky(string sColor);           // determine what colors of arky to draw
   void addArkySprites(Sprite &myObj);    // add arky media sprites to vector

   // accessors
   bool bGetInJournal() {return bInJournal;}

};

#endif
