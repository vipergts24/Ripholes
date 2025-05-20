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
#include "arky.h"
#include "cat.h"
#include "hiss.h"

#include "wmap.h"
#include "theme.h"
//#include "spooky.h"
#include "mountain.h"
#include "credits.h"


using namespace std;

// When construct and initialize Sound class
Sound::Sound(){
   uMusicVolume = 128;
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
   // Play the current file
   if ( !strcmp ( sFilename.c_str(), "theme" ) ) AS_MP3DirectPlay ( (u8*)theme, (u32)theme_size );
   if ( !strcmp ( sFilename.c_str(), "wmap"  ) ) AS_MP3DirectPlay ( (u8*)wmap,  (u32)wmap_size  );
   //if ( !strcmp ( sFilename.c_str(), "spooky"  )) AS_MP3DirectPlay ( (u8*)spooky, (u32)spooky_size );
   if ( !strcmp ( sFilename.c_str(), "mountain")) AS_MP3DirectPlay ( (u8*)mountain, (u32)mountain_size);
   if ( !strcmp ( sFilename.c_str(), "credits" )) AS_MP3DirectPlay ( (u8*)credits, (u32)credits_size);

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

void playArky(){
   AS_SoundQuickPlay(arky);
}

void playCat(){
   AS_SoundQuickPlay(cat);
}

void playHiss(){
   AS_SoundQuickPlay(hiss);
}

