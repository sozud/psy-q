$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
¥ The automatic sample execution system, which automatically reads the 
  pad data, has been implemented.

[Procedures for Automatic Executions]

1. How to "make" samples

> apdmake.bat all save
> apdmake.bat all load

2. How to Create Pad Data

Pad data are created by executing the pad data creation program (XXXXs.cpe,
where XXX is the directory name) for each sample directory.

(Example) For graphics/tuto

	  > run tuto1s.cpe

After execution of the above, a pad data file "tuto1.pad" is created.

The procedure up to this point has already been included in the ordinarily 
distributed CD-ROM, so unless you wish to change the pad data, no action is 
necessary.

3. Automatic Execution of Sample

> sample.bat all

All sample executed.
Refer to usage concerning the options of sample.bat.

4. After executions, the execution results are output to the files specified
 in /psx/sample/autopad/logfile.txt .


[Programming for Automatic Executions]

The following processes need to be included in the source code in order to have
automatic execution by reading pad data from files:

1. For Pad Data Creation Programs

(1) Immediately following the Program Start

	int APDSaveInit(long *addr,char *progname)
	addr ...	The designation of memory address where the pad data 
			is to be stored.
			If this is set to 0, the default value 0x80500000 is
			used.
	progname ...	Program name. The pad file is created with the name
			"progname.pad". (Directory is ignored, and creation takes 
			place in the current directory.)
	Return value ..	always 0.

(2) Reading the Pad

	u_long APDSave(long padd)
	padd   ... 	The value of pad read by PadRead()
	Return value.. padd

(3) At the End of the Program

	int APDSaveStop()
	Return value .. Successful:0, Failure:-1

2. For Pad Data Read Program

(1) Immediately following the Program Start

	int APDLoadInit(long *addr,char *progname)
	addr    ... 	The memory address designation for the pad data loading.
			If this is set to 0, the default value 0x80500000 
			is used.
	progname ...	Program name. The pad file reads this "progname.pad". 
			(Directory is ignored if specified, and the search
			takes place in the current directory.)
	Return value ..	Successful:0, Failure:-1

(2) Reading the Pad

	u_long APDLoad(int id)
	id     ... 	Controller number. It does not have any meaning at 
			this time.
	Return value ... Pad data.

(3) Set the Value to be Counted

	long APDSetCnt(long count)
	count ... 	The value to be counted. Recommended to be given the
			return value of VSync(n) (n>=0)  for each frame.

(4) At the End of the Program

	int APDLoadStop(long frame_cnt)
	frame_cnt ... 	Total frame count.
			It is simple to give the return value of VSync(-1), 
			but there are cases where it differs from the 
			actual frame count.

	Return value.. 	Successful:0, Failure:-1

	The result file output after program execution gives the sum, 
	minimum and maximum values for the count of (3), the value of 
	frame_cnt, sum of count/the value of frame_cnt of (4) and 
	progname of (1).



