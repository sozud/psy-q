/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				divproto.h
 *
 *		Copyright (C) 1993/1994 by Sony Corporation
 *			All rights Reserved
 *
 */
#include "mesh.h"
/*
 * Screen basic information
 */
#define	SCR_X	(640/2)				/* screen width */
#define	SCR_Y	(480/2)				/* screen width */
#define SCR_Z	(SCR_W)				/* distance to screen */
#define SCR_W	(SCR_X*2)
#define SCR_H	(SCR_Y*2)
#define FOGNEAR	1536				/* fog start point */
#define FOGFAR	2048				/* fog limit point */
#define OTSIZE	4096				/* ordering table size */
#define MAXHEAP	(1024*sizeof(POLY_FT4)/4)	/* primitive buffer */
#define MIN_SIZ	(128*128)			/* minimus primitive size */

/*
 * Geometry Attributes;
 */
typedef struct {
	SVECTOR	angle;		/* local-screen angle */
	SVECTOR	edge;		/* lowest edge of the screen */
	SVECTOR	home;		/* current home position */
	SVECTOR	offset;		/* screen offst */
	MATRIX	mat;		/* local-screen matrix */
} GEOMENV;
extern GEOMENV	GeomEnv;

/*
 * get min/max
 */
#define max(x,y)	((x)>(y)?(x):(y))
#define max3(x,y,z)	((x)>(y)?max(x,z):max(y,z))
#define max4(x,y,z,w)	(max(x,y)>max(z,w)?max(x,y):max(z,w))

#define min(x,y)	((x)<(y)?(x):(y))
#define min3(x,y,z)	((x)<(y)?min(x,z):min(y,z))
#define min4(x,y,z,w)	(min(x,y)<min(z,w)?min(x,y):min(z,w))

/*
 * dump parameters
 */
#define dumpVector(str, v)	\
	GPU_printf("%s=(%d,%d,%d)\n", str, (v)->vx, (v)->vy, (v)->vz)

#define dumpMatrix(x)	\
	GPU_printf("\t%5d,%5d,%5d\n",(x)->m[0][0],(x)->m[0][1],(x)->m[0][2]),\
	GPU_printf("\t%5d,%5d,%5d\n",(x)->m[1][0],(x)->m[1][1],(x)->m[1][2]),\
	GPU_printf("\t%5d,%5d,%5d\n",(x)->m[2][0],(x)->m[2][1],(x)->m[2][2])

/*
 * prototypes
 */
void divPolyClip(RECT *rect, int t);
POLY_FT4 *divPolyFT4(u_long *ot, SVECTOR *x0, SVECTOR *x1, SVECTOR *x2,
		     SVECTOR *x3, POLY_FT4 *tmpl, int ndiv, POLY_FT4 *heap,
		     u_long *scratch, long flg);

POLY_FT4 *divPolyFT4_2(u_long *ot, SVECTOR *x0, SVECTOR *x1, SVECTOR *x2,
		     SVECTOR *x3, POLY_FT4 *tmpl, int ndiv, POLY_FT4 *heap,
		     u_long *scratch, long flg);

int disp_clipw(RECT *rect);
int disp_clips(RECT *rect);
int draw_line4(u_char col,
		     SVECTOR *x0, SVECTOR *x1, SVECTOR *x2, SVECTOR *x3);
int zimenUpdate(u_long padd, int rxmask, int rymask);
int areaClipZ(RECT *clips, RECT *clipw, int limit);





