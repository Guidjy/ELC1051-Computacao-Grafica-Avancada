// GLFont.h: interface for the GLFont class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GLFONT_H
#define _GLFONT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <gl/gl.h>

class GLFont  
{
public:
	void printShadow(float x, float y, char *string);
	void print(float x, float y, char *string);
	void endText();
	void startText();
	GLFont();
	virtual ~GLFont();
};

#endif 
