#include "StdAfx.h"
#include "SkyBaseLib/File.h"

SkyImplementRTTI( sky::lib::file::CFileStream , sky::lib::memory::CMemoryStream );

namespace sky { namespace lib { namespace file { 

CFileStream::CFileStream( const skyString *path , void* pData , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
: CMemoryStream( path , pData , size , isEnableDelete , isReadOnly , isEnableChengeSize )
{
}
CFileStream::CFileStream( const skyString *path , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
: CMemoryStream( path , size , isEnableDelete , isReadOnly , isEnableChengeSize )
{
}

skyBool CFileStream::vReadLine( skyString *pBuf , u32 bufLen , u32 offset )
{
	SKY_ASSERT_MSG( m_pData != skyNull , _T( "There is no data yet. " ) ); //まだデータがありません。

	//１文字目を終端子にしておく（何も読めなかった場合、空白文字が返るようにする）
	CMemoryUtil_::Copy( pBuf , &EOT , sizeof ( skyString ) );

	//シーク位置がすでにファイル終端だった場合何も読まずに返す
	if ( m_Seek == m_Size - 1 )
	{
		return skyFalse;
	}

	//指定のオフセット値をプラスする
	m_Seek += offset;

	//次の改行コードまでの長さを図る
	u32 readLen; 
	skyBool isFound = CStringUtil_::StrchrDistance( ( skyString * )&m_pData[ m_Seek ] , EC , readLen );
	
	//０だった場合、何も読まずに返す
	if ( readLen == 0 )
	{
		return skyFalse;
	}

	if ( isFound == skyTrue )
	{
		//見つかった改行コードは結果に返さない
		readLen -= 1;
	}

	//この+1は終端子格納分
    SKY_ASSERT_MSG( ( readLen + 1 ) * sizeof( skyString ) <= bufLen , _T( "Buffer over run. dataSize=[ %d ] bufLen=[ %d ]" ), m_Size , bufLen );

	//読み込み結果と末尾に終端子も付けて返す
	CMemoryUtil_::Copy( pBuf , ( skyString * )&m_pData[ m_Seek ] , readLen * sizeof ( skyString ) );
	CMemoryUtil_::Copy( pBuf + readLen , &EOT, sizeof ( skyString ) ); //終端子もつける

	//読み込んだ文字分シークをすすめる
	m_Seek =  m_Seek + ( ( readLen + 2 ) * sizeof ( skyString ) ); //+2は終端子以降にシークするため。

	//カーソルがデータの終端を越えてしまったらデータの終端まで戻す
	if ( m_Seek >= m_Size )
	{
		m_Seek = m_Size - 1;
	}

	return skyTrue;
}

void CFileStream::vWriteLine( const skyString *pBuf , u32 len )
{
	SKY_ASSERT_MSG( m_pData != skyNull , _T( "There is no data yet. " ) ); //まだデータがありません。

	//次の改行コードまでの長さを図る
	u32 readLen = len;

	if ( len == 0 )
	{
		skyBool isFound = CStringUtil_::StrchrDistance( pBuf , EC , readLen );
	
		//０だった場合、何も読まずに返す
		if ( readLen == 0 )
		{
			return;
		}

		if ( isFound == skyTrue )
		{
			//見つかった改行コード分引く
			readLen -= 1;
		}
	}

	//改行コードまでの長さをサイズにする
	u32 bufSize = readLen * sizeof( skyString );

	//改行コードまでの文字を書きこむ
	vWriteStream( ( u8 * )pBuf , bufSize );
	vWriteStream( ( u8 * )&EC , sizeof ( skyString ) ); //改行コードを書く
}

} } }
