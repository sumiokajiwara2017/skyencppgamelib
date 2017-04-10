#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

void CSoket::Initialize()
{
	WSADATA wsaData;
	int err;

	err = WSAStartup( MAKEWORD( 2 , 0 ) , &wsaData );
	if ( err != 0 )
	{
		switch ( err )
		{
			case WSASYSNOTREADY:
				SKY_PANIC_MSG( _T( "�l�b�g���[�N�T�u�V�X�e�����l�b�g���[�N�ւ̐ڑ��������ł��Ă��Ȃ��B" ) );
				break;
			case WSAVERNOTSUPPORTED:
				SKY_PANIC_MSG( _T( "�v�����ꂽwinsock�̃o�[�W�������T�|�[�g����Ă��Ȃ��B" ) );
				break;
			case WSAEINPROGRESS:
				SKY_PANIC_MSG( _T( "�u���b�L���O����̎��s���ł��邩�A �܂��̓T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������Ă���B" ) );
				break;
			case WSAEPROCLIM:
				SKY_PANIC_MSG( _T( "winsock�������ł���ő�v���Z�X���ɒB�����B" ) );
				break;
			case WSAEFAULT:
				SKY_PANIC_MSG( _T( "�������ł���lpWSAData �͗L���ȃ|�C���^�ł͂Ȃ��B" ) );
				break;
		}
	}
}

void CSoket::Dispose()
{
	 WSACleanup();
}

#endif


} } }
