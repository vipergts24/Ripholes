#include <PA9.h>       // Include for PA_Lib
#include "../include/RipholesWrappers.h"
#include <utility>
//#include "gfx/level_01_gfx.cpp"
//#include "gfx/level_01_gfx.h"
#include "gfx/garage_gfx.cpp"
#include "gfx/garage_gfx.h"
#include "../include/Game.h"
#include "../include/StartMenu.h" //Include Start Menu Class
#include "../include/IngameGUI.h"
#include "../include/Player.h"
#include "../include/Level.h"
#include "../include/Sound.h"

using namespace std;

int main ( int argc, char ** argv )
{
   Game gaRipholes = Game ();
   string sLevel = "sprite01.txt";

   gaRipholes.titleScreen ();
   gaRipholes.worldMap ();
   //gaRipholes.playLevel ( sLevel );

 //  PA_LoadSpritePal(0, 5, (void*) garage1_Pal);
	//PA_LoadSpritePal(0, 6, (void*) garage2_Pal);
	//Sprite* garage_right = new Sprite(1, 0, (void*) garagedoor_right_Sprite, OBJ_SIZE_64X64, 5, 576<<8, 512<<8);
	//Sprite* garage_left = new Sprite(1, 0, (void*) garagedoor_left_Sprite, OBJ_SIZE_64X64, 6, 512<<8, 512<<8);
 // 
   while ( true )  
   {
     gaRipholes.update ();

     //garage_left->moveSprite(628<<8, 505<<8);
     //garage_right->moveSprite(692<<8, 505<<8);
     //s32 x1 = PA_GetSpriteX(0, garage_left->getId()) - 5;
     //s32 y1 = PA_GetSpriteY(0, garage_left->getId()) - 5;
     //s32 x2 = PA_GetSpriteX(0, garage_left->getId()) + (5 + PA_GetSpriteLx(0, garage_left->getId()));
     //s32 y2 = PA_GetSpriteY(0, garage_left->getId()) + (5 + PA_GetSpriteLy(0, garage_left->getId()));
     //s32 dragDist = 0;
     //s32 prevDist = 0;
     //s32 originX,originY;
     //if( Pad.Newpress.B && gaRipholes.leGetCurLvl()->pGetPlayer()->TouchingGround() )
     //{
	    // while( true )
     //   {
		   //  if( Stylus.Newpress )
     //      {
		   //     if( PA_StylusInZone(x1, y1, x2, y2) )
     //         {
				 //    //PA_OutputText(1,0,1,"ripped nearby");	
				 //    originX = Stylus.X;
				 //    originY = Stylus.Y;
				 //    while( true )
     //            {
					//     if ( dragDist >= 50 )
     //               {
					//	     //PA_OutputText(1,0,2,"Sprite ripped");
					//	     break;
					//     }
					//     else if ( Stylus.Released )
     //               {
					//	     //PA_OutputText(1,0,2,"Sprite not ripped");	
					//	     break;
					//     }
					//     else if(PA_Distance(originX,originY,Stylus.X,Stylus.Y) > prevDist)
     //               {
     //                  //inc animation frame
     //                  ++dragDist;
     //                  PA_MoveSpriteDistance(garage_left->getId(),PA_Distance(originX,originY,Stylus.X,Stylus.Y));
     //                  prevDist = PA_Distance(originX,originY,Stylus.X,Stylus.Y);
     //                  //PA_OutputText(1,0,3,"Drag Dist %d",dragDist);
     //                  //PA_OutputText(1,0,4,"prev Dist %d",prevDist);	
					//     }
					//     else if( PA_Distance(originX,originY,Stylus.X,Stylus.Y) < prevDist )
     //               {
     //                  //dec animation frame
					//	     --dragDist;
					//	     PA_MoveSpriteDistance(garage_left->getId(),PA_Distance(originX,originY,Stylus.X,Stylus.Y));
					//	     prevDist = PA_Distance(originX,originY,Stylus.X,Stylus.Y);
					//	     //PA_OutputText(1,0,3,"Drag Dist %d",dragDist);	
					//	     //PA_OutputText(1,0,4,"prev Dist %d",prevDist);	
					//     }
   					  
       //             PA_WaitForVBL();
			    //    }
		     //   }
			    // //else PA_OutputText(1,0,1,"not ripped nearby");
		     //}
		    // PA_WaitForVBL();
		    // //dragDist = 0;
		    // //prevDist = 0;
		    // //PA_OutputText(1,0,3,"Drag Dist %d",dragDist);	
		    // //PA_OutputText(1,0,4,"prev Dist %d",prevDist);
		    // if(Pad.Newpress.B) break;
	     // }
      //}
      
      PA_WaitForVBL();
   }

	return 0;
}