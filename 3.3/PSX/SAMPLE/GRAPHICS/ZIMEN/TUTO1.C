/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *				tuto0
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved;
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Jun,19,1995	suzu	
 *
 *			the ground
 :		           ’n–Ê
 */

#include "tuto.h"
#include "bgmap.h"

/*
 * if you want to see the clipping operation, define VIEW_CLIP
 */
#ifdef VIEW_CLIP
static int view_clip = 1;
#else
static int view_clip = 0;
#endif
/*
 * if you want to the rapp-rounded BG, define RAP_ROUND
 */
#ifdef RAP_ROUND
static int rap_round = 1;
#else
static int rap_round = 0;
#endif

/*
 * mesh environment
 */
#ifdef NO_DIV
#define CL_SUX	7			/* cell size mask (width) */
#define CL_SUY	7			/* cell size mask (height) */
#else
#define CL_SUX	9			/* cell size mask (width) */
#define CL_SUY	9			/* cell size mask (height) */
#endif
#define CL_UX	(1<<CL_SUX)		/* cell size (widht) */	
#define CL_UY	(1<<CL_SUY)		/* cell size (height) */
#define CL_NX	BG_MAPX			/* number of cells (x) */
#define CL_NY	BG_MAPY			/* number of cells (y) */

/*
 * grobal definitions
 */
typedef struct {		
	u_long		ot[OTSIZE];		/* ordering table */
	POLY_FT4	cell[CL_NY*CL_NX];	/* mesh cell */
	u_long		heap[MAXHEAP];		/* heap */
} DB;

static MCELL	_mcell[(CL_NY+1)*(CL_NX+1)];
static MESH	mesh = {
	_mcell,					/* mesh cells */
	_ctype,					/* charactor type */
	_map,					/* map */
	CL_NX+1,        CL_NY+1,		/* cell size */
	CL_UX,          CL_UY,			/* cell unit size */
	BG_MAPX,	BG_MAPY,		/* bg map size */
	0,	        0,			/* messh offset */
	-CL_NX*CL_UX/2, -CL_NY*CL_UY/2, 0,	/* right upper corner */
};
	
	
static void init_prim(DB *db);
static void reset_prim(POLY_FT4 *p);
static void set_heap(u_long *p);
static void div_addprim(u_long *ot, MCELL *mp, POLY_FT4 *p);

extern int enter_cnt;
int is_triangle = 0;
int is_div = 1;

main()
{
	DB	db[2];			/* double buffer */
	DB	*cdb;			/* current double buffer */
	RECT	clips, clipw;		/* clip area */
	u_long	*ot;			/* current OT */
	u_long	padd, opadd = 0;
	int	cnt;
	int	ox, oy;
	
	/* initialize double buffer */
	db_init(SCR_W, SCR_H, SCR_Z, 0, 0, 0);
	
	/* initialize fog parameters */
	SetFarColor(0, 0, 0);
	SetFogNearFar(FOGNEAR, FOGFAR, SCR_Z);	
	init_prim(&db[0]);		/* initialize primitive buffers #0 */
	init_prim(&db[1]);		/* initialize primitive buffers #1 */
	
	/* set clip area */
	if (view_clip)
		setRECT(&clips, -SCR_X/2, -SCR_Y/2,  SCR_W/2, SCR_H/2);
	else
		setRECT(&clips, -SCR_X, -SCR_Y, SCR_W, SCR_H);

	/* display start */
	SetDispMask(1);		
	
	/* set subdivision clip area */
	divPolyClip(&clips, MIN_SIZ);
	
	/* initialize  mesh */
	meshInit(&mesh);

	while (((padd = PadRead(1))&PADselect) == 0) {
		
		if (opadd == 0 &&  padd & PADstart)
			is_triangle  = (is_triangle +1)&0x01;
		opadd = padd;
		
		cdb = (cdb==db)? db+1: db;	/* swap double buffer ID */
		ClearOTagR(cdb->ot, OTSIZE);	/* clear ordering table */
		
		/* get clip area on the ground */
		cnt = VSync(1);
		areaClipZ(&clips, &clipw, FOGFAR);
		
		/* rot-trans-pers */
		if (rap_round) {
			mesh.ox = GeomEnv.offset.vx>>CL_SUX;
			mesh.oy = GeomEnv.offset.vy>>CL_SUY;
		}
#ifdef NO_DIV		
		/* rot-trans-pers MESH without subdivision */
		meshRotTransPers(cdb->ot, OTSIZE, &mesh, cdb->cell,
				  0, &clipw, &clips);
#else
		/* set heap buffer for subdivision */
		set_heap(cdb->heap);
		
		/* rot-trans-pers MESH with subdivision */
		meshRotTransPers(cdb->ot, OTSIZE, &mesh, cdb->cell,
				  div_addprim, &clipw, &clips);
#endif		
		FntPrint("t=%d\n", VSync(1)-cnt);
		
		/* update geometry */
		if (rap_round)
			zimenUpdate(padd, CL_UX, CL_UY);
		else
			zimenUpdate(padd, 0, 0);
			
		if (view_clip) {
			disp_clips(&clips);
			disp_clipw(&clipw);
		}
		
		/* swap double buffer */
		db_swap(cdb->ot+OTSIZE-1);
	}
	PadStop();
	StopCallback();
	return;
}

static void init_prim(DB *db)
{
	extern		u_long	bgtex[];	/* BG textureECLUT (4bit) */
	static u_short	clut, tpage;
	int		x, y, i;
	POLY_FT4	*p;
	CTYPE		*ctype;
	
	/* load texture */
	tpage = LoadTPage(bgtex+0x80, 0, 0, 640, 0, 256, 256);
	clut  = LoadClut(bgtex, 0, 480);
	
	/* set ctyep */
	for (ctype = mesh.ctype; ctype->u != 255; ctype++) {
		ctype->tpage = tpage;	/* tpage is common */
		ctype->clut  = &clut;	/* clut is common */
		ctype->nclut = 1;	/* 1 clut is used per 1 cell */	
	}
	
	/* set code */
	for (p = db->cell, i = 0; i < CL_NX*CL_NY; i++, p++) {
		SetPolyFT4(p);
		setRGB0(p, 128, 128, 128);
	}
}

/*
 *	SubDivision
 */
static POLY_FT4	*heap, *heap_base;
static POLY_FT4	tmpl;

static void set_heap(u_long *p)
{
	SetPolyFT4(&tmpl);
	setRGB0(&tmpl, 0x80, 0x80, 0x80);
	heap = (POLY_FT4 *)p;
}

#define NDIV	3
static void div_addprim(u_long *ot, MCELL *mp, POLY_FT4 *p)
{
	long z, flg;
	
	/* depth queue */
	DpqColor((CVECTOR *)&tmpl.r0, mp[0].p, (CVECTOR *)&p->r0);
		
	/* select subdivision */
	z = mp[0].x2.vz+mp[1].x2.vz+mp[CL_NX+1].x2.vz+mp[CL_NX+2].x2.vz;
	
	/* if subdivision is required, call divPolyFT4 */
	if (z < SCR_Z*2) {
		heap = divPolyFT4(ot,
				  &mp[      0].x3, &mp[      1].x3,
				  &mp[CL_NX+1].x3, &mp[CL_NX+2].x3,
				  p, NDIV, heap, getScratchAddr(0), flg);
	}
	else
		AddPrim(ot, p);
}

