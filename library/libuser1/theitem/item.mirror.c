#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void* allocofoot();
void actorcreatefromfile(struct actor* act, char* name);




char* mirror_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* mirror_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";




static void mirror_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void mirror_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float (*vbuf)[6];
	struct ofoot* opin;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	opin = (void*)(pin->foot[0]);
	vbuf = (void*)(opin->vbuf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	opin->vbuf_enq += 1;
}
static void mirror_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)mirror_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)mirror_read_tui(win, sty, act, pin);
	else if(fmt == _html_)mirror_read_html(win, sty, act, pin);
	else if(fmt == _json_)mirror_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)mirror_read_vbo(win, sty, act, pin);
	else mirror_read_pixel(win, sty, act, pin);
}
static void mirror_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mirror_list()
{
}
static void mirror_change()
{
}
static void mirror_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct ofoot* opin;
	if(0 == pin)return;

	//
	opin = allocofoot();

	//shader
	opin->vs = (u64)mirror_glsl_v;
	opin->fs = (u64)mirror_glsl_f;

	//texture
	opin->tex[0] = (u64)(act->buf);
	opin->tex_fmt[0] = hex32('r','g','b','a');
	opin->tex_w[0] = act->width;
	opin->tex_h[0] = act->height;

	//vertex
	opin->vbuf = (u64)memorycreate(4*6*6);
	opin->vbuf_fmt = vbuffmt_33;
	opin->vbuf_w = 6*4;
	opin->vbuf_h = 6;
	opin->method = 'v';

	//send!
	opin->shader_enq[0] = 42;
	opin->arg_enq[0] = 0;
	opin->tex_enq[0] = 42;
	opin->vbuf_enq = 0;
	opin->ibuf_enq = 0;
	pin->foot[0] = (u64)opin;
}
static void mirror_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void mirror_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "jpg/mirror.jpg");
}




void mirror_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('m', 'i', 'r', 'r', 'o', 'r', 0, 0);

	p->oncreate = (void*)mirror_create;
	p->ondelete = (void*)mirror_delete;
	p->onstart  = (void*)mirror_start;
	p->onstop   = (void*)mirror_stop;
	p->onlist   = (void*)mirror_list;
	p->onchoose = (void*)mirror_change;
	p->onread   = (void*)mirror_read;
	p->onwrite  = (void*)mirror_write;
}
