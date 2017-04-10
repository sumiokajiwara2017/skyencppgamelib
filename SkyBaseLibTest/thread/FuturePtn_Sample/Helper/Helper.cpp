#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../Data/Data.h"
#include "Helper.h"

Helper::Helper( const skyWString *pStr , DataSmPtr& data ) :
m_Str( pStr ) ,
m_Data( data )
{
}

Helper::~Helper( void )
{
}

void Helper::ThreadMain( void )
{
	// ���Ԃ��|���鏈��
	Slowly();

	// ���̕������ݒ肵���u�ԁA���C���X���b�h��GetData�ɐ�������
	m_Data->SetData( L"�f�[�^�̏������ł��܂����B" );

	// �Q�b�҂�
	ThreadWin32::GetThread()->Sleep( 2000 );

	SKY_PRINTF( L"%s" , m_Str.Get() );
}

void Helper::Slowly( void )
{
	ThreadWin32::GetThread()->Sleep( 3000 );
}