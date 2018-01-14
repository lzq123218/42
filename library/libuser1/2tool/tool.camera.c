#include "actor.h"
void yuyv2rgba(u8* src, u8* dst,
	int w1, int h1, int sx1, int sy1, int dx1, int dy1,
	int w2, int h2, int sx2, int sy2, int dx2, int dy2
);
void startvision();
void stopvision();



struct pictureobject
{
	void* buf;
	int len;
	int width;
	int height;
};
static struct pictureobject* vision = 0;




void camera_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	if(vision == 0)return;
	u8* screen;
	u8* buf;
	int j;
	int w = win->w;
	int h = win->h;
	int cx = w * (sty->cx) / 0x10000;
	int cy = h * (sty->cy) / 0x10000;
	int ww = w * (sty->wantw) / 0x10000;
	int hh = h * (sty->wanth) / 0x10000;

	screen = (u8*)(win->buf);
	buf = vision->buf;
	yuyv2rgba(buf, screen,
		640, 480,       0,       0,       0,       0,
		  w,   h, cx-ww/2, cy-hh/2, cx+ww/2, cy+hh/2
	);
}
void camera_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
void camera_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
void camera_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
void camera_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	if(vision == 0)
	{
		say("camera(%x,%x,%x)\n",win,act,sty);
	}
	else
	{
		say("%x,%x,%x,%x\n",
			vision->buf, vision->len,
			vision->width, vision->height
		);
	}
}
static void camera_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)camera_read_cli(win, act, sty);
	else if(fmt == __tui__)camera_read_tui(win, act, sty);
	else if(fmt == __html__)camera_read_html(win, act, sty);
	else if(fmt == __vbo__)camera_read_vbo(win, act, sty);
	else camera_read_pixel(win, act, sty);
}
static void camera_write(struct event* ev)
{
	int j;
	u8* buf;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 'v')
	{
		vision = (void*)key;
	}
}




static void camera_list()
{
}
static void camera_into()
{
}
static void camera_start()
{
	startvision();
}
static void camera_stop()
{
	stopvision();
}
void camera_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('c', 'a', 'm', 'e', 'r', 'a', 0, 0);

	p->start = (void*)camera_start;
	p->stop = (void*)camera_stop;
	p->list = (void*)camera_list;
	p->choose = (void*)camera_into;
	p->read = (void*)camera_read;
	p->write = (void*)camera_write;
}
void camera_delete()
{
}