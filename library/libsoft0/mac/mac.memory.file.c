//听说必须在头文件的前面
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
//头文件
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);




int writefile(u64 fd, u64 off, char* buf, u64 len)
{
	int ret;

	ret = lseek(fd, off, SEEK_SET);
	if(-1 == ret)
	{
		//say("errno:%d,seek:%llx\n", errno, off);
		return -2;
	}

	ret = write(fd, buf, len);
	if(-1 == ret)
	{
		//say("errno:%d,read:%llx,%llx\n", errno, off, len);
		return -1;
	}

	return ret;
}
int readfile(u64 fd, u64 off, char* buf, u64 len)
{
	int ret;

	ret = lseek(fd, off, SEEK_SET);
	if(-1 == ret)
	{
		//say("errno:%d,seek:%llx\n", errno, off);
		return -2;
	}

	ret = read(fd, buf, len);
	if(-1 == ret)
	{
		//say("errno:%d,read:%llx,%llx\n", errno, off, len);
		return -1;
	}

	return ret;
}
int stopfile(int fd)
{
	return close(fd);
}
int startfile(char* path, int flag)
{
	int ret = 0;
	if(0 == path)ret = -0xfff;
	if(0 == path[0])ret = -0xffe;
	if(ret < 0)goto fail;

	flag = O_RDWR;
	if('w' == flag)flag |= O_CREAT;

	ret = open(path, flag, S_IRWXU|S_IRWXG|S_IRWXO);
	if(ret > 0)return ret;

fail:
	if(ret <= 0)printf("%d@open\n",ret);
	return ret;
}
void deletefile()
{
}
void createfile()
{
}
