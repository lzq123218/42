#include "libboot.h"
void* devicelist(void*, int);
void* devicecommand(void*, int);
void* driverlist(void*, int);
void* drivercommand(void*, int);
void* systemlist(void*, int);
void* systemcommand(void*, int);
void* arterylist(void*, int);
void* arterycommand(void*, int);
void* arenalist(void*, int);
void* arenacommand(void*, int);
void* actorlist(void*, int);
void* actorcommand(void*, int);
//
int scene_recover(void*, int);
int scene_backup(void*, int);
int wire_add(void*, int);
int wire_del(void*, int);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




void term_ls(u8* buf, int len)
{
	if(buf[3] <= 0x20)
	{
		say("----------------\n");
		devicelist(0, 0);
		say("----------------\n");
		driverlist(0, 0);
		say("----------------\n");
		systemlist(0, 0);
		say("----------------\n");
		arterylist(0, 0);
		say("----------------\n");
		arenalist(0, 0);
		say("----------------\n");
		actorlist(0, 0);
		return;
	}

	buf += 3;
	if(0 == ncmp(buf, "device", 6))devicelist(0, 0);
	else if(0 == ncmp(buf, "driver", 6))driverlist(0, 0);
	else if(0 == ncmp(buf, "system", 6))systemlist(0, 0);
	else if(0 == ncmp(buf, "artery", 6))arterylist(0, 0);
	else if(0 == ncmp(buf, "arena", 5))arenalist(0, 0);
	else if(0 == ncmp(buf, "actor", 5))actorlist(0, 0);
	else say("ls(%s)\n", buf);
}




void term_prompt()
{
	say("[term@void]");
}
int termwrite(u8* buf, int len)
{
	int j;
	if(0 == buf)goto finish;
	if(buf[0] <= 0x20)goto finish;

	if(('q' == buf[0])&&(buf[1] <= 0x20))goto byebye;
	if(0 == ncmp(buf, "exit", 4))goto byebye;

	//
	if(0 == ncmp(buf, "ls", 2))
	{
		term_ls(buf, len);
	}
	else if(0 == ncmp(buf, "event", 5))
	{
		say("%.*s\n", len, buf);
	}
	else if(0 == ncmp(buf, "backup", 6))
	{
		scene_backup(buf+6, 0);
	}
	else if(0 == ncmp(buf, "recover", 7))
	{
		scene_recover(buf+7, 0);
	}
	else if(0 == ncmp(buf, "wireadd", 7))
	{
		wire_add(buf+7, 0);
	}
	else if(0 == ncmp(buf, "wiredel", 7))
	{
		wire_add(buf+7, 0);
	}
	else if(0 == ncmp(buf, "device", 6))devicecommand(buf+7, 0);
	else if(0 == ncmp(buf, "driver", 6))drivercommand(buf+7, 0);
	else if(0 == ncmp(buf, "system", 6))systemcommand(buf+7, 0);
	else if(0 == ncmp(buf, "artery", 6))arterycommand(buf+7, 0);
	else if(0 == ncmp(buf, "arena", 5))arenacommand(buf+6, 0);
	else if(0 == ncmp(buf, "actor", 5))actorcommand(buf+6, 0);

finish:
	term_prompt();
	return 1;

byebye:
	eventwrite(0,0,0,0);
	return -1;
}
void termread()
{
}