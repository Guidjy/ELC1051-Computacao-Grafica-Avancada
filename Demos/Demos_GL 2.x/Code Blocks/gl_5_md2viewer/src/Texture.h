#ifndef ___TEXTURE__H___
#define ___TEXTURE__H___

#include <GL/glut.h>
#include "Bmp.h"

class Texture
{
   Bmp   *img;
   GLuint ID;
   unsigned char *data;
public:
   Texture(char *filename)
   {
      img = new Bmp(filename);
      img->convertBGRtoRGB();
      data = img->getImage();
      if( data != NULL )
      {
         glEnable(GL_TEXTURE);
         glEnable(GL_TEXTURE_2D);
      
         buildTexture();
      }
   }

   void render()
   {
      glEnable(GL_TEXTURE);
      glEnable(GL_TEXTURE_2D);
      glBindTexture( GL_TEXTURE_2D, ID );
   }
   
private:
   void buildTexture()
   {
      glGenTextures( 1, &ID );
      glBindTexture( GL_TEXTURE_2D, ID );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      gluBuild2DMipmaps(GL_TEXTURE_2D, 
                        GL_RGB, 
                        img->getWidth(),
                        img->getHeight(), 
                        GL_RGB, 
                        GL_UNSIGNED_BYTE, 
                        data);
   }

};

#endif
