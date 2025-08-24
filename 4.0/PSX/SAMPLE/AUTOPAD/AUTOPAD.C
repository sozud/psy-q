/*
 * $PSLibId: Run-time Library Release 4.0$
 */
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include "autopad.h"

#define	APD_LOGFILE_TXT		"c:\\psx\\sample\\autopad\\logfile.txt"
#define	APD_LOGFILE_LEN		256
#define	ADDR_APDDATA_SAVE	(u_long *)0x80500000

static char	logfile[APD_LOGFILE_LEN+1];
static u_long	*addr_apdsave;
static u_long	*addr_current;
static long	pad_count;
static char	*program_name;
static long	hcount=0;
static long	hcnt_min=65000;
static long	hcnt_max=-1;

static char *make_filename(char *progname,char *extension,char *fname);

int APDSaveInit(u_long *addr,char *progname)
{
	addr_current = addr_apdsave = addr?addr:ADDR_APDDATA_SAVE;
	program_name = progname;
	pad_count    = 0;
#ifdef DEBUG
	printf("addr=0x%08x prog=%s\n",addr_apdsave,program_name);
#endif
	return(0);
}

u_long APDSave(u_long pad)
{
	if ( !pad_count ) {
		addr_current[0] = 1;
		addr_current[1] = pad;
	} else {
		if ( (pad==addr_current[1]) && (addr_current[0]<0x7fffffff) )
			++(addr_current[0]);
		else {
			addr_current += 2;
			addr_current[0] = 1;
			addr_current[1] = pad;
		}
	}
	++pad_count;
#ifdef DEBUG
	printf("pad=%08x data[0]=0x%04x data[1]=0x%08x\n"
			,pad,addr_current[0],addr_current[1]);
#endif
	return(pad);
}

int APDSaveStop()
{
	int		fd;
	char		fname[128];
	int		size;
	APDSAVE_HEADER	header;

/*
* 	パッドファイルへの書き込み
*/
/*------ get APD file name ------*/
	make_filename(program_name,EXTENTION_APD,fname);
/*----------- file open ---------*/
	if ( PCinit() != 0 ) {
		printf("PCinit() error!\n");
		return(-1);
	}
	fd = PCcreat(fname,0);
	if ( fd == -1 ) {
		printf("%s cannot create.\n",fname);
		return(-1);
	}
/*------------- write ----------*/
	/*---------- header ----------*/
	strcpy(header.label,APDHDR_LABEL);
	header.type      = APDHDR_TYPE;
	header.reserved1 = 0;
	header.size      = ((addr_current-addr_apdsave+1)*4+7)/8*8;
#ifdef DEBUG
	printf("ac=0x%08x ad=0x%08x header.size=%d\n"
			,addr_current,addr_apdsave,header.size);
#endif
	header.count     = pad_count;
	if ( PCwrite(fd,&header,sizeof(APDSAVE_HEADER)) != sizeof(APDSAVE_HEADER) ) {
		printf("PCwrite (header) error!\n");
		return(-1);
	}
	/*----------- data -----------*/
#ifdef DEBUG
	printf("addr=0x%08x prog=%s\n",addr_apdsave,program_name);
	printf("ad[0]=0x%08x ad[1]=0x%08x\n",addr_apdsave[0],addr_apdsave[1]);
#endif
	if ( PCwrite(fd,addr_apdsave,header.size) != header.size ) {
		printf("PCwrite (data) error!\n");
		return(-1);
	}
/*------------ close ----------*/
	PCclose(fd);
	printf("APDSaveStop\n");
	return(0);

}
	
int APDLoadInit(u_long *addr,char *progname)
{
	int		fd;
	int		i,len,error;
	char		fname[128];
	APDSAVE_HEADER	header;

/*
	結果出力ファイル名のチェック
*/
/*----------- initialize ------------*/
	if ( PCinit() != 0 ) {
		printf("PCinit() error!\n");
		return(-1);
	}
/*---------- open txt file ---------*/
	fd = PCopen(APD_LOGFILE_TXT,0,0);
	if ( fd == -1 ) {
		printf("%s cannot open.\n",APD_LOGFILE_TXT);
		return(-1);
	}
/*----------- read -----------*/
	/*------ file name -----*/
	PCread(fd,logfile,APD_LOGFILE_LEN);
	for ( i=0; i<256; i++ )
		if ( !isgraph(logfile[i]) ) break;
	printf("logfile[%d]=0x%x  ",i,logfile[i]);
	logfile[i] = NULL;
	
#ifdef DEBUG
	printf("logfile = %s\n",logfile);
#endif
	PCclose(fd);
/*
	パッドデータをロードするアドレスをセット
*/
	addr_apdsave = addr?addr:ADDR_APDDATA_SAVE;

/*
	パッドデータファイルを読み込む
*/

	program_name = progname;
/*------ get APD file name ------*/
	make_filename(progname,EXTENTION_APD,fname);
	printf("PADfile=%s\n",fname);
/*----------- initialize ------------*/
	if ( PCinit() != 0 ) {
		printf("PCinit() error!\n");
		return(-1);
	}
/*---------- open APD file ---------*/
	fd = PCopen(fname,0,0);
	if ( fd == -1 ) {
		printf("%s cannot open.\n",fname);
		return(-1);
	}
/*----------- read -----------*/
	/*------ header -----*/
	if ( PCread(fd,&header,sizeof(APDSAVE_HEADER)) != sizeof(APDSAVE_HEADER) ) {
		printf("not enough header.\n");
		return(-1);
	}
	if ( strcmp(header.label,APDHDR_LABEL) ) {
		printf("%s is not apd file.\n",fname);
		return(-1);
	}
	if ( header.type != APDHDR_TYPE ) {
		printf("%s is not apd file.\n",fname);
		return(-1);
	}
	/*------ padd data -----*/
	if ( PCread(fd,addr_apdsave,header.size) != header.size ) {
		printf("PCread error!\n");
		return(-1);
	}
/*----------- close -----------*/
	PCclose(fd);

	return(0);
}

u_long APDLoad()
{
	static u_long	pad;
	static u_long	count=0;
	
	if ( !count ) {
		count = *(addr_apdsave++);
		pad   = *(addr_apdsave++);
#ifdef DEBUG
		printf("count=0x%04x pad=0x%04x ",count,pad);
#endif
	}
	--count;
	return(pad);
}

int APDLoadStop(long frame_cnt)
{
	int	fd;
	char	buf[256];
	double	ave;
	
/*
	結果出力ファイルのオープン
*/
	if ( PCinit() != 0 ) {
		printf("PCinit() error!\n");
		return(-1);
	}
	if ( (fd=PCopen(logfile,2,0)) == -1 ) {
		if ( (fd=PCcreat(logfile,0)) == -1 ) {
			printf("%s cannot create.\n",logfile);
			return(-1);
		}
		strcpy(buf,"    Hcnt   min   max   frame  average  program\n");
		strcat(buf,"-----------------------");
/*		strcat(buf,"------------------------------");*/
		strcat(buf,"--------------------------------------------------\n");
		PCwrite(fd,buf,strlen(buf));
		printf("PCcreate\n");
	} else {
		PClseek(fd,0,2);	/* ファイルの最後にセット */
		printf("PClseek\n");
	}
	
	sprintf2(buf,"%8d%6d%6d%8d%9.2f  %s\n",
		hcount,hcnt_min,hcnt_max,frame_cnt
		,(double)(hcount)/frame_cnt,program_name);
	printf("%s",buf);
	PCwrite(fd,buf,strlen(buf));
	PCclose(fd);
/*
	プログラム終了判定のためのダミーファイル出力
*/
	if ( PCinit() != 0 ) {
		printf("PCinit() error!\n");
		return(-1);
	}
	if ( (fd=PCcreat("c:\\tmp\\APDtmp.txt",0)) == -1 ) {
		printf("%s cannot create.\n","c:\\tmp\\APDtmp.txt");
		return(-1);
	}
	strcpy(buf,"This is temporary file for AutoPad.\n");
	PCwrite(fd,buf,strlen(buf));
	PCclose(fd);
	
	printf("APDLoadStop\n");
	return(0);
}

static char *make_filename(char *progname,char *extension,char *fname)
{
	char	*tmp;


	if ( strrchr(progname,'/')  ) progname=strrchr(progname,'/')+1;
	if ( strrchr(progname,'\\') ) progname=strrchr(progname,'/')+1;

	strcpy(fname,progname);

	if ( tmp=strchr(fname,'.') ) *tmp=NULL;
	strcat(fname,extension);
	
	return(fname);
}

long APDSetCnt(long count)
{
/*
	static int	first=1;
	
	if ( first )
		first = 0;
	else { */
		if ( count<hcnt_min ) hcnt_min=count;
		if ( count>hcnt_max ) hcnt_max=count;
/*	}*/
	
	hcount += count;

	return(count);
}

#if 0
long APDSetHSync(long count)
{
/*
	static int	first=1;
	
	if ( first )
		first = 0;
	else { */
		if ( count<hcnt_min ) hcnt_min=count;
		if ( count>hcnt_max ) hcnt_max=count;
/*	}*/
	
	hcount += count;

	return(count);
}
#endif
