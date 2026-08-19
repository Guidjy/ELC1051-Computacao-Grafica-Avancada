#include <stdio.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////
class Frames{
   clock_t t1, t2;   
   long    cont_frames;
public:
   Frames()
   {
      t1 = clock();
      cont_frames = 0;
   }

   void getFrames()
   {
      double tempo;
      t2 = clock();
      tempo  = (double)(t2 - t1) / CLOCKS_PER_SEC;
      if( tempo >= 1.0)
      {
         t1 = t2;
         printf("\r %ld frames per second  ", cont_frames);
         cont_frames = 0;
      }
      cont_frames++;
   }
};
