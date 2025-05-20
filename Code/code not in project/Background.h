#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <utility>
#include <string>
#include <nds.h>
using namespace std;
#define norm_fix(x)			((x)<<8)
#define fix_norm(x)			((x)>>8)  //xxx truncates instead of rounds


class Background
{
   public:
      Background (u8 layer, u8 screen);
      Background (u8 layer, u8 level, u8 page, u16 width, u16 height);
      void scroll (u16 x, u16 y);
      void update();
      ~Background ();

   private:
      int bgNum;
};

#endif