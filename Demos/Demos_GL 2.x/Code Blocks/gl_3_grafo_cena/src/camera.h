//-------------------------------------------------------------------------
// classe para definicao da camera e classes derivadas
//   - ortho camera
//   - perspective camera
//
// by Cesar Pozzer 29/04/2001
//-------------------------------------------------------------------------

class camera: public node{
public:
   vetor eye, centro, up;
   camera(char *name): node(name)
   {
      eye.x = 0.0;    eye.y=0.0;    eye.z= 40.0;
      centro.x = 0.0; centro.y=0.0; centro.z = 0.0;
      up.x = 0.0;     up.y=1.0;     up.z=0.0;
   }

   camera()
   {
      eye.x = 0.0;    eye.y=0.0;    eye.z= 5.0;
      centro.x = 0.0; centro.y=0.0; centro.z = -1.0;
      up.x = 0.0;     up.y=1.0;     up.z=0.0;
   }

   void setPosition(float eyex, float eyey, float eyez, float cx, float cy, float cz, float upx, float upy, float upz)
   {
      eye.x = eyex;  eye.y=eyey;  eye.z= eyez;
      centro.x = cx; centro.y=cy; centro.z = cz;
      up.x = upx;    up.y=upy;    up.z=upz;
   }

   virtual void Render()
   {
   }
};


class ortho: public camera{
public:
   ortho(char *name): camera(name)
   {
   }

   ortho()
   {
   }

   //funcao nao necessaria
   void Render()
   {
      MostraDados("Camera ortografica");
   }

   //retorna 1 indicando que o no' e' uma camera. Demais no's nao retornam nada
   int SetupCamera()
   {
      return 1;
   }
};


class perspective: public camera{
public:
   double znear, zfar, abertura, aspect;
   perspective(char *name): camera(name)
   {
      abertura = 50.0;
      znear = 10;
      zfar = 1000;
      aspect = 1;
   }
   perspective()
   {

   }
   //funcao nao necessaria
   void Render()
   {
      MostraDados("Camera perspectiva");
   }

   void setPerspectiva(double n, double f, double abert, double asp)
   {
      znear = n;
      zfar = f;
      abertura = abert;
      aspect = asp;
   }

   //retorna 1 indicando que o no' e' uma camera. Demais no's nao retornam nada
   int SetupCamera()
   {

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity( );
      gluPerspective(abertura, aspect, znear, zfar);

      //glPushMatrix();
      /* position camera */
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity( );
      gluLookAt(eye.x, eye.y, eye.z,
              centro.x, centro.y, centro.z,
              up.x, up.y, up.z);
      //glPopMatrix();

   /*   glMatrixMode(GL_PROJECTION);
      glLoadIdentity( );
      gluPerspective(55,1,10,1000);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity( );
      gluLookAt(0,0,40, 0,0,0, 0,1,0);
   /**/
      return 1;
   }
};

