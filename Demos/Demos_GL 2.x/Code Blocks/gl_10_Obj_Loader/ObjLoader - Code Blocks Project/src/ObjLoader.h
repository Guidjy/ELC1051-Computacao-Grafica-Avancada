/*******************************************************************
*	ObjLoader
*	
*	Descrição: Arquivo com o necessário para carregar modelos no 
*	formato OBJ. 
*
*
*	Autor: Alex Thomas Almeida Frasson
*
*	Universidade Federal de Santa Maria - 2012/1
*******************************************************************/
#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <glew.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "stb_image.h"


class _ObjStream;
class _ObjFace;
class _ObjMaterial;
struct _ObjPoint3f;
struct _ObjUV;
struct _ObjRGB;


class _ObjMesh{
	public:
		_ObjMesh();
		bool load(std::string file);				//Carrega um modelo.			Exemplo: myMesh.load("C:\\Users\\Fulano\\Desktop\\arquivo.obj");
	protected:
		std::vector<_ObjPoint3f> v;					//Vertices
		std::vector<_ObjPoint3f> n;					//Vertex Normals
		std::vector<_ObjUV> uv;						//UV coordinates
		std::vector<_ObjFace> f;					//Faces
		std::vector<_ObjMaterial> m;				//Materials
		bool loaded;
		bool loadMaterial(std::string path, std::string name);	//Carrega o arquivo .mtl(material) associado ao arquivo .obj. O arquivo .mtl deve estar na mesma pasta do arquivo .obj.
		bool loadTexture(std::string path);
		void free();											//Free
};











struct _ObjPoint3f{
    float x, y, z;
};
struct _ObjUV{
    float u, v;
};
struct _ObjRGB{
	float r, g, b;
};

class _ObjMaterial{
	public:
		std::string name;
		_ObjRGB ambient;
		_ObjRGB diffuse;
		_ObjRGB specular;
		GLuint texture;
		bool textured;
		_ObjMaterial();
};

class _ObjFace{
	public: 
		int v[3];			//Vertex indices
		int n[3];			//Vertex Normals indices
		int uv[3];			//UV indices
		_ObjMaterial *material;
		_ObjFace();
};






#define OBJ_GROUP	10
#define OBJ_V       11
#define OBJ_VN      12
#define OBJ_VT      13
#define OBJ_F       14
#define OBJ_MTLLIB  15
#define OBJ_USEMTL  16
#define MTL_NEWMTL  17
#define MTL_KA      18
#define MTL_KD      19
#define MTL_KS      20
#define MTL_MAP_KD	21


class _ObjStream{
    public:
        int iterator, length;
        char *buffer;
        _ObjStream();
        _ObjStream(const char *path);
        bool load(const char *path);		//Carrega um arquivo, copia para o buffer e fecha o arquivo.
		bool good(void);					//True se não chegou ao fim do buffer.
        void free(void);					//Libera memória.
        int  getKeyword(void);				//Retorna o valor da próxima keyword encontrada.
        void ignore(int n, char c);			//Ignora até atingir 'n' caracteres ou o caracter 'c'. O caracter 'c' também será descartado.
        void ignore(char c);				//Ignora até atingir o caracter 'c'. O caracter 'c' também será descartado.
		void ignoreTillNext(void);			//Ignora os caracteres ' ', '\n' e '\t'.
		void begin(void);					//Iterador volta para o início.
		bool checkNumerical();
		bool checkNumerical(char c);
		struct _ObjPoint3f getPoint(void);		//Vértice e normal.
		struct _ObjUV getUV(void);				//Coordenadas UV.
		//class Face getFace(void);			//Face.
		void getFace(std::vector<_ObjFace> &face);
		void getName(std::string &name);	//Usemtl e Mtllib.
		void getRGB(struct _ObjRGB &color);
};







#endif