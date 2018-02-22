#include "actor.h"
u32 getrandom();




static void example_read_vbo(
	struct arena* win, struct actor* act,
	struct style* sty, struct compo* com)
{
}
static void example_read_pixel(
	struct arena* win, struct actor* act,
	struct style* sty, struct compo* com)
{
	u32 bg,fg;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_read_html(
	struct arena* win, struct actor* act,
	struct style* sty, struct compo* com)
{
}
static void example_read_tui(
	struct arena* win, struct actor* act,
	struct style* sty, struct compo* com)
{
}
static void example_read_cli(
	struct arena* win, struct actor* act,
	struct style* sty, struct compo* com)
{
}
static void example_read(
	struct arena* win, struct actor* act,
	struct style* sty, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_read_cli(win, act, sty, com);
	else if(fmt == _tui_)example_read_tui(win, act, sty, com);
	else if(fmt == _html_)example_read_html(win, act, sty, com);
	else if(fmt == _vbo_)example_read_vbo(win, act, sty, com);
	else example_read_pixel(win, act, sty, com);
}
static void example_write(struct event* ev)
{
}




static void example_list()
{
}
static void example_change()
{
}
static void example_start()
{
}
static void example_stop()
{
}
void example_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);
	p->irel = 0;
	p->orel = 0;

	p->onstart = (void*)example_start;
	p->onstop = (void*)example_stop;
	p->onlist = (void*)example_list;
	p->onchoose = (void*)example_change;
	p->onread = (void*)example_read;
	p->onwrite = (void*)example_write;
}
void example_delete()
{
}
