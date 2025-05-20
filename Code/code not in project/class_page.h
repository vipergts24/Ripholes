/* Ripholes in Rubbish
 * PROGRAMMER: Mee Cha
 * Class for Page
*/

 #if !defined(_PAGECLASS_H)
 #define _PAGECLASS_H

 using namespace std;

 class page {
 public:
    page();
	// NOTE: pixel perfect algorithm - vGetRipPath();
	// NOTE: dont know what this is for- getPlatform(); 
	// DELETE: environmental dood in lvlObjs, uGetMemoir();
	// NOTE: this is on UML diagram, but can be called in the constructor- createLvlObj();
	// bool bRip();				// return a boolean if rip was successful, in player class
	// DELETE: void ripObjs();
	// DELETE: bool bCheckInvCap();
	// DELETE: void objToInv();

	void update();
	*LevelObj loGetTouchedObj(); // NEW, iterates over list of objects
	bool bFlip();			// return a boolean if pageflip was successful
	bool bRmvObj();		//remove object from page
	void addHole();
	void addObj();

	// Access attributes of object
	u8 uGetPageId() const {return uPageID;}
	s32 sGetHealth() const {return sHealth;}

 private:
	u8 uPageID;
	s32 sHealth;
	//bg largeMap;
	List<LevelObj> lObjList; 
	// NOTE: dont know what this is for -bool haveRipped;

 }


 // Constructor: initialize attributes for page
 page::page() {
    // uPageID = TODO;
    sHealth = 100.0;
    // objList = TODO;
 }

 // constructor: use when given parameters
 page::page(u8 num) {
    uPageID = num;
    sHealth = 100.0;
    objList = ;
 }

 // Update in page class will call update on the list of objects it contains
 void page::update()
 {
	 lObjList.update();
 }

 
 // Class for page flip mechanic. Returns a boolean that determines if the flip was successful or not
 bool page::bFlip()
 {
	if (LSRel == true)			// check to see if the player wants to flip to a prev page
	{
		if (uGetPageID() != 1)		// continue page flip if player is not on first page
		{
			setCurrPage(uPageID - 1);  // subtract one page to get new page, function is in level
		}
	}
	else if (RSRel == true)
	{
		if (lvl.uGetPageID() != lvl.uGetTotalPages())
	}
 }
 

 bool page::bRmvObj()
 {

 }

 void page::addHole()
 {

 }

#endif  //_PAGECLASS_H
