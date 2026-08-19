
#ifndef _TEXTURA_H
#define _TEXTURA_H

#include <math.h>


#define GL_CLAMP_TO_EDGE	0x812F	// This is for our skybox textures

class Textura{  //: public appearance{
	unsigned int id;
	int textWidth, textHeight;
public:	

	Textura(char *filename)
	{
		initTexture(filename);
	}

	int getWidth(void)
	{
		return textWidth;
	}

	int getHeight(void)
	{
		return textHeight;
	}
	
	//faz a configuracao da textura
	void initTexture(char *filename)
	{
	    glGenTextures( 1, &id );
	    glBindTexture( GL_TEXTURE_2D, id );

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		// Build Mipmaps (builds different versions of the picture for distances - looks better)
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->rgb);


		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_SMOOTH);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_SMOOTH);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);


		char*	data = new char[1024 * 1024 * 3];
		FILE*	fp = fopen(filename, "rb");
		fread(data, 1, 1024 * 1024 * 3, fp);
		fclose(fp);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, data);

		delete [] data;
	}

	void render()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, id );
	}
};

#endif
