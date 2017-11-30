#include "actor.h"




struct hehe{
	int x;
	int y;
};
static struct hehe* snake;
static struct hehe a;
static struct hehe b;
//
static int len;
static int direction;
//
static int worldwidth;
static int worldheight;
static int foodx;
static int foody;
static int score=0;
static int die=0;




void snake_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	//create screen
	int j;
	int cx,cy,w,h;
	int t1, t2, t3, t4;
	cx = (win->w) * (sty->cx) / 0x10000;
	cy = (win->h) * (sty->cy) / 0x10000;
	w = (win->w) * (sty->wantw) / 0x10000;
	h = (win->h) * (sty->wanth) / 0x10000;
	if(w >= h)w=h;
	else h=w;
	drawsolid_rect(
		win, 0x888888,
		cx-w/2, cy-h/2,
		cx+w/2, cy+h/2
	);

	//snake
	j=0;
	while(1)
	{
		t1 = (cx-w/2) + w * snake[j].x / worldwidth;
		t2 = (cy-h/2) + h * snake[j].y / worldheight;
		t3 = (cx-w/2) + w * (snake[j].x+1) / worldwidth;
		t4 = (cy-h/2) + h * (snake[j].y+1) / worldheight;
		drawsolid_rect(win, 0xffffff, t1, t2, t3, t4);
		drawline_rect(win, 0x000000, t1, t2, t3, t4);

		j++;
		if(j>=len)break;
	}

	//food
	t1 = (cx-w/2) + w * foodx / worldwidth;
	t2 = (cy-h/2) + h * foody / worldheight;
	t3 = (cx-w/2) + w * (foodx+1) / worldwidth;
	t4 = (cy-h/2) + h * (foody+1) / worldheight;
	drawsolid_rect(win, 0x00ff00, t1, t2, t3, t4);
	drawline_rect(win, 0x000000, t1, t2, t3, t4);
}




static int htmlcubie(char* p, u32 color, int x, int y)
{
	return mysnprintf(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"background:#%06x;"
		"\"></div>",
		x*3.1, y*3.1, color
	);
}
void snake_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int j = 0;
	char* p = (char*)(win->buf);

	if(die == 1)
	{
		mysnprintf(p, 0x1000, "boooooooooooooooom");
		return;
	}

	p += mysnprintf(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"width:3.1%;"
		"height:3.1%;"
		"}"
		"</style>"
	);
	while(1)
	{
		p += htmlcubie(p, 0xffffff, snake[j].x, snake[j].y);

		j++;
		if(j>=len)break;
	}
 
	htmlcubie(p, 0xff00, foodx, foody);
}
void snake_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
void snake_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int j,t;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf);
	for(j=0;j<width*height*4;j++)p[j] = 0;

	j=0;
	while(1)
	{
		t = snake[j].x + snake[j].y * width;
		p[t<<2]='#';

		j++;
		if(j>=len)break;
	}

	t = foodx + foody*width;
	p[t<<2] = '@';
}
void snake_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("snake(%x,%x,%x)\n",win,act,sty);
}
void snake_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)snake_read_cli(win, act, sty);
	else if(fmt == __tui__)snake_read_tui(win, act, sty);
	else if(fmt == __html__)snake_read_html(win, act, sty);
	else if(fmt == __vbo__)snake_read_vbo(win, act, sty);
	else snake_read_pixel(win, act, sty);
}




void newfood()
{
	foodx=getrandom() % worldwidth;
	foody=getrandom() % worldheight;
}
void snake_write(struct event* ev)
{
	int j;
	u64 type = ev->what;
	u64 key = ev->why;
	if(die==1)return;

	if(type==0x72616863)
	{
		if(key=='a'){type=0x64626b;key=0x25;}
		else if(key=='w'){type=0x64626b;key=0x26;}
		else if(key=='d'){type=0x64626b;key=0x27;}
		else if(key=='s'){type=0x64626b;key=0x28;}
		else if(key==' '){type=0x2d70;}
	}

	else if(type==0x2d70)
	{
		if(direction==1){type=0x64626b;key=0x25;}
		else if(direction==2){type=0x64626b;key=0x27;}
		else if(direction==3){type=0x64626b;key=0x26;}
		else if(direction==4){type=0x64626b;key=0x28;}
		else return;
	}

	if(type==0x64626b)
	{
		if(key==0x25)
		{
			if( (snake[0].x-1 == snake[1].x) && (snake[0].y == snake[1].y) )return;
			if( (snake[0].x-1) < 0){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x--;
			direction=1;
		}
		else if(key==0x27)
		{
			if(snake[0].x+1 == snake[1].x)
			{
				if(snake[0].y == snake[1].y)return;
			}
			if( (snake[0].x+1) >= worldwidth){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x++;
			direction=2;
		}
		else if(key==0x26)
		{
			if( (snake[0].x == snake[1].x) && (snake[0].y-1 == snake[1].y) )return;
			if( (snake[0].y-1) < 0){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y--;
			direction=3;
		}
		else if(key==0x28)
		{
			if(snake[0].x == snake[1].x)
			{
				if(snake[0].y+1 == snake[1].y)return;
			}
			if( (snake[0].y+1) >= worldheight){die=1;return;}

			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y++;
			direction=4;
		}
		else return;
	}
	else return;

	//蛇身往前挪动
	j=0;
	while(1)
	{
		j++;
		if(j >= len)break;

		b.x = snake[j].x;
		b.y = snake[j].y;
		snake[j].x = a.x;
		snake[j].y = a.y;
		a.x = b.x;
		a.y = b.y;

		if(snake[j].x==snake[0].x)
		{
			if(snake[j].y==snake[0].y)
			{
				die=1;
				return;
			}
		}
	}

	//吃到食物
	if(snake[0].x==foodx)
	{
		if(snake[0].y==foody)
		{
			snake[j].x = a.x;
			snake[j].y = a.y;
			len++;
			newfood();
		}
	}
}




static void snake_list()
{
}
static void snake_choose()
{
}
static void snake_start()
{
	worldwidth = 32;
	worldheight = 32;

	newfood();
	snake[0].x = foodx;
	snake[0].y = foody;
	newfood();

	a.x = -1;
	b.x = -1;

	len=1;
	direction=1;

	die=0;
}
static void snake_stop()
{
}
void snake_create(void* base, void* addr)
{
	struct actor* p = addr;
	snake = base+0x300000;

	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('s', 'n', 'a', 'k', 'e', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->start = (void*)snake_start;
	p->stop = (void*)snake_stop;
	p->list = (void*)snake_list;
	p->choose = (void*)snake_choose;
	p->read = (void*)snake_read;
	p->write = (void*)snake_write;
}
void snake_delete()
{
}
