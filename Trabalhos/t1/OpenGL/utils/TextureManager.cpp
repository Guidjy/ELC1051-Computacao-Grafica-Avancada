
#include "TextureManager.h"
#include "GLUtils.h"

#include <iostream>

using std::cout;
using std::endl;

TextureManager* TextureManager::m_inst(0);

TextureManager* TextureManager::Inst()
{
	if (!m_inst)
		m_inst = new TextureManager();

	return m_inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif
}

TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif

	UnloadAllTextures();
	m_inst = 0;
}

void TextureManager::printImageType(FREE_IMAGE_TYPE type) {
	switch (type)
	{
	case FIT_UNKNOWN:
		cout << "FIT_UNKNOWN";
		break;
	case FIT_BITMAP:
		cout << "FIT_BITMAP";
		break;
	case FIT_UINT16:
		cout << "FIT_UINT16";
		break;
	case FIT_INT16:
		cout << "FIT_INT16";
		break;
	case FIT_UINT32:
		cout << "FIT_UINT32";
		break;
	case FIT_INT32:
		cout << "FIT_INT32";
		break;
	case FIT_FLOAT:
		cout << "FIT_FLOAT";
		break;
	case FIT_DOUBLE:
		cout << "FIT_DOUBLE";
		break;
	case FIT_COMPLEX:
		cout << "FIT_COMPLEX";
		break;
	case FIT_RGB16:
		cout << "FIT_RGB16";
		break;
	case FIT_RGBA16:
		cout << "FIT_RGBA16";
		break;
	case FIT_RGBF:
		cout << "FIT_RGBF";
		break;
	case FIT_RGBAF:
		cout << "FIT_RGBAF";
		break;
	default:
		break;
	}

	cout << endl;
}


void TextureManager::printImageColorType(FREE_IMAGE_COLOR_TYPE type) {
	cout << "Color type: ";
	switch (type)
	{
	case FIC_MINISWHITE:
		cout << "FIC_MINISWHITE";
		break;
	case FIC_MINISBLACK:
		cout << "FIC_MINISBLACK";
		break;
	case FIC_RGB:
		cout << "FIC_RGB";
		break;
	case FIC_PALETTE:
		cout << "FIC_PALETTE";
		break;
	case FIC_RGBALPHA:
		cout << "FIC_RGBALPHA";
		break;
	case FIC_CMYK:
		cout << "FIC_CMYK";
		break;
	default:
		break;
	}
	cout << endl;
}

bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	// Determine the format of the image.
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	// Image not found
	if (format == -1)
	{
		cout << "Could not find image: " << filename << " - Aborting." << endl;
		return false;
	}

	// Found image, but couldn't determine the file format.
	if (format == FIF_UNKNOWN)
	{
		return false;
	}

	// If we're here we have a known image format, so load the image into a bitmap
	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	// get bytes per pixel
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	printImageColorType(FreeImage_GetColorType(bitmap));
	printImageType(FreeImage_GetImageType(bitmap));

	FIBITMAP* convertedbitmap = FreeImage_ConvertToStandardType(bitmap, true);

	FreeImage_Unload(bitmap);

	bitmap = convertedbitmap;

	cout << "Converted image type: ";
	printImageType(FreeImage_GetImageType(bitmap));

	// Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
	// but only if the image is not already 32 bits (i.e. 8 bits per channel).
	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32)
	{
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
		bitmap32 = bitmap;
	}
	else
	{
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
		if (bitmap32 == NULL)
		{
			cout << "Unable to convert image to 32bits." << endl;
			bitmap32 = FreeImage_ConvertTo24Bits(bitmap);
			if (bitmap32 == NULL)
			{
				cout << "Unable to convert image to 24bits." << endl;
				return false;
			}
		}
	}

	// Some basic image info - strip it out if you don't care
	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);
	cout << "Image: " << filename << " is size: " << imageWidth << "x" << imageHeight << "." << endl;

	size.x = (float)imageWidth;
	size.y = (float)imageHeight;

	// Get a pointer to the texture data as an array of unsigned bytes.
	// Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
	GLubyte* textureData = FreeImage_GetBits(bitmap32);

	//OpenGL's image ID to map to
	GLuint gl_texID;

	//if this texture ID is in use, unload the current texture
	if (m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);

	//send to GPU
	glTexImage2D(GL_TEXTURE_2D,    // Type of texture
		0,                // Mipmap level (0 being the top level i.e. full size)
		GL_RGBA,          // Internal format
		imageWidth,       // Width of the texture
		imageHeight,      // Height of the texture,
		0,                // Border in pixels
		GL_BGRA,		  // Data format
		GL_UNSIGNED_BYTE, // Type of texture data
		textureData);     // The image data to use for this texture
	
	GLUtils::checkForOpenGLError(__FILE__, __LINE__);

	// Specify our minification and magnification filters
	GLint minificationFilter = GL_NEAREST;
	GLint magnificationFilter = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);

	// If we're using MipMaps, then we'll generate them here.
	// Note: The glGenerateMipmap call requires OpenGL 3.0 as a minimum.
	if (minificationFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minificationFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minificationFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minificationFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	GLUtils::checkForOpenGLError(__FILE__, __LINE__);


	// Unload the 32-bit colour bitmap
	FreeImage_Unload(bitmap32);

	// If we had to do a conversion to 32-bit colour, then unload the original
	// non-32-bit-colour version of the image data too. 
	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}

	//return success
	return true;
}

bool TextureManager::UnloadTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if (m_texID.find(texID) != m_texID.end())
	{
		glDeleteTextures(1, &(m_texID[texID]));
		m_texID.erase(texID);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

bool TextureManager::BindTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, bind it's texture as current
	if (m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed
	else
		result = false;
	
	GLUtils::checkForOpenGLError(__FILE__, __LINE__);

	return result;
}

void TextureManager::UnloadAllTextures()
{
	//start at the begginning of the texture map
	std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

	//Unload the textures untill the end of the texture map is found
	while (i != m_texID.end())
		UnloadTexture(i->first);

	//clear the texture map
	m_texID.clear();
}