/* Ripholes in Rubbish Level Class Source
 */

#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include "Level.h"

using namespace std;

//vector<string>* Level::vsGetTokens ( char* csLevelLine )
//{
//   vector<string>* vsLOStats = new vector<string>();
//   char* csTemp;
//
//   csTemp = strtok ( csLevelLine, " " );
//
//   while ( csTemp != NULL )
//   {
//      vsLOStats->push_back ( string ( csTemp ) );
//      csTemp = strtok ( NULL, " " );
//   }
//
//   return vsLOStats;
//}

Level::Level () // Don't use the default constructor
{
   // Construct members
   vpPageList = new vector<Page>();
   //guGUI = new IngameGUI();
}

Level::Level ( string sInputFile ) // Right now, just reads a file and ignores it
{
   // Construct Members
   vpPageList = new vector<Page>();
   //guGUI = new IngameGUI();

   // File Input
   char sInputLine [150];
   char* csTemp;
   FILE * fsLevelFile;
   string sFirstToken;
   vector<string> vsLineTokens;

   fsLevelFile = fopen ( sInputFile.c_str(), "r" );
   if ( fsLevelFile == NULL ) 
   {
      PA_OutputSimpleText(1, 1, 2, "File not found" );
   }

   int y = 2;

   // Scan characters from the file into a string
   while ( !feof ( fsLevelFile ) )
   {
      fgets ( sInputLine, 150, fsLevelFile ); // Take in the next line

      // Tokenize the line
      csTemp = strtok ( sInputLine, " " );
      while ( csTemp != NULL )
      {
         vsLineTokens.push_back ( string ( csTemp ) );
         csTemp = strtok ( NULL, " " );
      }

      sFirstToken = *(vsLineTokens.begin());

      if ( strcmp ( sFirstToken.c_str(), "page" ) == 0) // If the first token is Page
      {
         Page pgNewPage = Page( vpPageList->size() ); // Create a new page, and add it
         vpPageList->push_back ( pgNewPage );
      }
      else
      {
         PA_OutputText(0, 1, y, "Making a %s on page %d", sFirstToken.c_str(), vpPageList->size() );
         y += 2;
         vpPageList->back().createDood ( vsLineTokens );
      }

      vsLineTokens.clear ();
   }

   itCurrPage = vpPageList->begin(); // Iterator points to the beginning
   fclose ( fsLevelFile ); // Close the input file
}

Level::~Level ()
{
   delete vpPageList;
   //delete guGUI;
}

void Level::update ()
{
   this->itGetCurPg()->update();
   //igGetGUI()->update();
}

bool Level::rip ()
{
   return true;
}

void Level::flip ( bool bUp )
{
   return;
}

int Level::iGetTotalPgs ()
{
   return this->vpPageList->size();
}

Page* Level::itGetCurPg ()
{
   return &(*(this->itCurrPage));
}

//IngameGUI* Level::igGetGUI ()
//{
//   return &(this.guGUI);
//}