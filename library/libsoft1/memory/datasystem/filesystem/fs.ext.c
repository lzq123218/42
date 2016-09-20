#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int systemread( char* rdi,u64 rsi,u64 rcx);
int systemwrite(char* rdi,u64 rsi,u64 rcx);
int cleverread(u64,u64,u64,char*,u64,u64);
//用了别人的
void printmemory(char* addr,u64 size);
void say(char* fmt,...);




//memory
static char* fshome;
	static char* pbr;		//+0x10000
	static char* inodebuffer;	//+0x20000
static char* dirhome;
static char* datahome;

//disk
static u64 block0;
static u64 blocksize;
static u64 groupsize;
static u64 inodepergroup;
static u64 inodesize;




//输入值：请求的组号
//返回值：这个组里面inode表的第一个block号
static char blockrecord[512];
static u64 whichblock(u64 groupnum)
{
	//group descriptor从哪个扇区开始
	u64 sector;
	if(blocksize==2) sector=block0+4;
	else sector=block0+blocksize;

	//从这个扇区起往后多少扇区
	sector+=groupnum/(0x200/0x20);

	//肯定在这个扇区里面
	systemread(blockrecord,sector,1);

	//每0x20描述一个组，一个扇区有16个组的信息
	char* addr=blockrecord+8+(groupnum*0x20)%0x200;
	return *(u32*)addr;
}




//文件->一家人，inode->户口本里的记录，这个函数->管理员
//这个函数作用：每次指名要一个记录，管理员就从户口本里翻到那儿([0x400n+1号,0x400n+1号])再指给请求者看
//比如0x3352号:先读入[0x3001,0x3400]这1024个记录
//再比如0x182304号:读入[0x182001,182400]这1024个记录
//注意1:不管inodesize是120(0x400*120)还是128(0x400*128)还是256(0x400*256)，算出的值都能被0x200(一个扇区)整除
//注意2:每个组的inode数量一般为8192，是0x400的倍数
//注意3:inode首个的编号是1不是0
static u64 firstinodeincache;
static char* checkcacheforinode(u64 wanted)
{
	//内存里已经是这几个的话就直接返回
	u64 inodeoffset=(wanted-1)%0x400;
	if(wanted-inodeoffset == firstinodeincache)
	{
		return inodebuffer+inodeoffset*inodesize;
	}


	//不是就麻烦了
	char* rdi=inodebuffer;
	u64 this=wanted-inodeoffset;
	while(1)
	{
		//inode分散在各个group里面，可能需要从不同地方收集
		u64 groupnum=(this-1)/inodepergroup;		//算出此inode在第几块组
		u64 groupoffset=(this-1)%inodepergroup;	//在这一组内的偏移（几个）

		//算：哪个扇区,几个扇区
		u64 where;
		where=block0+blocksize*whichblock(groupnum);//这一组第一个inode所在扇区，不会出问题
		where+=groupoffset*inodesize/0x200;	//注意inodepergroup奇葩时这里出问题
		u64 count;
		count=inodepergroup-groupoffset;	//这一group内剩余多少个
		if(count>0x400)count=0x400;	//这一组里剩余的太多的话，多余的不要

		//read inode table
		//say("inode:%x@%x\n",this,where);
		//注意inodepergroup奇葩时这里出问题
		systemread(rdi,where,count*inodesize/0x200);

		//读满0x400个inode就走人
		rdi+=count*inodesize;		//注意inodepergroup奇葩时这里出问题
		this+=count;
		if(rdi-inodebuffer>=0x40000) break;
	}
	//mem2file(inodebuffer,"after.bin",0,0x40000);
	firstinodeincache=wanted-inodeoffset;
	return inodebuffer+inodeoffset*inodesize;
}




//输入值：要搞的inode号，这个inode对应的文件内部偏移
//返回值：<=0就是失败
//作用：从offset开始能读多少读多少(1M以内)
static int explaininode(u64 inode,u64 wantwhere)
{
	//函数调用之后,rsi为所请求的inode的内存地址，
	char* rsi=checkcacheforinode(inode);

	//检查是不是软链接
	u16 temp=(*(u16*)rsi)&0xf000;
	if(temp == 0xa000)
	{
		say("soft link:%x\n",rsi+0x28);
		return -1;
	}

	if(*(u16*)(rsi+0x28) == 0xf30a)	//ext4用新方式
	{
		//读extend头，拿点重要的变量
		//*(u16*)(rsi+0x28)		//这是个标志，等于0xf30a说明用extend方式
		int numbers=*(u16*)(rsi+0x28+2);		//有效项个数
			//*(u16*)(rsi+0x28+4);		//项中的存储容量，4
			//*(u16*)(rsi+0x28+6);		//树的深度
			//*(u32*)(rsi+0x28+8);	//树的代数

		//为循环准备变量，清空内存，给读取的数据空间
		int i;
		char* rdi;
		rsi=rsi+0x28;		//加完以后指向ext4_extend头
		for(rdi=datahome;rdi<datahome+0x100000;rdi++)
		{
			*(u8*)rdi=0;
		}
		for(i=0;i<numbers;i++)
		{
			//ext4_extend体结构如下:
			//[0,3]第一个逻辑块
			//[4,5]数据块个数
			//[6,7]数据块高16位
			//[8,b]数据块低32位
			//每个ext4_extend体，大小为12B
			rsi+=12;

			//逻辑上，这是文件的第几块
			u64 aaaaa=( *(u32*)rsi )*blocksize*0x200;		//逻辑上，这块的字节位置
			if(wantwhere+0x100000<=aaaaa)break;		//结束了

			//总共多少个扇区
			u64 count=( *(u16*)(rsi+4) )*blocksize;			//多少个扇区
			if(aaaaa+count*0x200<=wantwhere)continue;			//还没到

			//实际上，从第几个扇区开始
			u64 sector=*(u16*)(rsi+6);	//从extent体里得到高16位实际块号
			sector=(sector<<32) + ( *(u32*)(rsi+8) );	//加上低32位实际块号
			sector*=blocksize;	//乘以每块多少扇区，现在sector=分区内偏移多少个扇区
			sector+=block0;		//加上分区相对硬盘开始多少个扇区

			//蛋碎了，拼回来。。。传进去的参数为：
			//前三个；这一块的物理扇区号，扇区数，逻辑位置
			//后三个：目标位置，目标大小，需求位置
			cleverread
			(
				sector,count,aaaaa,
				datahome,0x100000,wantwhere
			);
		}

		return 1;
	}
	else		//ext2，3用老的直接间接块的方式
	{
	say("old@%x\n",rsi);
/*
	rsi+=0x28;
	for(i=0;i<8;i++)
	{
		if(*(u32*)rsi != 0)
		{
			u64 temp;
			say("    pointer@%x\n",rsi);

			temp=block0+(*(u32*)rsi)*blocksize;
			say("sector:%x\n",temp);

		        systemread(rdi,temp,blocksize);
			rdi+=0x200*blocksize;
		}

		rsi+=4;
	}
*/
	return 2;
	}
}


static void explaindirectory()
{
	int i;
	char* rdi=dirhome;
	char* rsi=datahome;

	for(i=0;i<0x100000;i++)
	{
		*(u8*)(rdi+i)=0;
	}
	while(1)
	{
		//几种情况结束的
		if(rdi>=dirhome+0x100000)break;
		if(rsi>=datahome+0x100000)break;
		if(*(u32*)rsi == 0)break;
		if(*(u16*)(rsi+4) == 0)break;
		//say("%x\n",*(u16*)(rsi+4));
		//printmemory(rsi,0x10);

		//[0,0x7]:type
		u8 type=*(u8*)(rsi+7);
		if(type==1)
		{
			*(u64*)rdi=0x656c6966;	//'file'
		}
		else if(type==2)
		{
			*(u64*)rdi=0x726964;		//'dir'
		}
		else if(type==7)
		{
			*(u64*)rdi=0x6b6e696c;	//'link'
		}
		else
		{
			*(u64*)rdi=0x66+(type<<8);
		}

		//[0x10,0x17]inode号
		u64 thisinode=*(u32*)rsi;
		*(u64*)(rdi+0x10)=thisinode;

		//[0x18,0x1f]:size，ext的目录表里面没有文件大小，需要到inode表里面寻找
		char* inodeaddr=checkcacheforinode(thisinode);
		*(u64*)(rdi+0x18)=*(u32*)(inodeaddr+4);

		//[0x20,0x3f]:名字
		i = *(u8*)(rsi+6);
		if(i>0x1f)
		{
			*(u8*)(rdi+0x3f)=0;
			*(u8*)(rdi+0x3e)='.';
			*(u8*)(rdi+0x3d)='.';
			*(u8*)(rdi+0x3c)='.';
			i=0x1b;
		}
		else
		{
			*(u8*)(rdi+0x20+i)=0;
			i--;
		}

		for(;i>=0;i--)
		{
			*(u8*)(rdi+0x20+i)=*(u8*)(rsi+8+i);
		}

		//最后指向下一个
		rsi+=*(u16*)(rsi+4);
		rdi+=0x40;
	}
}








static int ext_ls(char* to)
{
	return 1;
}
static int ext_cd(u64 id)
{
	//开搞
	int ret;
	for(ret=0;ret<0x10000;ret++)dirhome[ret]=0;

	ret=explaininode(id,0);
	if(ret>0)explaindirectory();
	return 1;
}
static int ext_load(u64 id,u64 offset,u64 size)
{
	explaininode(id,offset);
	return 1;
}
static int ext_store(u64 id,u64 offset,u64 size)
{
	return 1;
}
int explainexthead()
{
	u64* dstqword=(u64*)fshome;

	//func ls,cd,load,store
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0;
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=(u64)ext_ls;
	dstqword[5]=(u64)ext_cd;
	dstqword[6]=(u64)ext_load;
	dstqword[7]=(u64)ext_store;
	dstqword += 8;

	//变量们
	blocksize=*(u32*)(pbr+0x418);
	blocksize=( 1<<(blocksize+10) )/0x200;		//每块多少扇区
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x7a736b636f6c62;       //'blocksz'
	dstqword[2]=0x418;
	dstqword[3]=0x41b;
	dstqword[4]=blocksize;
	dstqword += 8;
	say("sectorperblock:%x\n",blocksize);

	//每组多少扇区
	groupsize=*(u32*)(pbr+0x420);
	groupsize=groupsize*blocksize;
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x7a7370756f7267;       //'groupsz'
	dstqword[2]=0x420;
	dstqword[3]=0x423;
	dstqword[4]=groupsize;
	dstqword += 8;
	say("sectorpergroup:%x\n",groupsize);

	//每组多少个inode
	inodepergroup=*(u32*)(pbr+0x428);
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x672f69;       //'i/g'
	dstqword[2]=0x428;
	dstqword[3]=0x42b;
	dstqword[4]=inodepergroup;
	dstqword += 8;
	say("inodepergroup:%x\n",inodepergroup);

	//每inode多少字节
	inodesize=*(u16*)(pbr+0x458);
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x7a7365646f6e69;       //'inodesz'
	dstqword[2]=0x458;
	dstqword[3]=0x459;
	dstqword[4]=inodesize;
	dstqword += 8;
	say("byteperinode:%x\n",inodesize);

	return 1;
}
int isext(char* addr)
{
	//0x53,0xef
	u64 temp=*(u16*)(addr+0x438);
	if( temp != 0xef53 ) return 0;

	//maybe isext
	return 4;
}
int mountext(u64 sector,char* addr)
{
	int ret=0;

	//得到本分区的开始扇区位置，再得到3个buffer的位置
	block0=sector;
	fshome=addr+0;
		pbr=fshome+0x10000;
		inodebuffer=fshome+0x20000;
	dirhome=addr+0x100000;
	datahome=addr+0x200000;

	//读分区前8扇区，检查magic值
	ret=systemread(pbr,block0,0x8);	//0x1000
	ret=isext(pbr);
	if( ret == 0 ) return -1;

	//读出关键数据
	ret=explainexthead();
	if(ret<0)return ret;

	//cd /
	firstinodeincache=0xffffffff;
	ext_cd(2);

	return 0;
}