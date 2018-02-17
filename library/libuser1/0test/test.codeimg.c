#include "actor.h"
void scale_image(void* src, void* dst,
	int sw, int sh, int sx1, int sy1, int sx2, int sy2,
	int dw, int dh, int dx1, int dy1, int dx2, int dy2);




#define DIM 1024
#define _sq(x) ((x)*(x))
#define r(n)(getrandom()%n)
u32* src = 0;




unsigned char RED1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2))*255);
}
unsigned char GREEN1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2-2*arccos(-1)/3))*255);
}
unsigned char BLUE1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2+2*arccos(-1)/3))*255);
}
unsigned char RED2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):RED2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char GREEN2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):GREEN2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char BLUE2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):BLUE2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char RED3(int i,int j)
{
	float x=0,y=0;int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return ln(k)*47;
}
unsigned char GREEN3(int i,int j)
{
	float x=0,y=0;
	int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return ln(k)*47;
}
unsigned char BLUE3(int i,int j)
{
	float x=0,y=0;int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return 128-ln(k)*23;
}
unsigned char RED4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*((n-80)/800)*((n-80)/800)*((n-80)/800);
}
unsigned char GREEN4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*power((n-80)/800,0.7);
}
unsigned char BLUE4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*squareroot((n-80)/800);
}
unsigned char RED6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)((i+DIM)*s+y)%2+(int)((DIM*2-i)*s+y)%2)*127;
}
unsigned char GREEN6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)(5*((i+DIM)*s+y))%2+(int)(5*((DIM*2-i)*s+y))%2)*127;
}
unsigned char BLUE6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)(29*((i+DIM)*s+y))%2+(int)(29*((DIM*2-i)*s+y))%2)*127;
}




static void codeimg_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	int cx = width * (sty->cx) / 0x10000;
	int cy = height * (sty->cy) / 0x10000;
	int ww = width * (sty->wantw) / 0x20000;
	int hh = height * (sty->wanth) / 0x20000;
	u32* dst = (u32*)(win->buf);

	scale_image(src, dst,
		1024, 1024, 0, 0, 1024, 1024,
		width, height, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
static void codeimg_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void codeimg_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void codeimg_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void codeimg_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("codeimg(%x,%x,%x)\n",win,act,sty);
}
static void codeimg_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)codeimg_read_cli(win, act, sty);
	else if(fmt == _tui_)codeimg_read_tui(win, act, sty);
	else if(fmt == _html_)codeimg_read_html(win, act, sty);
	else if(fmt == _vbo_)codeimg_read_vbo(win, act, sty);
	else codeimg_read_pixel(win, act, sty);
}
static void codeimg_write(struct event* ev)
{
}




static void codeimg_list()
{
}
static void codeimg_change()
{
}
static void codeimg_start()
{
	int rr,gg,bb;
	int x,y;
	src = startmemory(1024*1024*4);

	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			rr = RED4(x,y);
			gg = GREEN4(x,y);
			bb = BLUE4(x,y);
			src[(y*1024)+x] = 0xff000000 + (rr<<16) + (gg<<8) + bb;
		}
	}
}
static void codeimg_stop()
{
}
void codeimg_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('c', 'o', 'd', 'e', 'i', 'm', 'g', 0);

	p->start = (void*)codeimg_start;
	p->stop = (void*)codeimg_stop;
	p->list = (void*)codeimg_list;
	p->choose = (void*)codeimg_change;
	p->read = (void*)codeimg_read;
	p->write = (void*)codeimg_write;
}
void codeimg_delete()
{
}
