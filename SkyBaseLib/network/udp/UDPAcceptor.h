#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// UDP�T�[�o�[�\�P�b�g�N���X
/// ���ݒʐM���s���ꍇ�A�ʐM���s���v���O����������UDPAcceptor�i��M�p�j��UDPSoket�i���M�p�j���N�����čs���B
/// </summary>
class CUDPAcceptor : public CSoket
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CUDPAcceptor( u16 portNo , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE  );
	CUDPAcceptor( u16 portNo , const char *address , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE  ); //�}���`�L���X�g��M�p�iaddress�̓}���`�L���X�g�O���[�v�A�h���X��z��j
	void Constractor( u32 receiveTempBufSize );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CUDPAcceptor();

	/// <summary>
	/// ��M�҂��󂯎��s
	/// �����̃��\�b�h�͓�����While���[�v�����邽�ߐ��䂪�A���Ă��܂���B�ʓr�A�v���P�[�V���������s����ꍇ�́A���̏�����ʃX���b�h�Ŏ��s���Ă��������B
	/// </summary>
	void Accept();

	/// <summary>
	/// �N���C�A���g�v�������C�x���g�n���h��
	/// 0 : ��M�f�[�^�o�b�t�@�A�h���X( u8 * )
	/// 1 : ��M�f�[�^�o�b�t�@�T�C�Y
	/// �� ���������f�[�^�𑗐M�������ꍇ�̓f�[�^���M�v��( ReplySendRequest() )�Ƀf�[�^��ݒ肷��΃C�x���g�I���㒼���ɕԐM�����B
	/// �� UDP �̓x�X�g�G�t�H�[�g�^�̓]�����s���B�܂�A�p�P�b�g�̓��B���A���̓��B�������ۏ؂��Ȃ��B�f�[�^���擾���Ԃɕ��ׂĎg���Ă��_���Ȃ��Ƃ�����̂Œ��ӁB
	/// </summary>
	delegate::CDelegateHandler &ReceiveEventHndl();

	/// <summary>
	/// �f�[�^�ԐM�v���i�R�l�N�V�������m�����Ă���ۂ̉�����z�肵�Ă��邽�߁A�����A�h���X���K�v�Ȃ��j
	/// �N���C�A���g�v�������C�x���g�n���h�����ł��̊֐����ĂԂ��Ƃ�z�肵�Ă��܂��B
	/// Ethernet�̃t���[���T�C�Y�̏����1500�o�C�g�Ȃ̂ŁA���M�o�H���Ethernet�����݂���ꍇ������������IP�w�Ńp�P�b�g�������s���Ă��܂��B
    /// �������ꂽ�p�P�b�g��IP�w�ōĂь��������͂��Ȃ̂ŁA��{��薳�����A�����ɑΉ����Ă��Ȃ��@�퓙�����݂���ꍇ���Ȃ̂�1500�o�C�g��
    /// ���Ȃ��悤�ɃA�v���P�[�V�����Œ�������̂��]�܂����B
	/// </summary>
	void ReplySendRequest( void *pData , u32 size );

	/// <summary>
	/// �I�����N�G�X�g
	/// </summary>
	void AcceptEndRequest();

	/// <summary>
	/// �I������
	/// </summary>
	skyBool IsAcceptEnd();

	/// <summary>
	/// �\�P�b�g�n���h���擾
	/// </summary>
	socket_hndl GetHndl();

private:

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	enum eState
	{
		eState_Accept = 0 ,        //��M�҂�
		eState_AcceptEndRequest ,  //��M�҂��I�����N�G�X�g��
		eState_AcceptEnd ,         //��M�I��

	} m_eState;

	/// <summary>
	/// �ڑ�����
	/// </summary>
	CSoketAddressPtr m_spSoketAddress;

	/// <summary>
	/// �\�P�b�g�n���h��
	/// </summary>
	socket_hndl           m_SoketHndl;

	/// <summary>
	/// �f�[�^��M�ꎞ�o�b�t�@
	/// </summary>
	char*                 m_pTempReceiveBuffer;

	/// <summary>
	/// �f�[�^��M�ꎞ�o�b�t�@�T�C�Y
	/// </summary>
	u32                   m_TempReceiveBufferSize;

	/// <summary>
	/// �f�[�^�ԐM�v���o�b�t�@
	/// </summary>
	memory::CMemoryStream m_ReplySendBuffer;

	/// <summary>
	/// �N���C�A���g�ڑ��C�x���g�n���h��
	/// </summary>
	delegate::CDelegateHandler m_ReceiveEventHndl;
};

} } }
