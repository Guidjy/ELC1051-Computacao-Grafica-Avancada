
#ifndef _TEXTURA_H
#define _TEXTURA_H


#include <math.h>
#include "Image.h"


#define GL_CLAMP_TO_EDGE	0x812F	// This is for our skybox textures

#define TEXT_SKYBOX    1
#define TEXT_MD2       2
#define TEXT_NORMAL    3
#define TEXT_BILLBOARD 4


#define INVERTE_Y_OK  1
#define INVERTE_Y_NO  0

//a textura e' armazenada em um vetor, que possui valores R, G e B. Nao e' usada
//matriz bidimensional porque da erro no mapeamento se o tamanho default desta
//matriz for maior que o tamanho da imagem de textura, na funcao BindTexture().  


//IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE IMPORTANTE
//Ordem de execucao na hora de CRIAR a textura:
// - gerar textura ID
// - Bind Textura
// - Gerar mapa. 
// Se nao for nesta ordem, as texturas serao trocadas


class Textura{  //: public appearance{
	unsigned int id;
	Image   *image;
	int textWidth, textHeight;
public:	

	Textura(char *fileName, bool inverte, int modo)
	{
		image = new Image(fileName);
		textWidth  = image->width;
		textHeight = image->height;

		if(inverte)
			image->inverteY();
		//glEnable(GL_TEXTURE_2D);

		if( modo == TEXT_SKYBOX )
			intTextureSkyBox();
		if( modo == TEXT_MD2 )
			initTextureMD2();
		if( modo == TEXT_NORMAL )
			initTexture();
		if( modo == TEXT_BILLBOARD )
			initTextureBillboard();

		//apos gerada a textura do openGL, a imagem nao eh mais necessaria
		delete image;
	}

	int getWidth(void)
	{
		return textWidth;
	}

	int getHeight(void)
	{
		return textHeight;
	}

	void intTextureSkyBox(void)
	{
		// Generate a texture with the associative texture ID stored in the array
		glGenTextures(1, &id);

		// This sets the alignment requirements for the start of each pixel row in memory.
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

		// Bind the texture to the texture arrays index and init the texture
		glBindTexture(GL_TEXTURE_2D, id);

		// Build Mipmaps (builds different versions of the picture for distances - looks better)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->rgb);

		// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
		// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
		// but looks blochy and pixilated.  Good for slower computers though.  
			
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		// The default GL_TEXTURE_WRAP_S and ""_WRAP_T property is GL_REPEAT.
		// We need to turn this to GL_CLAMP_TO_EDGE, otherwise it creates ugly seems
		// in our sky box.  GL_CLAMP_TO_EDGE does not repeat when bound to an object.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void initTextureBillboard()
	{
		glEnable(GL_DEPTH_TEST);
		
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER	,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER	,GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->rgb);
	}

	//faz a configuracao da textura
	void initTextureMD2(void)
	{
		// création et initialisation de la texture opengl
		glGenTextures( 1, &id );
		glBindTexture( GL_TEXTURE_2D, id );

		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		
		//	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );
		//glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->rgb);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->rgb);
	}

	//faz a configuracao da textura
	void initTexture(void)
	{
		//glMatrixMode(GL_TEXTURE); 
		//glLoadIdentity( );
		//glMatrixMode(GL_MODELVIEW); 
		//glDisable(GL_TEXTURE_GEN_S);
		//glDisable(GL_TEXTURE_GEN_T);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);


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
		FILE*	fp = fopen("data\\textura.raw", "rb");
		fread(data, 1, 1024 * 1024 * 3, fp);
		fclose(fp);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, data);

		delete [] data;
      /**/

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->rgb);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->rgb);
	}/**/

	void render()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, id );
		//printf(" = %d", id);
	}
};

#endif
