/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="graphics/tuto/tuto12.cpe";
#endif
/*		 tuto12: special effect (line scroll) 
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------------------	
 *      1.00           95/04/03    	suzu    (from 'balls')
 *      1.01           97/03/09    	sachiko (added autopad)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define	FRAME_X		320		/* frame size (320x240) */
#define	FRAME_Y		240

u_long *balls(int nobj, int fx, int fy);
static int pad_read(int n);

main()
{
	DRAWENV	draw[2];
	DISPENV	disp[2];
	int	nobj = 32;	/* object number */
	int	cnt, id;
	u_long	*ot;
	
#if defined(APD_SAVE)
	APDSaveInit(0,progname);
#elif defined(APD_LOAD)
	if ( APDLoadInit(0,progname) ) return 0;
#endif
#ifndef APD_LOAD
	PadInit(0);		/* reset PAD */
#endif
	ResetGraph(0);		/* reset graphic subsystem (0:cold,1:warm) */
	SetGraphDebug(0);	/* set debug mode (0:off, 1:monitor, 2:dump) */
		
	/* initialize environment for double buffer */
	SetDefDrawEnv(&draw[0], 0,   0, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 240, 320, 240);
	SetDefDispEnv(&disp[0], 0, 240, 320, 240);
	SetDefDispEnv(&disp[1], 0,   0, 320, 240);
	draw[0].isbg = draw[1].isbg = 1;
	setRGB0(&draw[0], 60, 120, 120);
	setRGB0(&draw[1], 60, 120, 120);
		
	/* init font environment */
	FntLoad(960, 256);	
	SetDumpFnt(FntOpen(16, 16, 256, 200, 0, 512));

	/* initialize line scroll driver */
	LscrInit(320, 240, 0, 0, 0, 240);
	
	/* display */
	SetDispMask(1);		

	while ((nobj = pad_read(nobj)) > 0) {
		id = (id+1)&0x01;

		DrawSync(0);		/* wait for end of drawing */
#ifdef APD_LOAD
		cnt = APDSetCnt(VSync(0));
#else
		cnt = VSync(0);		/* wait for V-BLNK (1/60) */
#endif

		PutDispEnv(&disp[id]); /* update display environment */
		PutDrawEnv(&draw[id]); /* update drawing environment */
		
		ot = balls(nobj, FRAME_X, FRAME_Y);
		
		DrawOTag(ot);	/* draw balls */
		LscrFlush(id);	/* line scroll */

		FntPrint("tuto12: line scroll\n");
		FntPrint("sprite = %d\n", nobj);
		FntPrint("total time = %d\n", cnt);
		FntFlush(-1);
	}
#ifndef APD_LOAD
	PadStop();
#endif
#if defined(APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDLoadStop(VSync(-1));
#endif
	ResetGraph(1);
	StopCallback();
	return;
}

/*
 * Read controll-pad
 */
extern	int LscrOffset[];
static int pad_read(int n)
{
	extern int LscrOffset[];
	static int rate = 0;
	int	y;
/*	u_long	padd = PadRead(1);*/
	u_long	padd;

#if defined(APD_SAVE)
	padd = APDSave(PadRead(1));
#elif defined(APD_LOAD)
	padd = APDLoad(1);
#else
	padd = PadRead(1);
#endif
	
	/* update line scroll value */
#if defined(APD_SAVE)
	if ((padd =APDSave(PadRead(1)))&(PADLleft|PADLright)) {
#elif defined(APD_LOAD)
	if ((padd = APDLoad(1))&(PADLleft|PADLright)) {
#else
	if ((padd = PadRead(1))&(PADLleft|PADLright)) {
#endif
		if (padd&PADLleft)	rate += 16;
		if (padd&PADLright)	rate -= 16;

		for (y = 0; y < 240; y++) {
			LscrOffset[y] = rsin(y*4096/240)*rate/4096;
			/*printf("%4d", LscrOffset[y]);*/
		}
	}
	
	if(padd & PADLup)	n += 4;		/* left '+' key up */
	if(padd & PADLdown)	n -= 4;		/* left '+' key down */
	if(padd & PADk) 	return(-1);
	
	limitRange(n, 1, 2000);		
	return(n);
}		

