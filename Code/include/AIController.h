/*
   Ripholes in Rubbish
   AI Controller Class header
*/

#ifndef __AICONTROLLER_H__
#define __AICONTROLLER_H__

#include <cstring>
#include <string>
#include <utility>
#include <vector>
#include <PA9.h>
#include "Character.h"

// Collision type defines
// If you looked at our collision map, the map the map would be identical to the level except various
// colors indicate different properties as follows:
// 0: Magenta, empty
// 1: Black,   collision
// 2: Red,     death
// 3: Green,   bottom-only collision
// 4: Blue,    patrol turn-point 
#define COLL_MAG 0
#define COLL_BLK 1
#define COLL_RED 2
#define COLL_GRN 3
#define COLL_BLU 4

// Collision Array Indexes:
// 0 == left, 1 == right, 2 == up, 3 == down
#define COLL_LEFT   0
#define COLL_RIGHT  1
#define COLL_TOP    2
#define COLL_BOTTOM 3

using namespace std;

enum behavior {   PATROL, 
                  JUMP,
                  STATIC,
                  HORIZPLAT,
                  FAIL,
                  GRAV,
                  GROUNDPLAT};

//BehaviorEntry class indicating the types of behaviors
class BehaviorEntry
{
   public:
      BehaviorEntry ( s32 sDist, s32 sVelo, behavior bhBehav );
		~BehaviorEntry (){};

      pair<s32,s32> pUpdate ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] );

      // Accessors
      bool bStatic () { return ( bhType == STATIC ); }

      // Behavior-Specific
      pair<s32,s32> pPatrol      ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] );
	   pair<s32,s32> pJump       ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] );
      pair<s32,s32> pHorizPlat   ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] );
      pair<s32,s32> pGroundPlat  ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] );

   private:
      behavior       bhType;     // Type of behavior contained in this entry
      bool           bMidCycle;  // Is the behavior mid-cycle for the current update?
      bool           bPosDir;    // Is the behavior in the positive dir (X or Y) for the current update?
      s32            sVelo;      // What velocity should be added if there is no collision
      u8             uCount;     // Generic int counter for various things like jump interval, printing, etc
};

//AI Class Controller for objects containing various behaviors
class AIController
{
   public:
      AIController ( vector<string> &vsBehavs );
      AIController ( const AIController& aiCont );
		~AIController () { vbHavior.clear(); };
      
      pair<s32,s32> pUpdate ( pair<s32,s32> pCurVel, s16 iCollMapResults[4], s16 iCollObjResults[4] );
      bool bStatic ();

   private:
      vector<BehaviorEntry> vbHavior;
};

#endif
