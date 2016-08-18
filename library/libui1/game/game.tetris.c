#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//
void rect(int x1, int y1, int x2, int y2, DWORD bodycolor, DWORD framecolor);
void printdecimal(int data,int xyz,DWORD fg,DWORD bg);
void backgroundcolor(DWORD);
//
unsigned int getrandom();
void printmemory(char*,int);
void say(char*,...);




//
static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
}*haha;
//
typedef struct stucture
{
	int x;
	int y;
	int type;
	int direction;
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
	int x4;
	int y4;
}structure;
static structure that;
//
static int score=0;
static unsigned char* table;




static void cubie(int x,int y,int z)
{
	int aa;
	int startx,starty,endx,endy;
	DWORD bodycolor;

	aa=(haha->height*32)/40;

	if(haha->width<aa)
	{
		startx = (haha->width)*x/32;
		endx = (x+1)*(haha->width)/32 -1;
	}
	else
	{
		startx = (haha->width-aa)/2 + (x*aa/32);
		endx = (haha->width-aa)/2 + ((x+1)*aa/32) -1;
	}

	starty = (haha->height)*y/40;
	endy = (y+1)*(haha->height)/40 - 1;

	bodycolor=z>0?0xffffffff:0;

	rect(startx, starty, endx, endy, bodycolor, 0x44444444);
}
static void tetris_read_text()
{
	int x,y;
	int width=haha->width;
	int height=haha->height;
	char* p = (char*)(haha->pixelbuffer);

	for(x=0;x<width*height;x++)p[x]=0x20;
	if(height>=40)
	{
		for(y=0;y<40;y++)
		{
			for(x=0;x<32;x++)
			{
				if(table[y*32+x])
				{
					p[y*width+x]='#';
				}
			}
		}
		p[that.x1 + that.y1*width]='#';
		p[that.x2 + that.y2*width]='#';
		p[that.x3 + that.y3*width]='#';
		p[that.x4 + that.y4*width]='#';
	}
	else
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<32;x++)
			{
				if(table[32*(y+40-height) + x])
				{
					p[y*width+x]='#';
				}
			}
		}
		p[that.x1 + (that.y1-40+height)*width]='#';
		p[that.x2 + (that.y2-40+height)*width]='#';
		p[that.x3 + (that.y3-40+height)*width]='#';
		p[that.x4 + (that.y4-40+height)*width]='#';
	}
}
static void tetris_read_html()
{
}
static void tetris_read_pixel()
{
	int x,y;
	for(y=0;y<40;y++)
	{
		for(x=0;x<32;x++)
		{
			//say("%d ",table[y*32+x]);
			cubie(x,y,table[y*32+x]);
		}
		//say("\n");
	}
	//say("\n");

	//print cubies
	cubie(that.x1,that.y1,1);
	cubie(that.x2,that.y2,1);
	cubie(that.x3,that.y3,1);
	cubie(that.x4,that.y4,1);

	//print score
	//decimal(10,10,score);
}
static void tetris_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		tetris_read_text();
	}
/*
	//html
	else if()
	{
	}
*/

	//pixel
	else
	{
		tetris_read_pixel();
	}
}






static void generate()
{
	if(that.type==0)
	{
		//	0000
		if(that.direction==1 | that.direction==3)
		{
			that.x1=that.x;
			that.x2=that.x +1;
			that.x3=that.x +2;
			that.x4=that.x +3;
			that.y1=that.y2=that.y3=that.y4=that.y;
		}

		//	0
		//	0
		//	0
		//	0
		if(that.direction==0 | that.direction==2)
		{
			that.x1=that.x2=that.x3=that.x4=that.x;
			that.y1=that.y;
			that.y2=that.y +1;
			that.y3=that.y +2;
			that.y4=that.y +3;
		}
	}
	if(that.type==1)
	{
		//	000
		//	 0
		if(that.direction==0)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x;
			that.y4=that.y +1;
		}

		//	0
		//	00
		//	0
		if(that.direction==1)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y;
		}

		//	 0
		//	000
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x;
			that.y4=that.y -1;
		}

		//	 0
		//	00
		//	 0
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x;
			that.y4=that.y -1;
		}
	}
	if(that.type==2)
	{
		//	0
		//	00
		//	 0
		if(that.direction==0|that.direction==2)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x -1;
			that.y3=that.y -1;
			that.x4=that.x;
			that.y4=that.y+1;
		}

		//	 00
		//	00
		if(that.direction==1|that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}
	}
	if(that.type==3)
	{
		//	 0
		//	00
		//	0
		if(that.direction==0|that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}

		//	00
		//	 00
		if(that.direction==1|that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y -1;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}
	}
	if(that.type==4)
	{
		//	00
		//	0
		//	0
		if(that.direction==0)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}

		//	0
		//	000
		if(that.direction==1)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}

		//	 0
		//	 0
		//	00
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}

		//	000
		//	  0
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x +1;
			that.y4=that.y +1;
		}
	}
	if(that.type==5)
	{
		//	00
		//	 0
		//	 0
		if(that.direction==0)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}

		//	000
		//	0
		if(that.direction==1)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}

		//	0
		//	0
		//	00
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y +1;
		}

		//	  0
		//	000
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}
	}

	//	00
	//	00
	if(that.type==6)
	{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y +1;
	}
}


static int check()
{
	unsigned int temp;

	//left
	if(that.x1<0)return 1;
	if(that.x2<0)return 2;
	if(that.x3<0)return 3;
	if(that.x4<0)return 4;

	//right
	if(that.x1>31)return 5;
	if(that.x2>31)return 6;
	if(that.x3>31)return 7;
	if(that.x4>31)return 8;

	//collision
	if(table[32 * that.y1 + that.x1] != 0)return 9;
	if(table[32 * that.y2 + that.x2] != 0)return 10;
	if(table[32 * that.y3 + that.x3] != 0)return 11;
	if(table[32 * that.y4 + that.x4] != 0)return 12;

	//success
	return 0;
}


static void left()
{
	if(that.x1>0&&that.x2>0&&that.x3>0&&that.x4>0)
	{
		that.x --;
		that.x1 --;
		that.x2 --;
		that.x3 --;
		that.x4 --;
		if(check() != 0)
		{
			that.x ++;
			that.x1 ++;
			that.x2 ++;
			that.x3 ++;
			that.x4 ++;
		}
	}
}
static void right()
{
	if(that.x1<31&&that.x2<31&&that.x3<31&&that.x4<31)
	{
		that.x ++;
		that.x1 ++;
		that.x2 ++;
		that.x3 ++;
		that.x4 ++;
		if(check() != 0)
		{
			that.x --;
			that.x1 --;
			that.x2 --;
			that.x3 --;
			that.x4 --;
		}
	}
}
static void up()
{
	that.direction=(that.direction +1)%4;
	generate();
	if(check() != 0)
	{
		that.direction=(that.direction+3)%4;
	}
	generate();
}
static int down()
{
	int x,y,count;

	that.y ++;
	that.y1 ++;
	that.y2 ++;
	that.y3 ++;
	that.y4 ++;
	if(check() < 9)return 0;

	that.y --;
	that.y1 --;
	that.y2 --;
	that.y3 --;
	that.y4 --;

	table[32 * that.y1 + that.x1]=1;
	table[32 * that.y2 + that.x2]=1;
	table[32 * that.y3 + that.x3]=1;
	table[32 * that.y4 + that.x4]=1;

	y=39;
	while(1)
	{
		count=0;
		for(x=0;x<32;x++)
		{
			if(table[32*y+x]>0)count++;
		}

		if(count==32)
		{
			//
			for(x=y*32+31;x>32;x--)
			{
				table[x]=table[x-32];
			}
		}

		else
		{
			y--;
			if(y==0)break;
		}
	}

	//
	that.x=getrandom() %27+1;
	that.y=1;
	that.type=getrandom() % 7;
	that.direction=getrandom() & 0x3;
	generate();
	return 1;
}
static void tetris_write(QWORD type,QWORD key)
{
	int ret;
	if(type==0x7466656C207A7978)
	{
		down();
	}
	if(type==0x64626b)
	{
		if(key=='a'|key==0x25)
		{
			left();
		}
		else if(key=='d'|key==0x27)
		{
			right();
		}
		else if(key=='w' | key==' '|key==0x26)
		{
			up();
		}
		else if(key=='s'|key==0x28)
		{
			if(down()==1)return;
			if(down()==1)return;
			if(down()==1)return;
			if(down()==1)return;
			if(down()==1)return;
			if(down()==1)return;
			if(down()==1)return;
			if(down()==1)return;
		}
	}

	else return;
}




static void tetris_list()
{
}
static void tetris_choose()
{
}





static void tetris_start()
{
	//
	int x;
	backgroundcolor(0);

	//game data
	for(x= 0*32;x<40*32;x++) table[x]=0;
	for(x=40*32;x<41*32;x++) table[x]=1;
	that.x=getrandom() %27 +1;
	that.y=1;
	that.type=5;
	that.direction=2;
	generate();
}
static void tetris_stop()
{
}
void tetris_create(char* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;		//'window'
	this[1]=0x736972746574;		//'tetris'

	this[10]=(QWORD)tetris_start;
	this[11]=(QWORD)tetris_stop;
	this[12]=(QWORD)tetris_list;
	this[13]=(QWORD)tetris_choose;
	this[14]=(QWORD)tetris_read;
	this[15]=(QWORD)tetris_write;

	table=(unsigned char*)(addr+0x300000);
}
void tetris_delete()
{
}
