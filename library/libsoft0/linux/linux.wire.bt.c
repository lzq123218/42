#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
void say(char*,...);




//
static int fp=-1;




int systembt_read()
{
	return 1;
}
int systembt_write()
{
	return 1;
}




int systembt_list(char* towhere)
{
	int ret=system("hcitool scan");
	return ret;
}
int systembt_choose()
{
}




void systembt_start(char* p)
{
}
void systembt_stop()
{
}




void systembt_create()
{
}
void systembt_delete()
{
}