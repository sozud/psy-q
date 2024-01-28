/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *				mesh
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved;
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Nov,17,1993	suzu	
 *	2.00		Jan,17,1994	suzu	(rewrite)
 *	3.00		Jun,19,1995	suzu	(rewrite)
 *
 *			    mesh handler
 :		       メッシュデータの取り扱い
 */
#include <sys/types.h>
#include "mesh.h"

/*
 * initialize MESH structure: メッシュ構造体を初期化する
 */
void meshInit(MESH *mesh)
{
	MCELL	*mp;
	int	x, y, tx, ty;
	int	dtx   = mesh->ux;
	int	dty   = mesh->uy;
		
	mp = mesh->cell;
	for (y = 0, ty = mesh->x3.vy; y < mesh->ny; y++, ty += dty) {
		for (x = 0, tx = mesh->x3.vx; x < mesh->nx; x++, tx += dtx) {
			mp->x3.vx = tx;
			mp->x3.vy = ty;
			mp->x3.vz = 0;
			mp++;
		}
	}
}
		
/*
 * RotTransPers for MESH (with AddPrim())
 * You can specify your own AddPrim() function for primitive subdivision.
 * Primitive buffer must be initialized by SetPolyFT4() at first.
 * If clipw is NULL, clipping in the world is skipped. 
 * If clips is NULL, clipping in the screen is skipped. 
 * If addfunc is NULL, conventional AddPrim() is used to add to OT 
 * You should use GTE MESH functions for faster operation.
 *
 : MESH を RotTransPers() する。ついでに AddPrim() する。
 * 分割用に専用の AddPrim() を外から指定できる。
 * プリミティブバッファはあらかじめ初期化しておくこと。
 * clipw, clips が NULL の場合はクリップ処理は飛ばされる。	
 * addfunc が NULL の場合は、通常の AddPrim() が使用される	
 * より高速にするためには、GTE のメッシュ関数を使用すること
 */
/* ot;		(in)  OT entry: ＯＴエントリ			*/
/* otsize;	(in)  OT size: ＯＴサイズ			*/
/* mesh;	(in)  MESH: メッシュ				*/
/* buf:		(out) primitive buffer: プリミティブバッファ	*/
/* (*addfunc)()	(in)  addprim function: AddPrim 関数ポインタ	*/
/* clipw	(in)  clip window on the plane: 平面上のクリップ窓 */
/* clips	(in)  clip window in the screen: スクリーン上のクリップ窓 */

void meshRotTransPers(u_long *ot, int otsize, MESH *mesh, POLY_FT4 *buf,
	       void (*addfunc)(), RECT *clipw, RECT *clips)
{
	static	CVECTOR	col = {0x80,0x80,0x80};
	MCELL	*mp, *mp0;
	CTYPE	*ctype;
	POLY_FT4 *p, *p0;

	int	nx    = mesh->nx;	/* mesh number */
	int	ny    = mesh->ny;
	int	ox    = mesh->ox;	/* mesh right-upper corner position */
	int	oy    = mesh->oy;
	int	msk_x = mesh->mx-1;	/* map rap-round mask */
	int	msk_y = mesh->my-1;
	
	long	x, y, z;		/* work */
	int	wsx, wsy, wex, wey;	/* clip window in the world */
	int	ssx, ssy, sex, sey;	/* clip window in the screen */
	char	*map;
	int	flg;
	
	/* set clip rectangle in the world coordinate 
	 * clip value is quantized by cell size (mesh->ux, mesh->uy)
 	 */	 
	if (clipw) {
		wsx = (clipw->x-mesh->x3.vx)/mesh->ux;
		wex = (clipw->x+clipw->w-mesh->x3.vx)/mesh->ux+1;
		wsx = limitRange(wsx, 0, nx-1);
		wex = limitRange(wex, 0, nx-1);
		
		wsy = (clipw->y-mesh->x3.vy)/mesh->uy;
		wey = (clipw->y+clipw->h-mesh->x3.vy)/mesh->uy+1;
		wsy = limitRange(wsy, 0, ny-1);
		wey = limitRange(wey, 0, ny-1);
	}
	/* if clip window is not specified, use default */
	else {
		wsx = wsy = 0;
		wex = nx-1;
		wey = ny-1;
	}

	/* set clip rectangle in the screen coordinate */
	if (clips) {
		
		ReadGeomOffset(&x, &y);	/* add the current GTE offset */
		
		ssx = x+clips->x;		ssy = y+clips->y;
		sex = x+clips->x+clips->w;	sey = y+clips->y+clips->h;
	}
	/* if clip window is not specified, use default */
	else {
		ssx = ssy = 0;
		sex = 640; sey = 480;
	}
	
	/*
	 * RotTransPers.
	 * RotTransPers is calculated only in clipw rectangle area.
	 */
	mp0 = &mesh->cell[nx*wsy+wsx];
	p0  = &buf[(nx-1)*wsy+wsx];
	for (y = wsy; y < wey+1; y++, mp0 += nx, p0 += nx-1) 
		for (mp = mp0, p = p0, x = wsx; x < wex+1; x++, mp++, p++) {
			
			/* At least you shoud use RotTransPersN instead
			 * of singel RotTransPers()
			 */
			mp->x2.vz = RotTransPers(&mp->x3, 
					 (long *)&mp->x2, &mp->p, &mp->flg);
			
			/* If the translated point is out of range,
			 *  set clip flag.
			 */
			mp->clip = 0;
			if (mp->x2.vx > sex)	mp->clip |= 0x01;
			if (mp->x2.vx < ssx)	mp->clip |= 0x02;
			if (mp->x2.vy > sey)	mp->clip |= 0x04;
			if (mp->x2.vy < ssy)	mp->clip |= 0x08;
		}
			
	/* add to OT */
	col.cd = buf->code;
	mp0 = &mesh->cell[nx*wsy+wsx];
	p0  = &buf[(nx-1)*wsy+wsx];
	for (y = wsy; y < wey; y++, mp0 += nx, p0 += nx-1) {
		map = mesh->map[(y+oy)&msk_y];
		for (mp = mp0, p = p0, x = wsx; x < wex; x++, mp++, p++) {
			
			/* if all 4 point is out of range, skip */
			if (mp[0].clip&mp[1].clip&mp[nx].clip&mp[nx+1].clip)
				continue;

			/* if z is out of range, break */
			if ((z = mp[0].x2.vz) >= otsize || z < 0)
				continue;
			
			/* set vertex (x0,y0)-(x3,y3) */
			setXY4(p,
			       mp[   0].x2.vx, mp[   0].x2.vy,
			       mp[   1].x2.vx, mp[   1].x2.vy,
			       mp[  nx].x2.vx, mp[  nx].x2.vy,
			       mp[nx+1].x2.vx, mp[nx+1].x2.vy);

			/* get current character type .
			 * Character type table is rap-rounded by (msk_x,msk_y)
			 */
			ctype =	&mesh->ctype[map[(x+ox)&msk_x]-'0'];

			setUVWH(p, ctype->u, ctype->v,
				ctype->du-1, ctype->dv-1);
			
			p->tpage = ctype->tpage;
			p->clut  = *ctype->clut;

			/* add to OT */
			if (addfunc == 0) {
				DpqColor(&col, mp[0].p, (CVECTOR *)&p->r0);
				AddPrim(ot+z, p);
			}

			/* if addfunc is specified, jump to user function */
			else 
				(*addfunc)(ot+z, mp, p);
		}
	}
}
