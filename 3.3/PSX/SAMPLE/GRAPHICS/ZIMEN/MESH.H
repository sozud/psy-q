/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *				mesh.h
 *
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved;
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Nov,17,1993	suzu	
 *	2.00		Jan,17,1994	suzu	(rewrite)
 *
 *			    header for mesh
 :		       メッシュデータ用ヘッダ
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * Mesh cell geometry information
 : メッシュの各セルのジオメトリ情報
 */
typedef struct {
	SVECTOR	x3;		/* 3D point */
	SVECTOR	x2;		/* 2D point (after perspective trans.) */
	long	p;		/* depth queue */
	long	flg;		/* clip flag */
	u_short	clip;		/* is in clip area */
	
} MCELL;

/*
 * Cell charactor type.
 : メッシュで使用するキャラクタマップ
 */
typedef struct {
	u_char	u, v;		/* cell texture uv */
	u_char	du,dv;		/* texture width, height */
	u_short	tpage;		/* texture page */
	u_short	nclut;		/* number of CLUTs (for fog)  */
	u_short	*clut;		/* cell clut */
	u_long	attr;		/* attribute (reserved) */
} CTYPE;

/*
 * MESH structure
 * Note: Map size (mx,my) have to be 2^n.
 : メッシュ構造体
 * マップサイズ (mx,my) は２のべき乗を指定すること
 */
typedef struct {
	MCELL	*cell;		/* mesh CELL */
	CTYPE	*ctype;		/* mesh character type */
	char	**map;		/* map */
	int	nx, ny;		/* cell size */
	int	ux, uy;		/* cell unit size */
	int	mx, my;		/* map size */
	int	ox, oy;		/* mesh offset */
	SVECTOR	x3;		/* map position of the right-upper corner */
} MESH;
	
/*
 * prototypes
 */
void meshInit(MESH *mesh);
void meshRotTransPers(u_long *ot, int otsize, MESH *mesh, POLY_FT4 *buf,
		       void (*addfunc)(), RECT *clips, RECT *clipw);


