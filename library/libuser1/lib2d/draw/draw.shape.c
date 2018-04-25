#include "actor.h"




void draw8bit_rect(struct arena* win, u8 rgb, int x0, int y0, int x1, int y1)
{
	int x,y;
	u8* buf = win->buf;
	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			buf[y*320+x] = rgb;
		}
	}
}




void drawline(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1)
{
	int dx,dy,sx,sy,e1,e2;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	if(x0 < x1){dx = x1-x0;sx = 1;}
	else {dx = x0-x1;sx = -1;}
	if(y0 < y1){dy = y1-y0;sy = 1;}
	else {dy = y0-y1;sy = -1;}
	if(dx > dy){e1 = dx/2;}
	else {e1 = -dy/2;}

	rgb |= 0xff000000;
	while(1)
	{
		if((x0 == x1)&&(y0 == y1))break;

		if((x0 >= 0)&&(x0 < width)&&(y0 >= 0)&&(y0 < height))
		{buf[(y0*stride) + x0] = rgb;}

		e2 = e1;
		if(e2 >-dx){e1 -= dy;x0 += sx;}
		if(e2 < dy){e1 += dx;y0 += sy;}
	}
}
void drawbezier(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int xc, int yc)
{
	int x,y,t;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	for(t=0;t<1000;t++)
	{
		x = (1000-t)*(1000-t)*x1 + 2*t*(1000-t)*xc + t*t*x2;
		x /= 1000*1000;
		if(x<0|x>=width)continue;

		y = (1000-t)*(1000-t)*y1 + 2*t*(1000-t)*yc + t*t*y2;
		y /= 1000*1000;
		if(y<0|y>=height)continue;

		buf[y*stride + x] = rgb;
	}
}




void drawline_triangle(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawline(win, rgb, x1, y1, x2, y2);
	drawline(win, rgb, x1, y1, x3, y3);
	drawline(win, rgb, x2, y2, x3, y3);
}
void drawline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	int x,y,n;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	if(x1<x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	//say("(%x,%x),(%x,%x)\n",startx,starty,endx,endy);

	rgb |= 0xff000000;
	for(n=0;n<2;n++)
	{
		for(x=startx;x<endx;x++)
		{
			if(starty < 0)break;
			if(starty >= width)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((starty+n)*stride) + x] = rgb;
		}
		for(x=startx;x<endx;x++)
		{
			if(endy < n)break;
			if(endy > height-1)break;
			if(x > width-1)break;
			if(x < 0)x=0;
			buf[((endy-n)*stride) + x] = rgb;
		}
		for(y=starty;y<endy;y++)
		{
			if(startx < 0)break;
			if(startx >= width)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*stride) + startx+n] = rgb;
		}
		for(y=starty;y<endy;y++)
		{
			if(endx < n)break;
			if(endx > width-1)break;
			if(y > height-1)break;
			if(y < 0)y = 0;
			buf[(y*stride) + endx-n] = rgb;
		}
	}
}
void drawline_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius)
{
	int x, y, ret;
	int x1, y1, x2, y2;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	rgb |= 0xff000000;
	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		buf[ (y*stride) + x1 ] = rgb;
		buf[ (y*stride) + x2 ] = rgb;
	}
}
void drawline_oval(struct arena* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy)
{
}
void drawline_sector(struct arena* win, u32 rgb,
	int cx, int cy, int radius, int start, int end)
{
}




void drawsolid_triangle(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3)
{
}
void drawsolid_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	int x,y;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(startx < 0)startx = 0;
	if(endx >= width)endx = width-1;

	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	if(starty < 0)starty = 0;
	if(endy >= height)endy = height-1;

	rgb |= 0xff000000;
	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			buf[ (y*stride) + x ] = rgb;
		}
	}
}
void drawsolid_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius)
{
	int x, y, ret;
	int x1, y1, x2, y2;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	rgb |= 0xff000000;
	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		for(x=x1;x<=x2;x++)
		{
			buf[ (y*stride) + x ] = rgb;
		}
	}
}
void drawsolid_oval(struct arena* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy)
{
}
void drawsolid_sector(struct arena* win, u32 rgb,
	int cx, int cy, int radius, int start, int end)
{
}




void drawhyaline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2)
{
	u8 r,g,b,a;
	int x,y,z;
	int startx,endx,starty,endy;
	int width = win->width;
	int height = win->height;
	int stride = win->stride;
	u8* buf = (u8*)(win->buf);

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(startx < 0)startx = 0;
	if(endx >= width)endx = width-1;

	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	if(starty < 0)starty = 0;
	if(endy >= height)endy = height-1;

	a = (rgb>>24)&0xff;
	r = (rgb>>16)&0xff;
	g = (rgb>>8)&0xff;
	b = rgb&0xff;
	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			z = buf[4*(y*stride + x) + 0];
			buf[4*(y*stride + x) + 0] = (z*(0x100-a) + b*a)>>8;

			z = buf[4*(y*stride + x) + 1];
			buf[4*(y*stride + x) + 1] = (z*(0x100-a) + g*a)>>8;

			z = buf[4*(y*stride + x) + 2];
			buf[4*(y*stride + x) + 2] = (z*(0x100-a) + r*a)>>8;

			buf[4*(y*stride + x) + 3] = 0xff;
		}
	}
}




void drawarrorkey2d(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1, u8* buf, int t)
{
	u32 c;
	int cx = (x0+x1)/2;
	int cy = (y0+y1)/2;
	int r = (y1-y0)/2;
	drawsolid_circle(win, 0x404040, cx, cy, r);

	//x-: left
	if(buf[0]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx-r*2/3, cy, r/3);
	drawascii_fit(win, 0xffffff, cx-r, cy-r/3, cx-r/3, cy+r/3, buf[0]&0x7f);

	//x+: right
	if(buf[1]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx+r*2/3, cy, r/3);
	drawascii_fit(win, 0xffffff, cx+r/3, cy-r/3, cx+r, cy+r/3, buf[1]&0x7f);

	//y-: near
	if(buf[2]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx, cy+r*2/3, r/3);
	drawascii_fit(win, 0xffffff, cx-r/3, cy+r/3, cx+r/3, cy+r, buf[2]&0x7f);

	//y+: far
	if(buf[3]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx, cy-r*2/3, r/3);
	drawascii_fit(win, 0xffffff, cx-r/3, cy-r, cx+r/3, cy-r/3, buf[3]&0x7f);

	//z-: trigger
	if(buf[4]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx-r*2/3, cy-r*4/3, r/3);
	drawascii_fit(win, 0xffffff, cx-r, cy-r*5/3, cx-r/3, cy-r, buf[4]&0x7f);

	//z+: bumper
	if(buf[5]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx+r*2/3, cy-r*4/3, r/3);
	drawascii_fit(win, 0xffffff, cx+r/3, cy-r*5/3, cx+r, cy-r, buf[5]&0x7f);

	//press
	if(buf[6]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx, cy, r/3);
	drawascii_fit(win, 0xffffff, cx-r/3, cy-r/3, cx+r/3, cy+r/3, buf[6]&0x7f);

	//select or start
	if(buf[7]&0x80)c = 0xff00ff;
	else c = 0x808080;
	drawsolid_circle(win, c, cx+r*t*4/3, cy-r*2/3, r/3);
	drawascii_fit(win, 0xffffff, cx+r*t*4/3-r/3, cy-r, cx+r*t*4/3+r/3, cy-r/3, buf[7]&0x7f);
}