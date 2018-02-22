#include "actor.h"
u64 getdate();




static void clock_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void clock_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 c[7]={0xff,0xff00,0xffff,0xff0000,0xff00ff,0xffff00,0xffffff};
	int j,k;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	u64 date = getdate();
	u8* p = (u8*)&date;

	if(ww < hh)j = ww;
	else j = hh;
	drawsolid_circle(win, 0x222222, cx, cy, j);
	for(j=6;j>=0;j--)
	{
		drawdecimal(win, c[j], cx+64-(j*24), cy-8, p[j]);
	}
}
static void clock_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void clock_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void clock_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	u64 date = getdate();
	u8* p = (u8*)&date;

	say(	"_%02d_%02d_%02d_%02d_%02d_%02d_%02d\n",
		p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
}
static void clock_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)clock_read_cli(win, act, sty);
	else if(fmt == _tui_)clock_read_tui(win, act, sty);
	else if(fmt == _html_)clock_read_html(win, act, sty);
	else if(fmt == _vbo_)clock_read_vbo(win, act, sty);
	else clock_read_pixel(win, act, sty);
}
static void clock_write(struct event* ev)
{
}




static void clock_list()
{
}
static void clock_change()
{
}
static void clock_start()
{
}
static void clock_stop()
{
}
void clock_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('c', 'l', 'o', 'c', 'k', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->onstart = (void*)clock_start;
	p->onstop = (void*)clock_stop;
	p->onlist = (void*)clock_list;
	p->onchoose = (void*)clock_change;
	p->onread = (void*)clock_read;
	p->onwrite = (void*)clock_write;
}
void clock_delete()
{
}
