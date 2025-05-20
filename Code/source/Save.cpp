/* Ripholes in Rubbish
 * Save class source
 */

#include "../include/Save.h"

Save::Save ()
{
}

Save::~Save ()
{
}

// Data functions
void Save::loadGame ()
{
}

void Save::saveGame ( vector<u8> &vuLvls, vector< pair<u8,bool> > &vpColls )
{
   uLvls = 0;

   // Iterate over the vector of available levels and convert to a single (highest) number
   vector<u8>::iterator itLvls = vuLvls.begin();

   for ( ; itLvls < vuLvls.end(); itLvls++ )
   {
      if ( uLvls < *itLvls ) uLvls = *itLvls;
   }

   // Iterate over the vector of collectible pairs and convert to Save's array
   vector< pair<u8,bool> >::iterator itColls = vpColls.begin();
   for ( ; itColls < vpColls.end(); itColls++ )
   {
      if ( (*itColls).second )
      {
         bColls[(*itColls).first] = true;
      }
      else
      {
         bColls[(*itColls).first] = false;
      }
   }
}

// Accessors
u8 Save::uGetLvls ()
{
   return uLvls;
}

bool* Save::bGetColls ()
{
   return bColls;
}
