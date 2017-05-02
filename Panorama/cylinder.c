#include "cylinder.h"
#include <math.h>
void m_CylinderWarp(uchar*srcdata, uchar*dstdata, uint width, uint height,uint depth)
{
	int x, y,m_height,m_width=0;
	float32 Fov = 0;
	int f = 0;			//local camera focus
	Fov=360 / Hva;			// horizontal viewing angle
	float32 transition = tan(Fov/2);
	f = width / 2 * transition;
	for (x=0;x<height;x++)
	{
		for (y=0;y<width;y++)
		{
			m_width = f*atan2((x<<2)-width,f<<2) +f*atan2(width,f<<2);
			m_height = f*((y >> 2) - height) / sqrt(f*f + ((width >> 2) - x)*((width >> 2) - x))+(height>>2);
			dstdata[m_width]=srcdata[x*height+y*3]
		}
	}
}