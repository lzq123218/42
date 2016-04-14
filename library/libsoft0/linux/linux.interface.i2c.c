#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
#include<sys/ioctl.h>
static int fp=-1;
static unsigned char buf[16];




int systemi2c_write(unsigned char* buf,unsigned char reg)
{
	//write address
	int ret=write(fp,&reg,1);
	usleep(100);
	if(ret!=1)
	{
		//printf("(error write)%x:%x\n",reg,ret);
		return -1;
	}

	//write data
	ret=write(fp,buf,1);
	usleep(1000);
	if(ret!=1)
	{
		//printf("(error read)%x:%x\n",reg,ret);
		return -2;
	}
	return 1;
}
int systemi2c_read(unsigned char* buf,unsigned char reg)
{
	//write address
	int ret=write(fp,&reg,1);
	usleep(100);
	if(ret!=1)
	{
		//printf("(error write)%x:%x\n",reg,ret);
		return -1;
	}

	//read data
	ret=read(fp,buf,1);
	usleep(1000);
	if(ret!=1)
	{
		//printf("(error read)%x:%x\n",reg,ret);
		return -2;
	}
	return 1;
}
void systemi2c_choose(char* bus,int device)	//	"/dev/i2c-1","62"
{
	int ret;
	if(fp!=-1)close(fp);

	fp=open(bus,O_RDWR);
	ret=ioctl(fp,I2C_SLAVE,device);
}
void systemi2c_list(char* towhere)	//	enumerate all i2c host and device
{
	int x;
	int y;
	int ret;
	char ch;

	for(y=0;y<16;y++)
	{
		for(x=0;x<16;x++)
		{
			ret=ioctl(fp,I2C_SLAVE,x+(y<<4));
			ret=systemi2c_read(&ch,0);

			if(ret>0)printf("%.2x ",ch);
			else printf("-- ");
		}
		printf("\n");
	}
}




void starti2c()
{
	int ret;

	//open
	fp = open("/dev/i2c-1",O_RDWR);
	if(fp<0)
	{
		printf("error open\n");
		return;
	}

	//ioctl
	ret=ioctl(fp,I2C_SLAVE,0x62);
	if(ret < 0)
	{
		printf("error ioctl\n");
		return;
	}
}
void stopi2c()
{
	if(fp != -1)close(fp);
}
void initi2c()
{
}
void killi2c()
{
}
