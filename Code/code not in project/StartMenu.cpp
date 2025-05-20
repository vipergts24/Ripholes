#include "StartMenu.h"
#include "Start_Menu.h" // gif to include
#include "press_start.h" // gif to include

StartMenu::StartMenu(){
   
}   

void StartMenu::newGame(){
   
}
void StartMenu::initStartMenu(){
     
   PA_Init8bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	
	PA_SetBrightness(0, -31); // all black
	PA_SetBrightness(1, -31); // all black	
	
	// Load splash while hidden
	PA_LoadGif(	1, (void*)Start_Menu); // Gif File
	PA_LoadGif(	0, (void*)press_start);
	s8 i;
	// Transition to normal visible background
	for(i = -31; i <= 0; i++){
		PA_SetBrightness(0, i); 
		PA_SetBrightness(1, i); 
		PA_WaitForVBL();		   
	}   

	s16 time = 180; // 180 frames = 3 seconds
	while(!Stylus.Newpress){ // Display until time over or keys pressed
		time--; // time goes by
		PA_WaitForVBL();
	}		
	
	// Transition to black
	for(i = 0; i > -32; i--){
		PA_SetBrightness(0, i); 
		PA_SetBrightness(1, i); 
		PA_WaitForVBL();		   
	}  	

	// Now that it's all black, clean all that and you're ready to go !
	PA_ResetBgSys();
	PA_SetBrightness(0, 0); // normal
	PA_SetBrightness(1, 0); // normal
}   
