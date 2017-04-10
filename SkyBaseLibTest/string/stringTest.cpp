#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_STRING

#include "SkyBaseLib/String.h"

//�����ϊ������e�X�g
TEST( string , 1 ) 
{
	SKY_BASE_LIB_INIT();

	skyString *mStr = _T( "TestTest" );

	skyString *resultW = CStringUtil_::CreateString( mStr );
	CStringUtil_::DeleteString( resultW );
	skyString *resultM = CStringUtil_::CreateString( mStr );
	CStringUtil_::DeleteString( resultM );

	SKY_BASE_LIB_TERM();
}

static const skyString *test_key = _T( "KEY_WORD" );
static const skyString *test_rep = _T( "�u������" );
static const skyString *test_1 = _T( "<##KEY_WORD#>" );						//�L�[���[�h�����Ȃ��ς��[��
static const skyString *test_2 = _T( "aaaaaaaaaaaaaabbbbbccccc" );			//�L�[���[�h�������p�^�[��
static const skyString *test_3 = _T( "ccccc<##KEY_WORD#>dddddd" );			//�L�[���[�h�ƕ��������݂��Ă���p�^�[��
static const skyString *test_4 = _T( "eeee<##KEY_WORD#>fffff<##KEY_WORD#>" ); //�����̃L�[���[�h�����݂���p�^�[��
static const u32 destSize = 256;

//�u�������e�X�g
TEST( string , 2 )
{
	SKY_BASE_LIB_INIT();

	skyString destbuf[ destSize ];
	CStringUtil_::Replace( test_1 , test_key , test_rep , destbuf , destSize );
	CStringUtil_::Replace( test_2 , test_key , test_rep , destbuf , destSize );
	CStringUtil_::Replace( test_3 , test_key , test_rep , destbuf , destSize );
	CStringUtil_::Replace( test_4 , test_key , test_rep , destbuf , destSize );

	SKY_BASE_LIB_TERM();
}

//�X�L�����e�X�g
TEST( string , 3 ) 
{
	f32 a , b , c;
	SkyScanf( _T( "0.1 0.2 0.3" ) , _T( "%f %f %f" ) , &a , &b , &c ); 
	SKY_PRINTF( _T( "result %f %f %f \n" ) , a , b , c );
}

#endif