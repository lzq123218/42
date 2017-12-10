#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define PORT 0x3f8
void main(int argc,char** argv);
void out8(u32 port, u8 data);
//libui
void actorcreate(void*, void*);
void actordelete();
void arenacreate(void*, void*);
void arenadelete();
//libsoft
void arterycreate(void*, void*);
void arterydelete();
void systemcreate(void*, void*);
void systemdelete();
//libhard
void bodycreate(void*, void*);
void bodydelete();
void drivercreate(void*, void*);
void driverdelete();
//libboot
void debugcreate(void*, void*);
void debugdelete();
void basiccreate(void*, void*);
void basicdelete();




static int status = 0;
static u8* basic = 0;
static u8* body = 0;
static u8* memory = 0;
static u8* actor = 0;




//start must be at the first!!!!!!!!!!!!!!!!!
void start()
{
	main(0,0);
}




void death()
{
	//to avoid second call
	if(status == 0xdead)return;

	//libui
	if(actor != 0)
	{
		actordelete();
		arenadelete();
		actor = 0;
	}

	//libsoft
	if(memory != 0)
	{
		arterydelete();
		systemdelete();
		memory = 0;
	}

	//libhard
	if(body != 0)
	{
		bodydelete();
		driverdelete();
		body = 0;
	}
	//libboot
	if(basic != 0)
	{
		debugdelete();
		basicdelete();
		basic = 0;
	}

	status = 0xdead;
}
void* birth()
{
	int j;
	u8* universe = (u8*)0x1000000;
	for(j=0;j<0x1000000;j++)universe[j] = 0;

	basic = universe + 0x0;
	body = universe + 0x400000;
	memory = universe + 0x800000;
	actor = universe + 0xc00000;

out8(PORT, 'a');
	basiccreate(0, basic);
	debugcreate(0, basic);

out8(PORT, 'b');
	drivercreate(0, body);
	bodycreate(0, body);

out8(PORT, 'c');
	systemcreate(0, memory);
	arterycreate(0, memory);

out8(PORT, 'd');
	arenacreate(0, actor);
	actorcreate(0, actor);

out8(PORT, 'e');
	return (void*)0x1000000;
}
