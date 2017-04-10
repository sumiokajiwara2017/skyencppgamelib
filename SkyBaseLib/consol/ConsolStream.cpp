#include "StdAfx.h"
#include "SkyBaseLib/Console.h"
#include "SkyBaseLib/Debug.h"
#include "SkyBaseLib/Memory.h"

SkyImplementRTTI( sky::lib::consol::CConsolStream , sky::lib::stream::AbsStream );

namespace sky { namespace lib { namespace consol {

struct CConsolStreamMember
{
	/// <summary>
	///  ���o�̓n���h��
	/// </summary>
	HANDLE m_hStdOutput;
	HANDLE m_hStdInput;

	/// <summary>
	///  ���o�̓n���h��
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

	u32 len = CStringUtil_::Len( buf ); //������LenByte����Ȃ�Len�ɂȂ��Ă���̂͂킴�ƁBWriteConsole���Ə����Unicode���Ɣ��f���Ă����悤�ŁA�������݃T�C�Y�͕������ɂȂ��Ă���悤���B

	DWORD  dwWriteLen;
	WriteConsole ( m_pMember->m_hStdOutput , buf , len , &dwWriteLen , NULL );
	return dwWriteLen;
}

u32 CConsolStream::vReadStream( skyString  *pData , u32 len )
{
	//�����ɕ����I�[������悤�Ƀ[���N���A����
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