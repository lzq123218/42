#include "arena.h"
#define __buf__ hex32('b','u','f',0)
#define __win__ hex32('w','i','n',0)




//local
int windowcreate(void*);
int windowdelete();
int windowstart(void*);
int windowstop();
int windowlist();
int windowchoose();
int windowread();
int windowwrite(void* dst, void* src);
//
int websocket_write(int fd, void* buf, int len);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




//
static struct window* arena;
static int cur = 0;




void* arenastart(u64 type, u64 fd)
{
	struct window* win = 0;
	int j = 0;
	while(1)
	{
		win = &arena[j];
		if(win->type == 0)break;

		j++;
		if(j >= 0x100)return 0;
	}

	if(__win__ == type)
	{
		win->type = __win__;
		win->fmt = 0;
		win->irel = 0;
		win->orel = 0;

		windowstart(win);
	}
	else if(hex32('W','S',0,0) == type)
	{
		win->type = hex32('W', 'S', 0, 0);
		win->fmt = hex32('h','t','m','l');
		win->irel = 0;
		win->orel = 0;

		win->fd = fd;
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
int arenaread(struct window* dst, struct window* src)
{
	void* buf;
	if(__win__ == dst->type)
	{
		windowwrite(dst, &arena[0]);
	}
	else if(hex32('W','S', 0, 0) == dst->type)
	{
		websocket_write(dst->fd, src->buf, src->info[0]);
	}
	return 0;
}
int arenawrite(struct event* ev)
{
	void* ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;

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
	}
	return 0;
}
int arenalist()
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
int arenachoose()
{
}
void arenacreate(u8* addr)
{
	int j;
	for(j=0;j<0x400000;j++)addr[j] = 0;

	//create
	arena = (void*)addr;
	soundcreate(arena);
	videocreate(arena);
	windowcreate(arena);

	//
	arenastart(__win__, 0);

	//say("[c,f):createed arena\n");
}
void arenadelete()
{
	//
	//say("[c,f):deleteing arena\n");

	//1024*1024*4
	windowdelete();
}
