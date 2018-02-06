#include "actor.h"
#define accuracy 18
#define PI 3.1415926535897932384626433832795028841971693993151




void carveascii(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8 dat)
{
	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->rectcount;

	void* buf = (void*)(win->buf);
	float* vertex  = buf + 0x000000 + (pcount*12);
	float* normal  = buf + 0x200000 + (ncount*12);
	float* color   = buf + 0x400000 + (ccount*12);
	float* texture = buf + 0x600000 + (tcount*8);
	u16* index     = buf + 0xe00000 + (icount*2);

	win->vertexcount  += 4;
	win->normalcount  += 4;
	win->colorcount   += 4;
	win->texturecount += 4;
	win->rectcount   += 6;

	vertex[ 0] = cx-rx-fx;
	vertex[ 1] = cy-ry-fy;
	vertex[ 2] = cz-rz-fz;
	vertex[ 3] = cx+rx-fx;
	vertex[ 4] = cy+ry-fy;
	vertex[ 5] = cz+rz-fz;
	vertex[ 6] = cx-rx+fx;
	vertex[ 7] = cy-ry+fy;
	vertex[ 8] = cz-rz+fz;
	vertex[ 9] = cx+rx+fx;
	vertex[10] = cy+ry+fy;
	vertex[11] = cz+rz+fz;

	color[ 0] = 1.0;
	color[ 1] = 1.0;
	color[ 2] = 1.0;
	color[ 3] = 1.0;
	color[ 4] = 1.0;
	color[ 5] = 1.0;
	color[ 6] = 1.0;
	color[ 7] = 1.0;
	color[ 8] = 1.0;
	color[ 9] = 1.0;
	color[10] = 1.0;
	color[11] = 1.0;

	texture[0] = (dat&0xf)/15.9;
	texture[1] = ((dat>>4)+1)/8.0;
	texture[2] = ((dat&0xf)+1)/15.9;
	texture[3] = ((dat>>4)+1)/8.0;
	texture[4] = (dat&0xf)/15.9;
	texture[5] = (dat>>4)/8.0;
	texture[6] = ((dat&0xf)+1)/15.9;
	texture[7] = (dat>>4)/8.0;

	index[0] = pcount+0;
	index[1] = pcount+1;
	index[2] = pcount+3;
	index[3] = pcount+0;
	index[4] = pcount+2;
	index[5] = pcount+3;
}
void carvestring(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8* buf, int len)
{
	int j;
	float f;
	if(0 == buf)return;
	if(0 == len)
	{
		while(buf[len] > 0x20)len++;
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(buf[j] < 0x20){len = j;break;}
		}
	}
	if(len == 0)return;

	for(j=0;j<len;j++)
	{
		f = (float)(j-len/2)*2;
		carveascii(
			win, rgb,
			cx + (rx*f), cy + (ry*f), cz + (rz*f),
			rx, ry, rz,
			fx, fy, fz,
			buf[j]
		);
	}
}