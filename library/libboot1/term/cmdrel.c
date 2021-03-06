#include "libuser.h"
void* allocstyle();
void* allocpinid();
void* actorstart(void*, void*, void*, void*);
void* actorstop(void*, void*, void*, void*);
void* arenalist(u8* buf, int len);
void* actorlist(u8* buf, int len);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);




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
		sty->vc[0] = win->target.vc[0];
		sty->vc[1] = win->target.vc[1];
		sty->vc[2] = win->target.vc[2];

		sty->vr[0] = win->target.vr[0];
		sty->vr[1] = win->target.vr[1];
		sty->vr[2] = win->target.vr[2];

		sty->vf[0] = win->target.vf[0];
		sty->vf[1] = win->target.vf[1];
		sty->vf[2] = win->target.vf[2];

		sty->vu[0] = win->target.vu[0];
		sty->vu[1] = win->target.vu[1];
		sty->vu[2] = win->target.vu[2];
	}
	else
	{
		sty->vc[0] = w/2;
		sty->vc[1] = h/2;
		sty->vc[2] = 0.0;

		if(w<h)min = w/2;
		else min = h/2;
		sty->vr[0] = min;
		sty->vf[1] = min;
		sty->vu[2] = min;
	}

	actorcreate(0, act);
	actorstart(win, sty, act, pin);
	relationcreate(act, pin, _act_, win, sty, _win_);
	return 0;
}




//rel win@12 -> 2048@7
void relation(u8* buf, int len)
{
	int j,k;
	struct arena* win;
	struct actor* act;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 4)return;

	if(0 == ncmp(argv[1], "add", 3))
	{
		win = arenalist(argv[2], 0);
		act = actorlist(argv[3], 0);
		arenaactor(win, act);
		return;
	}

	if(0 == ncmp(argv[1], "del", 33))
	{
		return;
	}
}