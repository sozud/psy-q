/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*		  tuto13: multi window operation
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------------------	
 *      1.00           95/04/03    	suzu    (from 'balls')
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * Position Buffer
 */
typedef struct {		
	u_short x, y;			/* current point */
	u_short dx, dy;			/* verocity */
} POS;

/*
 * Window Strcutre
 */
#define MAXWIN	16
typedef struct {
	u_char	r0, g0, b0;		/* background color */
	RECT	clip;			/* window area */
	u_long	*ot;			/* OT */
} WINDOW;

/*
 * Limitations
 */
#define	FRAME_X		192		/* frame size (320x240) */
#define	FRAME_Y		120

static drawWindows(WINDOW *win, int nwin);
static setWindowPosition(WINDOW *win, int nwin);

u_long *balls(int nobj, int fx, int fy);
static int pad_read(int *n, int *nwin);
main()
{
	DRAWENV	draw[2];
	DISPENV	disp[2];
	WINDOW	window[MAXWIN];/* number of window */
	
	int	nobj = 1;	/* object number */
	int	nwin = 1;	/* window number */
	u_long	*ot;		/* current OT */
	int	i, cnt;		/* work */
	int	id;
	
	PadInit(0);		/* reset PAD */
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
	FntLoad(960, 256);		/* load basic font pattern */
	SetDumpFnt(FntOpen(16, 16, 600/*256*/, 200, 0, 512));

	/* initialize windows */
	for (i = 0; i < MAXWIN; i++) {
		/* set window position */
		setRECT(&window[i].clip,
			rand()%320-FRAME_X/2, rand()%240-FRAME_Y/2,
			FRAME_X, FRAME_Y);

		/* set window back color */
		setRGB0(&window[i], rand(), rand(), rand());

	}
	
	/* display */
	SetDispMask(1);		/* enable to display (0:inhibit, 1:enable) */

	while (pad_read(&nobj, &nwin) > 0) {
		
		id = (id+1)&0x01;

		DrawSync(0);		/* wait for end of drawing */
		cnt = VSync(0);		/* wait for V-BLNK (1/60) */

		PutDispEnv(&disp[id]); /* update display environment */
		PutDrawEnv(&draw[id]); /* update drawing environment */

		/* make balls' OT */
		ot = balls(nobj, FRAME_X, FRAME_Y);

		/* use same OT in this sample */
		for (i = 0; i < nwin; i++)
			window[i].ot = ot;
		
		drawWindows(window, nwin);

		FntPrint("tuto13: multi screen\n");
		FntPrint("sprite = %d, window = %d\n", nobj, nwin);
		FntPrint("total time = %d\n", cnt);
		FntFlush(-1);
	}
	PadStop();
	ResetGraph(1);
	StopCallback();
	return;
}

/*
 * Read controll-pad
 */
static int pad_read(int *n, int *nwin)
{
	static u_long	opadd;
	
	u_long	padd = PadRead(1);
	
	if(padd == PADLup)			(*n)  += 4;
	if(padd == PADLdown)			(*n)  -= 4;
	
	if(opadd == 0 && padd == PADRup)	(*nwin)++;	
	if(opadd == 0 && padd == PADRdown)	(*nwin)--;	
	
	if(padd & PADk) 	return(-1);
	
	limitRange(*n,    1, 2000);		/* see libgpu.h */
	limitRange(*nwin, 1, MAXWIN-1);		/* see libgpu.h */

	opadd = padd;
	return(*n);
}		

/*
 *	Draw many windows
 */
static drawWindows(WINDOW *win, int nwin)
{
	DRAWENV	parent, draw;

	GetDrawEnv(&parent);
	for (; nwin--; win++) {

		/* inherit DRAWENV from parent */
		draw = parent;

		/* change window size and position */
		draw.clip = win->clip;
		draw.clip.x += parent.clip.x;
		draw.clip.y += parent.clip.y;

		/* change offset */
		draw.ofs[0] = draw.clip.x;
		draw.ofs[1] = draw.clip.y;
		
		/* clip by parent window */
		clipRECT(&draw.clip, &parent.clip);
		
		/* change background color */
		setRGB0(&draw, win->r0, win->g0, win->b0);

		/* draw window contents */
		PutDrawEnv(&draw);
		DrawOTag(win->ot);
	}
	parent.isbg = 0;
	PutDrawEnv(&parent);
}

clipRECT(RECT *child, RECT *parent)
{
	if (child->x < parent->x) {
		child->w -= (parent->x - child->x);
		child->x = parent->x;
	}
	if (child->y < parent->y) {
		child->h -= (parent->y - child->y);
		child->y = parent->y;
	}
	if (child->x+child->w > parent->x+parent->w)
		child->w = parent->w + parent->w - child->x;
		
	if (child->y+child->h > parent->y+parent->h) 
		child->h = parent->y + parent->h - child->y;
}

