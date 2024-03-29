/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				div
 *				
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Nov,17,1993	suzu	
 *	2.00		Jan,17,1994	suzu	(rewrite)
 *	3.00		Sep,17,1995	suzu	(rewrite)
 *
 *			    sub-divide
 *			  (dmpsx version)
 :		   プリミティブを分割して登録
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <inline.h>
#include <gtemac.h>
/*	
 * Function "divPolyFT4()" divides 1 POLY_FT4 into 4 POLY_FT4s. 
 * The function is called recursively when more division is required, and
 * store common data in scratch-pad area for fast memory access.
 * If you want faster (but limited freedom) routine, use DivPolyFT4().
 * The vertex of divided primitives are numbered as follows:
 :	 
 * プリミティブを４分割する。ノード番号は以下の通り
 * ４分割以上はリカーシブコールを行なう。	
 * ワークエリアにはスクラッチパッドを使用する。
 * より高速な分割が必要な場合は、DivPolyFT4() を使用すること。
 * 分割後のプリミティブのノード番号は以下の通り。	
 * 
 *	0-------4-------1
 *	|	|	|
 *	5-------8-------6
 *	|	|	|
 *	2-------7-------3
 */

/*
 * divide a POLY_FT4 into 4 POLY_FT4s
 : 一つの POLY_FT4 プリミティブを４つに分割する
 */
#define div_prim(p0, p1, p2, p3)					\
	if ((div[p0].flg&div[p1].flg&div[p2].flg&div[p3].flg)==0){	\
		div[ 9] = div[p0],					\
		div[10] = div[p1],					\
		div[11] = div[p2],					\
		div[12] = div[p3],					\
		_divPolyFT4(div+9, ndiv-1, normalclip(div+9));	   	\
	} 

/*
 * RotTransPers (macro version)
 */
#define rtp(v)		{gte_ldv0(v);  gte_rtps();}

/*
 * NormalClip (macro version)
 */
#define nclip(v0,v1,v2)	\
	{gte_ldsxy3(*(long *)(v0),*(long *)(v1),*(long *)(v2));gte_nclip();}

/*
 * sigle u,v
 */
typedef struct {
	u_char	u0, v0;		
} UV;

/*
 * vertex attribute
 */
typedef struct {		
	SVECTOR	sv;	/* position in the world */
	DVECTOR	dv;	/* position in after RotTransPers */
	UV	uv;	/* texture UV */
	u_short	flg;	/* GTE flag (This indicates reliabilty of dv) */
} VERTEX;

/*
 * Another presentation of POLY_FT4
 */
typedef struct {
	u_long	tag;
	u_long	code;
	DVECTOR	dv0;
	UV	uv0;	u_short clut;
	DVECTOR	dv1;
	UV	uv1;	u_short tpage;
	DVECTOR	dv2;
	UV	uv2;	u_short pad1;
	DVECTOR	dv3;
	UV	uv3;	u_short pad2;
} FT4;

static u_long	*ot;		/* ordering table */
static FT4	*tmpl;		/* primitive templete */
static FT4	*heap;		/* heap pointer */
	

/* set clip area for division: クリップ範囲を設定する		*/
/* RECT *rect	clip area: クリップ範囲				*/
/* clip area is adjusted by GTE offset
 : クリップエリアは GTE オフセット値が自動的に加えられる
 */
int	min_x = -320, min_y = -240, max_x = 320, max_y = 240, th = 128*128;
void divPolyClip(RECT *rect, int t)
{
	long	ofx, ofy;
	ReadGeomOffset(&ofx, &ofy);

	min_x = rect->x+ofx; max_x = rect->x+rect->w+ofx;
	min_y = rect->y+ofy; max_y = rect->y+rect->h+ofy;
	th = t;
}

/* divide primitives and register to OT: 分割ポリゴンを OT に登録	*/
/* u_long *ot;		OT						*/
/* SVECTOR *x0...*x3;	vertex of the polygon				*/
/* POLY_FT4 *tmpl;	templete: 分割するプリミティブテンプレート	*/
/* int ndiv;		division order: 分割オーダー			*/
/* POLY_FT4 *heap;	primitive buffer: プリミティブバッファ		*/
/* u_long *scratch:	scratch pad address: スクラッチパッド 		*/

/* Note:
 * 	Primitive is divided in 4^ndiv subprimitevs.
 *	Divsion is interrupted if the size divided primitive is smaller
 *	than threshold. The size of the primitive is estimated by
 *	'NormalClip.
 *	Divided primitives are added to the same OT entry 'ot'.
 *	
 :	プリミティブは、4^ndiv 個に分割される。
 *	分割されたプリミティブの面積が一定値より小さくなれば分割はそこで
 *	中断される。プリミティブの面積は NormalClip で計算できる。
 *	分割されたプリミティブは、すべて同じ OT のエントリに登録される
 */

void _divPolyFT4(VERTEX *div, int ndiv, long d);
void set_flag(VERTEX *dp);
void half_vert(VERTEX *d0, VERTEX *d1, VERTEX *d2);
long normalclip(VERTEX *div);
void add_prim(VERTEX *d0, VERTEX *d1, VERTEX *d2, VERTEX *d3);

POLY_FT4 *divPolyFT4(u_long *_ot,
		     SVECTOR *x0, SVECTOR *x1, SVECTOR *x2, SVECTOR *x3,
		     POLY_FT4 *_tmpl, int ndiv, POLY_FT4 *_heap,
		     u_long *scratch, long flg)
{
	VERTEX	*div;

	tmpl  = (FT4    *)scratch; scratch += sizeof(POLY_FT4)/4;
	div   = (VERTEX *)scratch;
	ot    = _ot;
	*tmpl = *(FT4 *)_tmpl;
	heap  =  (FT4 *)_heap;
	
	div[0].sv = *x0; div[0].dv = tmpl->dv0; div[0].uv = tmpl->uv0;
	div[1].sv = *x1; div[1].dv = tmpl->dv1; div[1].uv = tmpl->uv1;
	div[2].sv = *x2; div[2].dv = tmpl->dv2; div[2].uv = tmpl->uv2;
	div[3].sv = *x3; div[3].dv = tmpl->dv3; div[3].uv = tmpl->uv3;
	
	set_flag(&div[0]);
	set_flag(&div[1]);
	set_flag(&div[2]);
	set_flag(&div[3]);
	
	_divPolyFT4(div, ndiv, normalclip(div));
	
	return((POLY_FT4 *)heap);
}

void _divPolyFT4(VERTEX *div, int ndiv, long d)
{
	/* detect error
	 *	max destortion: 16
	 */
	if (d < 0) d = -d;
	if (d  < th) {
		add_prim(&div[0], &div[1], &div[2], &div[3]); 
		return;
	}
	if (d < th<<2)
		ndiv = 1;
	
	/* RotTransPers of half vectors */
	/* 	local flag
	 *	bit0:	1: x < min_x	
	 *	bit1:	1: x > max_x	
	 *	bit2:	1: y < min_y	
	 *	bit3:	1: y > max_y
	 */
	half_vert(&div[4], &div[0], &div[1]);
	rtp(&div[4].sv);
	
	half_vert(&div[5], &div[0], &div[2]);			/* CPU */
	gte_stsxy(&div[4].dv); rtp(&div[5].sv);			/* GTE */
	
	set_flag(&div[4]); half_vert(&div[6], &div[1], &div[3]);/* CPU */
	gte_stsxy(&div[5].dv); rtp(&div[6].sv);			/* GTE */
	
	set_flag(&div[5]); half_vert(&div[7], &div[2], &div[3]);/* CPU */
	gte_stsxy(&div[6].dv);	rtp(&div[7].sv);		/* GTE */
	
	set_flag(&div[6]); half_vert(&div[8], &div[5], &div[6]);/* CPU */
	gte_stsxy(&div[7].dv); rtp(&div[8].sv);			/* GTE */
	
	set_flag(&div[7]);					/* CPU */
	gte_stsxy(&div[8].dv);					/* GTE */
	
	set_flag(&div[8]);					/* CPU */
	
	/* addprim */
	if (ndiv == 1) {
		add_prim(&div[0], &div[4], &div[5], &div[8]); 
		add_prim(&div[4], &div[1], &div[8], &div[6]); 
		add_prim(&div[5], &div[8], &div[2], &div[7]); 
		add_prim(&div[8], &div[6], &div[7], &div[3]); 
	}
	else {
		div_prim(0, 4, 5, 8); 
		div_prim(4, 1, 8, 6); 
		div_prim(5, 8, 2, 7); 
		div_prim(8, 6, 7, 3);
	}
}

/*
 * set min-max flag
 */
void set_flag(VERTEX *dp)
{
	short f = 0;
	
	if (dp->dv.vx < min_x) f |= 0x01;
	if (dp->dv.vx > max_x) f |= 0x02;
	if (dp->dv.vy < min_y) f |= 0x04;
	if (dp->dv.vy > max_y) f |= 0x08;
	dp->flg = f;
}

/*
 * calculate half vector
 */
void half_vert(VERTEX *d0, VERTEX *d1, VERTEX *d2)
{
	d0->sv.vx = (long)(d1->sv.vx+d2->sv.vx+1)>>1;
	d0->sv.vy = (long)(d1->sv.vy+d2->sv.vy+1)>>1;
	d0->sv.vz = (long)(d1->sv.vz+d2->sv.vz+1)>>1;	
	
	d0->uv.u0 = (long)(d1->uv.u0+d2->uv.u0+1)>>1;
	d0->uv.v0 = (long)(d1->uv.v0+d2->uv.v0+1)>>1;
}


/*
 * normal clip
 */
#define HUGE	0x7fffffff
long normalclip(VERTEX *div)
{
	long f0, f1, d0, d1;
	
	nclip(&div[0].dv, &div[1].dv, &div[2].dv);
	gte_stopz(&d0); gte_stflg(&f0);
	nclip(&div[3].dv, &div[2].dv, &div[1].dv);
	gte_stopz(&d1); gte_stflg(&f1);
	
	return(f0&f1? HUGE: d0+d1);
}

/*
 * register POLY_FT4 primitive
 */
void add_prim(VERTEX *d0, VERTEX *d1, VERTEX *d2, VERTEX *d3)
{
	if ((d0->flg&d1->flg&d2->flg&d3->flg) == 0) {
		tmpl->dv0 = d0->dv;	tmpl->uv0 = d0->uv;
		tmpl->dv1 = d1->dv;	tmpl->uv1 = d1->uv;
		tmpl->dv2 = d2->dv;	tmpl->uv2 = d2->uv;
		tmpl->dv3 = d3->dv;	tmpl->uv3 = d3->uv;
		
		*heap = *tmpl;
		addPrim(ot, heap);					
		heap++;							
	}
}
		
		
