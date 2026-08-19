#include "VLTerrain.h"
#include <GL/glut.h>

#include <math.h>

//class constructor
VLTerrain::VLTerrain()
{
   traversal = 0;
   ThresholdDetail = 1.0;
   contErro=0;
 
   //initialize the y index vector coord
   for(int i=1; i<24; i+=3)
      indices[i] = (unsigned char)4;

   init_quad();
   texTerrain = new Textura("data\\textura.raw");
}

//class destructor
VLTerrain::~VLTerrain(void)
{
   free(quad);    
}


void VLTerrain::setCamera( Camera *cam )
{
   camera = cam;
}

//ver funcao recorrente para determinar o numero de elementos
void VLTerrain::init_quad()
{
 	//open height field map
   int dim = DIM_TERRAIN+1, i;
	unsigned char *data = (unsigned char *)malloc(dim*dim);
	printf("Loading height grids...\n");

	FILE*	fp = fopen("data\\pozzer.raw", "rb");  //513x513
	fread(data, sizeof(unsigned char), dim*dim, fp);
	fclose(fp);

   /*for(i=0; i<dim; i++)
   {
      for(int j=0; j<dim; j++)
      {
         data[j+i*dim] = 100*(1+sin(j/10.0)*sin(i/10.0));//rand()%10;  
      }
   }/**/

   int size=0;
   //calculate the number of nodes of the tree
   for(i=DIM_TERRAIN; i>=1; i/=2)
      size += i*i;
  
   //allocates the tree
   quad = (Quadtree*)malloc(size * sizeof(Quadtree));
   build_quad(data);
   free(data);


   printf("Alocou %.3f Mb na arvore\n", size * sizeof(Quadtree)/(1024*1024.0) );
   printf("Quadtree %d bytes x %d nos\n", (int)sizeof(Quadtree), size );

}

void VLTerrain::build_quad(unsigned char *data)
{
   int dim = DIM_TERRAIN, n_blocks = 1, linha = (DIM_TERRAIN+1);//number of coordinates of each line of the matrix
   int l, c, index, offset1 = 0, offset2 = 0, oldDim = 0;
   int baseAddr;

   while(dim >= 1)
   {
      for(l=0; l<dim; l++)
      {
         for(c=0; c<dim; c++)
         {
            //deslocamento em q[] para armazenar os nos de um nivel da arvore
            index    = l*dim+c + offset1;               

            //deslocamento em vetCoord[] que contem os vertices a serem usados no nivel corrente da arvore
            baseAddr = l*linha*n_blocks + c*n_blocks; 
            quad[index].y[0] = (unsigned short)data[ baseAddr ]                              *ESCALA;
            quad[index].y[1] = (unsigned short)data[ baseAddr + n_blocks ]                   *ESCALA;
            quad[index].y[2] = (unsigned short)data[ baseAddr + (linha*n_blocks) ]           *ESCALA;
            quad[index].y[3] = (unsigned short)data[ baseAddr + (linha*n_blocks) + n_blocks ]*ESCALA;
   
            //ligacao aos filhos
            if(dim == DIM_TERRAIN)
            {
               quad[index].Q0    = NULL;
               quad[index].error = 0;
            }
            else //if( level >= 2 )
            {
               int cnst = offset2 + c*2 + l*dim*4;
               quad[index].Q0 = &quad[cnst            ];
               quad[index].Q1 = &quad[cnst         + 1];
               quad[index].Q2 = &quad[cnst + 2*dim    ];
               quad[index].Q3 = &quad[cnst + 2*dim + 1];

               quad[index].error = computeError(&quad[index], n_blocks);
            }

            //liga ao vizinho de cima e esquerda e diagonais de cima e baixo
            quad[index].up    = (l==0              )? &quad[index] : &quad[index - (DIM_TERRAIN/n_blocks)];
            quad[index].diag3 = (l==0 || c==(dim-1))? &quad[index] : &quad[index - (DIM_TERRAIN/n_blocks)+1];
            quad[index].diag2 = (c==0 || l==(dim-1))? &quad[index] : &quad[index + (DIM_TERRAIN/n_blocks)-1];
            quad[index].left  = (c==0              )? &quad[index] : &quad[index - 1];
            
            //numero da renderizacao.
            quad[index].traversal = -1;
         }
      }
      offset2  += oldDim*oldDim;
      offset1  += dim*dim;
      oldDim   =  dim;
      dim      /= 2;
      n_blocks *= 2;
   }
   //faz quad apontar para o no raiz
   quad = &quad[index];
}

//atribuir ao de cima o erro maximo dos filhos, que deve ser normalizado entre 0 e 1 ??
float VLTerrain::computeError(Quadtree *q, int level)
{
   //calcula o erro no quadrante local.
   float errRight, errBottom, errCenter, err;
   float filhos;
   float maxi, mini, delta;

   delta = (q->y[1] + q->y[3])/2.0;
   maxi  = max(delta, q->Q1->y[3]);
   mini  = min(delta, q->Q1->y[3]);
   errRight = (mini > 1) ? (maxi/mini) -1.0 : 0.0;

   delta = (q->y[2] + q->y[3])/2.0;
   maxi  = max(delta, q->Q2->y[3]);
   mini  = min(delta, q->Q2->y[3]);
   errBottom = (mini > 1) ? (maxi/mini) -1.0 : 0.0;

   delta = (q->y[0] + q->y[3])/2.0;
   maxi  = max(delta, q->Q2->y[1]);
   mini  = min(delta, q->Q2->y[1]);
   errCenter = (mini > 1) ? (maxi/mini) -1.0 : 0.0;

   err    = (errCenter + errBottom + errRight);
   filhos = max( max(q->Q0->error, q->Q1->error), max(q->Q2->error, q->Q3->error));

   if( filhos >= err && filhos>0.0)
      err+=0.1;
   return max(filhos, err);
}

void VLTerrain::renderMain(void)
{
   traversal++;
   idx        = 0; //usado no arrayList
   triangulos = 0;
   contErro   = 0;
   n_quadrantes = 0;
   texTerrain->render();
   render_aux_2(quad, 0, 0, 0);
}

void VLTerrain::render_aux_2(Quadtree *q, int level, int x, int z)
{
   if( cullingB==true && level < (MAX_RES-1) && level >= MIN_RES && culling(q, x, z, level) == true )
   {
      //q->L1 = q->L2 = q->L3 = q->L4 = -1; 
      q->traversal = -1; 
      return;
   }
   n_quadrantes++;
   if( n_quadrantes>max_render && debug==true)
   {
      return;
   }
   //if( n_quadrantes==1726 )
   //   n_quadrantes=1726;

   if(level >= MIN_RES )
   {
      int minimo = -1, maximo = -1; 
      if( q->up->traversal != traversal )
      {
         UP2 = UP3 = UP4 = -1; 
      }
      else
      {
         UP2 = q->up->L2;  UP3 = q->up->L3;  UP4 = q->up->L4;
         minimo = min(UP3, UP4);
         maximo = max(UP3, UP4);
      }
        
      if( q->left->traversal != traversal ) 
      {
         LEFT1 = LEFT2 = LEFT4 = -1; 
      }
      else
      {
         LEFT1 = q->left->L1; LEFT2 = q->left->L2; LEFT4 = q->left->L4;
         minimo = (minimo==-1) ? min(LEFT1, LEFT2) : min( min(LEFT1, LEFT2), minimo);
         maximo = (maximo==-1) ? max(LEFT1, LEFT2) : max( max(LEFT1, LEFT2), maximo);
      }   

      if( n_quadrantes==max_render && debug==true)
      {
         mipmap = find_level(q, x, z, level); 

         printf("\nLevel:%d ", level); 
         //printf(" LEFT1: %d LEFT2: %d UP3: %d UP4: %d Quad: %d MIP: %d ", LEFT1, LEFT2, UP3, UP4, n_quadrantes, find_level(q, level));
         printf(" LEFT1: %d LEFT2: %d UP3: %d UP4: %d DIAG2: %d DIAG3: %d LOD=%.2f", LEFT1, LEFT2, UP3, UP4, DIAG2, DIAG3, globalError);
         //printf(" UP4: %d  ", UP4 );
         //printf(" Erro0: %d Erro1:%d Erro2:%d ", q->E0, q->E1, q->E2); 
      }/**/

      //entra se for pelo menos 1 menor que o minimo
      if( (level+2) > minimo )
      {
         mipmap = find_level(q, x, z, level); 

         // 3 CASOS TERMINAIS - TEM QUE PARAR A RECURSAO
         //if( (DIAG3!=-1 && DIAG3<level) || level==minimo || level==(MAX_RES-1) || (( mipmap ==0) && (level+2) > maximo)  )
         if(  level==minimo || level==(MAX_RES-1) || (( mipmap ==0) && (level+2) > maximo)  )
         {
            render(q, level, mipmap, x, z);
            q->traversal = traversal;
            return;
         }
      }
   }
   q->traversal = traversal;

   level++;
   int shift = LARGURA>>(level);
   render_aux_2(q->Q0, level, x,       z); //em cada nivel, armazena o LOD de cada filho, exceto no prim quadrante, que nao tem influencia no LOD dos nives acima da quadtree
   render_aux_2(q->Q1, level, x+shift, z);
   render_aux_2(q->Q2, level, x,       z+shift);
   render_aux_2(q->Q3, level, x+shift, z+shift);
   q->L1 = max( q->Q1->L1, q->Q1->L2 );
   q->L2 = max( q->Q3->L1, q->Q3->L2 ); 
   q->L3 = max( q->Q2->L3, q->Q2->L4 ); 
   q->L4 = max( q->Q3->L3, q->Q3->L4 ); 
}


void VLTerrain::render(Quadtree *q, int level, char delta, int x, int z)
{
   char deltaUp   = abs(UP3-UP4);
   char deltaLeft = abs(LEFT1-LEFT2);
   idx = 0;

   glLineWidth (1);

   //define os 9 vertices 3D possiveis do quadrante sendo processado
   build_vertexes(x, z, level, q);

   DIAG3 = (q->diag3->traversal != traversal) ? -1 : q->diag3->L3;
   DIAG2 = (q->diag2->traversal != traversal) ? -1 : q->diag2->L1;

   if( LEFT1 > level+1 || UP3 > level+1)
   {
      contErro++;
      glLineWidth (4);
   }


   /*if( n_quadrantes==max_render && debug==true)
      printf("\nDIAG2: %d DIAG3: %d", DIAG2, DIAG3);
   /**/
   if( deltaLeft > 0 || LEFT1>level+1 || LEFT2>level+1) 
   {
      //teste - no caso de culling maluco, este teste eh interessante.
      //int   up   = UP3   - level; //esses valores nao podem ser > 1. Pode ser negativo
      //renderUp(        q, level, up    );
      if( abs(LEFT1-UP3) > 1 && UP3>-1)
            contErro++;
      renderUp(        q, level, 0    );
      renderRight(     q, level, delta);
      renderLeftBotton(q, level, x, z);
   }
   else if( deltaUp > 0 )
   {
      renderBotton( q, level, delta);
      renderLeft(   q, level, 0    );
      renderUpRight(q, level, x, z );
   }
   else
   {
	   int left = 0, up = 0;
      if( delta > -1 ) //CASO NORMAL
      {
         left = LEFT1 - level; //esses valores nao podem ser > 1. Pode ser negativo
         up   = UP3   - level; //esses valores nao podem ser > 1. Pode ser negativo

         //teste erro
         if( (left>1 || up > 1 || left<0  || up<0) && (LEFT1>0 && UP3>0))
         {
            //printf(" ERRRROOOOOO-1 ");
            contErro++;
            glLineWidth (4);
         }
		   renderUp    (q, level, up);
		   renderLeft  (q, level, left);
		   renderRight (q, level, delta);
		   renderBotton(q, level, delta);
      }
      else if( delta == -2 ) //CASO RECURSIVO ESPECIAL - LEFT
      { 
		   delta = find_level(q, x, z, level); 
         left = LEFT1 - level; 
         //teste erro
         if( left>1 || left<0)
         {
            //printf(" ERRRROOOOOO-2 ");
            contErro++;
            glLineWidth (4);
         }

         renderUp    (q, level, 0);
		   renderLeft  (q, level, left);

		   renderRight (q, level, -1);
		   renderBotton(q, level, delta);
      }
      else if( delta == -3 ) //CASO RECURSIVO ESPECIAL - RIGHT
      { 
		   delta = find_level(q, x, z, level); 
         up = UP3 - level;   

         //teste erro
         if( up>1 || up<0)
         {
            //printf(" ERRRROOOOOO-3 ");
            contErro++;
            glLineWidth (4);
         }

         renderUp    (q, level, up);
		   renderLeft  (q, level, 0);

		   renderRight (q, level, delta);
		   renderBotton(q, level, -1);
      }
   }
   glDrawElements(GL_TRIANGLES, idx, GL_UNSIGNED_BYTE, indices );
}/**/

//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************

void VLTerrain::renderLeftBotton(Quadtree *q, int level, int x, int z)
{
   tri(3, 0);
   tri(8, 7);
   glDrawElements(GL_TRIANGLES, idx, GL_UNSIGNED_BYTE, indices );

   q->Q3->traversal = traversal;
   q->Q3->L3 = q->Q3->L4 = level+1;
   q->L4 = level+1;

   UP3 = UP4 = -1;
   LEFT1 = q->Q2->left->L1; 
   LEFT2 = q->Q2->left->L2; 

   int shift = LARGURA>>(level+1);
   render(q->Q2, level+1, -2, x, z+shift);
   q->Q2->traversal = traversal;
   q->L3 = max( q->Q2->L3, q->Q2->L4 ); 
}

void VLTerrain::renderUpRight(Quadtree *q, int level, int x, int z)
{
   tri(0, 1);
   tri(5, 8);

   glDrawElements(GL_TRIANGLES, idx, GL_UNSIGNED_BYTE, indices );

   q->Q3->traversal = traversal;
   q->Q3->L1 = q->Q3->L2 = level+1;
   q->L2 = level+1;

   LEFT1 = LEFT2 = -1;
   UP3 = q->Q1->up->L3; 
   UP4 = q->Q1->up->L4; 

   int shift = LARGURA>>(level+1);
   render(q->Q1, level+1, -3, x+shift, z);
   q->Q1->traversal = traversal;
   q->L1 = max( q->Q1->L1, q->Q1->L2 ); 
}

void VLTerrain::renderLeft(Quadtree *q, int level, char delta)
{
   if(delta==1) 
   {
      tri(6, 3);
      tri(3, 0);
   }
   else
      tri(6, 0);
}
   
void VLTerrain::renderUp(Quadtree *q, int level, char delta)
{
   if(delta==1) 
   {
      tri(0, 1);
      tri(1, 2);
   }
   else
      tri(0, 2);
}

void VLTerrain::renderRight(Quadtree *q, int level, char delta)
{
   if( delta > -1 ) //para o caso de chamadas recursivas em Left_Botton
   {
      if(DIAG3 != -1)
      {
         if( (level + delta ) > (DIAG3+1) )
            delta = 0;
         else if( (level + delta ) < (DIAG3-1) )
            delta = 1;
      }  
      else if(UP2 != -1) 
      {
         if( (level + delta ) > (UP2+1) )
            delta = 0;
         else if( (level + delta ) < (UP2-1) )
            delta = 1;
      }
   }
   
   if(delta==1) 
   {
      tri(2, 5);
      tri(5, 8);
   }
   else
      tri(2, 8);

   if(delta < 0) delta = 0;
   q->Q1->L1 = q->Q1->L2 = level+delta;
   q->Q3->L1 = q->Q3->L2 = level+delta;
   q->Q1->traversal = traversal;
   q->Q3->traversal = traversal;
   q->L1 = q->L2 = level+delta;
}

//pode ser somente 1 maior que left.
void VLTerrain::renderBotton(Quadtree *q, int level, char delta)
{
   if( delta > -1 ) //para o caso de chamadas recursivas em UP_Right
   {
      if(DIAG2 != -1)
      {
         if( (level + delta ) > (DIAG2+1) )
            delta = 0;
         else if( (level + delta ) < (DIAG2-1) )
            delta = 1;
      }  
      else if(LEFT4 != -1) 
      {
         if( (level + delta ) > (LEFT4+1) )
            delta = 0;
         else if( (level + delta ) < (LEFT4-1) )
            delta = 1;
      }
   }
   
   if(delta==1) 
   {
      tri(7, 6);
      tri(8, 7);
   }
   else
      tri(8,6);

   if(delta < 0) delta = 0;
   q->Q2->L3 = q->Q2->L4 = level+delta;
   q->Q2->traversal = traversal;
   q->Q3->L3 = q->Q3->L4 = level+delta;
   q->Q3->traversal = traversal;
   q->L3 = q->L4 = level+delta;
}


void VLTerrain::build_vertexes(int x, int z, int level, Quadtree *q)
{
   int full = LARGURA>>(level);
   int half = full>>1;
   
   vert(0,  x,      ESCALA * q->y[0],     z     );
   vert(3,  x+half, ESCALA * q->Q0->y[1], z     );
   vert(6,  x+full, ESCALA * q->y[1],     z     );
   
   vert(9,  x,      ESCALA * q->Q0->y[2], z+half);
   vert(12, x+half, ESCALA * q->Q0->y[3], z+half);
   vert(15, x+full, ESCALA * q->Q3->y[1], z+half);
   
   vert(18, x,      ESCALA * q->y[2],     z+full);
   vert(21, x+half, ESCALA * q->Q3->y[2], z+full);
   vert(24, x+full, ESCALA * q->y[3],     z+full);
}



bool VLTerrain::culling(Quadtree *q, int x, int z, int level)
{
   int shift = LARGURA>>(level);
   //if( (x+shift) < 35001 && (z+shift) < 35001)
   //if( x > 27001 && z > 27001)
   //   return true;
   //return false;

   return !camera->visible( x ,  (x+shift) , 
                           z ,  (z+shift) );
}


//return by reference the appropriate detail for the bottom and right quadrants
char VLTerrain::find_level(Quadtree *q, int x, int z, int level)
{
   //camera->camPos.print();
  	//printf("\n %.2f %.2f %.2f", camera->camPos.x, camera->camPos.y, camera->camPos.z);
   //camera->camLock = camera->camPos;

 	//float	dx = fabs(x       - camera->camPos.x);
	//float	dy = fabs(q->y[0] - camera->camPos.y);
   //float	dz = fabs(z       - camera->camPos.z);

 	float	dx = fabs(x       - camera->camLockPos.x);
	float	dy = fabs(q->y[0] - camera->camLockPos.y);
   float	dz = fabs(z       - camera->camLockPos.z);


   float d = max(dx, max(dy, dz));
   float factor, result = level;

   factor = 10000/d;

   if(factor < 0.01) factor=0.01;
   if(factor > 1 )   factor=1;

   //if( n_quadrantes==max_render && debug==true)
   //{
   //   printf("\n Fator = %.1f - (%.1f %.1f %.1f)", fator, dx, dy, dz);
   //}

   if( (q->error * factor) > ThresholdDetail )
      result = level+1;

   globalError = (q->error * factor) - ThresholdDetail;

   return  (result <= level) ? 0 : 1;
}


// Load some data and put it into the quadtree.
void VLTerrain::ConvertData()
{
   FILE*	fp;
   int contZeros = 0;
   unsigned char r;

   printf("Converting terrain...\n");

   unsigned short *data = new unsigned short[512 * 512];
   unsigned char  *novo = new unsigned char[513 * 513];
	
   fp = fopen("data\\gc16at128.raw", "rb");
	fread(data, sizeof(unsigned short), 512 * 512, fp);
	fclose(fp);

   int i=0, j=0;
   for(int l=0; l<512; l++)
   {
        for(int c=0; c<512; c++)
        {
           
           if( data[j]==0 )
           {   
              r = rand()%200;   
              contZeros++;
              data[j++] = r+rand()%100;
              novo[i++] = r;
           }
           else
              novo[i++] = data[j++];

        }
        novo[i++] = 0;
   }
   for(int l=0; l<DIM_TERRAIN; l++)
      novo[i++] = 0;

	fp = fopen("data\\poz.raw", "wb");
   fwrite(novo, sizeof(unsigned char), 513*513, fp);
   fclose(fp);

   fp = fopen("data\\quad.raw", "wb");
   fwrite(data, sizeof(unsigned short), 512*512, fp);
   fclose(fp);
}