#include "lib\m_util.h"
#define filename "src\\01.bmp"
#define panoname "result\\gray.bmp"
void main()
{
	m_Bmp* mat;
	uchar rr=0 ;
	uchar gg=0 ;
	uchar bb=0 ;
	mat = Bmpread(filename);
	
	uint m_width= mat->m_Bmp_header.Height;
	uint m_height = mat->m_Bmp_header.Width;
	ushort depth = Bmp_GetDepth(mat);
	m_Bmp*pano=BmpCreate(m_width, m_height, depth);
	
	for (int i=0;i<m_height;i++)
	{
		for (int j=0;j<m_width;j++)
		{
			Bmp_GetPixelRGB(mat, i, j, &rr, &gg, &bb);
			/*printf("rr=%d\n", rr);
			printf("gg=%d\n", gg);
			printf("bb=%d\n", bb);*/
			rr = gg = bb = 0.3*(rr + gg + bb);
			Bmp_SetPixelRGB(pano, i, j, &rr, &gg, &bb);
		}
	}
	Bmpwrite(pano, panoname);
	return 0;
}