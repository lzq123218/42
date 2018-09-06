#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void quaternion_operation(float*, float*, float);
void matrixmultiply_4(float*, float*);




void mat4_print(mat4 m)
{
	int x,y;
	for(y=0;y<4;y++)
	{
		say("%f,%f,%f,%f\n",m[y][0], m[y][1], m[y][2], m[y][3]);
	}
}
void fixview(mat4 viewmatrix, struct arena* win)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = win->camera.vc[0];
	float cy = win->camera.vc[1];
	float cz = win->camera.vc[2];

	//uvn.n = front
	float nx = win->camera.vf[0];
	float ny = win->camera.vf[1];
	float nz = win->camera.vf[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = ny*1 - nz*0;
	float uy = nz*0 - nx*1;
	float uz = nx*0 - ny*0;
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above = cross(right, front)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;
	norm = squareroot(vx*vx + vy*vy + vz*vz);
	vx /= norm;
	vy /= norm;
	vz /= norm;

	viewmatrix[0][0] = ux;
	viewmatrix[0][1] = uy;
	viewmatrix[0][2] = uz;
	viewmatrix[0][3] = -cx*ux - cy*uy - cz*uz;

	viewmatrix[1][0] = vx;
	viewmatrix[1][1] = vy;
	viewmatrix[1][2] = vz;
	viewmatrix[1][3] = -cx*vx - cy*vy - cz*vz;

	viewmatrix[2][0] = -nx;
	viewmatrix[2][1] = -ny;
	viewmatrix[2][2] = -nz;
	viewmatrix[2][3] = cx*nx + cy*ny + cz*nz;

	viewmatrix[3][0] = 0.0;
	viewmatrix[3][1] = 0.0;
	viewmatrix[3][2] = 0.0;
	viewmatrix[3][3] = 1.0f;
	//mat4_print(viewmatrix);
}
void fixproj(mat4 projmatrix, struct arena* win)
{
	float a = PI/2;
	float n = 1.0;
	float w = (float)(win->width);
	float h = (float)(win->height);

	projmatrix[0][0] = 1.0 / tangent(a/2);
	projmatrix[0][1] = 0.0;
	projmatrix[0][2] = 0.0;
	projmatrix[0][3] = 0.0;

	projmatrix[1][0] = 0.0;
	projmatrix[1][1] = projmatrix[0][0] * w / h;
	projmatrix[1][2] = 0.0;
	projmatrix[1][3] = 0.0;

	projmatrix[2][0] = 0.0;
	projmatrix[2][1] = 0.0;
	projmatrix[2][2] = -1.0;	//	(n+f) / (n-f);
	projmatrix[2][3] = -2*n;	//	2*n*f / (n-f);

	projmatrix[3][0] = 0.0;
	projmatrix[3][1] = 0.0;
	projmatrix[3][2] = -1.0;
	projmatrix[3][3] = 0.0;
}
void fixmatrix(mat4 mvp, struct arena* win)
{
	int x;
	mat4 m;

	fixproj(mvp, win);
	fixview(m, win);
	mat4_multiply(mvp, m);
}




void invproj(mat4 projmatrix, struct arena* win)
{
	float a = PI/2;
	float n = 1.0;
	float w = (float)(win->width);
	float h = (float)(win->height);

	projmatrix[0][0] = tangent(a/2);
	projmatrix[0][1] = 0.0;
	projmatrix[0][2] = 0.0;
	projmatrix[0][3] = 0.0;

	projmatrix[1][0] = 0.0;
	projmatrix[1][1] = projmatrix[0][0] * h / w;
	projmatrix[1][2] = 0.0;
	projmatrix[1][3] = 0.0;

	projmatrix[2][0] = 0.0;
	projmatrix[2][1] = 0.0;
	projmatrix[2][2] = 0.0;
	projmatrix[2][3] = -1.0;

	projmatrix[3][0] = 0.0;
	projmatrix[3][1] = 0.0;
	projmatrix[3][2] = -0.5/n;
	projmatrix[3][3] = 0.5/n;
}
void invview(mat4 viewmatrix, struct arena* win)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = win->camera.vc[0];
	float cy = win->camera.vc[1];
	float cz = win->camera.vc[2];

	//uvn.n = front
	float nx = win->camera.vf[0];
	float ny = win->camera.vf[1];
	float nz = win->camera.vf[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = ny*1 - nz*0;
	float uy = nz*0 - nx*1;
	float uz = nx*0 - ny*0;
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above cross(right, front)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;
	norm = squareroot(vx*vx + vy*vy + vz*vz);
	vx /= norm;
	vy /= norm;
	vz /= norm;

	viewmatrix[0][0] = ux;
	viewmatrix[0][1] = vx;
	viewmatrix[0][2] = -nx;
	viewmatrix[0][3] = cx;

	viewmatrix[1][0] = uy;
	viewmatrix[1][1] = vy;
	viewmatrix[1][2] = -ny;
	viewmatrix[1][3] = cy;

	viewmatrix[2][0] = uz;
	viewmatrix[2][1] = vz;
	viewmatrix[2][2] = -nz;
	viewmatrix[2][3] = cz;

	viewmatrix[3][0] = 0.0f;
	viewmatrix[3][1] = 0.0f;
	viewmatrix[3][2] = 0.0f;
	viewmatrix[3][3] = 1.0f;
}
void invmvp(vec3 v, struct arena* win)
{
	float f;
	mat4 p;
	mat4 q;
	vec4 t = {v[0], v[1], v[2], 1.0};

	invview(p, win);
	invproj(q, win);
	mat4_multiply(p, q);

	f = p[3][0]*t[0] + p[3][1]*t[1] + p[3][2]*t[2] + p[3][3]*t[3];
	v[0] = (p[0][0]*t[0] + p[0][1]*t[1] + p[0][2]*t[2] + p[0][3]*t[3]) / f;
	v[1] = (p[1][0]*t[0] + p[1][1]*t[1] + p[1][2]*t[2] + p[1][3]*t[3]) / f;
	v[2] = (p[2][0]*t[0] + p[2][1]*t[1] + p[2][2]*t[2] + p[2][3]*t[3]) / f;
}




void camera_deltax(struct arena* win, float delta)
{
	float c,s;
	float tx,ty,tz;		//target
	float cx,cy,cz;		//camera
	float vx,vy,vz;		//vector(tar to cam)(origin)
	float px,py,pz;		//vector(tar to cam)(rotate)

	//target
	tx = win->target.vc[0];
	ty = win->target.vc[1];
	tz = win->target.vc[2];

	//camera
	cx = win->camera.vc[0];
	cy = win->camera.vc[1];
	cz = win->camera.vc[2];

	//vector = -front
	vx = cx-tx;
	vy = cy-ty;
	vz = cz-tz;

	c = cosine(delta);
	s = sine(delta);

	//rotate
	px = vx*c + vy*s;
	py = -vx*s + vy*c;
	pz = vz;

	//camera = target+vector
	win->camera.vc[0] = tx+px;
	win->camera.vc[1] = ty+py;
	win->camera.vc[2] = tz+pz;

	//front = -vector
	win->camera.vf[0] = -px;
	win->camera.vf[1] = -py;
	win->camera.vf[2] = -pz;
}
void camera_deltay(struct arena* win, float delta)
{
	float v[4];
	float q[4];
	float tx,ty,tz;		//target
	float cx,cy,cz;		//camera

	//target
	tx = win->target.vc[0];
	ty = win->target.vc[1];
	tz = win->target.vc[2];

	//camera
	cx = win->camera.vc[0];
	cy = win->camera.vc[1];
	cz = win->camera.vc[2];

	//vector = -front
	v[0] = cx-tx;
	v[1] = cy-ty;
	v[2] = cz-tz;

	//cos=dot(v1,v2) / (|v1|*|v2|)
	q[0] = cosine(delta)*cosine(delta);
	q[1] = (v[2]*v[2]) / (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if(q[0] < q[1])
	{
		//(v[2]>0&&delta>0) or (v[2]<0&&delta<0)
		if(v[2]*delta > 0)return;
	}

	//right = cross(front, (0,0,1))
	q[0] = v[1]*1 - v[2]*0;
	q[1] = v[2]*0 - v[0]*1;
	q[2] = v[0]*0 - v[1]*0;
	quaternion_operation(v, q, delta);

	//camera = target+vector
	win->camera.vc[0] = tx+v[0];
	win->camera.vc[1] = ty+v[1];
	win->camera.vc[2] = tz+v[2];

	//front = -vector
	win->camera.vf[0] = -v[0];
	win->camera.vf[1] = -v[1];
	win->camera.vf[2] = -v[2];
}
void camera_deltaxy(struct arena* win, int dx, int dy)
{
	float delta;

	if(0 != dx)
	{
		if(dx < 0)delta = -0.05;
		else if(dx > 0)delta = 0.05;
		camera_deltax(win, delta);
	}
	if(0 != dy)
	{
		if(dy < 0)delta = -0.02;
		else if(dy > 0)delta = 0.02;
		camera_deltay(win, delta);
	}
}
void camera_zoom(struct arena* win, float delta)
{
	float x = delta*(win->camera.vf[0]);
	float y = delta*(win->camera.vf[1]);
	float z = delta*(win->camera.vf[2]);

	win->camera.vc[0] += x;
	win->camera.vc[1] += y;
	win->camera.vc[2] += z;

	win->camera.vf[0] -= x;
	win->camera.vf[1] -= y;
	win->camera.vf[2] -= z;
}
void target_deltaxyz(struct arena* win, int x, int y, int z)
{
	float norm;
	float tx, ty;
	float dx, dy, dz;

	if(x < 0)x = -1;
	else if(x > 0)x = 1;
	if(y < 0)y = -1;
	else if(y > 0)y = 1;

	tx = win->camera.vf[0];
	ty = win->camera.vf[1];
	norm = squareroot(tx*tx+ty*ty);
	tx /= norm;
	ty /= norm;

	dx = 10.0*x*ty;
	dx += 10.0*y*tx;
	dy = -10.0*x*tx;
	dy += 10.0*y*ty;
	dz = 10*z;

	win->target.vc[0] += dx;
	win->target.vc[1] += dy;
	win->target.vc[2] += dz;

	win->camera.vc[0] += dx;
	win->camera.vc[1] += dy;
	win->camera.vc[2] += dz;
}




int actorinput_cameraevent(struct arena* win, struct event* ev)
{
	short* t;
	float x,y,z,w;
	int x0,y0,x1,y1,id,sign = -1;
	if(_vbo_ == win->fmt)sign = 1;

	if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)
		{
			win->target.vc[0] -= 10;
			win->camera.vc[0] -= 10;
			return 0;
		}
		else if(0x4d == ev->why)
		{
			win->target.vc[0] += 10;
			win->camera.vc[0] += 10;
			return 0;
		}
		else if(0x50 == ev->why)
		{
			win->target.vc[1] -= sign*10;
			win->camera.vc[1] -= sign*10;
			return 0;
		}
		else if(0x48 == ev->why)
		{
			win->target.vc[1] += sign*10;
			win->camera.vc[1] += sign*10;
			return 0;
		}
	}
	if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		if(t[3] & joyl_left)
		{
			win->target.vc[0] -= 10;
			win->camera.vc[0] -= 10;
			return 0;
		}
		if(t[3] & joyl_right)
		{
			win->target.vc[0] += 10;
			win->camera.vc[0] += 10;
			return 0;
		}
		if(t[3] & joyl_down)
		{
			win->target.vc[1] -= sign*10;
			win->camera.vc[1] -= sign*10;
			return 0;
		}
		if(t[3] & joyl_up)
		{
			win->target.vc[1] += sign*10;
			win->camera.vc[1] += sign*10;
			return 0;
		}
		if(t[3] & joyl_trigger)
		{
			target_deltaxyz(win, 0, 0, -1);
			return 0;
		}
		if(t[3] & joyl_bumper)
		{
			target_deltaxyz(win, 0, 0, 1);
			return 0;
		}
		if(t[3] & joyl_stick)
		{
			win->camera.vc[2] -= win->target.vc[2];
			win->target.vc[2] = 0;
			return 0;
		}
		if(t[3] & joyl_select)
		{
			return 0;
		}

		x0 = t[0];
		if(x0 < -8192)x0 = -1;
		else if(x0 > 8192)x0 = 1;
		else x0 = 0;

		y0 = t[1];
		if(y0 < -8192)y0 = -1;
		else if(y0 > 8192)y0 = 1;
		else y0 = 0;

		target_deltaxyz(win, x0, sign*y0, 0);
		return 0;
	}
	if(joy_right == (ev->what & joy_mask))
	{
		t = (void*)ev;
		if(t[3] & joyr_trigger)
		{
			camera_zoom(win, 0.1);
			return 0;
		}
		if(t[3] & joyr_bumper)
		{
			camera_zoom(win, -0.1);
			return 0;
		}
		if(t[3] & joyr_stick)
		{
			x = win->camera.vc[0] - win->target.vc[0];
			y = win->camera.vc[1] - win->target.vc[1];
			z = win->camera.vc[2] - win->target.vc[2];
			w = squareroot(x*x + y*y + z*z);

			win->camera.vf[0] = 0.0;
			win->camera.vf[1] = w*0.7071067811865476;
			win->camera.vf[2] = -w*0.7071067811865476;

			win->camera.vc[0] = win->target.vc[0];
			win->camera.vc[1] = win->target.vc[1] - win->camera.vf[1];
			win->camera.vc[2] = win->target.vc[2] - win->camera.vf[2];
			return 0;
		}
		if(t[3] & joyr_start)
		{
			return 0;
		}

		x0 = t[0];
		if(x0 < -8192)x0 = -1;
		else if(x0 > 8192)x0 = 1;
		else x0 = 0;

		y0 = t[1];
		if(y0 < -8192)y0 = -1;
		else if(y0 > 8192)y0 = 1;
		else y0 = 0;

		camera_deltaxy(win, x0, -y0);
	}

	id = (ev->why)>>48;
	if(0x4070 == ev->what)
	{
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		if(	(0 != win->input[0].z0)&&
			(0 != win->input[1].z0))
		{
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;
			if(0 == id)
			{
				x1 -= (win->input[1].x1);
				y1 -= (win->input[1].y1);
			}
			if(1 == id)
			{
				x1 -= (win->input[0].x1);
				y1 -= (win->input[0].y1);
			}

			x0 = (win->input[0].x1) - (win->input[1].x1);
			y0 = (win->input[0].y1) - (win->input[1].y1);

			if((x0*x0+y0*y0) < (x1*x1+y1*y1))camera_zoom(win, 0.05);
			else camera_zoom(win, -0.05);
		}
		else
		{
			x0 = win->input[id].x1;
			y0 = win->input[id].y1;
			x1 = (ev->why)&0xffff;
			y1 = ((ev->why)>>16)&0xffff;

			camera_deltaxy(win, x1-x0, y1-y0);
		}
	}
	else if(0x2b70 == ev->what)
	{
		if('f' == id)camera_zoom(win, 0.1);
		if('b' == id)camera_zoom(win, -0.1);
	}
	return 0;
}
int actorinput_camera(struct arena* win, struct event* ev)
{
	struct style* sty = 0;
	struct relation* orel = 0;

	//find the chosen actor
	orel = win->oreln;
	while(1)
	{
		if(0 == orel)break;
		if(_act_ == orel->dsttype)
		{
			sty = (void*)(orel->srcfoot);
			win->target.vc[0] = sty->vc[0];
			win->target.vc[1] = sty->vc[1];
			win->target.vc[2] = sty->vc[2];
			break;
		}
		orel = samesrcprevdst(orel);
	}

	//move it
	actorinput_cameraevent(win, ev);
	if(sty)
	{
		sty->vc[0] = win->target.vc[0];
		sty->vc[1] = win->target.vc[1];
		sty->vc[2] = win->target.vc[2];
	}
	return 0;
}




void actoroutput_posture_vbo(struct arena* win, struct style* sty)
{
	int j;
	vec3 tc, tr, tf;
	float* vc = win->target.vc;
	float* vr = win->target.vr;
	float* vf = win->target.vf;
	float* vu = win->target.vu;
	carveaxis(win);
	carveline_prism4(win, 0xff00ff, vc, vr, vf, vu);

	//rpg.hp.bg
	tc[0] = -0.75;
	tc[1] = 1.0-1.0/16;
	tc[2] = 0.0;
	tr[0] = 0.25;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0/16;
	tf[2] = 0.0;
	carvesolid2d_rect(win, 0xff0000, tc, tr, tf);

	//rpg.mp.bg
	tc[0] = -tc[0];
	carvesolid2d_rect(win, 0x0000ff, tc, tr, tf);

	//rpg.hp.fg
	tc[0] = 0.125-1.0;
	tc[1] = 1.0-0.125;
	tc[2] = -0.5;
	tr[0] = 0.125;
	tf[1] = 0.125;
	carvesolid2d_circle(win, 0x800000, tc, tr, tf);

	//rpg.mp.fg
	tc[0] = -tc[0];
	carvesolid2d_circle(win, 0x000080, tc, tr, tf);
}
void actoroutput_posture_pixel(struct arena* win, struct style* sty)
{
	int x0,y0,x1,y1;
	float* vc = win->target.vc;
	float* vr = win->target.vr;
	float* vf = win->target.vf;
	float* vu = win->target.vu;
	drawaxis(win);

	x0 = vc[0] - vr[0];
	y0 = vc[1] - vf[1];
	x1 = vc[0] + vr[0];
	y1 = vc[1] + vf[1];
	drawline_rect(win, 0xff00ff, x0, y0, x1, y1);

	//rpg.hp.bg
	drawsolid_rect(win, 0xff0000, 0, 0, win->width/4, win->height/16);

	//rpg.mp.bg
	drawsolid_rect(win, 0x0000ff, win->width*3/4, 0, win->width, win->height/16);

	//rpg.hp.fg
	drawsolid_circle(win, 0x800000, win->height/16, win->height/16, win->height/16);

	//rpg.mp.fg
	drawsolid_circle(win, 0x000080, (win->width)-(win->height/16), win->height/16, win->height/16);
}
int actoroutput_posture(struct arena* win, struct style* st)
{
	struct relation* orel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	//origin world
	orel = win->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);
		}

		orel = samesrcnextdst(orel);
	}

	//chosen actor
	if(_vbo_ == win->fmt)actoroutput_posture_vbo(win, sty);
	else actoroutput_posture_pixel(win, sty);
	return 0;
}