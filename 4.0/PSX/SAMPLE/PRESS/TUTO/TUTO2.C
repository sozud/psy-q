/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="press/tuto/tuto2.cpe";
#endif
/*			tuto2: simplest sample
 *
 *	Paralell execution of LoadImage() and DecDCTout().
 *	LoadImage is always faster than DecDCTout, so decoded image
 *	translation works without double-buffering.
 *	
 :	LoadImage() と、DecDCTout をインターリーブしてみる
 *	LoadImage() は、DecDCTout より圧倒的に速いので、ダブルバッファ
 *	をしていなくても動く。
 *	
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libpress.h>

u_long mdec_bs[] = {
#include "siro.bs"
};

main()
{
	int	i;
	int	width  = 256;
	int	height = 160;

	init_graph(width, height);		/* reset graphic system */
#if defined(APD_SAVE)
	APDSaveInit(0,progname);
#elif defined(APD_LOAD)
	if ( APDLoadInit(0,progname) ) return 0;
#endif
#ifndef APD_LOAD
	PadInit(0);
#endif
	DecDCTReset(0);				/* reset MDEC */
	disp_mdec(mdec_bs, width, height);	/* display */

#if defined(APD_SAVE)
	while (APDSave(PadRead(0)) != PADselect);
#elif defined(APD_LOAD)
	while (APDLoad(0) != PADselect);
#else
	while (PadRead(0) != PADselect);
#endif

#ifndef APD_LOAD
	PadStop();
#endif
#if defined(APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDSetCnt(VSync(0));
	APDLoadStop(VSync(-1));
#endif
	StopCallback();
	return;
}

disp_mdec(mdec_bs, width, height)
u_long	*mdec_bs;
int	width, height;
{
	static u_long	mdec_rl[256*256];	/* run-level buffer */
	static u_long	mdec_image[16*256/2];	/* image slice buffer */
	
	DECDCTTAB	table;
	RECT		rect;
	int		slice = 16*height/2;
	int		total = width*height/2;
	
	rect.w = 16;
	rect.h = height;
	rect.y = (240-height)/2;
	
	DecDCTvlcBuild(table);
	DecDCTvlc2(mdec_bs, mdec_rl, table);	/* decode VLC */
	DecDCTin(mdec_rl, 0);			/* send run-level */

	for (rect.x = 0; rect.x < width; rect.x += 16) {
		DecDCTout(mdec_image, slice);		/* recieve */
		DecDCToutSync(0);			/* wait */
		LoadImage(&rect, mdec_image);		/* load */
	}
}

init_graph(width, height)
int	width, height;
{
	DISPENV	disp;
	RECT	rect;
	
	ResetGraph(0);		/* reset graphic subsystem (0:cold,1:warm) */
	SetGraphDebug(0);	/* set debug mode (0:off, 1:monitor, 2:dump) */
	SetDispMask(1);		/* enable to display (0:inhibit, 1:enable) */
	SetDefDispEnv(&disp, 0, 0, width, 240);	/* setup display environment */
	PutDispEnv(&disp);	

	rect.w = 640;	rect.h = 480;
	rect.x =   0;	rect.y =   0;
	ClearImage(&rect, 0, 0, 0);
	
}	

