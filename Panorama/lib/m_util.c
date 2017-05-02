#include "m_util.h"
int Writeushort(ushort x, FILE*f)
{
	uchar size[2];
	size[1] = (uchar)((x & 0xff00) >> 8);
	size[0] = (uchar)((x & 0x00ff) >> 0);
	return (f&&fwrite(size, 2, 1, f));
}
int Writeuint(uint x, FILE*f)
{
	uchar size[4];/*Bmp use 32 bit ints*/
	size[3] = (uchar)((x & 0xff000000) >> 24);
	size[2] = (uchar)((x & 0x00ff0000) >> 16);
	size[1] = (uchar)((x & 0x0000ff00) >> 8);
	size[0] = (uchar)((x & 0x000000ff) >> 0);
	return (f&&fwrite(size, 4, 1, f) == 1);
}

int Readuint(uint*x, FILE*f)
{
	uchar size[4];
	if (x == NULL || f == NULL)
	{
		return 0;
	}
	if (fread(size, 4, 1, f) != 1)
	{
		return 0;
	}
	*x = (size[3] << 24 | size[2] << 16 | size[1] << 8 | size[0] );
	return 1;
}
int Readushort(ushort*x, FILE*f) 
{
	uchar size[2];
	if (x==NULL || f==NULL)
	{
		return 0;
	}
	if (fread(size, 2, 1, f) != 1)
	{
		return 0;
	}
	*x = (size[1] << 8 | size[0]);
	return 1;
}

int Readheader(m_Bmp*Bmp, FILE*f)
{
	if (Bmp == NULL||f==NULL)
	{
		return Bmp_INVALID_ARGUMENT;
	}
	/* The header's fields are read one by one, and converted from the format's
	little endian to the system's native representation. */
	if (!Readushort(&(Bmp->m_Bmp_header.Magic), f))			return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.Filesize), f))		return Bmp_IO_ERROR;
	if (!Readushort(&(Bmp->m_Bmp_header.Reserver1), f))		return Bmp_IO_ERROR;
	if (!Readushort(&(Bmp->m_Bmp_header.Reserver2), f))		return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.DataOffset), f))		return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.HeaderSize), f))		return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.Width), f))			return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.Height), f))			return Bmp_IO_ERROR;
	if (!Readushort(&(Bmp->m_Bmp_header.Planes), f))		return Bmp_IO_ERROR;
	if (!Readushort(&(Bmp->m_Bmp_header.BitPerPixel), f))	return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.CompressType), f))	return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.ImageDataSize), f))	return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.HpixelPermeter), f))	return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.VpixelPermeter), f))	return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.ColorUsed), f))		return Bmp_IO_ERROR;
	if (!Readuint(&(Bmp->m_Bmp_header.ColorRequired), f))	return Bmp_IO_ERROR;

	return Bmp_OK;
}
int Writeheader(m_Bmp*Bmp, FILE*f)
{
	if (Bmp==NULL||f==NULL)
	{
		return Bmp_IO_ERROR;
	}
	if (!Writeushort(&(Bmp->m_Bmp_header.Magic), f))				 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.Filesize), f))					 return Bmp_IO_ERROR;
	if (!Writeushort(&(Bmp->m_Bmp_header.Reserver1), f))			 return Bmp_IO_ERROR;
	if (!Writeushort(&(Bmp->m_Bmp_header.Reserver2), f))			 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.DataOffset), f))			 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.HeaderSize), f))			 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.Width), f))					 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.Height), f))					 return Bmp_IO_ERROR;
	if (!Writeushort(&(Bmp->m_Bmp_header.Planes), f))				 return Bmp_IO_ERROR;
	if (!Writeushort(&(Bmp->m_Bmp_header.BitPerPixel), f))		 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.CompressType), f))		 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.ImageDataSize), f))		 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.HpixelPermeter), f))		 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.VpixelPermeter), f))		 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.ColorUsed), f))				 return Bmp_IO_ERROR;
	if (!Writeuint(&(Bmp->m_Bmp_header.ColorRequired), f))		 return Bmp_IO_ERROR;
	return Bmp_OK;
}

m_Bmp* BmpCreate(uint width, uint height, ushort depth)
{
	m_Bmp*Bmp;
	int		bytes_per_pixel = depth >> 3;
	uint	bytes_per_row;

	if (height <= 0 || width <= 0)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
		return NULL;
	}

	if (depth != 8 && depth != 24 && depth != 32)
	{
		Bmp_error_code = Bmp_FILE_NOT_SUPPORTED;
		return NULL;
	}


	/* Allocate the bitmap data structure */
	Bmp = calloc(1, sizeof(m_Bmp));
	if (Bmp == NULL)
	{
		Bmp_error_code= Bmp_OUT_OF_MEMORY;
		return NULL;
	}


	/* Set header' default values */
	Bmp->m_Bmp_header.Magic = 0x4D42;
	Bmp->m_Bmp_header.Reserver1 = 0;
	Bmp->m_Bmp_header.Reserver2 = 0;
	Bmp->m_Bmp_header.HeaderSize = 40;
	Bmp->m_Bmp_header.Planes = 1;
	Bmp->m_Bmp_header.CompressType = 0;
	Bmp->m_Bmp_header.HpixelPermeter = 0;
	Bmp->m_Bmp_header.VpixelPermeter = 0;
	Bmp->m_Bmp_header.ColorUsed = 0;
	Bmp->m_Bmp_header.ColorRequired = 0;


	/* Calculate the number of bytes used to store a single image row. This is always
	rounded up to the next multiple of 4. */
	bytes_per_row = width * bytes_per_pixel;
	bytes_per_row += (bytes_per_row % 4 ? 4 - bytes_per_row % 4 : 0);


	/* Set header's image specific values */
	Bmp->m_Bmp_header.Width = width;
	Bmp->m_Bmp_header.Height = height;
	Bmp->m_Bmp_header.BitPerPixel = depth;
	Bmp->m_Bmp_header.ImageDataSize = bytes_per_row * height;
	Bmp->m_Bmp_header.Filesize= Bmp->m_Bmp_header.ImageDataSize + 54 + (depth == 8 ? Bmp_Palatte_Size : 0);
	Bmp->m_Bmp_header.DataOffset = 54 + (depth == 8 ? Bmp_Palatte_Size : 0);


	/* Allocate palette */
	if (Bmp->m_Bmp_header.BitPerPixel == 8)
	{
		Bmp->Palette = (uchar*)calloc(Bmp_Palatte_Size, sizeof(uchar));
		if (Bmp->Palette == NULL)
		{
			Bmp_error_code= Bmp_OUT_OF_MEMORY;
			free(Bmp);
			return NULL;
		}
	}
	else
	{
		Bmp->Palette = NULL;
	}


	/* Allocate pixels */
	Bmp->Data = (uchar*)calloc(Bmp->m_Bmp_header.ImageDataSize, sizeof(uchar));
	if (Bmp->Data == NULL)
	{
		Bmp_error_code = Bmp_OUT_OF_MEMORY;
		free(Bmp->Palette);
		free(Bmp);
		return NULL;
	}


	Bmp_error_code = Bmp_OK;

	return Bmp;
}

m_Bmp*Bmpread(char*filename)
{
	m_Bmp* Bmp;
	FILE *f;
	if (filename==NULL)
	{
		Bmp_error_code == Bmp_INVALID_ARGUMENT;
		return NULL;
	}
	/*Allocate*/
	Bmp = calloc(1, sizeof(m_Bmp));
	if (Bmp==NULL)
	{
		Bmp_error_code = Bmp_OUT_OF_MEMORY;
		return NULL;
	}
	/*open file*/
	f = fopen(filename, "rb");
	if (f==NULL)
	{
		Bmp_error_code = Bmp_FILE_NOT_FOUND;
		free (Bmp);
		return NULL;
	}
	/*Read header*/
	if (Readheader(Bmp, f) != Bmp_OK || Bmp->m_Bmp_header.Magic != 0X4D42)
	{
		Bmp_error_code = Bmp_FILE_INVALID;
		fclose(f);
		free(Bmp);
		return NULL;
	}
	if ((Bmp->m_Bmp_header.BitPerPixel != 32 && Bmp->m_Bmp_header.BitPerPixel != 24 &&
		Bmp->m_Bmp_header.BitPerPixel != 8 || Bmp->m_Bmp_header.HeaderSize != 40 || Bmp->m_Bmp_header.CompressType != 0))
	{
		Bmp_error_code = Bmp_FILE_NOT_SUPPORTED;
		fclose(f);
		free(Bmp);
		return NULL;
	}
	if (Bmp->m_Bmp_header.BitPerPixel == 8)
	{
		Bmp->Palette = (uchar*)malloc(Bmp_Palatte_Size * sizeof(uchar));
		if (Bmp->Palette == NULL)
		{
			Bmp_error_code = Bmp_OUT_OF_MEMORY;
			fclose(f);
			free(Bmp);
			return NULL;
		}
		if (fread(Bmp->Palette, sizeof(uchar), Bmp_Palatte_Size, f) != Bmp_Palatte_Size)
		{
			Bmp_error_code = Bmp_FILE_INVALID;
			fclose(f);
			free(Bmp);
			return NULL;
		}
	}
		else
		{
			Bmp->Palette == NULL;
		}
		/* allocate memory for image data*/
		Bmp->Data = (uchar*)malloc(Bmp->m_Bmp_header.ImageDataSize);
		if (Bmp->Data==NULL)
		{
			Bmp_error_code = Bmp_OUT_OF_MEMORY;
			fclose(f);
			free(Bmp->Palette);
			free(Bmp);
			return NULL;
		}
		/*read Bmp data*/
		if (fread(Bmp->Data,sizeof(uchar),Bmp->m_Bmp_header.ImageDataSize,f)!=Bmp->m_Bmp_header.ImageDataSize)
		{
			Bmp_error_code = Bmp_FILE_INVALID;
			fclose(f);
			free(Bmp->Data);
			free(Bmp->Palette);
			free(Bmp);
			return NULL;
		}
		fclose(f);
		Bmp_error_code = Bmp_OK;
		return Bmp;
	}

void Bmpwrite(m_Bmp* Bmp, const char* filename)
{
	FILE*	f;

	if (filename == NULL)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
		return;
	}
	/* Open file */
	f = fopen(filename, "wb");
	if (f == NULL)
	{
		Bmp_error_code = Bmp_FILE_NOT_FOUND;
		return;
	}


	/* Write header */
	if (Writeheader(Bmp, f) != Bmp_OK)
	{
		Bmp_error_code = Bmp_IO_ERROR;
		fclose(f);
		return;
	}


	/* Write palette */
	if (Bmp->Palette)
	{
		if (fwrite(Bmp->Palette, sizeof(uchar), Bmp_Palatte_Size, f) != Bmp_Palatte_Size)
		{
			Bmp_error_code = Bmp_IO_ERROR;
			fclose(f);
			return;
		}
	}
	/* Write data */
	if (fwrite(Bmp->Data, sizeof(uchar), Bmp->m_Bmp_header.ImageDataSize, f) != Bmp->m_Bmp_header.ImageDataSize)
	{
		Bmp_error_code= Bmp_IO_ERROR;
		fclose(f);
		return;
	}
	Bmp_error_code = Bmp_OK;
	fclose(f);
}


uint Bmp_GetWidth(m_Bmp*Bmp)
{
	if (Bmp==NULL)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
		return -1;
	}
	Bmp_error_code = Bmp_OK;
	return(Bmp->m_Bmp_header.Width);
}
uint Bmp_GetHeight(m_Bmp*Bmp)
{
	if (Bmp == NULL)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
		return -1;
	}
	Bmp_error_code = Bmp_OK;
	return Bmp->m_Bmp_header.Height;
}

ushort Bmp_GetDepth(m_Bmp*Bmp) 
{
	if (Bmp==NULL)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
		return -1;
	}
	Bmp_error_code = Bmp_OK;
	return Bmp->m_Bmp_header.BitPerPixel;
}
void Bmp_GetPixelRGB(m_Bmp*Bmp,uint x,uint y,uchar*r,uchar*g,uchar*b)
{
	uchar *pixel=NULL;
	uint bytes_per_row;
	uint bytes_per_pixel;
	if (Bmp==NULL||x<0||x>Bmp->m_Bmp_header.Width||y<0||y>Bmp->m_Bmp_header.Height)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
	}
	else
	{
		Bmp_error_code = Bmp_OK;
		bytes_per_pixel = Bmp->m_Bmp_header.BitPerPixel >> 3;
		bytes_per_row = Bmp->m_Bmp_header.ImageDataSize / Bmp->m_Bmp_header.Height;
		pixel = Bmp->Data + ((Bmp->m_Bmp_header.Height-y-1)*bytes_per_row + x*bytes_per_pixel);
	}
	if (Bmp->m_Bmp_header.BitPerPixel==8)
	{
		pixel = Bmp->Palette + *pixel * 4;
	}
	if (r)  *r = *(pixel + 2);
	if (g)  *g = *(pixel + 1);
	if (b)  *b = *(pixel + 0);
}

void Bmp_SetPixelRGB(m_Bmp* Bmp, uint x, uint y, uchar r, uchar g, uchar b)
{
	uchar*	pixel;
	uint	bytes_per_row;
	uchar	bytes_per_pixel;

	if (Bmp == NULL || x < 0 || x >= Bmp->m_Bmp_header.Width || y < 0 || y >= Bmp->m_Bmp_header.Height)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
	}

	else if (Bmp->m_Bmp_header.BitPerPixel != 24 && Bmp->m_Bmp_header.BitPerPixel != 32)
	{
		Bmp_error_code = Bmp_TYPE_MISMATCH;
	}

	else
	{
		Bmp_error_code = Bmp_OK;

		bytes_per_pixel = Bmp->m_Bmp_header.BitPerPixel >> 3;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = Bmp->m_Bmp_header.ImageDataSize / Bmp->m_Bmp_header.Height;

		/* Calculate the location of the relevant pixel (rows are flipped) */
		pixel = Bmp->Data + ((Bmp->m_Bmp_header.Height - y - 1) * bytes_per_row + x * bytes_per_pixel);

		/* Note: colors are stored in BGR order */
		*(pixel + 2) = r;
		*(pixel + 1) = g;
		*(pixel + 0) = b;
	}
}

void Bmp_GetPixelIndex(m_Bmp* Bmp, uint x, uint y, uchar* val)
{
	uchar*	pixel;
	uint	bytes_per_row;

	if (Bmp == NULL || x < 0 || x >= Bmp->m_Bmp_header.Width || y < 0 || y >= Bmp->m_Bmp_header.Height)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
	}

	else if (Bmp->m_Bmp_header.BitPerPixel != 8)
	{
		Bmp_error_code = Bmp_TYPE_MISMATCH;
	}

	else
	{
		Bmp_error_code = Bmp_OK;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = Bmp->m_Bmp_header.ImageDataSize / Bmp->m_Bmp_header.Height;

		/* Calculate the location of the relevant pixel */
		pixel = Bmp->Data + ((Bmp->m_Bmp_header.Height - y - 1) * bytes_per_row + x);


		if (val)	*val = *pixel;
	}
}

void Bmp_SetPixelIndex(m_Bmp* Bmp, uint x, uint y, uchar val)
{
	uchar*	pixel;
	uint	bytes_per_row;

	if (Bmp == NULL || x < 0 || x >= Bmp->m_Bmp_header.Width || y < 0 || y >= Bmp->m_Bmp_header.Height)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
	}

	else if (Bmp->m_Bmp_header.BitPerPixel != 8)
	{
		Bmp_error_code = Bmp_TYPE_MISMATCH;
	}

	else
	{
		Bmp_error_code = Bmp_OK;

		/* Row's size is rounded up to the next multiple of 4 bytes */
		bytes_per_row = Bmp->m_Bmp_header.ImageDataSize / Bmp->m_Bmp_header.Height;

		/* Calculate the location of the relevant pixel */
		pixel = Bmp->Data + ((Bmp->m_Bmp_header.Height - y - 1) * bytes_per_row + x);

		*pixel = val;
	}
}

void Bmp_GetPaletteColor(m_Bmp* Bmp, uchar index, uchar* r, uchar* g, uchar* b)
{
	if (Bmp == NULL)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
	}

	else if (Bmp->m_Bmp_header.BitPerPixel != 8)
	{
		Bmp_error_code = Bmp_TYPE_MISMATCH;
	}

	else
	{
		if (r)  	*r = *(Bmp->Palette + index * 4 + 2);
		if (g)	    *g = *(Bmp->Palette + index * 4 + 1);
		if (b)	*b = *(Bmp->Palette + index * 4 + 0);

		Bmp_error_code = Bmp_OK;
	}
}

void Bmp_SetPaletteColor(m_Bmp* Bmp, uchar index, uchar r, uchar g, uchar b)
{
	if (Bmp == NULL)
	{
		Bmp_error_code = Bmp_INVALID_ARGUMENT;
	}

	else if (Bmp->m_Bmp_header.BitPerPixel != 8)
	{
		Bmp_error_code = Bmp_TYPE_MISMATCH;
	}

	else
	{
		*(Bmp->Palette + index * 4 + 2) = r;
		*(Bmp->Palette + index * 4 + 1) = g;
		*(Bmp->Palette + index * 4 + 0) = b;
		Bmp_error_code = Bmp_OK;
	}
}

void Bmp_Free(m_Bmp* Bmp)
{
	if (Bmp == NULL)
	{
		return;
	}

	if (Bmp->Palette != NULL)
	{
		free(Bmp->Palette);
	}

	if (Bmp->Data != NULL)
	{
		free(Bmp->Data);
	}

	free(Bmp);

	Bmp_error_code = Bmp_OK;
}
Bmp_STATUS Bmp_GetError()
{
	return Bmp_error_code;
}
const char* Bmp_GetErrorDescription()
{
	if (Bmp_error_code > 0 && Bmp_error_code < Bmp_ERROR_NUM)
	{
		return Bmp_Error_string[Bmp_error_code];
	}
	else
	{
		return NULL;
	}
}
