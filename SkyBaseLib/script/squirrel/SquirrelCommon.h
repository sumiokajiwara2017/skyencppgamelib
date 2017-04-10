#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

/*
	�Q�lURL
	http://muffin.cias.osakafu-u.ac.jp/~matumoto/cgi-bin/xt.cgi?prog/squirrel
	http://monsho.blog63.fc2.com/blog-entry-20.html
*/

#define SW_USE_SQ_STD_LIB //�W�����C�u�������g�p����Ƃ����X�C�b�`
#define SW_DISPLAY_SQ_LOG //Squirrel�̓��샍�O���o���B
#define SW_USE_SKYALOCATOR_USE //SkyLib�̃A���P�[�^�[���g�p����

#include "SquirrelTypes.h"

#undef type //GoogleTest���Ƌ�������̂�define��������


/// <summary>
/// �`�������
/// </summary>
struct ApiInfo
{
	const skyString *pName;
	SQFUNCTION		funcAdr;
};

#define SQAPI( funcname )	 SQInteger funcname( HSQUIRRELVM v )		//�֐��錾�p
#define SQAPITBL( funcname ) { _T( #funcname ) , funcname },			//�e�[�u���錾�p

/*
	====����====
---------------------------------------------------------------------------------------------
	
	Squirrel�̓e�[�u���ƃX���b�g�Ƃ����T�O�����邪�A�e�[�u�����\������̂̓X���b�g�ŁA
	�X���b�g�Ƃ� (�L�[�ƒl) �̃y�A�̂��Ƃł��BNull �ȊO�̂��ׂĂ̒l���L�[�̖������ʂ���
	���Ƃ��\�ŁA�X���b�g�ɂ͔C�ӂ̒l�����蓖�Ă邱�Ƃ��ł��܂��B

---------------------------------------------------------------------------------------------

	��Squirrel����Ăяo�������֐��́A���̂悤�Ȍ^�łȂ���΂Ȃ�Ȃ�(fn���Ăяo�������֐�)�B
	�w SQInteger (*SQFUNCTION)(HSQUIRRELVM) �x (Squirrel�̃w�b�_�ɒ�`����Ă���j

---------------------------------------------------------------------------------------------

	��Squirrel��C++�̏�������P�t���[���ɂ�����V�[�P���X

		C++				Squirrel			Squirrel�i�K�v�ɉ����āj
		 �b				    
		 �b	  ����ړ���
		 �{---------------���{
		 				    �b
		 				    �b    ����ړ��� 
		 				    �{-----------------�{
							 				�@ �b
		 									   �b<<�X�V>>
		 									   �b
		 				    �{-----------------�{
		 				    �b   ������ړ�
		 				    �b
		 �{��---------------�{
		 �b	  ������ړ�
		 �b				    
 <<�`��>>�b				    
		 �b				    
		 �b				    
		 ��					

		 ��{�Q�[���̍X�V���������Squirrel�Ŏ��s����i���̉ߒ���C++���̃R�[�h���R�[�����邱�Ƃ͂��邾�낤�j
		 �`�搧���C++���ōs���B


*/

#endif