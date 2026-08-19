/*
 * IM - Image I/O Library
 * Copyright (c) 1996 CENPES/Petrobras, TeCGraf/PUC-Rio.
 * Antonio E. Scuri - scuri@tecgraf.puc-rio.br
 * 
 * IM.H
 * 
 * Contains the implementation of the main functions to read and write images.
 *
 * History:
 *          1997.04.05 - Added TGA, JPG, LED and PCL formats.
 *          1996.06.14 - Finished inicial version.
 *
 */

#ifndef _IM_H
#define _IM_H

#if	defined(__cplusplus)
extern "C" {
#endif



/* File Formats (first byte) */
enum	
{
  IM_BMP,  /* Windows Device Independent Bitmap - DIB (RGB not compressed, MAP compressed or not) */
  IM_PCX,  /* ZSoft Picture (RGB and MAP compressed or not) */
  IM_GIF,  /* Compuserve Graphics Interchange Format (RGB not suported, MAP always compressed) */
  IM_TIF,  /* TIFF - Tagged Image File Format (RGB and MAP compressed or not) */
  IM_RAS,  /* Sun Raster File (RGB and MAP compressed or not) */
  IM_SGI,  /* Silicon Graphics RGB, RGBA or BW (RGB and MAP compressed or not) */
  IM_JPG,  /* JPEG - Join Photographic Experts Group (RGB and MAP always compressed, MAP as GRAY only) */
  IM_LED,  /* IUP/LED - Linguagem de Especificacao de Dialogos (RGB not suported, MAP not compressed) */
  IM_TGA   /* True Vision Targa */
};


/* format Compression (second byte) */
enum 	
{
	IM_NONE       = 0x0000, /* No Compression. If the format is always compressed an error code is returned. */
	IM_DEFAULT    = 0x0100, /* Default compression for the image in the specified format. */
                          /* If the format does not suports compression no compression is assumed. */
	IM_COMPRESSED = 0x0200  /* When retreiving information: the image is compressed but the compression is not the default. */
};                        /* When saving: force the compression. If the format has no compression returns an error code. */


/* Image Type */
enum 	
{
	IM_RGB, /* 3 separeted buffers (Red, Green and Blue) */
	IM_MAP  /* 1 buffer and a color table */
};


/* Error Codes */
enum 	
{
	IM_ERR_NONE,   /* No error. */
	IM_ERR_OPEN,   /* Error while opening the filename. */
	IM_ERR_READ,   /* Error while reading the filename. */
	IM_ERR_WRITE,  /* Error while writing the filename. */
	IM_ERR_FORMAT, /* Invalid format. */
	IM_ERR_TYPE,   /* Invalid image type. Some formats can save only RGB or only Map images, or the image is RGB and you try to load a Map image. */
	IM_ERR_COMP    /* Compression not supported. */
};


/* Color encoding/decoding */
long imEncodeColor(unsigned char red, unsigned char green, unsigned char blue);
void imDecodeColor(unsigned char* red, unsigned char* green, unsigned char* blue, long color);


/* Image/File Infomation */
int imFileFormat(char *filename, int* format);
int imImageInfo(char *filename, int *width, int *height, int *type, int *pal_size);

/* RGB Image I/O */
int imLoadRGB(char *filename, unsigned char *red, unsigned char *green, unsigned char *blue);
int imSaveRGB(int width, int height, int format, unsigned char *red, unsigned char *green, unsigned char *blue, char *filename);

/* Map Image I/O */
int imLoadMap(char *filename, unsigned char *map, long *color);
int imSaveMap(int width, int height, int format, unsigned char *map, int pal_size, long *color, char *filename);


/* Converts RGB Images to and from Map Images including gray images. */
void imRGB2Map(int width, int height, unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *map, int pal_size, long *color);
void imMap2RGB(int width, int height, unsigned char *map, int pal_size, long *colors, unsigned char *red, unsigned char *green, unsigned char *blue);
void imRGB2Gray(int width, int height, unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *map, long *grays);
void imMap2Gray(int width, int height, unsigned char *color_map, int pal_size, long *colors, unsigned char *grey_map, long *grays);

                 
/* Changes the image size */
void imResize(int src_width, int src_height, unsigned char *src_map, int dst_width, int dst_height, unsigned char *dst_map);
void imStretch(int src_width, int src_height, unsigned char *src_map, int dst_width, int dst_height, unsigned char *dst_map);

/* returns the library current version */
char* imVersion(void);


/* allow to register calbacks to do things inside the read/write loop */
/* the callback returns a escape flag, if the aplication returns 1 the reading writing process stops. */
/* if the callback interrupts the return code of imLoad* and imSave* is IM_INTERRUPTED */
/* Global callbacks are set using format = IM_ALL, specific format callbacks are set using normal format identifiers. */
typedef int (*imCallback)(char *filename);
int imRegisterCallback(imCallback cb, int cb_id, int format);

#define IM_INTERRUPTED -1
#define IM_ALL -1

/* Predefined global callbacks */

/* Counter callback, informs the percentage of the reading(IM_READ)/writing(IM_WRITE) process */
#define IM_COUNTER_CB   0
typedef int (*imCounterCallback)(char *filename, int percent, int io);

enum
{
  IM_READ,
  IM_WRITE
};


/* Resolution callback, retrieves or sets the image resolution when writing or reading acordingly. */
/* Units can be in DPI (dots per inch) or DPC (dots per centimeter) or no resolution */
#define IM_RESOLUTION_CB   1
typedef int (*imResolutionCallback)(char *filename, double* xres, double* yres, int* res_unit);

enum
{
  IM_RES_NONE,
  IM_RES_DPI,
  IM_RES_DPC
};


#if defined(__cplusplus)
}
#endif

#endif  /* __IM_H */

/* IM.H */

