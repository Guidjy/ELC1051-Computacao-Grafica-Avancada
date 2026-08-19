// classe Camera
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "Camera.h"
#include "tools/math.h"
#include <GL/glut.h>
#include "./Tools/Vector3.h"


Camera::Camera()
{
	camPos.set(10000.0f, 10000.0f, 10000);
	camDir.set(0.0f, -1.0f, 1.0f);
	camVel = 0;

	abertura = 40.0;
	znear  = 1;
	zfar   = 2000000;
	aspect = 1;
   lock = false;
}	

void Camera::render()
{
   if( lock==false)
   {
      camLockPos = camPos;
      camLockDir = camDir;
   }
	setupCamera();

   float x, z;
   x = camDir.x*2000;
   z = camDir.z*2000;

	gluLookAt(x + camPos.x,          camPos.y,          z + camPos.z, 
             x + camPos.x+camDir.x, camPos.y+camDir.y, z + camPos.z+camDir.z, 
             0, 1, 0);
   setupFrustum();
}

//verificar na minha funcao original em camera.h
void Camera::setupCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity( );
	gluPerspective(abertura, aspect, znear, zfar);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
}

void Camera::setupFrustum()
{
	// Pre-calculus
	// Project cameraVector on the plane
	float camx = camLockDir.x;
	float camy = camLockDir.z;

	// Normalize it
	float norm = (float)sqrt(camx*camx + camy*camy);
	camx /= norm;
	camy /= norm;
   float abertura = 0.40f;

	// Sin and cos of the angle
	float sinangle  = (float)sin(abertura);
	float cosangle  = (float)cos(abertura);
	float sinmangle = (float)sin(-abertura);
	float cosmangle = (float)cos(-abertura);

	// Frustrum vectors
	frustrumLX = camx * cosangle - camy * sinangle;
	frustrumLY = camx * sinangle + camy * cosangle;

	frustrumRX = camx * cosmangle - camy * sinmangle;
	frustrumRY = camx * sinmangle + camy * cosmangle;

   glLineWidth (11.5);
   glColor3f(0, 1, 0);
   glBegin(GL_LINES);
      glVertex3f(camLockPos.x,                     2000, camLockPos.z);
      glVertex3f(camLockPos.x+camLockDir.x*100000, 2000, camLockPos.z+camLockDir.z*100000);

      glVertex3f(camLockPos.x,                   2000, camLockPos.z);
      glVertex3f(camLockPos.x+frustrumLX*100000, 2000, camLockPos.z+frustrumLY*100000);

      glVertex3f(camLockPos.x,                   2000, camLockPos.z);
      glVertex3f(camLockPos.x+frustrumRX*100000, 2000, camLockPos.z+frustrumRY*100000);

   glEnd();
   glLineWidth (1);
   /**/
}


//desloca na direcao da camera
void Camera::walk(float dist)
{
   //camPos += camDir*dist;
   camPos.x += camDir.x*dist;
   camPos.z += camDir.z*dist;
}

void Camera::rotate(float rx, float ry, float rz)
{
   //if( rx>2) rx = 2.0;
   
   camDir.set(0.0f, 0.0f, 1.0f);
   camDir.rotate(rx, ry, rz);
}


//desloca na direcao perpendicular da camera.
void Camera::move(float dist)
{
   Vector3 tmp;
   
   //find a perpendicular vector do direction
   tmp.set(0,0,0);
   tmp.x = camDir.z;
   tmp.z = -camDir.x;
   //tmp.normalize();

   camPos += tmp*dist;
}

void Camera::elevate(float dist)
{
   camPos.y += dist;
}

void Camera::lockPosition()
{
   if( lock==false)
      lock=true;
   else
      lock = false;
}

bool Camera::visible(int xmin, int xmax, int ymin, int ymax)
{
	float vxmin = xmin - camLockPos.x;
	float vxmax = xmax - camLockPos.x;

   float vymin = ymin - camLockPos.z;
	float vymax = ymax - camLockPos.z;

	// Test against the left frustrum line
	if (frustrumLX*vymin - frustrumLY*vxmin > 0 &&
		frustrumLX*vymax - frustrumLY*vxmin > 0 &&
		frustrumLX*vymin - frustrumLY*vxmax > 0 &&
		frustrumLX*vymax - frustrumLY*vxmax > 0)
	{
		return false;
	}
	// Test against the right frustrum line
	if (frustrumRX*vymin - frustrumRY*vxmin < 0 &&
		frustrumRX*vymax - frustrumRY*vxmin < 0 &&
		frustrumRX*vymin - frustrumRY*vxmax < 0 &&
		frustrumRX*vymax - frustrumRY*vxmax < 0)
	{
		return false;
	}

	// Some part is inside the frustrum
	return true;
}




//configuracao de parametros
//------------------------------------------------------------------
/*void Camera::setPos(float x, float y, float z)
{
	camPos.set(x, y, z);
}

void Camera::setDir(float x, float y, float z)
{
	camDir.set(x, y, z);
}

void Camera::setVel(float vel)
{
	camVel = vel;
}

void Camera::resseta()
{
	camPos.set(0.0f, 100.0f, 0.0f);
	camDir.set(10.0f, 10.0f, 10.0f);
	camVel = 0;
	rx = ry = rz = 0.0;
}
*/
