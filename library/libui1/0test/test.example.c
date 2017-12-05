#include "actor.h"




static void example_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)example_read_cli(win, act, sty);
	else if(fmt == __tui__)example_read_tui(win, act, sty);
	else if(fmt == __html__)example_read_html(win, act, sty);
	else if(fmt == __vbo__)example_read_vbo(win, act, sty);
	else example_read_pixel(win, act, sty);
}
static void example_write(struct event* ev)
{
}




static void example_list()
{
}
static void example_change()
{
}
static void example_start()
{
}
static void example_stop()
{
}
void example_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->start = (void*)example_start;
	p->stop = (void*)example_stop;
	p->list = (void*)example_list;
	p->choose = (void*)example_change;
	p->read = (void*)example_read;
	p->write = (void*)example_write;
}
void example_delete()
{
}