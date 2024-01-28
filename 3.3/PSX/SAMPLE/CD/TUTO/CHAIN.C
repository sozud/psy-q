/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			chain: chained CdRead()
 *			
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Jul.25,1995	suzu
 *
 *			     Chained CdRead
 *
 *	CdReadChain() calls CdRead() automatically when the previous CdRead()
 *	is finished. The reading parameters should be predetermined and
 *	defined in the arrays which is sent cdReadChain() as an argument.
 *
 *	* CdReadChain() uses CdReadCallback().
 *	* If one of CdRead() is failed, CdReadChain() retries from first.
 * 
 *	cdReadChainSync	returns number of unfinished CdRead reaquests.
 *
 *	cdReadChain	executes many CdRead()s according to the parameter
 *			array.
 *		
 :	CdRead() ������I������ۂɔ�������R�[���o�b�N CdReadCallbacK()
 *	���g�p���āA�z��ɓo�^���ꂽ���[�h�v�������X�Ɏ��s����
 *
 *	* cdReadChain() �� CdReadCallback() ��r���I�Ɏg�p���܂��B
 *	* ���[�h�Ɏ��s����ƁA�z��̐擪�ɖ߂��čŏ����烊�g���C���܂��B
 *
 *	cdReadChainSync	�������̃��[�h�̌���Ԃ��B
 *
 *	cdReadChain	�e�[�u���Ɏw�肳�ꂽ�p�����[�^���g�p���Ď��X��
 *			CdRead()�𔭍s����B
 *
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

static CdlLOC	*postbl;	/* position table */
static int	*scttbl;	/* sector number table */
static u_long	**buftbl;	/* destination buffer pointer table */
static int	ctbl;		/* current CdRead */
static int	ntbl;		/* total CdReads */

void cbread(u_char intr, u_char *result);
		   
int cdReadChainSync()
{
	return(ntbl - ctbl);
}
	    
int cdReadChain(CdlLOC *_postbl, int *_scttbl, u_long **_buftbl, int _ntbl)
{
	/* save pointers */
	postbl = _postbl;
	scttbl = _scttbl;
	buftbl = _buftbl;
	ntbl   = _ntbl;
	ctbl   = 0;
	
	CdReadCallback(cbread);
	CdControl(CdlSetloc, (u_char *)&postbl[ctbl], 0);
	CdRead(scttbl[ctbl], buftbl[ctbl], CdlModeSpeed);
	return(0);
}

void cbread(u_char intr, u_char *result)
{
	/*printf("cbread: (%s)...\n", CdIntstr(intr));*/
	if (intr == CdlComplete) {
		if (++ctbl == ntbl)
			CdReadCallback(0);
		else {
			CdControl(CdlSetloc, (u_char *)&postbl[ctbl], 0);
			CdRead(scttbl[ctbl], buftbl[ctbl], CdlModeSpeed);
		}
	}
	else {
		printf("cdReadChain: data error\n");
		ctbl = 0;
		CdControl(CdlSetloc, (u_char *)&postbl[ctbl], 0);
		CdRead(scttbl[ctbl], buftbl[ctbl], CdlModeSpeed);
	}
}
		
		
