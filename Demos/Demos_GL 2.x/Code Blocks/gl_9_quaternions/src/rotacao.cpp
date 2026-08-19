/**
*   Programa para ilustrar:
*   - Comparativo entre rotacao com angulos de Euler e quaternions
*   - Gimbal Lock
*   - rotacao em eixo arbitrario

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 06/02/2009 
*   Adaptado de http://www.gamedev.net/reference/articles/article1095.asp
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/


#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include "Quaternion.h"
#include "global.h"
#include "Matrix.h"

//rotacao com angulos de euler
void euler()
{
   glPushMatrix();
   glRotatef(roll,	0.0f, 0.0f, 1.0f);				// Rotate X, Y, Z
   glRotatef(yaw,	0.0f, 1.0f, 0.0f);
   glRotatef(pitch,	1.0f, 0.0f, 0.0f);
   eixos();
   glPopMatrix();
}

//rotacao com quaternion gerado a partir dos angulos de Euler
void quaternion_1()
{
   //versao 1
   //---------------------------------------------
   g_quat_1.eulerToQuat(pitch, yaw, roll); //x, y, z

   //versao 2
   //--------------------------------------------------------
   //Quaternion q_pitch; 
   //Quaternion q_yaw;
   //Quaternion q_roll;
   //Quaternion q_final;
   //q_pitch.eulerToQuat(pitch, 0,          0); //x, y, z degrees
   //q_yaw.eulerToQuat  (0,            yaw, 0); //x, y, z degrees
   //q_roll.eulerToQuat (0,            0,          roll); //x, y, z degrees
   //q_final = q_pitch * q_yaw * q_roll;
   
   Vector3 axis;
   float angle;
   g_quat_1.getAxisAngle(axis, angle);

   glPushMatrix();
   glRotatef(RADTODEG(angle), axis.x, axis.y, axis.z);		// multiply into matrix     
   eixos();
   glPopMatrix();
}

//rotacao com multiplicacao de quaternion 
void quaternion_2() //OK
{

   Quaternion key_quat; //x, y, z degrees
   key_quat.eulerToQuat(d_pitch, d_yaw, d_roll);
   g_quat_2 = g_quat_2 * key_quat;
    

   glPushMatrix();
   
   //versao 1 - geracao de eixo/angulo
   //---------------------------------------------
   //Vector3 axis; // temp axis angle representation
   //float angle;
   //g_quat_2.getAxisAngle(axis, angle);
   //glRotatef(RADTODEG(angle), axis.x, axis.y, axis.z);		// multiply into matrix     

   //versao 2 - geracao de matriz de transformacao do OpenGL
   //---------------------------------------------
   float m[16];
   g_quat_2.getMatrix(m);
   eixos_2();
   glMultMatrixf(m);

   eixos();
   glPopMatrix();
}

//rotacao em eixo arbitrario extraido a partir de um quaternion
void eixo_arbitrario_quat()
{
   //usa as informacoes do quaternion_2
   Vector3 axis;
   float lixo;
   g_quat_2.getAxisAngle(axis, lixo);    
   axis.normalize();
   
   Matrix m;
   m.rotacionaEixo(axis, g_angle); //eixo unitario, degrees
   
   eixos_3(axis);

   glPushMatrix();
   glMultMatrixf( m.getMatrix() );
   eixos();
   glPopMatrix();
}

//rotacao em eixo arbitrario extraido a partir dos angulos de euler
void eixo_arbitrario_euler()
{
   Vector3 axis(0,0,0);
   float angle = 0;
   if( d_yaw != 0 )
   {
      axis.set(0, 1, 0);
      angle = d_yaw;
   }
   if( d_roll != 0 )
   {
      axis.set(0, 0, 1);
      angle = d_roll;
   }
   if( d_pitch != 0 )
   {
      axis.set(1, 0, 0);
      angle = d_pitch;
   }
      
   Matrix m;
   m.rotacionaEixo(axis, angle); //eixo unitario, degrees
   g_mat = m * g_mat;

   glPushMatrix();
   glMultMatrixf(g_mat.getMatrix());
   eixos();
   glPopMatrix();
}
