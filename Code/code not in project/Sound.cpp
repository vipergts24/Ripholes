#include <cstring>
#include "Sound.h"
#include "rip.h"

using namespace std;

// When construct and initialize Sound class
Sound::Sound(){
	uMusicVolume = 128;
}

void Sound::playMusic(u8 uVolume, char* filename){
	//test to see if we have the file
	//filename.c_str();
	MP3FILE* file = FILE_OPEN(filename);
	if (!file){
		PA_InitText(1, 2);
		PA_OutputText(1, 0, 1, "FILE NOT FOUND");
	}
	FILE_CLOSE(file);

	AS_MP3StreamPlay(filename);
	AS_SetMP3Loop(true);
	AS_SetMP3Volume(uVolume);
}

void Sound::stopMusic(){
	AS_MP3Stop();
}
void Sound::playRip(){
	AS_SoundQuickPlay(rip);
}

void Sound::pauseMusic(){
	if(AS_GetMP3Status() & MP3ST_PAUSED){
		AS_MP3Unpause();
	}
	else{
		AS_MP3Pause();
	}		
}

void Sound::controlSound(){
	if(Pad.Newpress.L){
		if (uMusicVolume > 8){
			uMusicVolume -= 8;
		}
	}
	if(Pad.Newpress.R){
		if(uMusicVolume < 160){
			uMusicVolume += 8;
		}
	}
	if(Pad.Newpress.Select){
		pauseMusic();
	}
	if(Pad.Newpress.Y){
		stopMusic();
	}
	
}
