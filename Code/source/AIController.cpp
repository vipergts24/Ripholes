/*
   Ripholes in Rubbish
   AI Controller Class Source

   Handles all basic AI for our game
*/

#include "../include/AIController.h"

#define MAX_SPEED 768
#define MIN_SPEED 256
#define GRAVITY 68
#define FRICTION 65

//BehaviorEntry Constructor 
BehaviorEntry::BehaviorEntry ( s32 sDist, s32 sVelo, behavior bhBehav )
{
   bhType      = bhBehav;
   this->sVelo = sVelo;
   bMidCycle   = false;
   bPosDir     = true;
   uCount      = 0;
}

//Patrolling Behavior takes in a velocity and compares pixels on the collision map to see if it should change directions
pair<s32,s32> BehaviorEntry::pPatrol ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] )
{
   pair<s32,s32> pNewVel = make_pair ( (s32)0, (s32)0 );

   if ( !bMidCycle ) // If this is the first update, begin patrolling
   {
      pNewVel.first = sVelo;
      bMidCycle     = true;
   }
   else // not the first update, just set velocity
   {
      pNewVel.first = bPosDir ? sVelo : ( sVelo * -1 );
   }

   // Check left side against black and blue
   if ( ( iCollMapResults[COLL_LEFT]  == COLL_BLK || iCollObjResults[COLL_LEFT] == STATIC || iCollObjResults[COLL_LEFT] == PLATFORM) && ( pNewVel.first < 0 ) ) 
   {
      pNewVel.first *= -1;
      bPosDir = true;
   }
   if ( ( iCollMapResults[COLL_LEFT]  == COLL_BLU ) && ( pNewVel.first < 0 ) ) 
   {
      pNewVel.first *= -1;
      bPosDir = true;
   }
   // Check right side against black and blue
   if ( ( iCollMapResults[COLL_RIGHT] == COLL_BLK  || iCollObjResults[COLL_RIGHT] == STATIC || iCollObjResults[COLL_RIGHT] == PLATFORM) && ( pNewVel.first > 0 ) )
   {
      pNewVel.first *= -1;
      bPosDir = false;
   }
   if ( ( iCollMapResults[COLL_RIGHT] == COLL_BLU ) && ( pNewVel.first > 0 ) )
   {
      pNewVel.first *= -1;
      bPosDir = false;
   }

   return pNewVel;
}

//Behavior to make the AI Jump takes in a velocity and collision map points
pair<s32,s32> BehaviorEntry::pJump ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] )
{
	pair<s32,s32> pNewVel = make_pair ( (s32)0, (s32)0 );

   // Set the y-velocity
   if ( iCollMapResults[COLL_BOTTOM] == COLL_BLK || iCollObjResults[COLL_BOTTOM] == STATIC ) pNewVel.second = sVelo * -1;
   if ( iCollMapResults[COLL_BOTTOM] == COLL_GRN || iCollObjResults[COLL_BOTTOM] == PLATFORM ) pNewVel.second = sVelo * -1;

	return pNewVel;
}
//Behavior for a horizontal moving platform object that takes in a velocity, and two collision map points
pair<s32,s32> BehaviorEntry::pHorizPlat ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] )
{
   pair<s32,s32> pNewVel = make_pair ( (s32)0, (s32)0 );

   if(bhType == HORIZPLAT)pNewVel = pPatrol ( pCurVel, iCollMapResults, iCollObjResults );
   if(bhType == HORIZPLAT)pNewVel.second = GRAVITY * -1;

   return pNewVel;
}
//Update function for all the behaviors.  Returns a new location for the object after evaluating.
pair<s32,s32> BehaviorEntry::pUpdate ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] )
{  
   pair<s32,s32> pNewVel;

   switch ( bhType )
   {
      case PATROL:
         pNewVel = pPatrol ( pCurVel, iCollMapResults, iCollObjResults );
         break;
      case JUMP:
         pNewVel = pJump ( pCurVel, iCollMapResults, iCollObjResults );
         break;
      case HORIZPLAT:
         pNewVel = pHorizPlat ( pCurVel, iCollMapResults, iCollObjResults );
         break;
      case GROUNDPLAT:
         pNewVel = pHorizPlat ( pCurVel, iCollMapResults, iCollObjResults );
         break;
      case STATIC:
      case FAIL:
      case GRAV:
      default:
         pNewVel = make_pair ( (s32)0, (s32)0 );
         break;
   }
   return pNewVel;
}

// bhStrToBehav:
// Convert strings to behavior types based off of reading the behavior from the data file
behavior bhStrToBehav ( string sInput )
{
   if ( !strcmp ( sInput.c_str(), "patrol"    ) ) return PATROL;
   if ( !strcmp ( sInput.c_str(), "jump"      ) ) return JUMP;
   if ( !strcmp ( sInput.c_str(), "static"    ) ) return STATIC;
   if ( !strcmp ( sInput.c_str(), "horizplat" ) ) return HORIZPLAT;
   if ( !strcmp ( sInput.c_str(), "groundplat" ) ) return GROUNDPLAT;
   if ( !strcmp ( sInput.c_str(), "gravity" ) ) return GRAV;

   // The string didn't match any known behavior
   return FAIL;
}
//Constructor for AIController
AIController::AIController ( vector<string> &vsBehavs )
{
   vector<string>::iterator vsIter;
   s32 sDist = 0;
   s32 sVelo;
   behavior bhBehav;

   // Convert strings in vector to behavior entries
   for ( vsIter = vsBehavs.begin(); vsIter < vsBehavs.end(); vsIter++ )
   {
      bhBehav = bhStrToBehav ( *vsIter );
      vsIter++;
      sVelo   = (s32) atoi ( (*vsIter).c_str() );

      vbHavior.push_back ( BehaviorEntry ( sDist, sVelo, bhBehav ) );
   }
}
//Constructor for AIController
AIController::AIController ( const AIController& aiCont )
{
   this->vbHavior = vector<BehaviorEntry> ( aiCont.vbHavior );
}

// pUpdate returns the new position based on behaviors and collision detections
pair<s32,s32> AIController::pUpdate ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] )
{
   pair<s32,s32> pNewVel = make_pair ( (s32)0, (s32)0 );
   pair<s32,s32> pVelos;
   vector<BehaviorEntry>::iterator bhIter;

   // Get velocity changes from the behavior vector
   for ( bhIter = vbHavior.begin(); bhIter <= vbHavior.end(); bhIter++ )
   {
      pVelos = (*bhIter).pUpdate ( pCurVel, iCollMapResults, iCollObjResults );

      pNewVel.first  += pVelos.first;
      pNewVel.second += pVelos.second;
   }

   return pNewVel;
}

bool AIController::bStatic ()
{
   vector<BehaviorEntry>::iterator bhIter;

   // Iterate over behaviors and check if they're static
   for ( bhIter = vbHavior.begin(); bhIter < vbHavior.end(); bhIter++ )
   {
      if ( (*bhIter).bStatic() )
      {
         return true;
      }
   }

   return false;
}
