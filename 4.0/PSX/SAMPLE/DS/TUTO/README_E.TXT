$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
There is a simple tutorial for using libds.lib in this directory.


tuto0 - tuto11:
===============
These are the rewritten libcd tutorial for libds.lib.
Since the structure of the program has been kept as in tact as possible, 
these are not necessarily the most appropriate methods for libds.lib, 
but by comparing these with the tutorial of libcd, you should be able
to grasp the characteristics of libds.

tuto12:
=======
The sample for the comparison of the time required until the playing of 
the music for the cases of using forward seek to play DA and not.

	PADRup   Forward seek ON/OFF switch.
	PADRdown Play start.
	The selection of the direction key music
        (when the direction is being sought, the seek starts at the 
	beginning of the music).

The time required until the playing of "TIME TO START" displayed on 
the screen (frame count: 1/60 sec. units). You can grasp the amount of
time required for the seek through this.

Since the actual time required for the seek depends on the conditions 
(head position, file location and other environmental issues), the data
obtained by this sample should only be used as a guide to get rough idea.

tuto13:
=======
The sample for the comparison of the time required until the playing of 
the music for the cases of using forward seek to play XA and not.

	PADRup   Forward seek ON/OFF switch.
	PADRleft Forward seek ON _F Seek start at the beginning of 
                 the file.

Forward seek OFF_FSeek start at the beginning of the CD.
PADRdown Play start. Direction key XA channel selection.

The time required until the playing of "TIME TO START" displayed on the 
screen (frame count: 1/60 sec. units). You can grasp the amount of time 
required for the seek through this.
Since the actual time required for the seek depends on the conditions
(head position, file location and other environmental issues), the data 
obtained by this sample should only be used as a guide to get rough idea.

tuto14:
=======
The sample of comparison of the time required for continuous reading of 
multiple files (sequential recording on the CD and reading without 
seeking individual file at a time, but rather, continuously read in i
recorded sequence) and seeking and reading individual files at a time.
In the sample, the data sector is sectioned in 50 sector units, and the 
access is executed, with the set of 50 sectors regarded as a single file.

	PADRup Continuous read / Individual access read switch.
	PADRdown Read start.

"TIME TO READ" displayed on the screen is the time required to read 50 
sectors*10 (frame count: 1/60 sec. units).

Since the actual time required for the seek depends on the conditions 
(head position, file location and other environmental issues), the data 
obtained by this sample should only be used as a guide to get rough idea.
