#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void printascii(int x, int y, int size, u8 data, u32 fg, u32 bg);
void circlebody(int x, int y, int r, u32 color);
void circleframe(int x, int y, int r, u32 color);
void line(int x1,int y1,int x2,int y2, u32 color);
void backgroundcolor(u32);
u32 getrandom();
//
int diary(char*,int,char*,...);
void say(char*,...);




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
}*haha;

//
static char data[10][9];
static int turn;
static int px,py;




void xiangqi_read_pixel()
{
	int x,y;
	int cx,cy,half;
	u32 black, brown, red;
	u32 chesscolor, fontcolor;

	//
	cx = (haha->width)/2;
	cy = (haha->height)/2;
	if(cy*9 > cx*10)half = cx/9;
	else half = cy/10;

	//
	black=0;
	if( ((haha->pixelformat)&0xffffffff) == 0x61626772)
        {
		backgroundcolor(0x256f8d);
		red = 0xff;
		brown = 0x36878d;
        }
	else
	{
		backgroundcolor(0x8d6f25);
		red = 0xff0000;
		brown = 0x8d8736;
	}

	//heng
	for(y=-5;y<5;y++)
	{
		line(	cx - half*8,	cy + half*(2*y+1),
			cx + half*8,	cy + half*(2*y+1),	0);
	}

	//shu
	for(x=-4;x<5;x++)
	{
		line(	cx + x*half*2,	cy - half*9,
			cx + x*half*2,	cy - half*1,	0);
		line(	cx + x*half*2,	cy + half*9,
			cx + x*half*2,	cy + half*1,	0);
	}

	//pie,na
	line(	cx - half*2,	cy - half*9,
		cx + half*2,	cy - half*5,	0);
	line(	cx + half*2,	cy - half*9,
		cx - half*2,	cy - half*5,	0);
	line(	cx - half*2,	cy + half*9,
		cx + half*2,	cy + half*5,	0);
	line(	cx + half*2,	cy + half*9,
		cx - half*2,	cy + half*5,	0);
	//chess
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			//empty
			if(data[y][x] < 'A')continue;

			//>0x41
			else if(data[y][x] <= 'Z')
			{
				fontcolor = black;
			}

			//>0x61
			else if(data[y][x] <= 'z')
			{
				fontcolor = red;
			}

			if( (px == x)&&(py == y) )chesscolor = 0xabcdef;
			else chesscolor = brown;

			circlebody(
				cx + (2*x-8)*half,
				cy + (2*y-9)*half,
				half,
				chesscolor
			);

			printascii(
				cx + (2*x-8)*half - (half/8*8/2),
				cy + (2*y-9)*half - (half/8*16/2),
				half/8,
				data[y][x],
				fontcolor,
				0
			);
		}//forx
	}//fory
}
static int htmlcircle(char* p, int x, int y)
{
	u32 textcolor;
	char* hanzi;
	if(data[y][x] >= 'a')textcolor=0;
	else textcolor=0xff0000;

	switch(data[y][x])
	{
		case 'a':hanzi="车";break;
		case 'b':hanzi="马";break;
		case 'c':hanzi="象";break;
		case 'd':hanzi="士";break;
		case 'e':hanzi="将";break;
		case 's':hanzi="兵";break;
		case 'z':hanzi="炮";break;

		case 'A':hanzi="车";break;
		case 'B':hanzi="马";break;
		case 'C':hanzi="相";break;
		case 'D':hanzi="仕";break;
		case 'E':hanzi="帅";break;
		case 'S':hanzi="兵";break;
		case 'Z':hanzi="炮";break;

		default:return 0;
	}
	return diary(
		p, 0x1000,
		"<div class=\"circle\" style=\""
		"left:%d%;"
		"top:%d%;"
		"background:#ffff00;"
		"color:#%06x;"
		"\"><br>%s</div>",
		x*11, y*10,
		textcolor, hanzi
	);
}
static void xiangqi_read_html()
{
	int x,y;
	char* p = (char*)(haha->pixelbuffer) + 0x1000;

	p += diary(
		p, 0x1000,
		"<style type=\"text/css\">"
		".circle{"
		"position:absolute;"
		"border-radius:50%;"
		"width:10%;"
		"height:10%;"
		"text-align:center;"
		"}"
		"</style>"
	);
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			p += htmlcircle(p, x, y);
		}//forx
	}//fory
}
static void xiangqi_read_text()
{
}
static void xiangqi_read()
{
	u32 temp = (haha->pixelformat)&0xffffffff;
	//say("(@2048.read)temp=%x\n",temp);

	//text
	if(temp == 0x74786574)
	{
		xiangqi_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		xiangqi_read_html();
	}

	//pixel
	else
	{
		xiangqi_read_pixel();
	}
}




void xiangqi_write(u64* who, u64* what, u64* key)
{
        int cx, cy, half;
	int x, y;
	int min, max, t, u;

        cx = (haha->width)/2;
        cy = (haha->height)/2;
        if(cy*9 > cx*10)half = cx/9;
        else half = cy/10;

	if(*what == 0x7466656C207A7978)
	{
		x = (*key) & 0xffff;
		y = ( (*key) >> 16 ) & 0xffff;
		//say("%d,%d -> ",x,y);

		x = (x - cx + (9*half) )/half/2;
		y = (y - cy + (10*half) )/half/2;
		//say("%d,%d\n",x,y);

		if(x < 0)return;
		if(x > 8)return;
		if(y < 0)return;
		if(y > 9)return;
		if( (x==px) && (y==py) )return;

		//chess choosing
		if( (px<0) | (py<0) | (data[py][px] == 0) )
		{
			if( (data[y][x]>='a') && (data[y][x]<='z') && ((turn&1) == 0) )
			{
				px = x;
				py = y;
			}
			if( (data[y][x]>='A') && (data[y][x]<='Z') && ((turn&1) == 1) )
			{
				px = x;
				py = y;
			}
			return;
		}

		//chess going
		if(data[py][px] == 'S')		//兵
		{
			if(
				( (x == px) && (y == py+1) ) |
				( (y>4) && (x == px-1) && (y == py) ) |
				( (y>4) && (x == px+1) && (y == py) )
			)
			{
				if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
		else if(data[py][px] == 's')	//兵
		{
			if(
				( (x == px) && (y == py-1) ) |
				( (y<5) && (x == px-1) && (y == py) ) |
				( (y<5) && (x == px+1) && (y == py) )
			)
			{
				if( (data[y][x] < 'a') | (data[y][x] > 'z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
		else if(data[py][px] == 'Z')	//炮
		{
			if(data[y][x] == 0)
			{
				//横移
				if(y == py)
				{
					if(x < px){min = x; max=px;}
					else {min=px; max=x;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if( data[y][t] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}

				//竖移
				if(x == px)
				{
					if(y < py){min = y; max=py;}
					else {min=py; max=y;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[t][x] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}

			//打隔子
			else if(data[y][x] >= 'a')
			{
				if(y == py)
				{
					if(x < px){min = x; max=px;}
					else {min=px; max=x;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[y][t] != 0)u++;
					}
					if(u == 1)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}

				if(x == px)
				{
					if(y < py){min = y; max=py;}
					else {min=py; max=y;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[t][x] != 0)u++;
					}
					if(u == 1)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'z')	//炮
		{
			if(data[y][x] == 0)
			{
				//横移
				if(y == py)
				{
					if(x < px){min = x; max=px;}
					else {min=px; max=x;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if( data[y][t] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}

				//竖移
				if(x == px)
				{
					if(y < py){min = y; max=py;}
					else {min=py; max=y;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[t][x] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}

			//打隔子
			else if( (data[y][x] >= 'A') && (data[y][x] <= 'Z') )
			{
				if(y == py)
				{
					if(x < px){min = x; max=px;}
					else {min=px; max=x;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[y][t] != 0)u++;
					}
					if(u == 1)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}

				if(x == px)
				{
					if(y < py){min = y; max=py;}
					else {min=py; max=y;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[t][x] != 0)u++;
					}
					if(u == 1)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'A')	//车
		{
			if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
			{
				//横移
				if(y == py)
				{
					if(x < px){min = x; max=px;}
					else {min=px; max=x;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if( data[y][t] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}

				//竖移
				if(x == px)
				{
					if(y < py){min = y; max=py;}
					else {min=py; max=y;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[t][x] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'a')	//车
		{
			if( (data[y][x] < 'a') | (data[y][x] > 'z') )
			{
				//横移
				if(y == py)
				{
					if(x < px){min = x; max=px;}
					else {min=px; max=x;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if( data[y][t] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}

				//竖移
				if(x == px)
				{
					if(y < py){min = y; max=py;}
					else {min=py; max=y;}

					u = 0;
					for(t=min+1;t<max;t++)
					{
						if(data[t][x] != 0)u++;
					}
					if(u == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'B')	//马
		{
			if(data[y][x] < 'A' | data[y][x] > 'Z')
			{
				if( (x == px-2) && (y == py-1) | (y == py+1) )
				{
					if(data[py][px-1] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
				else if( (x == px+2) && (y == py-1) | (y == py+1) )
				{
					if(data[py][px+1] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
				else if( (y == py-2) && ( (x == px-1) | (x == px+1) ) )
				{
					if(data[py-1][px] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
				else if( (y == py+2) && ( (x == px-1) | (x == px+1) ) )
				{
					if(data[py+1][px] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'b')	//马
		{
			if(data[y][x] < 'a' | data[y][x] > 'z')
			{
				if( (x == px-2) && (y == py-1) | (y == py+1) )
				{
					if(data[py][px-1] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
				else if( (x == px+2) && (y == py-1) | (y == py+1) )
				{
					if(data[py][px+1] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
				else if( (y == py-2) && ( (x == px-1) | (x == px+1) ) )
				{
					if(data[py-1][px] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
				else if( (y == py+2) && ( (x == px-1) | (x == px+1) ) )
				{
					if(data[py+1][px] == 0)
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'C')	//象
		{
			if( (y<5) && ( (data[y][x] < 'A') | (data[y][x] > 'Z') ) )
			{
				if(
				( (x == px-2) && (y == py-2) && (data[py-1][px-1]==0) )|
				( (x == px-2) && (y == py+2) && (data[py+1][px-1]==0) )|
				( (x == px+2) && (y == py-2) && (data[py-1][px+1]==0) )|
				( (x == px+2) && (y == py+2) && (data[py+1][px+1]==0) )
				)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
		else if(data[py][px] == 'c')	//象
		{
			if( (y>4) && ( (data[y][x] < 'a') | (data[y][x] > 'z') ) )
			{
				if(
				( (x == px-2) && (y == py-2) && (data[py-1][px-1]==0) )|
				( (x == px-2) && (y == py+2) && (data[py+1][px-1]==0) )|
				( (x == px+2) && (y == py-2) && (data[py-1][px+1]==0) )|
				( (x == px+2) && (y == py+2) && (data[py+1][px+1]==0) )
				)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
		else if(data[py][px] == 'D')	//士
		{
			if( (x>=3)&&(x<=5)&&(y>=0)&&(y<=2) )
			{
				if(
					( (x == px-1) && (y == py-1) ) |
					( (x == px-1) && (y == py+1) ) |
					( (x == px+1) && (y == py-1) ) |
					( (x == px+1) && (y == py+1) )
				)
				{
					if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'd')	//士
		{
			if( (x>=3)&&(x<=5)&&(y>=7)&&(y<=9) )
			{
				if(
					( (x == px+1) && (y == py-1) ) |
					( (x == px+1) && (y == py+1) ) |
					( (x == px-1) && (y == py-1) ) |
					( (x == px-1) && (y == py+1) )
				)
				{
					if( (data[y][x] < 'a') | (data[y][x] > 'z') )
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}
		}
		else if(data[py][px] == 'E')	//将
		{
			if( (x>=3)&&(x<=5)&&(y>=0)&&(y<=2) )
			{
				if(
					( (x == px) && (y == py-1) ) |
					( (x == px) && (y == py+1) ) |
					( (x == px-1) && (y == py) ) |
					( (x == px+1) && (y == py) )
				)
				{
					if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}

			else if( (x == px) && (data[y][x] == 'e') )
			{
				u=0;
				for(t=py+1;t<y;t++)
				{
					if(data[t][px] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
		else if(data[py][px] == 'e')	//将
		{
			if( (x>=3)&&(x<=5)&&(y>=7)&&(y<=9) )
			{
				if(
					( (x == px) && (y == py-1) ) |
					( (x == px) && (y == py+1) ) |
					( (x == px-1) && (y == py) ) |
					( (x == px+1) && (y == py) )
				)
				{
					if( (data[y][x] < 'a') | (data[y][x] > 'z') )
					{
						data[y][x] = data[py][px];
						data[py][px] = 0;
						turn++;
					}
				}
			}

			else if( (x == px) && (data[y][x] == 'E') )
			{
				u=0;
				for(t=y+1;t<py;t++)
				{
					if(data[t][px] != 0)u++;
				}
				if(u == 0)
				{
say("%d,%d\n",x,y);
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}

		px = py = -1;
	}
}




static void xiangqi_list()
{
}
static void xiangqi_choose()
{
}




static void xiangqi_start()
{
	int j;
	char* p=(char*)data;

	px = py = -1;
	turn = 0;
	for(j=0;j<90;j++)p[j]=0;




	for(j=0;j<=4;j++)
	{
		//(black)JU,MA,XIANG,SHI,JIANG
		data[0][ j ]='A' + j;
		data[0][8-j]='A' + j;
		
		//(red)ju,ma,xiang,shi,jiang
		data[9][ j ]='a' + j;
		data[9][8-j]='a' + j;
	}
	for(j=0;j<5;j++)
	{
		//(red)SOLDIER
		data[3][j*2]='S';

		//(black)soldier
		data[6][j*2]='s';
	}

	//(red)PAO
	data[2][1]='Z';
	data[2][7]='Z';

	//(black)pao
	data[7][1]='z';
	data[7][7]='z';
}
static void xiangqi_stop()
{
}




void xiangqi_create(char* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x656d6167;
	this[1] = 0x6971676e616978;

	this[10]=(u64)xiangqi_start;
	this[11]=(u64)xiangqi_stop;
	this[12]=(u64)xiangqi_list;
	this[13]=(u64)xiangqi_choose;
	this[14]=(u64)xiangqi_read;
	this[15]=(u64)xiangqi_write;
}
void xiangqi_delete()
{
}
