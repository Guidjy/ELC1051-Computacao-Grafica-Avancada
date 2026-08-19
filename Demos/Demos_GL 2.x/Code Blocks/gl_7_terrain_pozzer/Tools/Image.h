// classe para fazer a leitura de imagens em diversos formatos e armazena-la
// em um vetor de char. E' usada a biblioteca IM, to Tecgraf.

#ifndef _IMG_H
#define _IMG_H


#include "im.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class Image{ 
	unsigned char *red, *green, *blue;
public:
	int width, height;
	int tem_img;
	unsigned char *rgb;

	Image()
	{
		width = height = 0;
		tem_img = 0;
		rgb = NULL;
	}

	Image(char *fileName)
	{
		int t, pal_size, err;

		printf("\nOpen: %s ", fileName);
		err = imImageInfo(fileName, &width, &height, &t, &pal_size);
		if (err)
		{
			ErrorMsg(err);
			makeTexture();
			return;
		}
		aloca();
		err = imLoadRGB(fileName, red, green, blue);
		if (err)
		{
			ErrorMsg(err);
			free(red);
			free(green);
			free(blue);
			makeTexture();
			return;
		}

		long cont, pos, num;

		//cria um mapa da textura a partir do R,G,B
		num = width * height; 
		printf(" %d %d ", width, height); 
		rgb = new unsigned char[ width * height * 4 ];
		
		for(cont = 0, pos=0; pos<num; cont+=4, pos++)
		{
			rgb[cont+0] =  *(red+pos);
			rgb[cont+1] =  *(green+pos);
			rgb[cont+2] =  *(blue+pos);

			// si le pixel est noir, on met alpha à 0. sinon, on le met à 255.
			if( (rgb[cont] == 0) && (rgb[cont+1] == 0) && (rgb[cont+2] == 0) )
				rgb[cont+3] = 0;
			else
				rgb[cont+3] = 255;
		}
		free(red);
		free(green);
		free(blue);

		printf(" - OK");

		tem_img = 1;
	}
	
	~Image()
	{
		delete []rgb;
	}

	//faz a alocacao da matriz para conter a imagem
	int aloca()
	{
		int size;
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
			printf(" (%d x %d) ", width, height);
		}
		return 1;
	}

	//Faz a inversao (espelhamento) da imagem no eixo y pois o IM le a imagem de 
	//baixo para cima. Origem=(0,0) do sistema cartesiano
	void inverteY()
	{
		int l, c, p, pos1, pos2;
		unsigned char cor;
		if(tem_img)
		{
			for(l = 0; l < height/2; l++)
			{
				for(c = 0; c < width; c++)
				{
					for(p = 0; p < 4; p++)
					{
						pos1 = l*width*4 + c*4 + p;
						pos2 = (height-l-1)*width*4 + c*4 + p;

						cor       = rgb[pos1];
						rgb[pos1] = rgb[pos2];
						rgb[pos2] = cor;
					}
				}
			}

		}
	}

	//gera um padrao quadriculado que e usado se a textura nao for carregada de arquivo
	void makeTexture(void)
	{
		int l, c, pos;
		width = height = 32;
		rgb = new unsigned char[ width * height * 4 ];
		for(l = 0; l < height; l++)
		{
			for(c = 0; c < width; c++)
			{
				pos = l*width*4 + c*4;
					
				rgb[pos+0] = ( ((l&0x2)==0) ^ ((c&0x2)==0))*255;
				rgb[pos+1] = l*10.0;
				rgb[pos+2] = c*16.0;
				rgb[pos+3] = 255;
			}
		}
	}

	void ErrorMsg(int err)
	{
		switch (err)
		{
			case IM_ERR_OPEN:
				printf(" - %s", "ERROR Opening File.\n");
			break;
			case IM_ERR_READ:
				printf(" - %s","ERROR Reading File.\n");
			break;
			case IM_ERR_WRITE:
				printf(" - %s","ERROR Writing File.\n");
			break;
			case IM_ERR_TYPE:
				printf(" - %s","ERROR: Image not Suported.\n");
			break;
			case IM_ERR_FORMAT:
				printf(" - %s","ERROR: Invalid Format.\n");
			break;
			case IM_ERR_COMP:
				printf(" - %s","ERROR: Compression not suported.\n");
			break;
			default:
				printf(" - %s","ERROR: Unknown Error.\n");
		}
	}
};

#endif
