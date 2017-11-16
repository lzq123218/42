#include "actor.h"
double sine(double);
double cosine(double);
double squareroot(double);




void carvetriangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index =    buf + 0xe00000 + (icount*2);

	win->info[8] += 3;
	win->info[9] += 3;
	win->info[10] += 3;
	win->info[14] += 3;

	//color
	color[0] = rr;
	color[1] = gg;
	color[2] = bb;

	color[3] = rr;
	color[4] = gg;
	color[5] = bb;

	color[6] = rr;
	color[7] = gg;
	color[8] = bb;

	//normal
	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;

	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;

	normal[6] = 0.0;
	normal[7] = 0.0;
	normal[8] = 1.0;

	//vertex
	vertex[0] = x1;
	vertex[1] = y1;
	vertex[2] = z1;

	vertex[3] = x2;
	vertex[4] = y2;
	vertex[5] = z2;

	vertex[6] = x3;
	vertex[7] = y3;
	vertex[8] = z3;

	//index
	index[0] = pcount + 0;
	index[1] = pcount + 1;
	index[2] = pcount + 2;
}
void carverect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,	//center xyz
	float rx, float ry, float rz,	//width = |rvector|*2
	float fx, float fy, float fz)	//height = |fvector|*2
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	//u32 tcount = win->info[11];
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index =    buf + 0xe00000 + (icount*2);

	win->info[8] += 4;
	win->info[9] += 4;
	win->info[10] += 4;
	win->info[14] += 6;

	//color
	color[0] = rr;
	color[1] = gg;
	color[2] = bb;

	color[3] = rr;
	color[4] = gg;
	color[5] = bb;

	color[6] = rr;
	color[7] = gg;
	color[8] = bb;

	color[9] = rr;
	color[10] = gg;
	color[11] = bb;

	//normal
	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;

	normal[3] = 0.0;
	normal[4] = 0.0;
	normal[5] = 1.0;

	normal[6] = 0.0;
	normal[7] = 0.0;
	normal[8] = 1.0;

	normal[9] = 0.0;
	normal[10] = 0.0;
	normal[11] = 1.0;

	//vertex
	vertex[ 0] = cx - rx - fx;
	vertex[ 1] = cy - ry - fy;
	vertex[ 2] = cz - rz - fz;

	vertex[ 3] = cx + rx - fx;
	vertex[ 4] = cy + ry - fy;
	vertex[ 5] = cz + rz - fz;

	vertex[ 6] = cx - rx + fx;
	vertex[ 7] = cy - ry + fy;
	vertex[ 8] = cz - rz + fz;

	vertex[ 9] = cx + rx + fx;
	vertex[10] = cy + ry + fy;
	vertex[11] = cz + rz + fz;

	//index
	index[0] = pcount + 0;
	index[1] = pcount + 1;
	index[2] = pcount + 2;

	index[3] = pcount + 1;
	index[4] = pcount + 2;
	index[5] = pcount + 3;
}




void carvecircle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz)
{
}