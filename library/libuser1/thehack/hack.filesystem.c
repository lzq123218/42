#include "libuser.h"
int windowread(int type, void* buf);
u64 file_check(void*, int);




static u8 fsbuf[0x1000];
static u8* buffer;
void fs_prep(void* name)
{
	int ret;
	u64 type;

	ret = openreadclose(name, 0, fsbuf, 0x1000);
	if(ret <= 0)return;
	fsbuf[ret] = 0;

	type = file_check(fsbuf, ret);
	say("type=%.8s\n", &type);
}




static void fs_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh, fsbuf, 0x1000);
}
static void fs_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fs_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fs_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"fs\" style=\"width:50%%;height:100px;float:left;background-color:#465097;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void fs_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fs_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fs_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)fs_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)fs_read_tui(win, sty, act, pin);
	else if(fmt == _html_)fs_read_html(win, sty, act, pin);
	else if(fmt == _json_)fs_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)fs_read_vbo(win, sty, act, pin);
	else fs_read_pixel(win, sty, act, pin);
}
static void fs_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	if(_drag_ == type)
	{
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		fs_prep(buffer);
	}
}
static void fs_get()
{
}
static void fs_post()
{
}
static void fs_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fs_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fs_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void fs_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = fsbuf;
	if(_copy_ == act->type)act->buf = memorycreate(0x1000);
}




void fs_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('f', 's', 0, 0);

	p->oncreate = (void*)fs_create;
	p->ondelete = (void*)fs_delete;
	p->onstart  = (void*)fs_start;
	p->onstop   = (void*)fs_stop;
	p->onget    = (void*)fs_get;
	p->onpost   = (void*)fs_post;
	p->onread   = (void*)fs_read;
	p->onwrite  = (void*)fs_write;
}
