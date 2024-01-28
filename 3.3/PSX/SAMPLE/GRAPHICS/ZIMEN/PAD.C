/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				pad
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Jun,20,1995	suzu	
 *
 *		Generate matrix for the ground drawing
 :			地面用マトリクス
 */
#include "tuto.h"

/*
 * If you want the top down view, define HOME
 */
/*#define HOME*/
#ifdef HOME
GEOMENV	GeomEnv = {
	2048,     0,     0, 0,		/* local-screen angle */
	0,  SCR_Y/2, SCR_Z, 0,		/* loowest edge of the view */
	0,        0, SCR_Z, 0,		/* current position */
	0,        0, SCR_Z, 0,		/* current position */
};
#else
GEOMENV	GeomEnv = {
	1024,     0,  -512, 0,	/* local-screen angle */
	0,  SCR_Y/2, SCR_Z, 0,	/* loowest edge of the view */
	256,   -256,     0, 0,	/* current position */
	256,   -256,     0, 0,	/* current position */
};
#endif

/*
 * update the world-screen matrix with rap-rounded by (rx, ry)
 * Rap-rounding is needed for rap-rounded 3D BG mode.
 : ワールドスクリーンマトリクスを更新する。その際に (rx,ry) でラップラウンド
 * する。ラップラウンドは、繰り返し BG に必要
 */
/* u_long padd;		controller input: コントローラの入力
 * int rx, ry;	  	rap-round mask: ラップラウンド値
 */
int zimenUpdate(u_long padd, int rx, int ry)
{
	SVECTOR	dvs;	/* moving direction (in the screen) */
	SVECTOR dvw;	/* moving direction (in the screen) */
	SVECTOR	edgew;	/* the lowest edge of the screen (in the world) */
	SVECTOR	home;	/* your position (in the world) */
	VECTOR	vec;	/* translation vector */
	MATRIX	rott;	/* inversed (transposed) rotation matrix */
	
	/* clear direction vector */
	setVector(&dvs, 0, 0, 0);
	
	/* rotate in x direction (right and left) */
	if (padd & PADRup)	GeomEnv.angle.vx += -8;
	if (padd & PADRdown)	GeomEnv.angle.vx +=  8;
	
	/* rotate in z direction (up and down) */
	if (padd & PADRleft) 	GeomEnv.angle.vz += -8;
	if (padd & PADRright)	GeomEnv.angle.vz +=  8;
	
	/* rotate in y direction */
	if (padd & PADR1)	GeomEnv.angle.vy +=  8;
	if (padd & PADR2) 	GeomEnv.angle.vy += -8;
	
	/* move in x direction (right and left) */
	if (padd & PADLleft)	dvs.vx =  16;
	if (padd & PADLright) 	dvs.vx = -16;

	/* move in z direction (foreward and backward) */
	if (padd & PADLup)	dvs.vz =  16;
	if (padd & PADLdown) 	dvs.vz = -16;
	
	/* move in the world z position (up and down) */
	if (padd & PADL1)	GeomEnv.home.vz +=  8;
	if (padd & PADL2) 	GeomEnv.home.vz += -8;
	
	/* limit angle of your neck */
	limitRange(GeomEnv.angle.vx, 1024, 3072); 
	
	/* make local-screen matrix */
	RotMatrix(&GeomEnv.angle, &GeomEnv.mat);		

	/* get inverse matrix of local-screen matrix. 
	 * note that the transposed matrix is equal to inversed matrix,
	 * if the matrix is "unitary".
	 */
	TransposeMatrix(&GeomEnv.mat, &rott);

	/* get the world position of lower screen edge */
	ApplyMatrixSV(&rott, &GeomEnv.edge, &edgew);

	/* limit your z (height) position in the world */
	GeomEnv.home.vz = max(GeomEnv.home.vz, -edgew.vz);
	
	/* translate the direction vector from the screen to the world */
	ApplyMatrixSV(&rott, &dvs, &dvw);

	/* update your position (in the world) */
	GeomEnv.home.vx += dvw.vx;
	GeomEnv.home.vy += dvw.vy;
	
	/* rapp-round (if needed) */
	if (rx) {
		GeomEnv.offset.vx += dvw.vx;
		GeomEnv.home.vx &= rx-1;
	}
	if (ry) {
		GeomEnv.offset.vy += dvw.vy;
		GeomEnv.home.vy &= ry-1;
	}

	/* calculate translation matrix 
	 * Your position should be (0,0,0) in the screen coordinate, therefore
	 * the vector GeomEnv.home have to be translated to (0,0,0) by
	 * RotTran() operation. To accomplish this, translation vector must be
	 * as follows:
	 */
	ApplyMatrix(&GeomEnv.mat, &GeomEnv.home, &vec);
	vec.vx = -vec.vx;
	vec.vy = -vec.vy;
	vec.vz = -vec.vz;
	
	/* set the GTE registers */
	TransMatrix(&GeomEnv.mat, &vec);		
	SetRotMatrix(&GeomEnv.mat);		
	SetTransMatrix(&GeomEnv.mat);		
	
#ifdef DEBUG
	dumpVector("angle", &GeomEnv.angle);
	dumpVector("edge", &edgew);
	dumpVector("ofs ", &GeomEnv.offset);
	dumpVector("home", &GeomEnv.home);
#endif	
	return(0);
}
