#if !defined(_HELPERDOOD_H)
#define _HELPERDOOD_H

#include "LevelObject.h"
#include "hitBox.h"
#include "Sprite.h"


using namespace std;

class HelperDood : public LevelObject
{

	public:

		HelperDood(pair<s32,s32> inPos, u8 pid, bool isRippable);

		void update();

		~HelperDood();

	private:
};












#endif
