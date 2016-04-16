#define QWORD unsigned long long
#define DWORD unsigned int
//
void background_start(DWORD size,void* addr);
void shape_start(DWORD size,void* addr);
void ascii_start(DWORD size,void* addr);
void unicode_start(DWORD size,void* addr);
//
void rectangle(QWORD leftup,QWORD rightdown,DWORD color);
void colordecimal(int x,int y,int z,unsigned int color);
void backgroundcolor(DWORD);
//
void writewindow(QWORD,QWORD);
unsigned int getrandom();
void printmemory(char*,int);
void say(char*,...);




static int table[4][4];
static int width=0;
static int height=0;




static void cubie(int x,int y,int z)
{
	int color;
	switch(z)
	{
		case 0:color=0x55555555;break;
		case 2:color=0xfffffff0;break;
		case 4:color=0xffffffc0;break;
		case 8:color=0x995000;break;
		case 16:color=0xc05000;break;
		case 32:color=0xb03000;break;
		case 64:color=0xff0000;break;
		case 128:color=0xffffa0;break;
		case 256:color=0xffff80;break;
		case 512:color=0xffff00;break;
		case 1024:color=0xffffb0;break;
		case 2048:color=0xffffffff;break;
		case 4096:color=0xffffffff;break;
	}
	//for(i=x*160+5;i<x*160+155;i++)
	//for(j=y*160+5;j<y*160+155;j++)
	//point(i,j,color);
	rectangle(
		( x*width/4+4 ) + ( (y*height/4+4) << 16 ),
		( (x+1)*width/4-4 ) + ( ((y+1)*height/4-4) << 16 ),
		color
	);
	if(z!=0) colordecimal(4+x*(width/32),4+y*(height/64),z,0);
}




//斗兽场......
static void left2048()
{
	int x,y;
	int dst,temp;

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
			}
		}
	}
}








static void the2048_write(QWORD type,QWORD key)
{
	//kbd
	if(type != 0x64626b)return;

	//left,right,up,down
	if(key == 0x25)left2048();
	else if(key == 0x27)right2048();
	else if(key == 0x26)up2048();
	else if(key == 0x28)down2048();
	else return;

	//new number?
	new2048();
}
static void the2048_read()
{
	int x,y;

	say("2048{\n");
	for(y=0;y<4;y++)
	{
say("	%d	%d	%d	%d\n",table[y][0],table[y][1],table[y][2],table[y][3]);
		for(x=0;x<4;x++)
		{
			cubie(x,y,table[y][x]);
		}
	}
	say("}\n");
}
static void the2048_choose()
{
}
static void the2048_list()
{
}








static void the2048_start(DWORD size,void* addr)
{
	//1.init
	int x,y;

	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//2或4
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			table[y][x]=0;
		}
	}
	new2048();

	//
	//writewindow( 0x657a6973 , 512+(512<<16) );
}
static void the2048_stop()
{
}
void the2048_init(char* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;		//'window'
	this[1]=0x38343032;			//'2048'
	this[2]=(QWORD)the2048_start;
	this[3]=(QWORD)the2048_stop;
	this[4]=(QWORD)the2048_list;
	this[5]=(QWORD)the2048_choose;
	this[6]=(QWORD)the2048_read;
	this[7]=(QWORD)the2048_write;
}
void the2048_kill()
{
}
