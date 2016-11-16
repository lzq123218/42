#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createserver();
int deleteserver();
int startserver(u8* addr, int port, u8* dir, int opt);
int stopserver(u64 x);
int listserver(u8*);
int chooseserver(u8*, u8*, int, u8*);
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
//
void notify_create(u64* p);
void notify_delete(u64 fd);
int serve_http(u64 fd, u8* buf, u64 len);
int serve_websocket(u64 fd, u8* buf, u64 len);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int movsb(void*,void*,int);
int copy(u8*,u8*);
int ncopy(u8*,u8*,int);
//
void printmemory(void*, int);
void say(void*, ...);




//
struct node
{
	//8*4=32
	u64 fd;
	u64 time;
	u64 type;       //[0,f]none, [0x10,0x1f]tcp, [0x20,0x2f]ws
	u64 next;

	//32
	u8 addr[32];
};
static struct node known[16];
static int max = 0;

//
static u8* datahome=0;




void known_print()
{
	int k;
	u64 fd;
	char* str;

	say("\nknown{\n");
	for(k=0;k<max;k++)
	{
		fd = known[k].fd;
		str = known[k].addr;
		//say("%llx,%x",fd,str);
		say("   [%lld]%s\n", fd, str);
	}
	say("}%d\n",max);
}
void known_delete(u64 fd)
{
	int j;
	//say("[%d]leave\n", fd);

	for(j=0;j<max;j++)
	{
		if(known[j].fd == fd)
		{
			//左移
			movsb(&known[j], &known[j+1], (max-1-j)*sizeof(struct node));
			max--;

			//
			known[max].fd = 0;

			//
			known_print();
			break;
		}
	}
}
void known_create(u64* p)
{
	int j,k;
	u8* str = (void*)p[0];
	u64 fd = p[2];

	//user max
	if(max >= 8)
	{
		writeserver(fd, "too many", 0, 8);
		notify_delete(fd);
		return;
	}

	//say("[%d]%s\n", fd, (void*)str);
	for(j=0;j<=max;j++)
	{
		if(known[j].fd == fd)
		{
			known[j].type = 0;
			break;
		}

		if(known[j].fd < fd)
		{
			//右移
			movsb(&known[j+1], &known[j], (max-j)*sizeof(struct node));
			max++;

			//
			known[j].fd = fd;
			known[j].type = 0;
			ncopy(known[j].addr, str, 32);

			//
			known_print();
			break;
		}
	}
}
void known_read(u64* p)
{
	int ret;
	int count;
	u64 index;
	u64 fd = p[2];

	//读
	count = readserver(fd, datahome, 0, 0x100000);
	if(count <= 0)
	{
		notify_delete(fd);
		return;
	}
	datahome[count] = 0;

	//找
	for(index=0;index<max;index++)
	{
		if(fd == known[index].fd)break;
	}
	if(index >= max)return;


//--------------------------------------------------------
	//0:	刚来的
	//10:	websocket
	//1?:	websocket + authstage
	//20:	ssh
	//2?:	ssh + authstage
//--------------------------------------------------------


	//tcp
	if(known[index].type == 0)
	{
		ret = serve_http(fd, datahome, count);

		//http,closed
		if(ret <= 0)return;

		//websocket,handshaked
		else if(ret == 0x10)known[index].type == 0x10;
	}

	//websocket
	else if(known[index].type <= 0x1f)
	{
		ret = serve_websocket(fd, datahome, count);
	}

	//ssh
	else if(known[index].type <= 0x2f)
	{
		//ret = serve_secureshell(fd, datahome, count);
	}
}




void notify_create(u64* p)
{
	int ret = p[1] & 0xffff;

	if(ret == 0x406e) known_read(p);
	else if(ret == 0x2b6e) known_create(p);
	else if(ret == 0x2d6e) known_delete(p[2]);

	writeserver(p[2], 0, 0, 0);
}
void notify_delete(u64 fd)
{
	known_delete(fd);
	stopserver(fd);
}




static int server_read(u64* p)
{
	return 0;
}
static int server_write(u8* p)
{
	return 0;
}
static int server_list(u8* p)
{
	return 0;
}
static int server_choose(u8* p)
{
	return 0;
}
static int server_start(u8* p)
{
	//0.0.0.0:2222/sdcard
	return startserver("0,0,0,0", 2222, "", 0);
}
static int server_stop()
{
	int j;
	for(j=max-1;j>=0;j--)
	{
		notify_delete(known[j].fd);
	}
	return stopserver(0);
}
int server_create(u8* world, u64* p)
{
	//
	int j;
	char* q = (void*)known;
	for(j=0;j<64*16;j++)
	{
		q[j] = 0;
	}
	datahome=world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x73;

	p[10]=(u64)server_start;
	p[11]=(u64)server_stop;
	p[12]=(u64)server_list;
	p[13]=(u64)server_choose;
	p[14]=(u64)server_read;
	p[15]=(u64)server_write;
	return 0;
}
int server_delete()
{
	return 0;
}