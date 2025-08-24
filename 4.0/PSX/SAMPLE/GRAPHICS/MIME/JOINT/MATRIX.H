/*
 * $PSLibId: Run-time Library Release 4.0$
 */
/***********************************************
 *	matrix.h
 *
 *	Copyright (C) 1996 Sony Computer Entertainment Inc.
 *		All Rights Reserved.
 ***********************************************/
extern void EigenVector(MATRIX *m, VECTOR *t);
extern void EigenVec2Mat(VECTOR *ev, MATRIX *smat);
extern long preVectorNormal(VECTOR *v);
extern long myVectorNormal(VECTOR *v0, VECTOR *v1);
