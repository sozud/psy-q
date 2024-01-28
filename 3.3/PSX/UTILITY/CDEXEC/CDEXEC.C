/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*************************************************************************

  DTL-H2000 �� CD-ROM/CD-Emulator ��� PSX.EXE �����s�����郂�W���[��
 �ipatchx������ԂŃu�[�g��������@�j
 
	1995/05/19	v1.1	yoshi

==========================================================================

  ���݁APC����H2000�Ƀv���O����(.CPE�t�@�C��)�����[�h���Ď��s������ꍇ�A
  �O������ patchx.cpe �����s���鎖���K�v�ł��B

	�i��j
	DOS> run patchx  
	DOS> run main
  
  ����ɑ΂��āACD-ROM/CD-Emulator ����u�[�g�v���O����(PSX.EXE)��ǂ݂�����
  ���s������ɂ́Apatchx.cpe �̌�ɂ��� cdexec.cpe �����s���ĉ������B

	DOS> run patchx
	DOS> run cdexec

  �i���Ӂj
  �uresetps 0�v�͎g�p���Ȃ��ŉ������B
  �uresetps 0�v�ł́Apatchx ����Ȃ���ԂŃu�[�g����ׁA����ɓ��삵�Ȃ�
  �ꍇ������܂��B

*************************************************************************

   A module for executing PSX.EXE on a CD-ROM/CD-Emulator with DTL-H2000 
   (A way to boot with patchx executed.)

	May 19, 1995	ver. 1.1	yoshi

==========================================================================

  At present, in order to load a program (.cpe file) from PC to carry 
  out, patchx.cpe must be carried out in advance.

	Example:
	DOS> run patchx  
	DOS> run main
  
  On the other hand, in order to read a boot program (PSX.EXE) 
  on the CD-ROM/CD-Emulator to carry out, cdexec.cpe must be carried 
  out after patchx.cpe execution.

	DOS> run patchx
	DOS> run cdexec

  Caution:
   Never use 'resetps 0'.
   PSX.EXE may not work properly with 'resetps 0' because it is booted
   without patchx execution.

**********************************************************************/

main()
{
	_96_remove();
	_96_init();
	LoadExec("cdrom:\\PSX.EXE;1", 0x801fff00, 0);
		/* File name, stack pointer, stack size ::
	   	�t�@�C����  �X�^�b�N�|�C���^  �X�^�b�N�T�C�Y */
}



