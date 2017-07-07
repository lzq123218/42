#include "actor.h"




void rect(
	void* win,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);
void drawdecimal(
	void*, int data, int size,
	int x, int y, u32 fg, u32 bg);




//
static int num;
static u8 buffer[256];




u32 the2048_length(int val)
{
	if(val == 0)return 0;
	if(val <= 8)return 1;
	if(val <= 64)return 2;
	if(val <= 512)return 3;
	if(val <= 8192)return 4;
	if(val <= 65536)return 5;
	return 6;
}
u32 the2048_color(int val)
{
	switch(val)
	{
		case    0:	return 0x555555;
		case    2:	return 0xfffff0;
		case    4:	return 0xffffc0;
		case    8:	return 0x995000;
		case   16:	return 0xc05000;
		case   32:	return 0xb03000;
		case   64:	return 0xff0000;
		case  128:	return 0xffffa0;
		case  256:	return 0xffff80;
		case  512:	return 0xffff00;
		case 1024:	return 0xffffb0;
		case 2048:	return 0x233333;
		case 4096:	return 0x783d72;
		case 8192:	return 0xd73762;
	}
	return 0;
}
static void cubie(
	struct arena* win, int data,
	int x0, int y0, int x1, int y1)
{
	u32 color;
	u32 length;

	//color
	color = the2048_color(data);
	if( ( (win->fmt)&0xffffffff) == 0x61626772)	//bgra->rgba
	{
		color	= 0xff000000
			+ ((color&0xff)<<16)
			+ (color&0xff00)
			+ ((color&0xff0000)>>16);
	}
	rect(win,
		x0, y0,
		x1, y1,
		color, 0
	);

	//decimal
	length = the2048_length(data);
	if(length == 0)return;
	drawdecimal(
		win, data, 4,
		-(length*16)+(x0+x1)/2, -32+(y0+y1)/2,
		0, 0
	);
}
static void the2048_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	int x,y;
	int cx,cy,w,h;
	int (*table)[4] = (void*)buffer + num*16*4;

	//position
	cx = (win->w) * (rel->cx) / 0x10000;
	cy = (win->h) * (rel->cy) / 0x10000;
	w = (win->w) * (rel->wantw) / 0x10000;
	h = (win->h) * (rel->wanth) / 0x10000;
	if(w >= h)w=h;
	else h=w;

	//cubies
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			cubie(
				win,
				table[y][x],
				cx + (x-2)*w/4,
				cy + (y-2)*h/4,
				cx + (x-1)*w/4,
				cy + (y-1)*h/4
			);
		}
	}
}
static void the2048_read_html(struct arena* win, struct actor* act, struct relation* rel)
{
	int x,y;
	u32 color;
	int (*table)[4] = (void*)buffer + num*16*4;
	u8* buf = (u8*)(win->buf);

	buf += fmt(
		buf, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"color:#000;"
		"width:25%%;"
		"height:25%%;"
		"}"
		"</style>"
	);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x] == 0)continue;

			color = the2048_color(table[y][x]);
			buf += fmt(
				buf, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%d%%;"
				"top:%d%%;"
				"background:#%06x;"
				"\">%d</div>",
				25*x, 25*y,
				color, table[y][x]
			);
		}
	}
}
static void the2048_read_tui(struct arena* win, struct actor* act, struct relation* rel)
{
	int x,y,j,k,ret;
	u8 src[10];
	int (*table)[4] = (void*)buffer + num*16*4;

	u8* buf = (u8*)(win->buf);
	int w = win->w;
	int h = win->h;

	for(x=0;x<w*h*4;x++)buf[x]=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			data2decstr(table[y][x], src);
			ret = w*(4*y+1) + 8*x + 2;
			ret <<= 2;

			//color
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					buf[ret + (j*w*4) +(k*4) +3] = 4;
				}
			}


			//number
			j=k=0;
			for(j=0;j<10;j++)
			{
				if(src[j] == 0)break;

				buf[ret + k] = src[j];
				k += 4;
			}
		}
	}
}
static void the2048_read_cli()
{
	int (*table)[4] = (void*)buffer + num*16*4;
	say("%d	%d	%d	%d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
	say("%d	%d	%d	%d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
	say("%d	%d	%d	%d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
	say("%d	%d	%d	%d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
	say("\n");
}
static void the2048_read(struct relation* rel)
{
	struct arena* win = rel->parent_this;
	struct actor* act = rel->child_this;
	u64 fmt = win->fmt;
	u64 dim = win->dim;

	//cli
	if(dim == 1)the2048_read_cli();

	//text
	else if(fmt == 0x74786574)the2048_read_tui(win, act, rel);

	//html
	else if(fmt == 0x6c6d7468)the2048_read_html(win, act, rel);

	//pixel
	else the2048_read_pixel(win, act, rel);
}




//斗兽场......
static void left2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = (void*)buffer + num*16*4;

	//4对祭品
	for(y=0;y<4;y++)
	{
		//前三个会去作死
		for(x=0;x<3;x++)
		{
		//前三个里面没死的才会去作死
		if(table[y][x]!=0)
		{
			//开始作死
			dst=x+1;
			for(;dst<4;dst++)
			{
				//空位置就找下一个
				if(table[y][dst] == 0)continue;

				//跟自己不一样，没法欺负就跑
				if(table[y][dst] != table[y][x])break;

				//找到相同的只能活一个，依次排队准备下次
				table[y][x]=2*table[y][x];	//吃掉别人
				table[y][dst]=0;		//扔尸体
				x=dst;		//别
				break;
			}
		}//if
		}//forx

		//活着的排好队
		dst=0;
		for(x=0;x<4;x++)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[y][dst]=temp;
			dst++;
		}
		}
	}//fory
}
static void right2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = (void*)buffer + num*16*4;

	for(y=0;y<4;y++)
	{
		for(x=3;x>0;x--)
		{
		if(table[y][x]!=0)
		{
			dst=x-1;
			for(;dst>=0;dst--)
			{
				if( table[y][dst] == 0 )continue;
				if( table[y][dst] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[y][dst]=0;
				x=dst;
				break;
			}
		}//if
		}//forx

		dst=3;
		for(x=3;x>=0;x--)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[y][dst]=temp;
			dst--;
		}
		}
	}//fory
}
static void up2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = (void*)buffer + num*16*4;

	for(x=0;x<4;x++)
	{
		for(y=0;y<3;y++)
		{
		if(table[y][x]!=0)
		{
			dst=y+1;
			for(;dst<4;dst++)
			{
				if( table[dst][x] == 0 )continue;
				if( table[dst][x] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[dst][x]=0;
				y=dst;
				break;
			}
		}//if
		}//fory

		dst=0;
		for(y=0;y<4;y++)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[dst][x]=temp;
			dst++;
		}
		}
	}//forx
}
static void down2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = (void*)buffer + num*16*4;

	for(x=0;x<4;x++)
	{
		for(y=3;y>0;y--)
		{
		if(table[y][x]!=0)
		{
			dst=y-1;
			for(;dst>=0;dst--)
			{
				if( table[dst][x] == 0 )continue;
				if( table[dst][x] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[dst][x]=0;
				y=dst;
				break;
			}
		}//if
		}//forx

		dst=3;
		for(y=3;y>=0;y--)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[dst][x]=temp;
			dst--;
		}
		}
	}//fory
}
static void new2048()
{
	int x,y;
	int who,temp;
	int (*table)[4] = (void*)buffer + num*16*4;

	//how many blank cubie
	who=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x]==0)who++;
		}
	}
	if(who==0)return;

	//where
	temp = getrandom();
	if(temp<0) temp = -temp;
	who = temp % who;

	//value
	temp = getrandom()&0x3;
	if(temp>=2)temp=4;
	else temp=2;

	//put
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//here=0 : i "can" put here
			if(table[y][x]==0)
			{
				//who=0 : i "want" put here
				if(who!=0)who--;
				else
				{
					table[y][x]=temp;
					return;
				}
			}//if(empty)
		}//for(x)
	}//for(y)
}
static void the2048_write(struct event* ev)
{
	//kbd
	u8 key;
	int j;
	int* p;
	int* q;
	//say("%x,%x\n",ev->why, ev->what);

	//
	if(ev->what == 0x64626b)
	{
		key = (ev->why)&0xff;
		if( (key>=0x25) && (key<=0x28) )
		{
			//
			p = (void*)buffer + 64*num;
			num = (num+1)%4;
			q = (void*)buffer + 64*num;
			for(j=0;j<16;j++)q[j] = p[j];

			//
			if(key == 0x25)left2048();
			else if(key == 0x26)up2048();
			else if(key == 0x27)right2048();
			else if(key == 0x28)down2048();

			//new number?
			new2048();
		}
	}
	else if(ev->what == 0x72616863)
	{
		if(key == 0x8)
		{
			if(num>0)num--;
		}
	}
}








static void the2048_list()
{
}
static void the2048_choose()
{
}
static void the2048_stop()
{
}
static void the2048_start()
{
	int j;
	for(j=0;j<256;j++)buffer[j] = 0;

	//
	num = 0;
	new2048();
}
void the2048_delete()
{
}
void the2048_create(void* base, struct actor* act)
{
	act->type = hexof('g','a','m','e',0,0,0,0);
	act->name = hexof('2','0','4','8',0,0,0,0);
	act->first = 0;
	act->last = 0;

	act->start = (void*)the2048_start;
	act->stop = (void*)the2048_stop;
	act->list = (void*)the2048_list;
	act->choose = (void*)the2048_choose;
	act->read = (void*)the2048_read;
	act->write = (void*)the2048_write;
}