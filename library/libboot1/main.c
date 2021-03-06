typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
//libuser1
void freeactor();
void initactor(void*);
//libuser0
void freearena();
void initarena(void*);
//libsoft1
void freeartery();
void initartery(void*);
//libsoft0
void freesystem();
void initsystem(void*);
//libhard1
void freedriver();
void initdriver(void*);
//libhard0
void freedevice();
void initdevice(void*);
int deviceread_all(void*);
int devicewrite_ev(void*);
//libboot1
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
//libboot0
void* death();
void* birth();
//
void loop();
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//i
int arg2utf8(void*, void*);
int termwrite(void*, int);
//o
void printmemory(void*, int);
void say(void*, ...);




void* beforedawn()
{
	//allocate
	void* addr = birth();

	//libboot
	initstdin( addr+0x000000);
	initstdout(addr+0x100000);
	initstdev( addr+0x200000);
	initstdrel(addr+0x300000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x400000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0x800000);

	//libuser
	initarena(addr+0xc00000);
	initactor(addr+0xc00000);

	return addr;
}
void afterdusk()
{
	//libuser
	freeactor();
	freearena();

	//libsoft
	freeartery();
	freesystem();

	//libhard
	freedriver();
	freedevice();

	//libboot
	freestdev();
	freestdrel();
	freestdout();
	freestdin();

	//cleanup
	death();
}
int main(int argc, char* argv[])
{
	//before
	u64 j,k;
	u8* addr = beforedawn();

	//cmdline
	termwrite("\n@42@\n", 1);
	for(j=1;j<argc;j++)
	{
		k = arg2utf8(argv[j], addr);
		say("%.*s\n", k, addr);

		if(termwrite(addr, k) < 0)goto byebye;
	}

	//before mainloop: load file as ui
	//if(0 == srcctx)srcctx =
	//if(0 == loadctx(srcctx))emptyctx();

	//mainloop
	loop();

	//after mainloop: backup world
	//if(0 == dstctx)dstctx =
	//if(0 == storectx(dstctx))dump();
	openwriteclose("universe.bin",0,addr,0x1000000);

byebye:
	//after
	afterdusk();
	return 0;
}




/*
#include <efi.h>
#include <efilib.h>
EFI_STATUS efi_hack(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table);
EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table)
{
	efi_hack(handle, table);

	main(0, 0);

	return EFI_SUCCESS;
}
void atexit(){}
*/




#ifdef __ANDROID__
#include <stdio.h>
#include <stdlib.h>
void app_dummy();
void setapp(void*);
void android_main(void* app)
{
	app_dummy();

	setapp(app);
	main(0, 0);

	exit(0);
}
#endif
