#ifndef M_UTIL_H_
#define M_UTIL_H_
#include <stdio.h>
#include <stdlib.h>
#ifndef uchar
typedef  unsigned char uchar;
#endif
#ifndef uint
typedef  unsigned  int uint;
#endif
#ifndef ushort
typedef  unsigned short ushort;
#endif
#ifndef float32
typedef  float float32;
#endif
#define harf(x)  x/2;
typedef enum
{
	Bmp_OK = 0,									 /* No error */
	Bmp_ERROR,								 /* General Error */
	Bmp_OUT_OF_MEMORY,			 /* Out of memory*/
	Bmp_IO_ERROR,                          /* File input/output is error*/
	Bmp_FILE_NOT_FOUND,            /* Bmp file doesn't exist*/
	Bmp_FILE_NOT_SUPPORTED,    /* File is not a support Bmp varient */
	Bmp_INVALID_ARGUMENT,		 /* An argument is invalid or out of range*/
	Bmp_FILE_INVALID,					 /* File is not a Bmp image or is an invalid Bmp*/
	Bmp_TYPE_MISMATCH,              /* Bmp image depth is not same*/
	Bmp_ERROR_NUM
}Bmp_STATUS;
typedef struct Bmp_header
{
	ushort Magic;
	uint Filesize;
	ushort Reserver1;
	ushort Reserver2;
	uint DataOffset;
	uint HeaderSize;
	uint Width;
	uint Height;
	ushort Planes;
	ushort BitPerPixel;
	uint CompressType;
	uint ImageDataSize;
	uint HpixelPermeter;
	uint VpixelPermeter;
	uint ColorUsed;
	uint ColorRequired;
}m_Bmp_header;
struct _Bmp
{
	m_Bmp_header m_Bmp_header;
	uchar* Palette;
	uchar* Data;
};
struct m_Mat
{
	uint Width;
	uint Height;
	uchar* Data;
};
struct point2f 
{
	uint x;
	uint y;
}m_point;
static char*Bmp_Error_string[] =
{
	"",
	"general error",
	"out of memory",
	"file input/output is error",
	"Bmp file is not exist",
	"file is not a support Bmp varient",
	"an argumnet is invalid or out of memory",
	"file is not a Bmp image or an invalid Bmp",
	"Bmp image depth is not same",
};
/*argument about varient and function*/
#define  Bmp_Palatte_Size  (256*4)
static Bmp_STATUS Bmp_error_code;
typedef struct _Bmp m_Bmp;
/*forward declear*/
int Readushort(ushort*x,FILE*f);
int Readuint(uint*x,FILE*f);

int Writeushort(ushort x, FILE*f);
int Writeuint(uint x, FILE*f);

int Readheader(m_Bmp*Bmp, FILE*f);
int Writeheader(m_Bmp*Bmp, FILE*f);

m_Bmp*Bmpread(char *filename);
void Bmpwrite(m_Bmp*Bmp, const char*filename);

uint Bmp_GetWidth(m_Bmp*Bmp);
uint Bmp_GetHeight(m_Bmp*Bmp);

ushort Bmp_GetDepth(m_Bmp*Bmp);

void Bmp_GetPixelRGB(m_Bmp*Bmp, uint x, uint y, uchar*r, uchar*g, uchar*b);
void Bmp_SetPixelRGB(m_Bmp* Bmp, uint x, uint y, uchar r, uchar g, uchar b);

void Bmp_GetPixelIndex(m_Bmp* Bmp, uint x, uint y, uchar* val);
void Bmp_SetPixelIndex(m_Bmp* Bmp, uint x, uint y, uchar val);

void Bmp_GetPaletteColor(m_Bmp* Bmp, uchar index, uchar* r, uchar* g, uchar* b);
void Bmp_SetPaletteColor(m_Bmp* Bmp, uchar index, uchar r, uchar g, uchar b);

m_Bmp* BmpCreate(uint width, uint height, ushort depth);
Bmp_STATUS Bmp_GetError();
const char* Bmp_GetErrorDescription();
#define Bmp_CHECK_ERROR( output_file, return_value ) \
	if ( Bmp_GetError() != Bmp_OK )													\
	{																				\
		fprintf( ( output_file ), "Bmp error: %s\n", Bmp_GetErrorDescription() );	\
		return( return_value );														\
	}																				\

#endif
