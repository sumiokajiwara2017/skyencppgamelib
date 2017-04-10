#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// TCP�N���C�A���g�\�P�b�g�N���X
/// TCP�w�̂������ő������f�[�^�̓T�[�o�[�ɕK���͂��B�i�f���Ƃ������j�󂪂Ȃ���΁j
/// </summary>
class CTCPSoket : public CSoket
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTCPSoket( u16 portNo , const char *address , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE );
	CTCPSoket( const CSoketAddressPtr &spAddress , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE );
	void Constractor( u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTCPSoket();

	/// <summary>
	/// �ڑ�
	/// </summary>
	void Connect();

	/// <summary>
	/// �ڑ�����
	/// </summary>
	void DisConnect();

	/// <summary>
	/// ���M�i��{�K���͂��B�͂��Ȃ���Η�O����������B���̗�O��catch���Ď����ōđ�����������邱�Ƃ��ł���j
	/// </summary>
	u32  Send( const void* pData , u32 size );

	/// <summary>
	/// ��M
	/// </summary>
	u32  Receive( void* pBuf , u32 size );

	/// <summary>
	/// �\�P�b�g�n���h���擾
	/// </summary>
	socket_hndl GetHndl();

private:

	/// <summary>
	/// �ڑ�����
	/// </summary>
	CSoketAddressPtr m_spSoketAddress;

	/// <summary>
	/// Connect�ς݃t���O
	/// </summary>
	skyBool m_IsConnect;

	/// <summary>
	/// �\�P�b�g�n���h��
	/// </summary>
	socket_hndl m_SoketHndl;

	/// <summary>
	/// �f�[�^��M�ꎞ�o�b�t�@
	/// </summary>
	char* m_pTempReceiveBuffer;

	/// <summary>
	/// �f�[�^��M�ꎞ�o�b�t�@�T�C�Y
	/// </summary>
	u32 m_TempReceiveBufferSize;
};

} } }
