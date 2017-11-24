#include "actor.h"




//
static u8 buffer[8][8];




static void chess_read_pixel(struct arena* win, struct actor* act, struct style* rel)
{
	u32 color;
	int x,y;
	int cx = (win->w) * (rel->cx) / 0x10000;
	int cy = (win->h) * (rel->cy) / 0x10000;
	int w = (win->w) * (rel->wantw) / 0x10000 / 8;
	int h = (win->h) * (rel->wanth) / 0x10000 / 8;
	if(w >= h)w=h;
	else h=w;

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			drawsolid_rect(win, color,
				cx+(x-4)*w, cy+(y-4)*h,
				cx+(x-3)*w, cy+(y-3)*h
			);
			if(buffer[y][x] == 0)continue;

			drawascii(win,
				buffer[y][x], 2,
				cx+(x-4)*w, cy+(y-4)*h,
				0xffff00ff, 0
			);
		}
	}
}
static void chess_read_vbo(struct arena* win, struct actor* act, struct style* rel)
{
	u32 color;
	int x,y;
	float xxx, yyy;

	float cx = (float)(rel->cx) / 65536.0 - 0.5;
	float cy = (float)(rel->cy) / 65536.0 - 0.5;
	float w = (float)(rel->wantw) / 65536.0;
	float h = (float)(rel->wanth) / 65536.0;

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			xxx = cx + (x+x-7)*w/16;
			yyy = cy - (y+y-7)*h/16;
			carvesolid_prism4(
				win, color,
				xxx, yyy, 0.0,
				w/16, 0.0, 0.0,
				0.0, h/16, 0.0,
				0.0, 0.0, 0.0
			);
		}
	}
}
static void chess_read_html(struct arena* win, struct actor* act, struct style* rel)
{
}
static void chess_read_text(struct arena* win, struct actor* act, struct style* rel)
{
}
static void chess_read(struct arena* win, struct actor* act, struct style* rel)
{
	//text 
	if(win->fmt == hex32('t','e','x','t'))chess_read_text(win, act, rel);

	//html
	else if(win->fmt == hex32('h','t','m','l'))chess_read_html(win, act, rel);

	//vbo
	else if(win->fmt == hex32('v','b','o',0))chess_read_vbo(win, act, rel);

	//pixel
	else chess_read_pixel(win, act, rel);
}
static void chess_write(struct event* ev)
{
	//say("@chess:%x,%x\n", ev->why, ev->what);
}




static void chess_list()
{
}
static void chess_choose()
{
}
static void chess_start()
{
	int j,k;
	for(k=0;k<8;k++)
	{
		for(j=0;j<8;j++)
		{
			buffer[k][j] = 0;
		}
	}

	//
	for(j=0;j<8;j++)
	{
		buffer[1][j] = 'p';
		buffer[6][j] = 'P';
	}

	//
	buffer[0][0] = 'r';
	buffer[0][1] = 'n';
	buffer[0][2] = 'b';
	buffer[0][3] = 'k';
	buffer[0][4] = 'q';
	buffer[0][5] = 'b';
	buffer[0][6] = 'n';
	buffer[0][7] = 'r';

	//
	buffer[7][0] = 'R';
	buffer[7][1] = 'N';
	buffer[7][2] = 'B';
	buffer[7][3] = 'Q';
	buffer[7][4] = 'K';
	buffer[7][5] = 'B';
	buffer[7][6] = 'N';
	buffer[7][7] = 'R';
}
static void chess_stop()
{
}
void chess_create(void* base, void* addr)
{
	struct actor* act = addr;

	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex64('c', 'h', 'e', 's', 's', 0, 0, 0);
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)chess_start;
	act->stop = (void*)chess_stop;
	act->list = (void*)chess_list;
	act->choose = (void*)chess_choose;
	act->read = (void*)chess_read;
	act->write = (void*)chess_write;
}
void chess_delete()
{
}
