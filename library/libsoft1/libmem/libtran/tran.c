#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int cmp(void*,void*);
int hexstr2data(u8*,u64*);
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




//
static int tran_list(u8* arg1)
{
	return 0;
}
static int tran_choose(u8* arg)
{
	return 0;
}
static int tran_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int tran_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int tran_start(u8* p)
{
	return 0;
}
static int tran_stop()
{
	return 0;
}
int tran_create(u8* softaddr,u64* p)
{
	u8* q;

	//
	guys = softaddr;
	fshome = softaddr+0x100000;
	dirhome = softaddr+0x200000;
	datahome = softaddr+0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x6e617274;

	p[10]=(u64)tran_start;
	p[11]=(u64)tran_stop;
	p[12]=(u64)tran_list;
	p[13]=(u64)tran_choose;
	p[14]=(u64)tran_read;
	p[15]=(u64)tran_write;

	return 0x80;
}
int tran_delete()
{
	return 0;
}
