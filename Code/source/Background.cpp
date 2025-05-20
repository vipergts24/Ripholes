/*
   Ripholes in Rubbish 
   Background Class Source

   All Backgrounds are loaded in dynamically through a file system as well as the collision map

*/

#include "../include/Background.h"

//Background Constructor takes in a name from the data file, screen, layer, and if it is a level bg
Background::Background ( string sName, u8 uScreen, u8 uLayer, bool bLevel )
{
   //Had some problems loading into Fat so we were allocating on the heap
   //future iteration can probably fix this up and keep it in the stack
   string* ssTempA = new string ( sName );
   string* ssTempB = new string ( sName );
   string* ssTempC = new string ( "/bg/coll/" );
   string* ssTempD = new string ( "minimap_" );
   string* ssTempE = new string ( sName );
   
   ssTempC->append ( *ssTempB );
   ssTempC->append ( "_coll_Map.bin" );
   ssTempD->append ( *ssTempE );

   // Init member vars
   csName = new string ( *ssTempA );
   csColl = new string ( *ssTempC );
   miniMe = NULL;
   this->uScreen = uScreen;
   this->uLayer  = uLayer;
   this->bLevel  = bLevel;

   REG_IME = 0x00;

   PA_FatBgLoad    ( (*csName).c_str(), (*csName).c_str() ); // Loads the BG to RAM, assigns a reference char*
   PA_FatRamBgLoad ( uScreen, uLayer,   (*csName).c_str() ); // Loads the BG to VRAM from RAM  
   PA_FatUnloadBg  ( (*csName).c_str() );                    // Removes the BG from RAM
   PA_FatFreeBgBuffers ();                                   // Clear RAM Bg buffers

   REG_IME = 0x01;

   // Load collision map if it is a level background
	if ( bLevel )
   {
      PA_FatLoadFile((*csColl).c_str(), (*csColl).c_str());
      miniMe = new Background(*ssTempD, 1, 2, false);//Loads Minimap Background
   }

   // Clean up string crap
   delete ssTempA;
   delete ssTempB;
   delete ssTempC;
   delete ssTempD;
   PA_WaitForVBL();//Waits for the next frame
}

Background::~Background ()
{
	PA_DeleteBg ( uScreen, uLayer );// Clear the bg from VRAM 
   if ( bLevel )
   {
      PA_FatUnloadFile((*csColl).c_str());
      delete miniMe;//Deletes minimap bg if deleting a level bg
   }
   delete csName;
   delete csColl;
}
