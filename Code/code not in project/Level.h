/* Ripholes in Rubbish Level Class Header
 */

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <cstdio>
#include <string>
#include <PA9.h>
#include "efs_lib.h"

//#include "RipholesWrappers.h"
#include "Page.h"
//#include "IngameGUI.h"

using namespace std;

class Level  
{
   private:
      vector<Page>*          vpPageList;
      vector<Page>::iterator itCurrPage;
      //IngameGUI              guGUI;

      vector<string>* vsGetTokens ( char* csLevelLine );

   public:
      Level ();                              // Don't use the default constructor
      Level ( string sInputFile );
      ~Level ();
      void update ();

      // Mechanic related functions
      bool rip ();
      void flip ( bool bUp );

      // "Getters"
      int iGetTotalPgs ();
      Page* itGetCurPg ();
      //IngameGUI* igGetGUI ();
};

#endif