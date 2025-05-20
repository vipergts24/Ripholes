// 0 palib refs

#if !defined(_HITBOX_H)
#define _HITBOX_H

#include <PA9.h>
#include <string>

using namespace std;

class hitBox {
public:

   typedef struct{
      s8 x, y, flipx;
   }pointInfo; 

   pointInfo left, right, up, down;
   u8 flipped;

   hitBox(string name){
      setHitBox(name);
   };

   ~hitBox(){};

   void setHitBox(string name){
      switch(getValue(name)){
         case 1: //player hitbox
				left.x=2; 
				left.y=8; 
				right.x=29; 
				right.y=8; 
				down.x=10; 
				down.y=31; 
				up.x=5; 
				up.y=0;
				left.flipx = 0;
				right.flipx = 0;
				up.flipx = 10;
				down.flipx = 11;
            flipped = 0;
            break;
         case 2:
			 left.x=4; 
				left.y=15; 
				right.x=26; 
				right.y=15; 
				down.x=14; 
				down.y=29; 
				up.x=15; 
				up.y=2;
				left.flipx = 0;
				right.flipx = 0;
				up.flipx = 0;
				down.flipx = 0;
            flipped = 0;//scribz hitbox
            break;
         case 3: //garage/door hitbox
            left.x = 0;
            left.y = 0;
            right.x = 64;
            right.y = 0;
            up.x = 0;
            up.y = 0;
            down.x = 0;
            down.y = 64;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
         case 4: //log
            left.x = 6;
            left.y = 55;
            right.x = 61;
            right.y = 52;
            up.x = 28;
            up.y = 41;
            down.x = 31;
            down.y = 62;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
         case 5: //rock
            left.x = 9;
            left.y = 45;
            right.x = 56;
            right.y = 45;
            up.x = 30;
            up.y = 28;
            down.x = 30;
            down.y = 60;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
         case 6: //collectible 32x32
            left.x = 0;
            left.y = 0;
            right.x = 32;
            right.y = 0;
            up.x = 0;
            up.y = 0;
            down.x = 0;
            down.y = 32;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
         case 7: //collectible 32x32
            left.x = 0;
            left.y = 0;
            right.x = 45;
            right.y = 0;
            up.x = 0;
            up.y = 16;
            down.x = 0;
            down.y = 32;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
            case 8: //platform_cloud
            left.x = 2;
            left.y = 16;
            right.x = 59;
            right.y = 16;
            up.x = 30;
            up.y = 15;
            down.x = 30;
            down.y = 24;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
            case 9: //spring
            left.x = 0;
            left.y = 10;
            right.x = 22;
            right.y = 10;
            up.x = 10;
            up.y = 10;
            down.x = 10;
            down.y = 32;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
         default:
            left.x = 0;
            left.y = 0;
            right.x = 0;
            right.y = 0;
            up.x = 0;
            up.y = 0;
            down.x = 0;
            down.y = 0;
            left.flipx = 0;
            right.flipx = 0;
            up.flipx = 0;
            down.flipx = 0;
            flipped = 0;
            break;
      }
   }

   int getValue(string name){
      if(name == "player") return 1;
      else if(name == "scrib") return 2;
      else if(name == "garageleft" || name == "garageright" || name == "door") return 3;
      else if(name == "log") return 4;
      else if(name == "rock") return 5;     
      else if(name == "collectible_crayon_red" || 
         name == "collectible_crayon_orange" || 
         name == "collectible_crayon_yellow" || 
         name == "collectible_crayon_green" || 
         name == "collectible_crayon_blue" || 
         name == "collectible_crayon_purple" ) return 6;
      else if(name == "door") return 7;
      else if(name == "platform_cloud") return 8;
      else if(name == "spring") return 9;
      else return 0;
   }
};

#endif
