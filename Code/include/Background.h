/*
   Ripholes in Rubbish
   Background Class Header
*/

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <string>
#include <cstdio>
#include <cstring>
#include <PA9.h>

#define LEVEL_LENGTH 64

using namespace std;

class Background
{
   public:
      Background ( string sName, u8 uScreen, u8 uLayer, bool bLevel );
      ~Background ();
      void uSetCollisionMap();
      int uGetColl () {return uColl;}
      int getSlotNum(){ return *slotNum; }
      u8 uGetLayer(){return uLayer;}

   private:
      string* csName;
      string* csColl;
      int* slotNum;
      int uColl;
      bool bLevel;
      u8 uScreen;
      u8 uLayer;
      Background* miniMe;
};

#endif
