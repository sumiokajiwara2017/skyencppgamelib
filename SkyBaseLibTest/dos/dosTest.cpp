#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DOS

#include "SkyBaseLib/Dos.h"

static skyString *TEST1_BAT_FILE_PATH = _T( "data\\test\\dos\\Test.bat" ); //�p�X�͕K��\\��؂�
static skyString *TEST1_BAT_FILE_PATH_2 = _T( "notepad" );
static skyString *TEST1_BAT_FILE_RESULT_PATH = _T( "data\\test\\dos\\TestRs.txt" ); //�p�X�͕K��\\��؂�

TEST( dos , 1 ) 
{
	SKY_BASE_LIB_INIT();

	skyString ioBuf[ 1024 ];
	CStringUtil_::Sprintf( ioBuf , 1024 , TEST1_BAT_FILE_PATH );

	//�c�����Ńo�b�`�����s����
	CDosUtil_::ExecDosCommand( ioBuf , TEST1_BAT_FILE_RESULT_PATH );

	CStringUtil_::Sprintf( ioBuf , 1024 , TEST1_BAT_FILE_PATH_2 );

	//�c�����Ńm�[�g�p�b�h���N������
	CDosUtil_::ExecDosCommand( ioBuf , TEST1_BAT_FILE_RESULT_PATH );
	
	SKY_BASE_LIB_TERM();
}

#endif