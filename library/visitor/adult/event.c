#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//event
int uievent(QWORD* first,QWORD* second);
int softevent(QWORD* first,QWORD* second);
int hardevent(QWORD* first,QWORD* second);
int bootevent(QWORD* first,QWORD* second);
//
void say(char*,...);




int stillalive=1;




int writeevent()
{
	stillalive=0;
	return 0;
}
int readevent(QWORD* first,QWORD* second)
{
        int ret;
        //say("here\n");
        if(stillalive==0)
        {
                first[0]=0;
                return 0;
        }

        //调试端口有没有消息
        ret=bootevent(first,second);
        if(ret>0)return 11;

        //硬件中断完成状态报告
        ret=hardevent(first,second);
        if(ret>0)return 22;

        //输入/网络/系统事件
        ret=softevent(first,second);
        if(ret>0)return 33;

        //窗口关闭,窗口大小变化等情况
        ret=uievent(first,second);
        if(ret>0)return 44;
}