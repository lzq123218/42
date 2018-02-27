#include "actor.h"




static int px, py, turn;
static u8 data[19*19];




static void weiqi_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void weiqi_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 c;
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	//rgb? bgr?
	if( ((win->fmt)&0xffffff) == 0x626772)c = 0x256f8d;
	else c = 0x8d6f25;

	drawsolid_rect(win, c, cx-ww, cy-hh, cx+ww, cy+hh);
	for(y=-9;y<10;y++)		//heng
	{
		drawline(win, 0,
			cx - ww*18/19, cy + y*hh*2/19,
			cx + ww*18/19, cy + y*hh*2/19
		);
	}
	for(x=-9;x<10;x++) 		//shu
	{
		drawline(win, 0,
			cx + x*ww*2/19, cy - hh*18/19,
			cx + x*ww*2/19, cy + hh*18/19
		);
	}
	for(y = cy - hh*12/19; y <= cy + hh*12/19; y += hh*12/19)
	{
		for(x = cx - ww*12/19; x <= cx + ww*12/19; x += ww*12/19)
		{
			drawsolid_circle(win, 0, x, y, ww/3/19);
		}
	}

	//zi
	for(y=-9;y<=9;y++)
	{
		for(x=-9;x<=9;x++)
		{
			if(data[(y+9)*19 + x+9] == 'b')c = 0x444444;
			else if(data[(y+9)*19 + x+9] == 'w')c = 0xffffff;
			else continue;

			drawsolid_circle(
				win, c,
				cx + x*ww*2/19,
				cy + y*hh*2/19,
				ww/19
			);
		}
	}
}
static void weiqi_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int x,y;
	float m,n;
	float cx = sty->i_cx;
	float cy = sty->i_cy;
	float cz = sty->i_cz;
	float ww = sty->i_rx;
	float hh = sty->i_fy;
	float dd = sty->i_uz;
	carvesolid_rect(
		win, 0xf9d65b,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
	for(y=-9;y<10;y++)
	{
		carveline(
			win, 0x222222,
			cx-ww*18/19, cy+(y*hh*2/19), ww/19/4,
			cx+ww*18/19, cy+(y*hh*2/19), ww/19/4
		);
	}
	for(x=-9;x<10;x++)
	{
		carveline(
			win, 0x222222,
			cx+(x*ww*2/19), cy-hh*18/19, ww/19/4,
			cx+(x*ww*2/19), cy+hh*18/19, ww/19/4
		);
	}
	for(n=cy-hh*12/19;n<=cy+hh*12/19;n+=hh*12/19)
	{
		for(m=cx-ww*12/19;m<=cx+ww*12/19;m+=ww*12/19)
		{
			carvesolid_circle(
				win, 0x0,
				m, n, ww/19/4,
				ww/19/4, 0.0, 0.0,
				0.0, 0.0, 1.0
			);
		}
	}
}
static void weiqi_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int x,y,j,k,ret,color;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);

	//
	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			//position
			ret = y*width + x*2;
			ret <<= 2;

			//color
			if( (px == x) && (py == y) )color = 7;
			else if(data[(y*19) + x] == 'b')color = 4;
			else if(data[(y*19) + x] == 'w')color = 1;
			else continue;

			//
			p[ret] = 0x20;
			p[ret + 3] = color;
			p[ret + 4] = 0x20;
			p[ret + 7] = color;
		}
	}
}
static void weiqi_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void weiqi_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)weiqi_read_cli(win, sty, act, com);
	else if(fmt == _tui_)weiqi_read_tui(win, sty, act, com);
	else if(fmt == _vbo_)weiqi_read_vbo(win, sty, act, com);
	else if(fmt == _html_)weiqi_read_html(win, sty, act, com);
	else weiqi_read_pixel(win, sty, act, com);
}




void weiqi_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
	char val;
	int x,y;
	u64 what = ev->what;
	u64 key = ev->why;

	if(what == _kbd_)
	{
		if(key == 0x48)   //up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x4b)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x4d)   //right
		{
			if(px>=18)return;
			px++;
		}
		else if(key == 0x50)   //down
		{
			if(py>=18)return;
			py++;
		}
	}
	else if(what == _char_)
	{
		if(key == 0x20)
		{
			if((turn&1)==0)data[(py*19)+px] = 'b';
			else data[(py*19)+px] = 'w';
			turn++;
		}
	}
	else if(what == 0x2d70)
	{
		x=key & 0xffff;
		y=(key >> 16) & 0xffff;
		//say("%d,%d\n",x,y);

		x = (x*19)>>16;
		y = (y*19)>>16;
		//say("%d,%d\n",x,y);

		if(x<0)return;
		if(x>18)return;
		if(y<0)return;
		if(y>18)return;

		if((turn&0x1) == 0)data[(y*19) + x] = 'b';
		else data[(y*19) + x] = 'w';
		turn++;
	}
}
static void weiqi_list()
{
}
static void weiqi_choose()
{
}
static void weiqi_stop()
{
}
static void weiqi_start()
{
	int x,y;

	turn=0;
	px=py=0;

	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			data[y*19 + x] = 0;
		}
	}
}
static void weiqi_delete()
{
}
static void weiqi_create()
{
}




void weiqi_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('w', 'e', 'i', 'q','i', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)weiqi_create;
	p->ondelete = (void*)weiqi_delete;
	p->onstart  = (void*)weiqi_start;
	p->onstop   = (void*)weiqi_stop;
	p->onlist   = (void*)weiqi_list;
	p->onchoose = (void*)weiqi_choose;
	p->onread   = (void*)weiqi_read;
	p->onwrite  = (void*)weiqi_write;
}