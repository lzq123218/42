#define QWORD unsigned long long




static void ssh_list()
{
}
static void ssh_switch()
{
}
static void ssh_read()
{
}
static void ssh_write()
{
}
static void ssh_start(QWORD type,char* p)
{
}
static void ssh_stop()
{
}
void ssh_init(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x687373;		//id
	p[2]=(QWORD)ssh_start;
	p[3]=(QWORD)ssh_stop;
	p[4]=(QWORD)ssh_list;
	p[5]=(QWORD)ssh_switch;
	p[6]=(QWORD)ssh_read;
	p[7]=(QWORD)ssh_write;
}
void ssh_kill()
{
	ssh_stop();
}