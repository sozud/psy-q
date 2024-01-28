/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				clip
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Jun,20,1995	suzu	
 *
 *			   Area Clip
 :			エリアクリップ
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

static void screen_to_world(SVECTOR *s, SVECTOR *w);
static void rot_trans_pers(SVECTOR *w, SVECTOR *s);
static get_z_cross_point(SVECTOR *pw, SVECTOR *vw, SVECTOR *ww, int limit);

/*
 * calculate mini-max of vertex: mini-max を求める
 */
#define max(x,y)	((x)>(y)?(x):(y))
#define max3(x,y,z)	((x)>(y)?max(x,z):max(y,z))
#define max4(x,y,z,w)	(max(x,y)>max(z,w)?max(x,y):max(z,w))

#define min(x,y)	((x)<(y)?(x):(y))
#define min3(x,y,z)	((x)<(y)?min(x,z):min(y,z))
#define min4(x,y,z,w)	(min(x,y)<min(z,w)?min(x,y):min(z,w))

/*
 * caluculate viewing area on the plane z = 0
 * 平面 z = 0 でスクリーン上に投影される領域を求める
 */
/* RECT clips;		(input)  clip window (in the screen)
 * RECT clipw;		(output) clip window (on the z = 0 plane)
 * int	limit;		(input)  far clip point
 */
int areaClipZ(RECT *clips, RECT *clipw, int limit)
{
	
	SVECTOR	vs[4], vw[4];	/* screen area (in screen and world) */
	SVECTOR	ww[4];		/* wall edge (in world) */
	SVECTOR	ps, pw;		/* yourself (in screen and world) */
	int	h;		/* projection */
	int	i, ret;
	long	flg, p;
	
	/* set clip window on 2D  */
	int	min_x = clips->x, max_x = clips->x+clips->w;
	int	min_y = clips->y, max_y = clips->y+clips->h; 
	
	/* set clipping margin (temporary) */
	int	am = (max(clips->w, clips->h))/20;	
	
	/* locate clip window in the screen coordinate */
	h = ReadGeomScreen();
	setVector(&vs[0], min_x, min_y, h);
	setVector(&vs[1], max_x, min_y, h);
	setVector(&vs[2], min_x, max_y, h);
	setVector(&vs[3], max_x, max_y, h);

	/* ps: your standing position in the screen (obviously 0, 0, 0) 
	 * pw: your standing position in the world 
	 */
	setVector(&ps, 0, 0, 0);
	screen_to_world(&ps, &pw);
	
	/* get the cross point between line pw->vw and plane z = 0 */
	for (i = 0; i < 4; i++) {
		
		/* get clip window position in the world */
		screen_to_world(&vs[i], &vw[i]);
		
		/* if cross point is overflow, adjust */
		if (get_z_cross_point(&pw, &vw[i], &ww[i], limit)) {

			/* return on  screen screen coordinate */
			rot_trans_pers(&ww[i], &vs[i]);

			/* change the clip window */
			limitRange(vs[i].vx, min_x-am, max_x+am);
			limitRange(vs[i].vy, min_y-am, max_y+am);

			/* get the world position of the new clip window */
			screen_to_world(&vs[i], &vw[i]);

			/* retry */
			get_z_cross_point(&pw, &vw[i], &ww[i], limit);
		}
	}
	/*draw_rotline4(255, &ww[0], &ww[1], &ww[3], &ww[2]);*/
		
	/* set the clip window on plane z = 0 */
	clipw->x = min4(ww[0].vx, ww[1].vx, ww[2].vx, ww[3].vx); 
	clipw->y = min4(ww[0].vy, ww[1].vy, ww[2].vy, ww[3].vy); 
	clipw->w = max4(ww[0].vx, ww[1].vx, ww[2].vx, ww[3].vx) - clipw->x; 
	clipw->h = max4(ww[0].vy, ww[1].vy, ww[2].vy, ww[3].vy) - clipw->y; 

	return(0);
}

/*
 * translate from the screen coordinate to the world coordinate.
 : スクリーン座標からワールド座標を求める
 */
/* SVECTOR *s;		(input)  position in the screen coordinate
 * SVECTOR *w;		(output) position in the world coordinate
 */
static  void screen_to_world(SVECTOR *s, SVECTOR *w)
{
	MATRIX		rot, rott;
	SVECTOR		t;
	
	/* get current rot-trans matrix */
	ReadRotMatrix(&rot);
	
	/* push matrix since ApplyMatrixSV breaks the GTE matrix register */
	PushMatrix();
	
	/* screen = Rt * (world - t); */
	t.vx = s->vx - rot.t[0];
	t.vy = s->vy - rot.t[1];
	t.vz = s->vz - rot.t[2];
	
	/* If rotation matrix is "unitary", transposed matrix is equal to
	 * inversed matrix.   
	 */
	TransposeMatrix(&rot, &rott);
	ApplyMatrixSV(&rott, &t, w);

	/* recover matrix */
	PopMatrix();
}

/*
 * Calculate the cross point between line pw->vw and plane z = 0.
 * if the cross point is very far, it is limited in "limit".
 : 点 pw と vw を結ぶ直線と平面 z = 0 との交点をもとめる。
 * もし交点が十分遠ければ、limit に制限される
 */
/* SVECTOR *pw;	(input)  home poisition int the world
 * SVECTOR *vw;	(input)  screen edge position int the world
 * SVECTOR *ww: (output) cross point on the z = 0 plane
 * int limit;	(input)  far clip
 */
#define HUGE	0x7fff		/* max value of 16bit: 16bit の最大値 */
static get_z_cross_point(SVECTOR *pw, SVECTOR *vw, SVECTOR *ww, int limit)
{
	int ret = 0;
	long x, y;
	
	/* if the line doesn't cross with the plane, clip the result */
	if (vw->vz - pw->vz >= 0) {
		x = pw->vx + (long)pw->vz*(vw->vx-pw->vx);
		y = pw->vy + (long)pw->vz*(vw->vy-pw->vy);
		/*FntPrint("overflow(%d,%d)\n", x, y);*/
	}
	/* if there is a cross point, then calculate */
	else {
		x = pw->vx - (long)pw->vz*(vw->vx-pw->vx)/(vw->vz-pw->vz);
		y = pw->vy - (long)pw->vz*(vw->vy-pw->vy)/(vw->vz-pw->vz);

	}
	/* limit in 16bit because ww is "short" type vector */
	limitRange(x, -HUGE, HUGE);	ww->vx = x;
	limitRange(y, -HUGE, HUGE);	ww->vy = y;
	ww->vz = 0;
	
	/* clip in "limit" value, if clip is operateted, return -1. */
	if (ww->vx < pw->vx-limit) ret = -1, ww->vx = pw->vx-limit;
	if (ww->vx > pw->vx+limit) ret = -1, ww->vx = pw->vx+limit;
	if (ww->vy < pw->vy-limit) ret = -1, ww->vy = pw->vy-limit;
	if (ww->vy > pw->vy+limit) ret = -1, ww->vy = pw->vy+limit;

	/* normal return */
	return(ret);
}

/*
 * translate from the world coordinate to the screen coordinate with
 * perspective translation.
 * Not that this function is not influenced by GTE offset
 : ワールド座標からスクリーン座標変換したのち透視変換を行なう
 * これは、GTE オフセットに影響されないことに注意
 */
/*
 * SVECTOR *w:	(input)  position in thw world
 * SVECTOR *s;	(output) position in the screen
 */
static void rot_trans_pers(SVECTOR *w, SVECTOR *s)
{
	long	ofx, ofy;
	long	flg, p;
	
	/* push the current GTE offset */
	ReadGeomOffset(&ofx, &ofy);

	/* clear the GTE offset to 0 */
	SetGeomOffset(0, 0);

	/* rotation-translation-perspective translation */
	RotTransPers(w, (long *)s, &p, &flg);

	/* pop the previous GTE offset */
	SetGeomOffset(ofx, ofy);
}
