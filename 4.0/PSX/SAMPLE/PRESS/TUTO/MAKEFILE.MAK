CC = ccpsx -O3 -Xo$80010000
CCS = ccpsx -O3 -Xo$80010000 -DAPD_SAVE
CCL = ccpsx -O3 -Xo$80010000 -DAPD_LOAD
APDOBJ = \psx\lib\autopad.obj

all: tuto1 tuto2 tuto3 tuto4 tuto5 tuto6 tuto7
	
mdec_bs.obj : mdec_bs.c
	ccpsx -c mdec_bs.c
tuto1:
	$(CC) tuto1.c -otuto1.cpe,tuto1.sym 
tuto2: 
	$(CC) tuto2.c -otuto2.cpe,tuto2.sym
tuto3: mdec_bs.obj 
	$(CC) tuto3.c mdec_bs.obj -otuto3.cpe,tuto3.sym 
tuto4: 
	$(CC) tuto4.c -otuto4.cpe,tuto4.sym
tuto5: 
	$(CC) tuto5.c -otuto5.cpe,tuto5.sym 
tuto6: mdec_bs.obj
	$(CC) tuto6.c mdec_bs.obj  -otuto6.cpe,tuto6.sym 
tuto7: mdec_bs.obj
	$(CC) tuto7.c mdec_bs.obj  -otuto7.cpe,tuto7.sym 

apd_save: mdec_bs.obj
	$(CCS) tuto1.c $(APDOBJ) -otuto1s.cpe 
	$(CCS) tuto2.c $(APDOBJ) -otuto2s.cpe
	$(CCS) tuto3.c mdec_bs.obj $(APDOBJ) -otuto3s.cpe
	$(CCS) tuto4.c $(APDOBJ) -otuto4s.cpe
	$(CCS) tuto5.c $(APDOBJ) -otuto5s.cpe
	$(CCS) tuto6.c mdec_bs.obj $(APDOBJ) -otuto6s.cpe 
	$(CCS) tuto7.c mdec_bs.obj $(APDOBJ) -otuto7s.cpe 
apd_load: mdec_bs.obj
	$(CCL) tuto1.c $(APDOBJ) -otuto1l.cpe 
	$(CCL) tuto2.c $(APDOBJ) -otuto2l.cpe
	$(CCL) tuto3.c mdec_bs.obj $(APDOBJ) -otuto3l.cpe
	$(CCL) tuto4.c $(APDOBJ) -otuto4l.cpe
	$(CCL) tuto5.c $(APDOBJ) -otuto5l.cpe
	$(CCL) tuto6.c mdec_bs.obj $(APDOBJ) -otuto6l.cpe 
	$(CCL) tuto7.c mdec_bs.obj $(APDOBJ) -otuto7l.cpe 

for_menu: mdec_bs.obj
	$(CC) \psx\lib\none3.obj tuto1.c             -otuto1.cpe
	$(CC) \psx\lib\none3.obj tuto2.c             -otuto2.cpe
	$(CC) \psx\lib\none3.obj tuto3.c mdec_bs.obj -otuto3.cpe
	$(CC) \psx\lib\none3.obj tuto4.c             -otuto4.cpe
	$(CC) \psx\lib\none3.obj tuto5.c             -otuto5.cpe
	$(CC) \psx\lib\none3.obj tuto6.c mdec_bs.obj -otuto6.cpe
	$(CC) \psx\lib\none3.obj tuto7.c mdec_bs.obj -otuto7.cpe
	cpe2x tuto1.cpe
	cpe2x tuto2.cpe
	cpe2x tuto3.cpe
	cpe2x tuto4.cpe
	cpe2x tuto5.cpe
	cpe2x tuto6.cpe
	cpe2x tuto7.cpe


