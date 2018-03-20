#include "actor.h"



static u8 buffer[16];




static void minesweep_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void minesweep_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
{
}
static void minesweep_list()
{
}
static void minesweep_choose()
{
}
static void minesweep_stop(struct actor* act, struct pinid* pin)
{
}
static void minesweep_start(struct actor* act, struct pinid* pin)
{
}
static void minesweep_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void minesweep_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = startmemory(16);
}




void minesweep_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('m', 'i', 'n', 'e', 's', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)minesweep_create;
	p->ondelete = (void*)minesweep_delete;
	p->onstart  = (void*)minesweep_start;
	p->onstop   = (void*)minesweep_stop;
	p->onlist   = (void*)minesweep_list;
	p->onchoose = (void*)minesweep_choose;
	p->onread   = (void*)minesweep_read;
	p->onwrite  = (void*)minesweep_write;
}