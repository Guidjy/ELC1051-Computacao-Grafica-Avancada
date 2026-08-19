// Frustrum2.cpp: implementation of the Frustrum2 class.
//
//////////////////////////////////////////////////////////////////////

#include "Frustrum2.h"
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Frustrum2::Frustrum2(Vector3 cameraPosition, Vector3 cameraDirection, float angle)
{
	this->cameraPosition = cameraPosition;
	this->cameraDirection = cameraDirection;
	this->angle = angle;

	// Pre-calculus
	// Project cameraVector on the plane
	float camx = cameraDirection.x;
	float camy = cameraDirection.z;

	// Normalize it
	float norm = (float)sqrt(camx*camx + camy*camy);
	camx /= norm;
	camy /= norm;

	// Sin and cos of the angle
	float sinangle  = (float)sin(angle);
	float cosangle  = (float)cos(angle);
	float sinmangle = (float)sin(-angle);
	float cosmangle = (float)cos(-angle);

	// Frustrum vectors
	frustrumLX = camx * cosangle - camy * sinangle;
	frustrumLY = camx * sinangle + camy * cosangle;

	frustrumRX = camx * cosmangle - camy * sinmangle;
	frustrumRY = camx * sinmangle + camy * cosmangle;
}

Frustrum2::~Frustrum2()
{

}

/** Tests visibility of a rectangle on the horizontal plane
*/
bool Frustrum2::visible(int xmin, int ymin, int xmax, int ymax)
{
	float vxmin = xmin - cameraPosition.x;
	float vymin = ymin - cameraPosition.z;
	float vxmax = xmax - cameraPosition.x;
	float vymax = ymax - cameraPosition.z;

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

/*float Frustrum2::distanceFromCameraSquared(int x, int y)
{
	float dx = x - cameraPosition.x;
	float dy = y - cameraPosition.z;
	return dx*dx+dy*dy;
}*/


