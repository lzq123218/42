﻿#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//3张表的位置
static struct diskinfo* diskinfo;
static struct mytable* mytable;
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）
//键盘输入
static BYTE buffer[128];//键盘输入专用
static bufcount=0;
//log位置
static BYTE* logbuf;
static QWORD logoffset;
//标签
static int tag=0;




void printlog()
{
	int x,y;
	for(x=0;x<1024;x++)
	{
		for(y=0;y<640-64;y++)
		{
			point(x,y,0x88888888);
		}
	}
	//出
	for(y=0;y<36;y++)
	{
		//*(QWORD*)(logbuf+0x80*y)=0x0030313233343536;
		string(0,y,logbuf+logoffset+0x80*y);
	}
	//入
	string(0,36,buffer);
}
void printdisk()
{
	int x,y;
	for(x=0;x<1024;x++)
	{
		for(y=0;y<640-64;y++)
		{
			point(x,y,0x88888888);
		}
	}

	char* p=(char*)diskinfo;
	for(y=0;y<3;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,y,p[0x40*y+x]);
		}
	}
}
void printbitmap(QWORD start,QWORD end,QWORD typestr)
{
	//
	QWORD starty=start/1024*16;
	QWORD startx=start%1024;
	QWORD endy=starty+0xf;
	QWORD endx;
	QWORD color=start*0xfedcba/1024/36;
	QWORD nextline=(start/1024)*1024+1024;
	QWORD this=start;
	QWORD isfirst=1;
	int x,y;

	//具体内容
	while(1)		//一行行得
	{
		//得到这一轮的开始与结束
		starty=this/1024*16;
		startx=this%1024;
		endy=starty+0x10;
		if(end<nextline)
		{
			//这行就结束的话
			endx=end%1024;
		}
		else
		{
			endx=1023;
		}
		say("(%x,%x)->(%x,%x)\n",startx,starty,endx,endy);

		//按照得到的坐标打印就行
		for(y=starty;y<endy;y++)			//左右边界线
		{
			point(startx,y,0xff0000);
			point(endx,y,0xff0000);
		}
		for(x=startx+1;x<endx;x++)		//体
		{
			for(y=starty;y<endy;y++)
			{
				point(x,y,color);
			}
		}
		if(isfirst)			//顶部边界线
		{
			for(x=startx;x<endx;x++)
			{
				point(x,starty,0xff0000);
			}
			isfirst=0;
		}

		//下一个start，或者已经结束了就break
		this=nextline;
		if(this>end)break;
		nextline+=1024;
		if(nextline>1024*36)break;
	}

	//框框
	for(x=startx+1;x<endx;x++)		//下部边界线
	{
		point(x,endy-1,0xff0000);
	}
	for(x=endx;x<1024;x++)
	{
		point(x,endy-16,0xff0000);
	}

	//字符串
	//say("start:%x,end:%x,average:%x\n",start,end,(start+end)/2);
	x=((start+end)/2)%1024/8;
	y=(start+end)/2/1024;
	string(x,y,(char*)typestr);
}
void printpartition()
{
	char* p;
	QWORD x,y;
	for(x=0;x<1024;x++)
	{
		for(y=0;y<640-64;y++)
		{
			point(x,y,0x88888888);
		}
	}

//二.仔细看每一个磁盘由很多分区构成
	//1.最大扇区号是几
	p=(char*)mytable;
	QWORD temp;

	QWORD maxsector=0;
	for(x=0;x<16;x++)
	{
		temp=*(QWORD*)(p+x*0x40+8);
		if(temp == 0)break;
		if(temp>maxsector)maxsector=temp;
	}
	say("%x\n",maxsector);
	QWORD displaymax=1;
	for(x=0;x<64;x++)
	{
		//say("%x\n",maxsector);
		if(maxsector <= 0)break;
		else
		{
			maxsector>>=1;
			displaymax<<=1;
		}
	}
	say("%x\n",displaymax);

	//3.标记头尾
	p=(char*)mytable;
	for(y=0;y<8;y++)
	{
		//算出开始扇区结束扇区
		QWORD startsec=*(QWORD*)(p+0x40*y);
		if(startsec==0)break;
		QWORD endsec=*(QWORD*)(p+0x40*y+8);
		QWORD typestr=(QWORD)&p[0x40*y+0x10];

		//
		QWORD start=((double)(startsec*1024*36)/(double)displaymax);
		//hexadecimal(start/8,20,startsec);
		QWORD end=((double)(endsec*1024*36)/(double)displaymax);
		//hexadecimal(end/8,21,endsec);

		say("[%llx,%llx],%s,[%x,%x]\n",startsec,endsec,typestr,start,end);

		//打印硬盘的大致图像
		printbitmap(start,end,typestr);
	}

}
void printfile()
{
	char* p;
	QWORD x,y;
	for(x=0;x<1024;x++)
	{
		for(y=0;y<640-64;y++)
		{
			point(x,y,0x88888888);
		}
	}

//三.每个分区里面的文件和文件夹
	p=(char*)dir;
	for(y=0;y<2;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,10+y,p[0x40*y+x]);
		}
	}
}
void tagcontect()
{
	//画分隔线，写标签名
	int i,j;
	for(j=640-32;j<640;j++)
		for(i=0;i<1024;i++)
			point(i,j,0xffffffff);

	//清屏
	DWORD color;
	//color=0xff<<(tag*3);
	color=0x44444444;
	for(j=640-64;j<640-32;j++)
		for(i=0;i<1024;i++)
			point(i,j,color);
	for(j=640-32;j<640;j++)
		for(i=tag*160;i<tag*160+160;i++)
			point(i,j,color);

	//名称
	string(0,39,"journal");
	string(20,39,"disk");
	string(40,39,"partition");
	string(60,39,"file");
}
void printworld()
{
	//基本内容
	tagcontect();

	//具体内容
	if(tag==0) printlog();
	if(tag==1) printdisk();
	if(tag==2) printpartition();
	if(tag==3) printfile();

	//写屏
	writescreen();
}
void main()
{
	initmaster();

	whereisdiskinfo(&diskinfo);
	whereisparttable(&mytable);
	whereisdir(&dir);
	whereislogbuf(&logbuf);

	//
	disk(1,1);
	mount(0,0);

	while(1)
	{
		//1.这次显示啥
		printworld();

		//2.等事件
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		//say("%x\n",key);

		//3.干啥事
		switch(type)
		{
			case 0:return;
			case 1:
			{
				if(key==0x1b)return;
				else if(key==0x8)
				{
					if(bufcount!=0)
					{
						bufcount--;
						buffer[bufcount]=0;
					}
				}
				else if(key==0x40000050)	//left	0x4b
				{
					if(tag>0)tag--;
				}
				else if(key==0x4000004f)	//right	0x4d
				{
					if(tag<3)tag++;
				}
				else if(key==0x40000052)	//up	0x48
				{
					if(logoffset>0x80)	logoffset-=0x80;
				}
				else if(key==0x40000051)	//down	0x50
				{
					logoffset=(logoffset+0x80)%0x100000;
				}
				else
				{
					buffer[bufcount]=key&0xff;
					bufcount++;
				}
				break;
			}
			case 3:
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;
				say("(%d,%d)\n",x,y);

				//
				if( (y>640-32) && (x<640) )
				{
					tag=x/160;
				}
				break;
			}
		}
	}//while(1)
}
