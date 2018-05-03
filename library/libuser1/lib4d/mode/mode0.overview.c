#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define _json_ hex32('j','s','o','n')
#define _xml_ hex32('x','m','l',0)
#define _art_ hex32('a','r','t',0)
#define _fd_ hex32('f','d',0,0)
void* allocstyle();
void* allocpinid();
void* actorcreate(void*, u8*);
void* actordelete(void*, u8*);
void* actorstart(void*, void*);
void* actorstop(void*, void*);
//
void draw8bit_rect(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1);
void carvedrone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);
void carvesnowman(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz);




static struct object* obj = 0;
static struct element* ele = 0;
static struct arena* arena = 0;
static struct actor* actor = 0;




void overview_create(void* addr)
{
	obj = addr - 0x400000;
	ele = addr - 0x300000;
	arena = addr + 0x000000;
	actor = addr + 0x100000;
}
void overview_delete()
{
}
int arenaactor(struct arena* win, struct actor* act)
{
	int min;
	int w = win->width;
	int h = win->height;
	int d = (w+h) / 2;
	struct style* sty;
	struct pinid* pin;

	sty = allocstyle();
	if(0 == sty)return 0;

	pin = allocpinid();
	if(0 == pin)return 0;

	if(_vbo_ == win->fmt)
	{
		sty->cx = win->target.cx;
		sty->cy = win->target.cy;
		sty->cz = win->target.cz;

		sty->rx = win->target.rx;
		sty->ry = win->target.ry;
		sty->rz = win->target.rz;

		sty->fx = win->target.fx;
		sty->fy = win->target.fy;
		sty->fz = win->target.fz;

		sty->ux = win->target.ux;
		sty->uy = win->target.uy;
		sty->uz = win->target.uz;
	}
	else
	{
		sty->cx = w/2;
		sty->cy = h/2;
		sty->cz = 0.0;

		if(w<h)min = w/2;
		else min = h/2;
		sty->rx = min;
		sty->fy = min;
		sty->uz = min;
	}

	actorcreate(act, 0);
	actorstart(act, pin);
	relationcreate(win, sty, _win_, act, pin, _act_);
	return 0;
}
int arenalogin(struct arena* win)
{
	struct actor* act;
	int j = win->menudata;
	if(j<0)return 0;

	act = &actor[j];
	if(0 == act->type)return 0;

	arenaactor(win, act);
	return 0;
}
int arenaprev(struct arena* win)
{
	if(win->menudata > 0)win->menudata -= 1;
	return 0;
}
int arenanext(struct arena* win)
{
	int j = win->menudata + 1;
	if(0 != actor[j].type)win->menudata += 1;
	return 0;
}




void overview_read_pixel(struct arena* win)
{
	struct relation* rel;
	u32 c,d;
	int x,y,j,k;
	int w = win->width;
	int h = win->height;

	drawline(win, 0x0000ff, 0, h*1/4, w-1, h*1/4);
	drawline(win, 0x00ff00, 0, h*2/4, w-1, h*2/4);
	drawline(win, 0xff0000, 0, h*3/4, w-1, h*3/4);

	//actor
	for(j=0;j<64;j++)
	{
		c = actor[j].type & 0xff;
		if(0 == c)break;
		else if(j == win->menudata)c = 0xffff00ff;
		else if((c >= 'a')&&(c <= 'z'))c = 0x40808080;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(
			win, c,
			(x+0)*w/8+1, (y+0)*h/32+1,
			(x+1)*w/8-1, (y+1)*h/32-1,
			(u8*)&actor[j].name, 8
		);
	}

	//arena
	d = h/4;
	for(j=0;j<64;j++)
	{
		c = arena[j].type;
		if(0 == c)break;
		else if(win == &arena[j])c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(
			win, c,
			(x+0)*w/8+1, (y+0)*h/32+1+d,
			(x+1)*w/8-1, (y+1)*h/32-1+d,
			(u8*)&arena[j].fmt, 8
		);
	}

	//artery
	d = h/2;
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)continue;
		x = j%8;
		y = j/8;
		drawicon_1(
			win, 0x80ffffff,
			(x+0)*w/8+1, (y+0)*h/32+1+d,
			(x+1)*w/8-1, (y+1)*h/32-1+d,
			(u8*)&ele[j].type, 8
		);
	}

	//system
	d = h*3/4;
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;
		x = (j%64)%8;
		y = (j%64)/8;
		drawicon_1(
			win, 0x80ffffff,
			(x+0)*w/8+1, (y+0)*h/32+1+d,
			(x+1)*w/8-1, (y+1)*h/32-1+d,
			(u8*)&obj[j].name, 8
		);
	}

	//actor.irel
	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)continue;

		rel = actor[j].irel;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline_arrow(win, 0xc0ffc0,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1+48)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1)*h/64
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1+32)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1)*h/64
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xc0ffc0,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1+16)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1)*h/64
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffffff,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1)*h/64
				);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//arena.irel
	for(j=0;j<64;j++)
	{
		if(0 == arena[j].type)break;

		rel = arena[j].irel;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline_arrow(win, 0xc0ffc0,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1+48)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1+16)*h/64
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1+32)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1+16)*h/64
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1+16)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1+16)*h/64
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					(4*(k%8)+3)*w/32,
					(2*(k/8)+1)*h/64,
					(4*(j%8)+1)*w/32,
					(2*(j/8)+1+16)*h/64
				);
			}

			rel = samedstnextsrc(rel);
		}
	}
/*
	//element.irel
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)break;

		rel = ele[j].irel;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline(win, 0xc0ffc0,
					(2*(j%8)+1)*w/16,
					(2*(j/8)+1+32)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1+48)*h/64
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline(win, 0xffffff,
					(2*(j%8)+1)*w/16,
					(2*(j/8)+1+32)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1+32)*h/64
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline(win, 0xc0ffc0,
					(2*(j%8)+1)*w/16,
					(2*(j/8)+1+32)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1+16)*h/64
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline(win, 0xffc0ff,
					(2*(j%8)+1)*w/16,
					(2*(j/8)+1+32)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1)*h/64
				);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//object.irel
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)break;

		rel = obj[j].irel;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline(win, 0xffffff,
					(2*((j%64)%8)+1)*w/16,
					(2*((j%64)/8)+1+48)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1+48)*h/64
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline(win, 0xffc0ff,
					(2*((j%64)%8)+1)*w/16,
					(2*((j%64)/8)+1+48)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1+32)*h/64
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline(win, 0xffc0ff,
					(2*((j%64)%8)+1)*w/16,
					(2*((j%64)/8)+1+48)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1+16)*h/64
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline(win, 0xffc0ff,
					(2*((j%64)%8)+1)*w/16,
					(2*((j%64)/8)+1+48)*h/64,
					(2*(k%8)+1)*w/16,
					(2*(k/8)+1)*h/64
				);
			}
			rel = samedstnextsrc(rel);
		}
	}
*/
}
void overview_read_vbo(struct arena* win)
{
	struct relation* rel;
	int x,y,j,k,c;
	int w = win->width;
	int h = win->height;

	carveline2d(win, 0x0000ff, -1.0, 0.5, 0.0, 1.0, 0.5, 0.0);
	carveline2d(win, 0x00ff00, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
	carveline2d(win, 0xff0000, -1.0,-0.5, 0.0, 1.0,-0.5, 0.0);

	//actor
	for(j=0;j<64;j++)
	{
		c = actor[j].type & 0xff;
		if(0 == c)break;
		else if(j == win->menudata)c = 0xffff00ff;
		else if((c >= 'a')&&(c <= 'z'))c = 0x40808080;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		carvesolid2d_rect(
			win, 0x404040,
			(x*2-7)/8.0, (31-y*2)/32.0, 0.0,
			4.0/33, 0.0, 0.0,
			0.0, 1.0/34, 0.0
		);
		carvestring2d_center(
			win, c,
			(x*2-7)/8.0, (31-y*2)/32.0, -0.01,
			1.0/16, 0.0, 0.0,
			0.0, 1.0/32, 0.0,
			(u8*)&actor[j].name, 8
		);
	}

	//arena
	for(j=0;j<64;j++)
	{
		c = arena[j].type;
		if(0 == c)break;
		else if(win == &arena[j])c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		carvesolid2d_rect(
			win, 0x404040,
			(x*2-7)/8.0, (15-y*2)/32.0, 0.0,
			4.0/33, 0.0, 0.0,
			0.0, 1.0/34, 0.0
		);
		carvestring2d_center(
			win, c,
			(x*2-7)/8.0, (15-y*2)/32.0, -0.01,
			1.0/16, 0.0, 0.0,
			0.0, 1.0/32, 0.0,
			(u8*)&arena[j].fmt, 8
		);
	}

	//artery
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)continue;
		x = j%8;
		y = j/8;
		carvesolid2d_rect(
			win, 0x404040,
			(x*2-7)/8.0, (-1-y*2)/32.0, 0.0,
			4.0/33, 0.0, 0.0,
			0.0, 1.0/34, 0.0
		);
		carvestring2d_center(
			win, 0x80ffffff,
			(x*2-7)/8.0, (-1-y*2)/32.0, -0.01,
			1.0/16, 0.0, 0.0,
			0.0, 1.0/32, 0.0,
			(u8*)&ele[j].type, 8
		);
	}

	//system
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;
		x = (j%64)%8;
		y = (j%64)/8;
		carvesolid2d_rect(
			win, 0x404040,
			(x*2-7)/8.0, (-17-y*2)/32.0, 0.0,
			4.0/33, 0.0, 0.0,
			0.0, 1.0/34, 0.0
		);
		carvestring2d_center(
			win, 0x80ffffff,
			(x*2-7)/8.0, (-17-y*2)/32.0, -0.01,
			1.0/16, 0.0, 0.0,
			0.0, 1.0/32, 0.0,
			(u8*)&obj[j].name, 8
		);
	}

	//actor.irel
	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)continue;

		rel = actor[j].irel;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				carveline2d_arrow(win, 0xc0ffc0,
					((k%8)*4-13)/16.0, (-17-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (31-(j/8)*2)/32.0, -0.01
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				carveline2d_arrow(win, 0xc0ffc0,
					((k%8)*4-13)/16.0, (-1-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (31-(j/8)*2)/32.0, -0.01
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				carveline2d_arrow(win, 0xc0ffc0,
					((k%8)*4-13)/16.0, (15-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (31-(j/8)*2)/32.0, -0.01
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				carveline2d_arrow(win, 0xffffff,
					((k%8)*4-13)/16.0, (31-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (31-(j/8)*2)/32.0, -0.01
				);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//arena.irel
	for(j=0;j<64;j++)
	{
		if(0 == arena[j].type)break;

		rel = arena[j].irel;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				carveline2d_arrow(win, 0xc0ffc0,
					((k%8)*4-13)/16.0, (-17-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (15-(j/8)*2)/32.0, -0.01
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				carveline2d_arrow(win, 0xc0ffc0,
					((k%8)*4-13)/16.0, (-1-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (15-(j/8)*2)/32.0, -0.01
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				carveline2d_arrow(win, 0xffffff,
					((k%8)*4-13)/16.0, (15-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (15-(j/8)*2)/32.0, -0.01
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				carveline2d_arrow(win, 0xffc0ff,
					((k%8)*4-13)/16.0, (31-(k/8)*2)/32.0, -0.01,
					((j%8)*4-15)/16.0, (15-(j/8)*2)/32.0, -0.01
				);
			}

			rel = samedstnextsrc(rel);
		}
	}
}
void overview_read_8bit(struct arena* win)
{
	int x,y;
	int j,c;
	for(j=0;j<64;j++)
	{
		if(j == win->menudata)c = 0x80;
		else c = 0x42;

		x = j%4;
		y = j/4;
		draw8bit_rect(
			win, c,
			 81+x*40, 61+y*10,
			118+x*40, 68+y*10
		);
	}
}
void overview_read_html(struct arena* win)
{
	int j;
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(buf+len, 0x100000-len, "<actor>");
	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;
		len += mysnprintf(
			buf+len, 0x100000-len,
			"%.8s ", (u8*)&actor[j].name
		);
	}
	len += mysnprintf(buf+len, 0x100000-len, "</actor>\n");

	win->len = len;
}
void overview_read_tui(struct arena* win)
{
	int j,k,x,y;
	int ww = ((win->stride)/2)&0xfffc;
	int hh = (win->height)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == win->menudata)k=1;
		else k=2;

		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].name, 8);
	}
}
void overview_read_cli(struct arena* win)
{
}
void actoroutput_overview(struct arena* win)
{
	if(win->fmt == _cli_)overview_read_cli(win);
	else if(win->fmt == _tui_)overview_read_tui(win);
	else if(win->fmt == _vbo_)overview_read_vbo(win);
	else if(win->fmt == _html_)overview_read_html(win);
	else if(win->fmt == _8bit_)overview_read_8bit(win);
	else overview_read_pixel(win);
}




void overview_drag(struct arena* win, int x0, int y0, int x1, int y1)
{
	struct object* o;
	struct element* e;
	struct arena* p;
	struct actor* q;

	if((x0==x1)&&(y0==y1))
	{
		win->menudata = x1 + (y1*8);
		if(y1<8)
		{
			q = &actor[win->menudata];
			if(0 == q->type)return;
			actorcreate(q, 0);
		}
		else if(y1<16)
		{
			y1 = y1-8;
			say("@arena:%d\n", (y1*8)+x1);
			//arenacreate(0,0);
		}
		else if(y1<24)
		{
			y1 = y1-16;
			say("@element:%d\n", (y1*8)+x1);
		}
		else if(y1<32)
		{
			y1 = y1-24;
			say("@object:%d\n", (y1*8)+x1);
		}
		return;
	}

	if(y0 < 8)
	{
		if(y1 < 8)
		{
			say("actor@%d -> actor@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 16)
		{
			y1 = y1-8;
			p = &arena[x1 + (y1*8)];
			if(0 == p->type)return;

			q = &actor[x0 + (y0*8)];
			if(0 == q->type)return;

			arenaactor(p, q);
		}
		else if(y1 < 24)
		{
			y1 = y1-16;
			say("actor@%d -> element@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 32)
		{
			y1 = y1-24;
			say("actor@%d -> object@%d\n", x0+(y0*8), x1+(y1*8));
		}
	}
	else if(y0 < 16)
	{
		y0 = y0-8;
		if(y1 < 8)
		{
			p = &arena[x0 + (y0*8)];
			if(0 == p->type)return;

			q = &actor[x1 + (y1*8)];
			if(0 == q->type)return;

			actorcreate(q, 0);
			relationcreate(q, 0, _act_, p, 0, _win_);
		}
		else if(y1 < 16)
		{
			y1 = y1-8;
			say("arena@%d -> arena@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 24)
		{
			y1 = y1-16;
			say("arena@%d -> element@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 32)
		{
			y1 = y1-24;
			say("arena@%d -> object@%d\n", x0+(y0*8), x1+(y1*8));
		}
	}
	else if(y0 < 24)
	{
		y0 = y0-16;
		if(y1 < 8)
		{
			say("element@%d -> actor@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 16)
		{
			y1 = y1-8;
			say("element@%d -> arena@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 24)
		{
			y1 = y1-16;
			say("element@%d -> element@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 32)
		{
			y1 = y1-24;
			say("element@%d -> object@%d\n", x0+(y0*8), x1+(y1*8));
		}
	}
	else if(y0 < 32)
	{
		y0 = y0-24;
		if(y1 < 8)
		{
			say("object@%d -> actor@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 16)
		{
			y1 = y1-8;
			say("object@%d -> arena@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 24)
		{
			y1 = y1-16;
			say("object@%d -> element@%d\n", x0+(y0*8), x1+(y1*8));
		}
		else if(y1 < 32)
		{
			y1 = y1-24;
			say("object@%d -> object@%d\n", x0+(y0*8), x1+(y1*8));
		}
	}
}
void actorinput_overview(struct arena* win, struct event* ev)
{
	short* t;
	struct actor* act;
	int j, k;
	int x, y, z;
	int width = win->width;
	int height = win->height;

	j = (ev->what)&0xff;
	k = ((ev->what)>>8)&0xff;

	if(j == 'p')
	{
		x = (ev->why)&0xffff;
		x = (x*8) / width;
		y = ((ev->why)>>16)&0xffff;
		y = (y*32) / height;
		z = ((ev->why)>>48)&0xffff;
		if(z > 10)z = 10;

		if('@' == k)
		{
			if(y < 8)win->menudata = x + (y*8);
		}
		else if('+' == k)
		{
			if(y < 8)win->menudata = x + (y*8);
		}
		else if('-' == k)
		{
			j = win->touchdown[z].x;
			j = (j*8) / width;
			k = win->touchdown[z].y;
			k = (k*32) / height;

			if((j<0)|(j>=8))return;
			if((k<0)|(k>=32))return;
			if((x<0)|(x>=8))return;
			if((y<0)|(y>=32))return;

			overview_drag(win, j, k, x, y);
		}
	}
	else if(_char_ == ev->what)
	{
		if((0xd == ev->why)|(0xa == ev->why))
		{
			arenalogin(win);
		}
		else if(0x435b1b == ev->why)
		{
			arenanext(win);
		}
		else if(0x445b1b == ev->why)
		{
			arenaprev(win);
		}
	}
	if(_joy_ == ev->what)
	{
		t = (short*)&ev->why;
		if(_ka_ == t[2])
		{
			arenalogin(win);
			return;
		}
		else if(_dl_ == t[2])
		{
			arenaprev(win);
			return;
		}
		else if(_dr_ == t[2])
		{
			arenanext(win);
			return;
		}
	}
	else if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)
		{
			arenaprev(win);
		}
		else if(0x4d == ev->why)
		{
			arenanext(win);
		}
	}
}