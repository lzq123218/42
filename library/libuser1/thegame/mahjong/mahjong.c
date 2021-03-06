#include "libuser.h"



static u8 data[144];




static void mahjong_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mahjong_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mahjong_get()
{
}
static void mahjong_post()
{
}
static void mahjong_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mahjong_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mahjong_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void mahjong_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(144);
}




void mahjong_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onstart  = (void*)mahjong_start;
	p->onstop   = (void*)mahjong_stop;
	p->onget    = (void*)mahjong_get;
	p->onpost   = (void*)mahjong_post;
	p->onread   = (void*)mahjong_read;
	p->onwrite  = (void*)mahjong_write;
}
