#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <gtk/gtk.h>
void eventwrite(u64,u64);
void say(char*,...);




//
static pthread_t id;




void* uievent(void* p)
{
	GtkWidget *window;
	GtkWidget *button;
	int argc = 1;
	char* argv[2];
	void* haha=argv;
	argv[0] = "./a.out";
	argv[1] = 0;
	gtk_init (&argc, (void*)&haha);

	//
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 512, 512);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show (window);

	//
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	//
	gtk_main();
	eventwrite(0,0);
	return 0;
}




void windowchange()
{
}
void windowlist()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
}
void windowstop()
{
}
void windowcreate()
{
	pthread_create(&id, NULL, uievent, NULL);
}
void windowdelete()
{
}
