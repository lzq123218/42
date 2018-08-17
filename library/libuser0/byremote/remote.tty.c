#include "libuser.h"
void* arterycreate(u64, void*);
int arterydelete(void*);
void* systemcreate(u64, void*);
int systemdelete(void*);




int uartserver_read(struct arena* win)
{
	return 0;
}
int uartserver_write(struct arena* win)
{
	return 0;
}
int uartserver_delete(struct arena* win)
{
	return 0;
}
int uartserver_create(struct arena* win, void* str)
{
	int j;
	void* tmp;
	if(0 == str)str = "/dev/ttyUSB0";

	tmp = systemcreate(_uart_, str);
	if(tmp == 0)return 0;

	relationcreate(win, 0, _win_, tmp, 0, _fd_);
	return 0;
}