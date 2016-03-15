#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
void say(char*,...);
static char foldername[256];
static DIR* folderbody=0;
static struct dirent*  ent;




void initfolder()
{
}
void killfolder()
{
}
int openfolder(char* name)
{
	struct stat     statbuf;
	int             ret;

	ret=stat(name,&statbuf);
	if(ret==-1)
	{
		say("not exist\n");
		return 0;
	}

	if(!(statbuf.st_mode & S_IFDIR))
	{
		say("not folder\n");
		return 0;
	}

	strncpy(foldername,name,256);
	folderbody=opendir(name);
	return 1;
}
void closefolder()
{
	if(folderbody!=0)
	{
		closedir(folderbody);
		folderbody=0;
	}
}




void listfolder()
{
	if(folderbody==0)return;

	//
	rewinddir(folderbody);
	while(1)
	{
		ent=readdir(folderbody);
		if(ent==NULL)return;
		say("%s\n",ent->d_name);
	}
}
void switchfolder(char* name)
{
	closefolder();
	openfolder(name);
}
void readfolder(char* dest)
{
	//next one
	ent=readdir(folderbody);
	if(ent==NULL)return;

	//get name
	snprintf(dest,256,"%s",ent->d_name);
}
void writefolder()
{
}