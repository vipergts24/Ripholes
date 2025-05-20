/* Ripholes in Rubbish
 * PROGRAMMER: Mee Cha
 * Header file for Page
*/

#ifndef __Page_H__
#define __Page_H__
#include <PA9.h>
#include <vector>
#include <string>
#include <cstdlib>

//#include "Player.h"

using namespace std;

class Page {
public:
   Page(u8 num);
   ~Page();

	void update();

//LevelObject loGetTouchedObj();	

	void rmvObj();						// Remove object from Page
	//void addObj(LevelObject &myObj);
	void createDood(vector<string> &vString);				

	// Accessor functions
	u8 uGetPageId() const;
	s32 sGetHealth() const;

	//void initStrInt();				// mapping out the strings and int for reading in file input
   int iDoodType(string s);

 private:
	u8 uPageID;
	u8 uBgNum;
	s32 sHealth;
	//vector<LevelObject> vObjList; 
	//vector<LevelObject>::iterator loIter;
	//static map<string, strInt> msiDoodVal;
	//PAGfx_struct bg;

//PAGfx_struct *bg[3] = {&SparklyBG1, &SparklyBG2, &SparklyBG3}; 
//
//int currentBG = 0;
//
////then you use :
//PA_EasyBgLoadPtr(screen, bg number, bg[currentBG]);
//
////just change curretnBG to load different backgrounds :)

 };

#endif 
 