//-------------------------------------------------------------------------
// classe para definicao da camera 
// by Cesar Pozzer 
//-------------------------------------------------------------------------



//***** COMENTARIOS *****
//a camera devera ter AI para saber qual cena deve ser mostrada. Ela
//deve receber mensagens de Actors indicando quais cenas sao mais 
//importantes, alem de Actor, quando os mesmos estiverem realizando
//certos tipos de acoes.

#ifndef _CAMERA_H
#define _CAMERA_H	

#include "./Tools/Vector3.h"
#include <stdio.h>

class Camera{
public:
	//posicao, direcao e velocidade da camera
	Vector3 camPos, camDir;
   Vector3 camLockPos, camLockDir;
	
   //parametros de projecao da camera
	float  abertura, znear, zfar, aspect;

	float camVel;

	Camera();

	//configuracao de parametros
	//------------------------------------------------------------------
	//void setPos(float x, float y, float z);
	//void setDir(float x, float y, float z);
	//void setVel(float vel);

	//void setTrans(float x, float y, float z);

	void rotate(float rx, float ry, float rz);
   void walk(float dist);
   void move(float dist);
   void elevate(float dist);

   void render();
   void lockPosition();
	bool visible(int xmin, int ymin, int xmax, int ymax);

private:
	float frustrumLX, frustrumLY, frustrumRX, frustrumRY;
   bool lock;

  	void  setupCamera();
   void  setupFrustum();

};

#endif
