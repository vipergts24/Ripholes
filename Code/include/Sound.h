// Sound class header

#ifndef __SOUND_H__
#define __SOUND_H__

#include <PA9.h>
#include <string>
#include <cstring>

using namespace std;

void playRip();
void playJump();
void playCrumple();
void playPaste();
void playError();
void playPageFlip();
void playDamage();
void playMedRabble();
void playHiRabble();
void playWriting();
void playSelected();
void playScrib();
void playCollected();
void playSpring();
void playArky();
void playCat();
void playHiss();
void stopMusic();

class Sound
{
	public:
		Sound();		
		~Sound();
		u8 uMusicVolume;
		//u8 adjustVolume(u8 uVolume);
		void playMusic(u8 uVolume, string sFilename);
		void pauseMusic();
		void controlSound();
      void stopMusic();
};

#endif
