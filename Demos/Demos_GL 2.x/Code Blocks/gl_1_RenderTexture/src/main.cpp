////////////////////////////////////////////////////////////////////////////
// Demo Render To Texture
//
// - ilustra:
//      * o uso de textura
//      * renderizacao para textura de objetos 2D e 3D
//      * leitura do framebuffer
//      * analise de desempenho
//      * mipmap
//
//  Adaptacao por:
//  Cesar Tadeu Pozzer
//  UFSM 04/2010 - pozzer@inf.ufsm.br
//
// AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-05-16
// UPDATED: 2010-03-24
//
//   Versao original com das formas de recuperacao do color buffer:
//   - http://www.songho.ca/opengl/gl_fbo.html
//   - http://www.songho.ca/opengl/files/fbo.zip
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>

#include <cstdlib>
#include <stdio.h>
#include <GL/glext.h>

#define RENDER_2D 2
#define RENDER_3D 3

void displayCB();
void exitCB();
void initGL();

void criaTextura();
void setaTextura();

void desenhaObjetos2D();
void desenhaObjetos3D();

//A dimensao da textura nao precisa se igual a dimensao da tela.
//Foi usado glViewport para permitir isso. Mas se as duas forem do
//mesmo tamanho nao vai haver distorcoes entre a imagem real e a
//mostrada via textura
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;
const int TEXTURE_WIDTH = 700;
const int TEXTURE_HEIGHT = 700;

GLuint textureId;          // ID of texture
bool textureMode = true;   //textura ou renderizacao direta
int  renderMode = RENDER_3D;   //renderizacao 2D ou 3D - Default eh 3D
float ang = 0;

//callback de display do opengl
void displayCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Sleep(10);
    //desenha na tela um quadrado com a textura lida do framebuffer pela funcao setaTexturaxD()
    //----------------------------------------------------------------------
    if( textureMode == true )
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBindTexture(GL_TEXTURE_2D, textureId);
        glBegin(GL_QUADS);
            glColor4f(1, 1, 1, 1);
            glTexCoord2f(1, 1);  glVertex2d(SCREEN_WIDTH,SCREEN_HEIGHT);
            glTexCoord2f(0, 1);  glVertex2d(0,SCREEN_HEIGHT);
            glTexCoord2f(0, 0);  glVertex2d(0,0);
            glTexCoord2f(1, 0);  glVertex2d(SCREEN_WIDTH,0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //desenha a geometria diretamente
    //-------------------------------------------------------------------------
    else
    {
        ang += 0.14;
        if (renderMode == RENDER_3D )
           desenhaObjetos3D();
        else
           desenhaObjetos2D();
    }
    glutSwapBuffers();
}


void desenhaObjetos3D()
{
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(40, 1, 1, 20);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity( );
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

   gluLookAt(0, 0, 13, //from. Posicao onde a camera esta posicionada
             0, 0, 0,  //to. Posicao absoluta onde a camera esta vendo
             0, 1, 0); //up. Vetor Up.


   glRotatef(ang, 1, 1, 0);
   glColor3f(0, 1, 1);
   glutWireTeapot(3);
   glTranslated(-1, 2, 1);
   glColor3f(1, 0, 0);
   glutWireTeapot(2);
   glTranslated(-3, -2, -3);
   glColor3f(1, 0, 0);
   glutWireTeapot(2);
}

void desenhaObjetos2D()
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity( );
    //desenha alguma coisa na tela
    //--------------------------------------------
    glBegin(GL_TRIANGLES);
       glColor3f(1,0,0);
       glVertex2f(0, 0);
       glColor3f(0,1,0);
       glVertex2f(SCREEN_WIDTH, 0);
       glColor3f(0,0,1);
       glVertex2f(SCREEN_WIDTH/2.0, SCREEN_HEIGHT);
    glEnd();

    //desenha alguma coisa pesada na tela - 200.000 vertices
    //--------------------------------------------
    glBegin(GL_POINTS);
    for(int i=0; i<200000; i++)
    {
       glColor3f((rand()%300)/300.0,(rand()%300)/300.0,(rand()%300)/300.0);
       glVertex2f(rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT);
    }
    glEnd();
}

//inicializa a textura para guardar a informacao do framebuffer de cor (ColorBuffer)
void criaTextura()
{
    // create a texture object
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    //deve estar habilitado o mipmap senao nao funciona
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    //desabilita a textura corrente
    glBindTexture(GL_TEXTURE_2D, 0);
}

void setaTextura()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //indica que tudo vai ser desenhado no back_buffer e apos lido dele
    //Se nada for especificado, tudo sera desenhado e lido do buffer atual.
    //As opcoes sao: GL_FRONT_LEFT, GL_FRONT_RIGHT, GL_BACK_LEFT, GL_BACK_RIGHT, GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);

    //glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER

    //gerante que a viewport de desenho vai ser do tamanho da textura. Se fosse maior daria
    //overflow na funcao glCopyTexImage2D. Neste caso teria que ser usado glCopyTexSubImage2D
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);

    if (renderMode == RENDER_3D )
       desenhaObjetos3D();
    else
       desenhaObjetos2D();

    glFlush ();

    // copy the pixels from framebuffer to a texture with textureId
    //-------------------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0); //Tambem pode ser usado: glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glBindTexture(GL_TEXTURE_2D, 0);

    //glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
}


void exitCB()
{
    //glDeleteTextures(1, &textureId); //da pau.
}

void keyCB(unsigned char key, int x, int y)
{
   if( key == '2' )
      renderMode = RENDER_2D;
   else if( key == '3' )
      renderMode = RENDER_3D;
   else if( key == ' ' )
      textureMode = ! textureMode;

   printf("\n\nRender to texture = %d", textureMode);
   printf("\nRender Mode       = %d", renderMode);

   setaTextura();
}

void initGL()
{
    glShadeModel(GL_SMOOTH);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
    glEnable(GL_TEXTURE_2D);

    glClearColor(1, 1, 1, 1);
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // register exit callback
    atexit(exitCB);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);   // display mode
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);              // window size
    glutInitWindowPosition(500, 100);                           // window location
    glutCreateWindow("Pressione (2,3) para modo renderizacao ou espaco para alternar render to texture");
    glutDisplayFunc(displayCB);
    glutIdleFunc(displayCB);
    glutKeyboardFunc(keyCB);

    initGL();

    criaTextura();
    setaTextura();

    printf("\nRender to texture = %d", textureMode);
    printf("\nRender Mode       = %d", renderMode);

    glutMainLoop();

    return 0;
}
