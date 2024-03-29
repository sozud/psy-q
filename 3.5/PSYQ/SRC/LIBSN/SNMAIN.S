;	SN Systems replacement for main module
;	in API lib

	opt	c+

DIPSW	equ	$1F802040	;byte, read only

	section	.rdata
	section	.text
	section	.ctors
	section	.dtors
	section	.data
	section	.sdata
	section	.sbss
	section	.bss

	xdef	__SN_ENTRY_POINT,__main,__do_global_dtors

	xdef	__heapbase,__heapsize
	xdef	__text,__textlen
	xdef	__data,__datalen
	xdef	__bss,__bsslen

	xref	InitHeap,main,_stacksize,_ramsize

	section	.text

;
; This is the program entry point.
; 1) Clear program BSS section to zero
; 2) Set stack and heap
; 3) Call user entry point i.e. main()
; 4) Jmp back to downloader stub (should be call exit() or something?)
;
; Note:	default ram size is 8 Megabytes
;	default stack size is 32K
;	stack position is top of RAM
;	heap is all of RAM from end of prog to lowest stack addr
;
; Use can override these settings by declaring suitable values
; for these variables in his C code module along with main().
; e.g.
;	_stacksize=0x00002000; /* set 8K stack */
;	  _ramsize=0x00100000; /* and 1MB ram  */
;
; If user does not specify override values for these variables
; by defining them as unsigned int then the default values will
; be loaded from the SNDEF module in LIBSN.LIB
;
; Note that:
; 1) if the user does not override the _stacksize and _ramsize vars then
; the defaults can still be accessed if declared as external unsigned int
;
; 2) other external unsigned ints which can be referenced are:-
;
;	external unsigned int __heapbase,__heapsize
;	external insigned int __text,__textlen
;	external unsigned int __data,__datalen
;	external unsigned int __bss,__bsslen
;
; These latter variables should be treated as READ ONLY.
; (You can of course declare them as pointers if you prefer).
;
__SN_ENTRY_POINT:

	la	v0,sect(.sbss)
	la	v1,sectend(.bss)
@clrit:
	opt	at-
	sw	zero,0(v0)
	addiu	v0,v0,4
	sltu	at,v0,v1
	bne	at,zero,@clrit
	nop
	opt	at+

; This was the old way to set ram-top. Read mem config from DIP switches.
;
;	lui	a0,DIPSW>>16
;	lb	v0,DIPSW&$FFFF(a0)	;read dip settings
;	nop
;	andi	v0,v0,%00110000	;mem size in bits 4 & 5
;	srl	v0,v0,2
;	la	a0,MemSizes
;	addu	a0,a0,v0
;	lw	v0,0(a0)	;put stack at top of RAM
;	nop

	lw	v0,_ramsize	;this is the new way; because there
	nop			; are no switches on new hardware.

	sub	v0,v0,8		;but leave room for two parameters
	lui	t0,$8000	;(mem seg for kernel cached RAM)
	or	sp,v0,t0	;set stack in kseg0

	la	a0,sectend(.bss)	; a0 = heap base
	sll	a0,a0,3
	srl	a0,a0,3			;remove mem seg bits
	lw	v1,_stacksize
	nop
	subu	a1,v0,v1	;calc a1 = top of heap
	subu	a1,a1,a0	; -heap base, => a1 = size of heap
	sw	a1,__heapsize
	or	a0,a0,t0	;heap in kseg0
	sw	a0,__heapbase

	sw	ra,__ra_temp
	la	gp,sect(.sdata)
	move	fp,sp
	jal	InitHeap
	addi	a0,a0,4	;don't know why they do this.

	lw	ra,__ra_temp
	nop

	jal	main
	nop

; Will fall through here if main() returns.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Moved to before "break" instruction and converted from subroutine
; 7-26-96 by Mike Fulton @ SCEA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

__do_global_dtors

	lw	t0,__initialised

	subiu	sp,sp,16

	sw	s0,4(sp)
	sw	s1,8(sp)
	sw	ra,12(sp)

	beq	t0,zero,@exit
	nop

	la	s0,sect(.dtors)
	la	s1,(sectend(.dtors)-sect(.dtors))/4
	beq	s1,zero,@exit
	nop

@loop	lw	t0,0(s0)
	addiu	s0,s0,4

	jalr	t0
	subiu	s1,s1,1

	bne	s1,zero,@loop
	nop

@exit
	break	$1

; Never get here!	

	lw	ra,12(sp)
	lw	s1,8(sp)
	lw	s0,4(sp)

	addiu	sp,sp,16

	jr	ra
	nop

;
; main() will call this before doing user code. Init other stuff here.
;
__main	lw	t0,__initialised

	subiu	sp,sp,16

	sw	s0,4(sp)
	sw	s1,8(sp)
	sw	ra,12(sp)

	bne	t0,zero,@exit
	li	t0,1

	sw	t0,__initialised

	la	s0,sect(.ctors)
	la	s1,(sectend(.ctors)-sect(.ctors))/4
	beq	s1,zero,@exit
	nop

@loop	lw	t0,0(s0)	;loop for all C++ global constructors
	addiu	s0,s0,4

	jalr	t0	; call C++ constructor
	subiu	s1,s1,1

	bne	s1,zero,@loop
	nop

@exit	lw	ra,12(sp)
	lw	s1,8(sp)
	lw	s0,4(sp)

	addiu	sp,sp,16

	jr	ra
	nop

	section	.data

__initialised	dw	0
__heapbase	dw	0
__heapsize	dw	0
__text	dw	sect(.text)
__textlen	dw	sectend(.text)-sect(.text)
__data	dw	sect(.data)
__datalen	dw	sectend(.data)-sect(.data)
__bss	dw	sect(.bss)
__bsslen	dw	sectend(.bss)-sect(.bss)

	section	.sbss

__ra_temp	dsw	1

	end

