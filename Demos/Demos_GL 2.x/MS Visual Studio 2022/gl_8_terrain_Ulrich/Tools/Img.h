// classe para fazer a leitura de imagens em diversos formatos e armazena-la
// em uma matriz. E' usada a biblioteca IM, to Tecgraf.

#ifndef _IMG_H
#define _IMG_H


#include "im.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXDIM 262144 //512x512


class Image{ 
	unsigned char *red, *green, *blue;
public:
	int width, height;
	int tem_img;
	GLubyte rgb[MAXDIM][3];

	Image(char *fileName)
	{
		int w, h, t, pal_size, err;

		printf("\nVai abrir arquivo em Image.h:  %s ", fileName);

		err = imImageInfo(fileName, &w, &h, &t, &pal_size);
		if (err)
		{
			ErrorMsg(err);
			return;
		}
		aloca(w, h);
		err = imLoadRGB(fileName, red, green, blue);
		if (err)
		{
			ErrorMsg(err);
			free(red);
			free(green);
			free(blue);
			tem_img = 0;
		}
		tem_img = 1;

		long cont, num;

		//cria um mapa da textura a partir do R,G,B
		num = width * height; 
		for(cont = 0; cont<num; cont++)
		{
			rgb[cont][0] = (GLubyte) *(red+cont);
			rgb[cont][1] = (GLubyte) *(green+cont);
			rgb[cont][2] = (GLubyte) *(blue+cont);
		}
		free(red);
		free(green);
		free(blue);
	}
	
	//faz a alocacao da matriz para conter a imagem
	int aloca(int w, int h)
	{
		int size;
		width = w;
		height = h;
		size  = width*height;
		red   = (unsigned char *)malloc(size);
		green = (unsigned char *)malloc(size);
		blue  = (unsigned char *)malloc(size);
    
		if (!red || !green || !blue)
		{
			tem_img = 0;
			printf("Insuficient Memory.\n");
			if (red) free(red);
			if (red) free(green);
			if (red) free(blue);
			return 0;
		}
		else
		{
			tem_img = 1;
			printf("\nAlocou imagem de %d por %d pixels\n", width, height);
		}
		return 1;
	}

	void ErrorMsg(int err)
	{
		switch (err)
		{
			case IM_ERR_OPEN:
				printf("\n%s", "Error Opening File.\n");
			break;
			case IM_ERR_READ:
				printf("\n%s","Error Reading File.\n");
			break;
			case IM_ERR_WRITE:
				printf("\n%s","Error Writing File.\n");
			break;
			case IM_ERR_TYPE:
				printf("\n%s","Image not Suported.\n");
			break;
			case IM_ERR_FORMAT:
				printf("\n%s","Invalid Format.\n");
			break;
			case IM_ERR_COMP:
				printf("\n%s","Compression not suported.\n");
			break;
			default:
				printf("\n%s","Unknown Error.\n");
		}
	}
};

#endif
