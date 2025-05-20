/* Ripholes in Rubbish
* PROGRAMMER: Mee Cha
* Header file for Page
*/

#ifndef __Page_H__
#define __Page_H__
#include <PA9.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#include "Background.h"
#include "Event.h"
#include "HelperDood.h"
#include "Hole.h"
#include "LevelObject.h"
#include "PageHealth.h"
#include "Player.h"
#include "Sprite.h"
#include "Collectible.h"

using namespace std;

class Page {
	public:
		Page(u8 num, string sBgName, u8 uLvlId);
		~Page();

		void update (bool bDrawSprites, bool bDrawBg, bool moveObjects, pair<s32,s32> psAvaPos, bool bIsCollected, Sprite * magic);

		vector<LevelObject*>::iterator loGetTouchedObj();
      vector<LevelObject*>::iterator getObj(u8 spriteId);
		void createDood(vector<string> &vString);
      void deleteBg();

      // mutators
		void rmvObj(vector<LevelObject*>::iterator loIter); // Remove object from Page
		void addObj(LevelObject &myObj);
		void flipBg ();
      void decHealth(u8 num);  // decrease the health of the page given a sucessful rip
      void checkDrawTrans(bool bSetTrue, Player* ava); // sets bool to true so we know to draw the transparent avatar
      void setDrawTransFalse();
		void hideAllSprites();

		// Accessor functions
		u8  uGetPageId() const {return uPageID;}
      Background *  bgGetPageBg() const {return bgPage;}
      int  uGetColl();
		s32 sGetHealth() const {return myHealth->sGetHealth();}
		s32 sgetObjX(LevelObject* obj);
		s32 sgetObjY(LevelObject* obj);
		s32 sgetObjLength(LevelObject* obj);
		s32 sgetObjHeight(LevelObject* obj);
		vector<LevelObject*>* vgetObjList(){ return vObjList;}
		//vector<LevelObject*>* veGetEvents(){ return eventList;}


      //vector<LevelObject*>* vgetEventList(){ return eventList;}
		//vector<Hole*>* vGetHoleList() {return vHoleList;}

		PageHealth* phGetPH() {return myHealth;}
      Sprite* sGetTransAva() {return sAvaTrans;}
      Sprite* sGetTextBox() {return text;}

		// mapping out the strings and int for reading in file input
		int iDoodType(string s);

	private:
		u8 uPageID;
		u8 uLevelId;
		u8 uBgNum;
		bool bgLoaded;
		string sBg;
		vector<LevelObject*>* vObjList;
      vector<LevelObject*>::iterator loIter;
		PageHealth* myHealth;
		Background* bgPage;
      Sprite* sAvaTrans;
      Sprite* text;
      pair<s32,s32> psPos; 
      bool bDrawTrans;  // set to true if the transparent avatar needs to be drawn
};

#endif 
