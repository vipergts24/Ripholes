/* Ripholes in Rubbish
 * PROGRAMMER: Mee Cha
 * Header file for Journal 
*/

#ifndef __Journal_H__
#define __Journal_H__
#include <vector>

using namespace std;


class Journal {
public:
	Journal();

	void exit();

private:
	vector<u8> vuMemoirList;
	bool bInJournal;

}

#endif 
 