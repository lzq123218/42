#include "arena.h"
#define _win_ hex32('w','i','n',0)
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
#define _VNC_ hex32('V','N','C',0)
#define _vnc_ hex32('v','n','c',0)




//
int videocreate(void*);
int videodelete();
//
int soundcreate(void*);
int sounddelete();
//local
int windowcreate(void*);
int windowdelete();
int windowlist();
int windowchoose();
int windowstart(void*);
int windowstop();
int windowread();
int windowwrite(void* win);
//remote
int wsclient_start(void* win);
int wsclient_stop(void* win);
int wsclient_read(void* win);
int wsclient_write(void* win);
int wsserver_start(void* win);
int wsserver_stop(void* win);
int wsserver_read(void* win);
int wsserver_write(void* win);
int vncclient_start(void* win);
int vncclient_stop(void* win);
int vncclient_read(void* win);
int vncclient_write(void* win);
int vncserver_start(void* win);
int vncserver_stop(void* win);
int vncserver_read(void* win);
int vncserver_write(void* win);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




//
static struct window* arena;
static struct style* style = 0;
static int winlen = 0;
static int stylen = 0;
void* arenaalloc()
{
	int j;
	struct window* win;
	while(1)
	{
		win = &arena[j];
		if(win->type == 0)break;

		j++;
		if(j >= 0x100)return 0;
	}
	return win;
}
void* stylealloc()
{
	struct style* sty = (void*)style + stylen;
	stylen += sizeof(struct style);

	sty->cx = 0.0;
	sty->cy = 0.0;
	sty->cz = 0.0;

	sty->rx = 0.0;
	sty->ry = 0.0;
	sty->rz = 0.0;

	sty->fx = 0.0;
	sty->fy = 0.0;
	sty->fz = 0.0;

	sty->ux = 0.0;
	sty->uy = 0.0;
	sty->uz = 0.0;

	return sty;
}




void* arenastart(u64 type, u64 fd)
{
	int j = 0;
	struct window* win = arenaalloc();
	if(0 == win)return 0;

	if(_win_ == type)
	{
		win->type = _win_;
		win->fmt = 0;
		win->irel = 0;
		win->orel = 0;

		windowstart(win);
	}
	else if(_ws_ == type)
	{
		win->type = _ws_;
		win->fmt = hex32('d','a','t','a');
		win->irel = 0;
		win->orel = 0;

		//be client, accept data
		wsclient_start(win);
	}
	else if(_WS_ == type)
	{
		win->type = _WS_;
		win->fmt = hex32('h','t','m','l');
		win->irel = 0;
		win->orel = 0;

		//be server, output data
		win->fd = fd;
		wsserver_start(win);
	}
	else if(_vnc_ == type)
	{
		win->type = _vnc_;
		win->fmt = hex32('d','a','t','a');
		win->irel = 0;
		win->orel = 0;

		//be client, accept data
		vncclient_start(win);
	}
	else if(_VNC_ == type)
	{
		win->type = _VNC_;
		win->fmt = 0;
		win->irel = 0;
		win->orel = 0;

		//be server, output data
		win->fd = fd;
		vncserver_start(win);
	}

	return win;
}
int arenastop(struct window* win)
{
	if(win == 0)return 0;
	windowstop(win);

	win->type = 0;
	win->fmt = 0;
	win->irel = 0;
	win->orel = 0;
	return 0;
}
int arenaread(struct window* win)
{
	void* buf;
	if(_win_ == win->type)
	{
		windowwrite(win);
	}
	else if(hex32('W','S', 0, 0) == win->type)
	{
		wsserver_write(win);
	}
	return 0;
}
int arenawrite(struct event* ev)
{
	int j;
	void* ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	//say("@arenawrite:%llx,%llx,%llx\n\n\n\n\n", why, what, where);

	if(hex32('w','+',0,0) == what)
	{
		ret = arenastart(why, where);
		if(ret == 0)
		{
			say("error@w+\n");
			return 0;
		}
	}
	else if(hex32('w','-',0,0) == what)
	{
		ret = (void*)where;
		arenastop(ret);
	}
	else
	{
		if(where >= 0x1000)return 0;

		for(j=0;j<0x100;j++)
		{
			if((_WS_ == arena[j].type)&&(where == arena[j].fd))
			{
				wsserver_read(&arena[j]);
			}
		}
	}
	return 0;
}
int arenalist(u8* buf)
{
	int j;
	for(j=0;j<0x100;j++)
	{
		if(0 == arena[j].type)break;
		say("[%03x]: %.8s,%.8s\n", j, &arena[j].type, &arena[j].fmt);
	}

	if(0 == j)say("empty arena\n");
	return 0;
}
int arenachoose(u8* buf)
{
	//xiangqi.black@win2.center
	if(0 == buf)return 0;
	if(0 == ncmp(buf, "win", 3))arenastart(_win_, 0);
	else say("@arena: %s\n", buf);
}
void arenacreate(u8* addr)
{
	int j;
	for(j=0;j<0x400000;j++)addr[j] = 0;

	arena = (void*)(addr+0x000000);
	//actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	//pinid = (void*)(addr+0x300000);

	//create
	//remotecreate(arena);
	soundcreate(arena);
	videocreate(arena);
	windowcreate(arena);

	//
	arenastart(_win_, 0);

	//say("[c,f):createed arena\n");
}
void arenadelete()
{
	//
	//say("[c,f):deleteing arena\n");

	//1024*1024*4
	windowdelete();
	videodelete();
	sounddelete();
	//remotedelete();
}
