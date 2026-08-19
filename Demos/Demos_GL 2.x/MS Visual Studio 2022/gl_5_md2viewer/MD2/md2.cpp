//----------------------------------------------------------
//class para tratamento de arquivos MD2
//Adapatada de um programa desenvolvido por alguem que falava Frances. Perdi a referencia.
//Adaptacao: Cesar Tadeu Pozzer
//           pozzer@inf.ufsm.br
//----------------------------------------------------------

#include "md2.h"
#include <math.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


// table de 162 vecteurs normaux
static vec3_t   vertexNormals[ NUMVERTEXNORMALS ] = {
   #include   "anorms.h"
};

// table de produits scalaires précalculés
static double   vertexNormal_dots[ SHADEDOT_QUANT ][256] = {
   #include   "anormtab.h"
};


static double   *shadedots = vertexNormal_dots[0];
static vec3_t   lcolor;


///////////////////////////////////////////////

vec3_t      g_lightcolor   = { 1.0, 1.0, 1.0 };
int         g_ambientlight   = 32;
float       g_shadelight   = 128;
float       g_angle         = 0.0;

///////////////////////////////////////////////


// --------------------------------------------
// constrcteur.
// --------------------------------------------

Md2::Md2(  )
{
   num_xyz      = 0;         // vertices
   num_frames   = 0;         // frames (= séquence d'animation)
   num_glcmds   = 0;         // commandes opengl

   vertices         = NULL;         // vertices
   lightnormals   = NULL;         // index vecteur normal
   glcmds      = NULL;         // commandes opengl

   bTextured = true;
   Scale     = 1.0;
}


// --------------------------------------------
// destructeur.
// --------------------------------------------

Md2::~Md2( void )
{
   // évite les fuites de mémoire
   delete [] vertices;
   delete [] glcmds;
   delete [] lightnormals;

   delete texture;
}


int Md2::getNumFrames()
{
   return num_frames;
}


void Md2::load(const char *path, const char *name, const char *text )
{
   strcpy(this->name, name);

   char tmp[100];
   strcpy(tmp,path);
   strcat(tmp,name);
   strcpy(fileName, tmp);

   strcpy(tmp,path);
   strcat(tmp,text);
   strcpy(textName, tmp);

   LoadModel( fileName );
   LoadSkin(  textName );
}

char * Md2::getName()
{
   return name;
}


// --------------------------------------------
// LoadModel() - charge le model uniquement.
// --------------------------------------------

bool Md2::LoadModel( const char *filename )
{
   FILE     * fp;          // pointeur fichier
   md2_t    * header;      // header du model
   char     * buffer;      // stockage de toutes les frames
   frame_t  * frame;       // variable temporaire
   vec3_t   * ptrverts;    // pointeur sur vertices
   int      * ptrnormals;  // pointeur sur lightnormals

	printf("\n\nCarregando arquivo  %s \n", filename);

   // tentative d'ouverture du fichier
   if( (fp = fopen( filename, "rb" )) == NULL )
   {
      printf("\nError opening MD2 file: %s", filename);
      getchar();
      exit(0);
   }

   // allocation de mémoire pour le header du model
   if( !(header = new md2_t) )
   {
      printf("\nErro de alocacao do MD2");
      getchar();
      return false;
   }


   // on lit le header
   fread( header, sizeof( md2_t ), 1, fp );

   if( (header->ident != MD2_IDENT) && (header->version != MD2_VERSION) )
   {
      delete header;
      fclose( fp );
      return false;
   }

   // on récupère les variables necessaires
   num_frames   = header->num_frames;
   num_xyz      = header->num_xyz;
   num_glcmds   = header->num_glcmds;

	printf("Vertices/frame: %d \nFrames:         %d \nGL commands:    %d", num_xyz, num_frames, num_glcmds);

   // allocation de mémoire
   vertices       = new vec3_t[ num_xyz * num_frames ];
   lightnormals   = new int[ num_xyz * num_frames ];
   buffer         = new char[ num_frames * header->framesize ];
   glcmds         = new int[ num_glcmds ];

   fseek( fp, header->ofs_frames, SEEK_SET );   // les frames
   fread( buffer, num_frames * header->framesize, 1, fp );

   fseek( fp, header->ofs_glcmds, SEEK_SET );   // les directives opengl
   fread( glcmds, num_glcmds, sizeof( int ), fp );

   // on initialise le tableau de vertices
   for( int j = 0; j < num_frames; j++ )
   {
      frame       = (frame_t*)&buffer[ header->framesize * j ];
      ptrverts    = &vertices[ num_xyz * j ];
      ptrnormals  = &lightnormals[ num_xyz * j ];

      for( int i = 0; i < num_xyz; i++ )
      {
         ptrverts[i][0] = frame->scale[0] * frame->verts[i].v[0] + frame->translate[0];
         ptrverts[i][1] = frame->scale[1] * frame->verts[i].v[1] + frame->translate[1];
         ptrverts[i][2] = frame->scale[2] * frame->verts[i].v[2] + frame->translate[2];

         ptrnormals[i] = frame->verts[i].lightnormalindex;
      }
   }


   // on ferme le fichier et on libère la mémoire
   fclose( fp );

   transformModel();
   calculateBoundingBox( );

   delete header;
   delete [] buffer;

   //teste Pozzer
  // for(int i=0; i< num_xyz * num_frames; i++)
	//printf(" %d", lightnormals[i]);
	printf("\nnum_xyz * num_frames =  %d\n\n", num_xyz * num_frames);

   return true;
}

//aplica trasformacoes para deixar o modelo nos eixos corretos
void Md2::transformModel()
{
   Vector3 v;
   for( int i = 0; i < num_xyz*num_frames; i++ )
   {
      v.set(vertices[i][0], vertices[i][1], vertices[i][2]);
      v.rotatez(PI2);
      v.rotatex(PI2);

      vertices[i][0] = v.x;
      vertices[i][1] = v.y;
      vertices[i][2] = v.z;
   }
}

void Md2::scale(float s)
{
   this->Scale = s;
   for( int i = 0; i < num_xyz*num_frames; i++ )
   {
      vertices[i][0] *= s;
      vertices[i][1] *= s;
      vertices[i][2] *= s;
   }
   calculateBoundingBox();
}


// --------------------------------------------
// LoadSkin() - charge la texture.
// --------------------------------------------

void Md2::LoadSkin(const char *fileName )
{
   texture = new Texture(fileName);
}



// --------------------------------------------
// ProcessLighting() - calcule l'éclairage.
// --------------------------------------------

void Md2::ProcessLighting( void )
{
   float lightvar = (float)((g_shadelight + g_ambientlight)/256.0);

   lcolor[0] = g_lightcolor[0] * lightvar;
   lcolor[1] = g_lightcolor[1] * lightvar;
   lcolor[2] = g_lightcolor[2] * lightvar;

   shadedots = vertexNormal_dots[((int)(g_angle * (SHADEDOT_QUANT / 360.0))) & (SHADEDOT_QUANT - 1)];
}

void Md2::calculateBoundingBox( void )
{
   float x, y, z, minx, miny, minz, maxx, maxy, maxz;
   maxx = maxy = maxz = MIN_FLOAT;
   minx = miny = minz = MAX_FLOAT;

   //faz a verificacao apenas no primeiro quadro da animacao
   //no intervalo [ 0, num_xyz ]
   for( int i = 0; i < num_xyz; i++ )
   {
      x = vertices[i][0];
      y = vertices[i][1];
      z = vertices[i][2];

      if(x<minx)
         minx = x;
      else if(x>maxx)
         maxx = x;
      if(y<miny)
         miny = y;
      else if(y>maxy)
         maxy = y;
      if(z<minz)
         minz = z;
      else if(z>maxz)
         maxz = z;
   }

   modelBB[0].set(minx, miny, minz);
   modelBB[1].set(maxx, miny, minz);
   modelBB[2].set(maxx, miny, maxz);
   modelBB[3].set(minx, miny, maxz);

   modelBB[4].set(minx, maxy, minz);
   modelBB[5].set(maxx, maxy, minz);
   modelBB[6].set(maxx, maxy, maxz);
   modelBB[7].set(minx, maxy, maxz);

   //object dimentions with associated scale.
   width  = fabs(minx)+fabs(maxx);
   lenght = fabs(minz)+fabs(maxz);
}


Vector3 Md2::getBoundingBox(int index)
{
   return modelBB[index];
}

//metodo que desenha apenas o primeiro frame, sem animacao e interpolacao.
void Md2::render(void)
{
   int      *ptricmds = glcmds;     // commandes opengl
   int      i;                      // variable de contrôle

   texture->render();

	// on inverse le sens de dessin des vertices car lorsqu'on va
   // commencer à dessiner, on va prendre les sommets dans l'ordre
   // de la liste, par conséquent on dessinera les points de chaque
   // triangle dans le sens des aiguilles d'une montre
   glPushAttrib( GL_POLYGON_BIT );
   glFrontFace( GL_CW );

   // on élimine les faces arrières
   glEnable( GL_CULL_FACE );
   glCullFace( GL_BACK );

   // calcul de l'éclairage
   ProcessLighting();



   // on dessine chaque triangle!
   while( i = *(ptricmds++) ) //while (i!=0)
   {
      if( i < 0 )
      {
         glBegin( GL_TRIANGLE_FAN );
         i = -i;
      }
      else
      {
         glBegin( GL_TRIANGLE_STRIP );
      }

      for( ; i > 0; i--, ptricmds += 3 )
      {
         // ptricmds[0]  :   coordonnée de texture s
         // ptricmds[1]  :   coordonnée de texture t
         // ptricmds[2]  :   index sommet à dessiner

         float l = shadedots[ lightnormals[ ptricmds[2] ] ];

         // on applique la couleur d'éclairage
         glColor3f( l * lcolor[0], l * lcolor[1], l * lcolor[2] );
         //glColor3f( 1,1,1);

         // on ajuste les coordonnées de texture
         glTexCoord2f( ((float*)ptricmds)[0], ((float*)ptricmds)[1] );

         // on calcule la normale du triangle en court (pour l'éclairage)
         // >>> facultatif si l'on n'utilise pas les sources de lumières opengl
         glNormal3dv( vertexNormals[ lightnormals[ ptricmds[2] ] ] );

         // on dessine le vertex
         glVertex3dv( vertices[ ptricmds[2] ] );
      }

      glEnd();
   }


   glDisable( GL_CULL_FACE );
   glPopAttrib();
}


// --------------------------------------------
// Animate() - joue l'animation commencant à
// starte et finissant à end avec interpolation
// des sommets.
// --------------------------------------------

void Md2::Animate( int start, int end, float speed, float *interpol, int *curr_frame, bool anim)
{
   static   vec3_t   vertlist[ MAX_VERTS ];   // nouveaux sommets
   vec3_t  *curr_vlist;              // sommets frame courante
   vec3_t  *next_vlist;              // sommets frame suivante
   int     *ptricmds = glcmds;       // commandes opengl
   int      i;                       // variable de contrôle
   int      next_frame;

   if( start < 0 || end < 0 || start >= num_frames || end >= num_frames )
   {
      printf("Erro na amimacao - extrapolacao de frames %d %d   %d", start, end, num_frames );
      getchar();
      exit(0);
   }

   //quando a animacao eh trocada, o curr_frame deve ser colocado no novo intervalo
   //e deve ser igual para o modelo e para a weapon
   if( *curr_frame < start || *curr_frame > end )
   {
      *curr_frame = start;
   }

   //interpol indica a posicao entre cada frame. Quando for maior que 1, um
   //novo frame deve ser usado. Eh incrementada por speed, que indica a velocidade
   //da animacao.
   if( *interpol >= 1.0 && anim == true )
   {
      //pega a parte inteira da interpolacao, que varia de [1, N] quadros.
      int n_quadros = (int)(*interpol);

      //guarda a parte fracionaria da interpolacao
      *interpol = *interpol-(int)(*interpol);

      //se N > (end-start) --> deu ciclos. Pegar o resto da divisao pelo ciclo.
      n_quadros = n_quadros%(end-start+1);

      //se o novo incr. extrapolar o intervalo [start, end], fazer caminhamento ciruclar
      if( n_quadros + *curr_frame <= end )
      {
         *curr_frame += n_quadros;
         //printf("\n CURR1   %d   ",curr_frame );
      }
      else
      {
         //printf("\n (%d, %d) CURR2  %d n_quadros %d  ",start, end, curr_frame,  n_quadros );
         *curr_frame = start + (end - *curr_frame);
         //printf("\n (%d, %d) CURR2  %d n_quadros %d  ",start, end, curr_frame,  n_quadros );
      }

      if( *curr_frame >= end )
         *curr_frame = start;
   }
   next_frame = *curr_frame + 1;
   if( next_frame >= end )
      next_frame = start;


   // on crée la liste des sommets de la frame courante et de la frame suivante
   // à partir de la liste complète
   curr_vlist = &vertices[ num_xyz * *curr_frame ];
   next_vlist = &vertices[ num_xyz * next_frame ];

   // interpolation entre les sommets de la frame courante et ceux de la
   // frame suivante: on évite ainsi une animation hachée
   for( i = 0; i < num_xyz ; i++ )
   {
      vertlist[i][0] = curr_vlist[i][0] + *interpol * (next_vlist[i][0] - curr_vlist[i][0]);
      vertlist[i][1] = curr_vlist[i][1] + *interpol * (next_vlist[i][1] - curr_vlist[i][1]);
      vertlist[i][2] = curr_vlist[i][2] + *interpol * (next_vlist[i][2] - curr_vlist[i][2]);
   }

   glEnable(GL_COLOR_MATERIAL);

   if( !bTextured )
      glDisable( GL_TEXTURE_2D );

	// on applique la texture
   texture->render();

   // on inverse le sens de dessin des vertices car lorsqu'on va
   // commencer à dessiner, on va prendre les sommets dans l'ordre
   // de la liste, par conséquent on dessinera les points de chaque
   // triangle dans le sens des aiguilles d'une montre
   glPushAttrib( GL_POLYGON_BIT );
   glFrontFace( GL_CW );

   // on élimine les faces arrières
   glEnable( GL_CULL_FACE );
   glCullFace( GL_BACK );

   // calcul de l'éclairage
   ProcessLighting();

   int cont=0;
   // on dessine chaque triangle!
   while( i = *(ptricmds++) ) //while (i!=0)
   {
     //printf("cont = %d %d ", cont++, i);
     if( i < 0 )
      {
         glBegin( GL_TRIANGLE_FAN );
         i = -i;
      }
      else
      {
         glBegin( GL_TRIANGLE_STRIP );
      }


      // ptricmds[0]  :   coordonnée de texture s
      // ptricmds[1]  :   coordonnée de texture t
      // ptricmds[2]  :   index sommet à dessiner
      for( ; i > 0; i--, ptricmds += 3 )
      {
         float l = shadedots[ lightnormals[ ptricmds[2] ] ];

         // on applique la couleur d'éclairage
         glColor3f( l * lcolor[0], l * lcolor[1], l * lcolor[2] );

         // on ajuste les coordonnées de texture
         glTexCoord2f( ((float*)ptricmds)[0], ((float*)ptricmds)[1] );

         // on calcule la normale du triangle en court (pour l'éclairage)
         // >>> facultatif si l'on n'utilise pas les sources de lumières opengl
         glNormal3dv( vertexNormals[ lightnormals[ ptricmds[2] ] ] );

		 //		 printf(" %d=%d ", lightnormals[ptricmds[2]], ptricmds[2]);

         //printf("\n xxx = %d ", lightnormals[ 2]  );


         // on dessine le vertex
         glVertex3dv( vertlist[ ptricmds[2] ] );
      }

      glEnd();
   }
   //getchar();

   glDisable( GL_CULL_FACE );
   glPopAttrib();

   if( anim == true )
      *interpol += speed;
}


