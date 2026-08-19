#ifndef _VLTERRAIN_H
#define _VLTERRAIN_H	


#ifdef _WIN32
	#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "Camera.h"

#include "defs.h"
#include "./tools/textura.h"


//#define tri(a,b,c)    ( indices[idx++] = a, indices[idx++] = b, indices[idx++] = c, triangulos ++ )
#define tri(a,b)      ( indices[idx++] = a, indices[(++idx)++] = b, triangulos ++ )
#define vert(i,x,y,z) ( coordVert[i] = x,   coordVert[i+1] = y,     coordVert[i+2] = z )


/////////////////////////////////////////////////////////////////////////////
//typedef struct Quadtree;
typedef struct qua
{
   struct qua    *Q0, *Q1, *Q2, *Q3; //ponteiro para os 4 filhos
   struct qua    *up, *left, *diag3, *diag2;  //quadrantes vizinhos: esquerda, direita e diagonal_cima_dir
   unsigned short y[4];              //coord y dos cantos do quadrado
   char           L1, L2, L3, L4;    //nivel de LOD dos filhos nos quadrantes 2, 3, 4
   int            traversal;         //numero da renderizacao.
   float          error;
}Quadtree;



class VLTerrain
{
public:
   float         coordVert[27];//stores (x,y,z) triples for each trinangle's vertex.
   unsigned char indices[24];  //sotres triangle indexes (3 indexes for each triangle)

   int   idx;             //index in the arrayList
   int   triangulos;      //triangles counter
   float ThresholdDetail; //set maximum error allowed - lod
   int   contErro;
   Camera *camera;

   float globalError;

   //class constructor and destructor
   VLTerrain(void);
   ~VLTerrain(void);

   //main routine to render the mesh.
   void renderMain(void);
   void setCamera( Camera *camera );
private:
   char DIAG2, DIAG3, UP2, UP3, UP4, LEFT1, LEFT2, LEFT4, mipmap;
   Textura  *texTerrain;   //pointer to the texture object
   Quadtree *quad;         //pointer to the root of the three
   int       traversal;    //rendering travelsal number
   int       n_quadrantes; //used for debuging

   void render_aux_2(Quadtree *q,int level, int x, int z);
   void render(Quadtree *q, int level, char delta, int x, int z);

   void renderLeft  (Quadtree *q, int level, char delta);
   void renderUp    (Quadtree *q, int level, char delta);
   void renderRight (Quadtree *q, int level, char delta);
   void renderBotton(Quadtree *q, int level, char delta);

   void renderLeftBotton(Quadtree *q, int level, int x, int z);
   void renderUpRight(   Quadtree *q, int level, int x, int z);

   char  find_level(Quadtree *q, int x, int z, int level);
   void build_vertexes(int x, int z, int level, Quadtree *q);
   void build_quad(unsigned char *data);
   float computeError(Quadtree *q, int level);
   void init_quad();
   
   //float computeError2(int i, int level); //versao 25
   //void LoadData();  //versao 25
   void ConvertData(); //versao 25

   bool culling(Quadtree *q, int x, int z, int level);
};

#endif