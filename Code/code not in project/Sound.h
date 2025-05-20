#if !defined(_SOUND_H)
#define _SOUND_H

#include <PA9.h>
#include <cstring>

using namespace std;

class Sound{
	public:
		Sound();		
		~Sound();
		u8 uMusicVolume;
		//u8 adjustVolume(u8 uVolume);
		void playMusic(u8 uVolume, char* filename);
		void playRip();
		void pauseMusic();
		void stopMusic();
		void controlSound();
		
};

#endif
