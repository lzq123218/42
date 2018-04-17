#include "actor.h"
int term_write(void*);
int login_write(void*, void*);
int vkbd_write(void*, void*);
//
int playwith2d(struct arena* win, struct event* ev);
int playwith3d(struct arena* win, struct event* ev);
void camera_event(struct arena* win, struct event* ev);
void target_event(struct arena* win, struct event* ev);




//
static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
void helpin_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x000000;
	compo = addr + 0x100000;
}
int delete_topone(struct arena* win)
{
	struct relation* rel;
	struct relation* tmp;
	if(win == 0)return 0;
	if(win->irel == 0)return 0;

	rel = win->irel;
	while(1)
	{
		tmp = samedstnextsrc(rel);
		if(tmp == 0)break;

		rel = tmp;
	}
	relationdelete(rel);
	return 1;
}
int actorinput(struct arena* win, struct event* ev)
{
	u64 why,what;
	int x,y,ret;
	struct actor* act;
	struct compo* com;
	struct relation* rel;
	struct relation* tmp;
	//say("%llx,%llx,%llx\n",ev->why,ev->what,ev->where);

	why = ev->why;
	what = ev->what;

//-----------------virtual keyboard---------------------
	if('p' == (what&0xff))
	{
		if(0x2d70 == what)
		{
			x = why&0xffff;
			y = (why>>16)&0xffff;
			if(x<y)ret = x>>4;
			else ret = y>>4;

			//open or close vkbd
			if((x+ret > win->width) && (y+ret > win->height))
			{
				if(win->vkbd)win->vkbd = 0;
				else win->vkbd = 1;
				goto lastword;
			}
		}

		//call vkbd
		if(0 != win->vkbd)
		{
			ret = vkbd_write(win, ev);
			if(0 != ret)goto lastword;
		}

		//rotate camera
		if((_vbo_ == win->fmt)&&(0 == win->edit))
		{
			camera_event(win, ev);
		}
	}

//------------------------consume myself--------------------
	//empty window
	if(0 == win->irel)
	{
		if((_cli_ == win->fmt)&&(_char_ == what))
		{
			term_write(ev);
			goto lastword;
		}
	}

	if(_kbd_ == what)
	{
		if(why == 0xfb)
		{
			if(win->theone)win->theone = 0;
			else win->theone = 1;
			goto lastword;
		}
		else if(why == 0xfc)
		{
			if(win->edit)win->edit = 0;
			else win->edit = 1;
			goto lastword;
		}
		else if((0x48==why)|(0x50==why)|(0x4b==why)|(0x4d==why))
		{
			target_event(win, ev);
		}
	}

	if(win->theone)
	{
		login_write(win, ev);
		goto lastword;
	}

	if(win->edit)
	{
		if(what == _char_)delete_topone(win);
		else if(_vbo_ == win->fmt)playwith3d(win, ev);
		else playwith2d(win, ev);
		goto lastword;
	}

//------------------------pass over-----------------------
	rel = win->irel;
	if(0 == rel)goto lastword;
	while(1)
	{
		tmp = samedstnextsrc(rel);
		if(tmp == 0)break;

		rel = tmp;
	}

	act = (void*)(rel->srcchip);
	com = (void*)(rel->srcfoot);
	act->onwrite(act, com, 0, 0, ev, 0);

lastword:
	return 1;
}