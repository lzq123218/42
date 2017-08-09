#include "actor.h" 
void line(void*,
	int x1,int y1,
	int x2,int y2,
	u32 color);
void circleframe(void*,
	int x, int y,
	int r, u32 color);
void backgroundcolor(void*,
	u32);




static int turn;
static char data[9];




void ooxx_read_pixel(struct arena* win, struct actor* act, struct style* rel)
{
	int x,y;
	int cx,cy,w,h;
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;
	if(w >= h)w=h;
	else h=w;

	//heng
	line(win,
		cx-w/2, cy-h/6,
		cx+w/2, cy-h/6,
		0xffffffff);
	line(win,
		cx-w/2, cy+h/6,
		cx+w/2, cy+h/6,
		0xffffffff);

	//shu
	line(win,
		cx-w/6, cy-h/2,
		cx-w/6, cy+h/2,
		0xffffffff);
	line(win,
		cx+w/6, cy-h/2,
		cx+w/6, cy+h/2,
		0xffffffff);

	//ox
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(data[3*y + x] == 'o')
			{
				circleframe(win,
					cx+(x-1)*w/3,
					cy+(y-1)*h/3,
					w/12,
					0xff
				);
			}
			else if(data[3*y + x] == 'x')
			{
				line(win,
					cx+(4*x-5)*w/12, cy+(4*y-5)*h/12,
					cx+(4*x-3)*w/12, cy+(4*y-3)*h/12,
					0xff0000
				);
				line(win,
					cx+(4*x-5)*w/12, cy+(4*y-3)*h/12,
					cx+(4*x-3)*w/12, cy+(4*y-5)*h/12,
					0xff0000
				);
			}
		}//forx
	}//fory
}
static void ooxx_read(struct arena* win, struct actor* act, struct style* rel)
{
	ooxx_read_pixel(win, act, rel);
}




void ooxx_write(struct event* ev)
{
	char val;
	int x,y;
/*
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;
*/
	int min = 512;
	if(ev->what == 0x2d70)
	{
		x=(ev->why) & 0xffff;
		if(x>min)return;

		y=( (ev->why) >> 16 ) & 0xffff;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
say("%d,%d\n",x,y);
		if(data[y*3 + x] != 0)return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y*3 + x] = val;
		turn++;
	}
}




static void ooxx_list()
{
}
static void ooxx_choose()
{
}
static void ooxx_start()
{
	int j;

	turn=0;
	for(j=0;j<9;j++)data[j]=0;
}
static void ooxx_stop()
{
}
void ooxx_create(char* base,void* addr)
{
	struct actor* p = addr;

	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex32('o', 'o', 'x', 'x');

	p->start = (void*)ooxx_start;
	p->stop = (void*)ooxx_stop;
	p->list = (void*)ooxx_list;
	p->choose = (void*)ooxx_choose;
	p->read = (void*)ooxx_read;
	p->write = (void*)ooxx_write;
}
void ooxx_delete()
{
}
