#include "StdAfx.h"
#include "SkyBaseLib/Console.h"
#include "SkyBaseLib/Debug.h"
#include "SkyBaseLib/Memory.h"

SkyImplementRTTI( sky::lib::consol::CConsolStream , sky::lib::stream::AbsStream );

namespace sky { namespace lib { namespace consol {

struct CConsolStreamMember
{
	/// <summary>
	///  入出力ハンドル
	/// </summary>
	HANDLE m_hStdOutput;
	HANDLE m_hStdInput;

	/// <summary>
	///  入出力ハンドル
	/// </summary>
	skyString   m_szBuf[ CONSOL_IO_BUF_LEN ];
};

skyBool CConsolStream::vOpen()
{
	if ( m_IsOpen == skyTrue ) return skyTrue;

	BOOL rs = AllocConsole();
	m_pMember->m_hStdOutput = CreateFile( _T( "CONOUT$" ), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	m_pMember->m_hStdInput  = CreateFile( _T( "CONIN$" ), GENERIC_READ , FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	return rs;
}
skyBool CConsolStream::vClose()
{
	if ( m_IsOpen == skyFalse ) return skyTrue;

    CloseHandle( m_pMember->m_hStdOutput );
    CloseHandle( m_pMember->m_hStdInput );
	return FreeConsole();
}
u32	CConsolStream::vWriteStream( const skyString *format , ... )
{
	skyString	buf[ stream::WRITE_STREAM_BUF_SIZE ];
	VA_LIST( buf , stream::WRITE_STREAM_BUF_SIZE , format );

	u32 len = CStringUtil_::Len( buf ); //ここがLenByteじゃなくLenになっているのはわざと。WriteConsoleだと勝手にUnicodeだと判断してくれるようで、書き込みサイズは文字数になっているようだ。

	DWORD  dwWriteLen;
	WriteConsole ( m_pMember->m_hStdOutput , buf , len , &dwWriteLen , NULL );
	return dwWriteLen;
}

u32 CConsolStream::vReadStream( skyString  *pData , u32 len )
{
	//末尾に文字終端がくるようにゼロクリアする
	sky::lib::memory::CMemoryUtil::ClearZero( pData , sizeof( skyString ) * len );

	DWORD  dwReadLen;
	ReadConsole( m_pMember->m_hStdInput , pData , len , &dwReadLen , NULL );
	return dwReadLen;
}
CConsolStream::CConsolStream()
: stream::AbsStream( stream::eStreamKind_console )
{
	m_pMember = NEW_() CConsolStreamMember;
}

CConsolStream::~CConsolStream()
{
	vClose();
	DEL m_pMember;
}

} } }