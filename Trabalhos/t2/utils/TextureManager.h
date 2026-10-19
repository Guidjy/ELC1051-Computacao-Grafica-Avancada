//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#ifndef TextureManager_H
#define TextureManager_H

#include <windows.h>
#include "GL/glew.h"
#include "Freeimage/FreeImage.h"
#include <map>
#include <glm\glm.hpp>

class TextureManager
{
public:
	static TextureManager* Inst();
	virtual ~TextureManager();

	glm::vec3 size;

	//load a texture an make it the current texture
	//if texID is already in use, it will be unloaded and replaced with this texture
	bool LoadTexture(const char* filename,	//where to load the file from
		const unsigned int texID,			//arbitrary id you will reference the texture by
											//does not have to be generated with glGenTextures
		GLenum image_format = GL_RGB,		//format the image is in
		GLint internal_format = GL_RGB,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);					//border size

	//free the memory for a texture
	bool UnloadTexture(const unsigned int texID);

	//set the current texture
	bool BindTexture(const unsigned int texID);

	//free all texture memory
	void UnloadAllTextures();

	void printImageType(FREE_IMAGE_TYPE type);

	void printImageColorType(FREE_IMAGE_COLOR_TYPE type);
protected:
	TextureManager();

	static TextureManager* m_inst;
	std::map<unsigned int, GLuint> m_texID;
};

#endif