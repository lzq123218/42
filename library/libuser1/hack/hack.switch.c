#include "actor.h"
#define _UDP_ hex32('U','D','P',0)
#define _fd_ hex32('f','d',0,0)
void* systemread(int fd);
int systemcreate(u64, void*);




static void switch_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
}
static void switch_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	int len = act->len;
	u8* buf = act->buf;
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
	drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1, buf, len);
}
static void switch_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)switch_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)switch_read_tui(win, sty, act, pin);
	else if(fmt == _html_)switch_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)switch_read_vbo(win, sty, act, pin);
	else switch_read_pixel(win, sty, act, pin);
}




static void switch_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
{
	struct relation* rel;
	struct actor* tmp;
	u8* src;
	u8* dst;
	int len;
	int j;

	if(_act_ == ev->what)
	{
		rel = (void*)(ev->why);
		tmp = (void*)(rel->srcchip);
		len = tmp->len;
		src = tmp->buf;
		dst = (act->buf)+(act->len);

		for(j=0;j<len;j++)dst[j] = src[j];
		dst[j] = 0;

		act->len += len;
	}
}
static void switch_stop(struct actor* act, struct pinid* pin)
{
}
static void switch_start(struct actor* act, struct pinid* pin)
{
}
static void switch_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if(0 != act->buf)
	{
		stopmemory(act->buf);
	}
	act->buf = 0;
}
static void switch_create(struct actor* act, u8* buf)
{
	int fd;
	void* addr;
	if(0 == act)return;
	else if(_orig_ == act->type)act->buf = startmemory(0x100000);

	fd = systemcreate(_UDP_, "127.0.0.1:2222");
	if(fd <= 0)return;

	addr = systemread(fd);
	relation_write(act, 0, _act_, addr, 0, _fd_);
}
static void switch_list(u8* buf)
{
}
static void switch_choose(u8* buf)
{
}




void switch_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s','w','i','t','c','h',0,0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)switch_create;
	p->ondelete = (void*)switch_delete;
	p->onstart  = (void*)switch_start;
	p->onstop   = (void*)switch_stop;
	p->onlist   = (void*)switch_list;
	p->onchoose = (void*)switch_choose;
	p->onread   = (void*)switch_read;
	p->onwrite  = (void*)switch_write;
}
