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
	// 時間が掛かる処理
	Slowly();

	// この文字列を設定した瞬間、メインスレッドがGetDataに成功する
	m_Data->SetData( L"データの準備ができました。" );

	// ２秒待つ
	ThreadWin32::GetThread()->Sleep( 2000 );

	SKY_PRINTF( L"%s" , m_Str.Get() );
}

void Helper::Slowly( void )
{
	ThreadWin32::GetThread()->Sleep( 3000 );
}