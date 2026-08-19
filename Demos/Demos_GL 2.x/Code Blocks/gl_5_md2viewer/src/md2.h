//----------------------------------------------------------
//class para tratamento de arquivos MD2
//Adapatada de um programa desenvolvido por alguem que falava Frances. Perdi a referencia. 
//Adaptacao: Cesar Tadeu Pozzer
//           pozzer@inf.ufsm.br
//----------------------------------------------------------

#ifndef   _MD2___H
#define   _MD2___H

#include   <GL/glut.h>
#include   "Texture.h"
#include   "Vector3.h"


#define MAX_FLOAT  1000000000
#define MIN_FLOAT  -MAX_FLOAT


//
//   animations         début      fin
// -------------------------------------
//0   STAND               0          39 
//1   RUN                40          45
//2   ATTACK             46          53 
//3   PAIN_A             54          57 
//4   PAIN_B             58          61 
//5   PAIN_C             62          65 
//6   JUMP               66          71 
//7   FLIP               72          83 
//8   SALUTE             84          94 
//9   FALLBACK           95         111 
//   WAVE               112         122 
//   POINT              123         134 
//   CROUCH_STAND       135         153 
//   CROUCH_WALK        154         159 
//   CROUCH_ATTACK      160         168 
//   CROUCH_PAIN        169         172 
//   CROUCH_DEATH       173         177 
//   DEATH_FALLBACK     178         183 
//   DEATH_FALLFORWARD  184         189 
//   DEATH_FALLBACKSLOW 190         197 
//   BOOM               198         198 
//

//anims[22]={-1,39,45,53, 57, 61, 65, 71, 83, 94, 111, 122, 134, 153, 159, 168, 172, 177, 183, 189, 197, 198};


typedef float vec3_t[3];

// nombre de vecteurs normaux précalculés
#define NUMVERTEXNORMALS      162

// produits scalaires précalculés pour le vertex lighting...
#define SHADEDOT_QUANT         16

// ces définitions proviennent de qfiles.h (code source de Quake II)
// elles ne sont pas toutes utilisées ici.
#define MAX_TRIANGLES     4096
#define MAX_VERTS         15000
#define MAX_FRAMES        512
#define MAX_MD2SKINS      32
#define MAX_SKINNAME      64

// numéro de version
#define   MD2_VERSION      8

// identificateur "IDP2" ou 844121161
#define MD2_IDENT          (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')



// info pour un seul point
typedef struct
{
   unsigned char   v[3];            // sommet "compressé"
   unsigned char   lightnormalindex;   // index normale pour éclairage
} vertex_t;


// informations pour une seule séquence
typedef struct
{
   float      scale[3];      // multiplier verts par cette valeur
   float      translate[3];  // puis translater par ce vecteur
   char       name[16];      // nom de la frame
   vertex_t   verts[1];      // variable redimensionnée
} frame_t;



// header md2
typedef struct
{
   int      ident;           // identificateur fichier MD2 : "IDP2"
   int      version;         // doit être égal à 8

   int      skinwidth;       // largeur de la texture
   int      skinheight;      // hauteur de la texture
   int      framesize;       // taille en octet par frame

   int      num_skins;       // nombre de textures
   int      num_xyz;         // nombre de vertices
   int      num_st;          // nombre de coordonnées de texture
   int      num_tris;        // nombre de triangles
   int      num_glcmds;      // nombre de commandes opengl
   int      num_frames;      // nombre total de frames

   int      ofs_skins;       // offset pour les noms des skins (64 octets chacun)
   int      ofs_st;          // offset pour les coordonnées de texture s-t
   int      ofs_tris;        // offset pour les triangles
   int      ofs_frames;      // offset pour les données des frames (points)
   int      ofs_glcmds;      // offset pour les commandes opengl
   int      ofs_end;         // offset fin du fichier

} md2_t;



//estrutura para definir o estado da animacao. Armazena o fator de interpolacao
// e o quadro corrente. O next_quadro eh calculado na Animate().
typedef struct
{
   float interp;
   int   cur;
}posicao_t;



// ============================================
// CMD2Model - notre classe objet pour un .md2
// ============================================

class Md2
{
private:
   char name[100];      ///object name
   char fileName[100];  ///file that contains the data
   char textName[100];  ///file that contains the texture
   
   // variables membres
   int         num_frames;      // nombre de frames
   int         num_xyz;         // nombre de vertices de cada frame
   int         num_glcmds;      // nombre de commandes opengl

   vec3_t      *vertices;       // liste de sommets
   int         *glcmds;         // commandes opengl
   int         *lightnormals;   // liste d'index de normales précalculées
  
   bool         bTextured;
   
   //variaveis pozzer
   Texture     *texture;       // texture
   Vector3      modelBB[8];     //bounding box

   float Scale;

public:
   float       width, lenght;  //dimentions

   // constructeur/destructeur
   Md2( );
   ~Md2( void );

   int getNumFrames();

   /**
    * Reads the file and alocate necesary data
    */
   void load(char *path, char *name, char *text );
   
   void transformModel();
   void scale(float s);

   /**
    * Returns the fileName of this md2 object
    */
   char * getName();

   // fonctions
   bool      LoadModel( const char *filename );
   void      LoadSkin( char *filename );

   //void    Animate( int start, int end, float percent );
   void      render( );
   void      Animate( int start, int end, float speed, float *interpol, int *curr_frame, bool anim);

   void      ProcessLighting( void );
   void      calculateBoundingBox( void );
   Vector3   getBoundingBox(int index);
   //void    setScale(float sx, float sy, float sz);

};

#endif
