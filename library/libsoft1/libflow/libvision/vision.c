#define u64 unsigned long long
//
int startvision();
int stopvision();
//
void printmemory(void*, int);
void say(void*, ...);




void vision_explain(u64* p)
{
	//say("%llx\n",p[0]);
}




static int vision_list()
{
	return 0;
}
static int vision_choose()
{
	return 0;
}
static int vision_read()
{
	return 0;
}
static int vision_write()
{
	return 0;
}
static int vision_start()
{
	return startvision();
}
static int vision_stop()
{
	return stopvision();
}
void vision_create(void* w, u64* p)
{
	p[0]=0x776f6c66;	//type
	p[1]=0x6e6f69736976;	//id
	p[2]=(u64)vision_start;
	p[3]=(u64)vision_stop;
	p[4]=(u64)vision_list;
	p[5]=(u64)vision_choose;
	p[6]=(u64)vision_read;
	p[7]=(u64)vision_write;
}
void vision_delete()
{
}

