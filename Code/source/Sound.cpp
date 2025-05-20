// a few init and sound system related palib refs

#include "../include/Sound.h"
#include "rip.h"
#include "jump.h"
#include "crumple.h"
#include "paste.h"
#include "error.h"
#include "playerdmg.h"
#include "selected.h"
#include "scrib.h"
#include "collected.h"
#include "spring.h"

#include "wmap.h"
#include "theme.h"
#include "mountain.h"


using namespace std;

// When construct and initialize Sound class
Sound::Sound(){
   uMusicVolume = 127;
   
}

Sound::~Sound(){
   stopMusic();
}

void Sound::playMusic(u8 uVolume, string sFilename)
{
   // If music is playing, stop it
   if ( AS_GetMP3Status() != MP3ST_STOPPED ) AS_MP3Stop();
   AS_SetMP3Loop   ( true );
   AS_SetMP3Volume ( uVolume );
   AS_SetMasterVolume (uVolume);
   // Play the current file
   if ( !strcmp ( sFilename.c_str(), "theme" ) ) AS_MP3DirectPlay ( (u8*)theme, (u32)theme_size );
   else if ( !strcmp ( sFilename.c_str(), "wmap"  ) ) AS_MP3DirectPlay ( (u8*)wmap,  (u32)wmap_size  );
   else if ( !strcmp ( sFilename.c_str(), "mountain"  ) ) AS_MP3DirectPlay ( (u8*)mountain,  (u32)mountain_size  );
}

void Sound::stopMusic()
{
   AS_MP3Stop();
}

void Sound::pauseMusic(){
   if(AS_GetMP3Status() & MP3ST_PAUSED){
      AS_MP3Unpause();
   }
   else{
      AS_MP3Pause();
   }

   PA_WaitForVBL();
}

void Sound::controlSound(){
   if(Pad.Held.L){
      if (uMusicVolume > 8){
         uMusicVolume -= 8;
      }
   }
   if(Pad.Held.R){
      if(uMusicVolume < 160){
         uMusicVolume += 8;
      }
   }
   if(Pad.Newpress.Select){
      pauseMusic();
   }
}

void playRip(){
   AS_SoundQuickPlay(rip);
}

void playJump(){
   AS_SoundQuickPlay(jump);
}

void playCrumple(){
   AS_SoundQuickPlay(crumple);
}

void playPaste(){
   AS_SoundQuickPlay(paste);
}

void playError(){
   AS_SoundQuickPlay(error);
}

void playDamage(){
   AS_SoundQuickPlay(playerdmg);
}

void playSelected(){
   AS_SoundQuickPlay(selected);
}

void playScrib(){
   AS_SoundQuickPlay(scrib);
}

void playCollected(){
   AS_SoundQuickPlay(collected);
}

void playSpring(){
   AS_SoundQuickPlay(spring);
}

