#include "libsoft.h"
#define _mbr_ hex32('m','b','r',0)
#define _ext_ hex32('e','x','t',0)
#define _fat_ hex32('f','a','t',0)
#define _hfs_ hex32('h','f','s',0)
#define _ntfs_ hex32('n','t','f','s')




int check_mbr(u8* addr)
{
	//第一个扇区末尾必须有0x55，0xaa这个标志
	u64 temp=*(u16*)(addr+0x1fe);
	if(temp != 0xaa55 ) return 0;

	//但是mbr没有特别的标志，只能勉强用55aa确定
	//所以只能放在check type的最后
	return _mbr_;
}




//
static int mbrrecord(u8* from, u8* dst)
{
	u8 H,L;
	u32 flag;		//(mbr+0x1be)+0
	u32 type;		//(mbr+0x1be)+4
	u32 start;		//(mbr+0x1be)+8
	u32 size;		//(mbr+0x1be)+c
	u64* dstqword;

	//
	type = *(u8*)(from+4);
	if(type == 0)return 0;

	start = *(u32*)(from+8);
	size = *(u32*)(from+0xc);

	//类型，子类型，开始，结束
	dstqword = (u64*)dst;
	dstqword[0] = start;		//start
	dstqword[1] = start + size - 1;	//end

	//拓展分区要递归
	if( (type == 0x5) | (type == 0xf) )
	{
		say("extend@start\n");
		dstqword[1] = '~';
		return 0;
	}

	//其他普通分区
	if(	(0x4 == type) |
		(0x6 == type) |
		(0xb == type) )
	{
		//say("fat\n");
		dstqword[2] = _fat_;
	}
	else if(0x7 == type)
	{
		//say("ntfs\n");
		dstqword[2] = _ntfs_;
	}
	else if(0x83 == type)
	{
		//say("ext\n");
		dstqword[2] = _ext_;
	}
	else
	{
		//say("unknown:%x\n",type);
		H = ( (type>>4)&0xf ) + 0x30;
		if(H>0x39)H += 7;
		L = (type&0xf) + 0x30;
		if(L>0x39)L += 7;

		dstqword[2] = H + (L<<8);
	}

	say("[%012x,%012x]:	%8.8s\n",
		dstqword[0], dstqword[1],
		&dstqword[2]
	);
	return 0x10;	//这次翻译了多少
}




//[+0x1be,+0x1fd],每个0x10,总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小
void parse_mbr(u8* src, u8* dst)
{
	int j,ret;
	for(j=0;j<0x10000;j++)dst[j] = 0;

	//主分区
	ret = mbrrecord(src+0x1be,dst);
	if(ret > 0)dst += 0x80;

	ret = mbrrecord(src+0x1ce,dst);
	if(ret > 0)dst += 0x80;

	ret = mbrrecord(src+0x1de,dst);
	if(ret > 0)dst += 0x80;

	ret = mbrrecord(src+0x1ee,dst);
	if(ret > 0)dst += 0x80;
}




int mbrclient_start(struct object* obj, void* of, struct element* ele, void* ef, u8* buf, int len)
{
	u8 tmp[0x4000];
	parse_mbr(buf, tmp);
	return 0;
}