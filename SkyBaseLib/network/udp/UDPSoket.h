#pragma once

namespace sky { namespace lib { namespace net {

/// UDP�N���C�A���g�\�P�b�g�N���X
/// UDP�w�̂͑��M�m�F�����Ă���Ȃ��̂ŁA�������M�m�F������ꍇ�͎��O�Ńv���g�R���������Ă��G
/// </summary>
class CUDPSoket : public CSoket
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CUDPSoket();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CUDPSoket();

//---Send

	/// <summary>
	/// ���M�p�ʐM�o�H���J��( TCP�Ƃ͈Ⴂ�A�n���h�V�F�C�N���������߃R�l�N�V�����͕K�v�Ȃ��B�I�[�v���̂݁j
	/// </summary>
	void Open( u16 portNo , const char *address );
	void Open( const CSoketAddressPtr &spAddress );

	/// <summary>
	/// ���M�p�ʐM�o�H����� 
	/// </summary>
	void Close();

	/// <summary>
	/// �f�[�^���M
	/// �������f�[�^�͕K���͂��킯�ł͂Ȃ��BUDP�̑��ݒʐM�ŕK���f�[�^���M�𐬌����������ꍇ�A��M������ǂ��܂ł̃f�[�^���͂������𑗂��Ă��炤�K�v������B
	/// ����̎�M�����ł����������\�������邽�߁A��M�����̍đ�������s��Ȃ��Ƃ����Ȃ��B
	/// Ethernet�̃t���[���T�C�Y�̏����1500�o�C�g�Ȃ̂ŁA���M�o�H���Ethernet�����݂���ꍇ������������IP�w�Ńp�P�b�g�������s���Ă��܂��B
    /// �������ꂽ�p�P�b�g��IP�w�ōĂь��������͂��Ȃ̂ŁA��{��薳�����A�����ɑΉ����Ă��Ȃ��@�퓙�����݂���ꍇ���Ȃ̂�1500�o�C�g��
    /// ���Ȃ��悤�ɃA�v���P�[�V�����Œ�������̂��]�܂����B
	/// </summary>
	u32  Send( const void* pData , u32 size );

	/// <summary>
	/// �f�[�^�̎�M������
	/// </summary>
	u32  Receive( void* pData , u32 size , u32 tryNum = 8  );

//---SendBroadCast

	/// <summary>
	/// �u���[�h�L���X�g���M�p�ʐM�o�H���J��
	/// </summary>
	void OpenBroadCast( u16 portNo , const char *address );

	/// <summary>
	/// �u���[�h�L���X�g���M�p�ʐM�o�H����� 
	/// </summary>
	void CloseBroadCast();

	/// <summary>
	/// �u���[�h�L���X�g�f�[�^���M�i�ŋ߂̃��[�^�[�̓u���[�h�L���X�g���M��ʂ��Ȃ��ݒ肪�f�t�H���g�ł���ꍇ�������炵���̂œ͂��Ȃ����Ƃ������炵���j
	/// �Ⴆ�΃l�b�g���[�N��ɂƂ���A�v���P�[�V�����������オ���Ă��Ȃ������u���[�h�L���X�g�������ReceiveBroadCast�̌��ʂŒT�邱�Ƃ��ł���B
	/// </summary>
	u32  SendBroadCast( const void* pData , u32 size );

	/// <summary>
	/// �f�[�^�̎�M������(��������ԐM�����������d�v�ɂȂ邽�߁AfromAddress�ł��̏�������j
	/// </summary>
	u32  ReceiveBroadCast( void* pData , u32 size , const CSoketAddressPtr &spFromAddress , u32 tryNum = 8 );

//---SendMultiCast

	/// <summary>
	/// �}���`�L���X�g���M�p�ʐM�o�H���J��
	/// netInterfaceAdr�͔��M���@�킪�����̃l�b�g���[�N�J�[�h��t���Ă����ꍇ�A�ǂ̃l�b�g���[�N�J�[�h����}���`�L���X�g���o�͂��邩���w�肷�邽�߂̃A�h���X�B
	/// �R�}���h�v�����v�g��[ ipconfig ]�Ƒł��ĂłĂ���C���^�[�l�b�g�@��̃A�h���X���ǂ�ł������̂œ����΂悢�B
	/// �l�b�g���[�N�@���IP�A�h���X���v���O�����Ŏ擾����ꍇ�AWindows�Ȃ�IP Helper API���g�p����B
	/// TTL�Ƃ�IP�p�P�b�g�̃��[�e�B���O�ɂ����鐶�����ԁB�p�P�b�g��1�̃��[�^�[��ʉ߂��閈��1�����Z����A0�ɂȂ����ꍇ�p�P�b�g�͔p�������B
	/// </summary>
	void OpenMultiCast( u16 portNo , const char *address , char *netInterfaceAdr , s32 ttlNum );

	/// <summary>
	/// �}���`�L���X�g���M�p�ʐM�o�H����� 
	/// </summary>
	void CloseMultiCast();

	/// <summary>
	/// �}���`�L���X�g�f�[�^���M
	/// </summary>
	u32  SendMultiCast( const void* pData , u32 size );

	/// <summary>
	/// �f�[�^�̎�M������(��������ԐM�����������d�v�ɂȂ邽�߁AfromAddress�ł��̏�������j
	/// </summary>
	u32  ReceiveMultiCast( void* pData , u32 size , const CSoketAddressPtr &spFromAddress , u32 tryNum = 8 );

private:

//---Send

	/// <summary>
	/// ���M�ڑ�����
	/// </summary>
	CSoketAddressPtr m_spSendSoketAddress;

	/// <summary>
	/// ���M�\�P�b�gOpen�ς݃t���O
	/// </summary>
	skyBool m_IsOpen;

	/// <summary>
	/// �\�P�b�g�n���h��
	/// </summary>
	socket_hndl m_SendSoketHndl;

//---SendBroadCast

	/// <summary>
	/// �u���[�h�L���X�g�ڑ�����
	/// </summary>
	CSoketAddressPtr m_spBroadCastSoketAddress;

	/// <summary>
	/// �u���[�h�L���X�g�\�P�b�gOpen�ς݃t���O
	/// </summary>
	skyBool m_IsOpenBroadCast;

	/// <summary>
	/// �u���[�h�L���X�g�\�P�b�g�n���h��
	/// </summary>
	socket_hndl m_BroadCastSoketHndl;

//---SendMultiCast

	/// <summary>
	/// ���M�ڑ�����
	/// </summary>
	CSoketAddressPtr m_spMultiCastSoketAddress;

	/// <summary>
	/// �}���`�L���X�g�\�P�b�gOpen�ς݃t���O
	/// </summary>
	skyBool m_IsOpenMultiCast;

	/// <summary>
	/// �}���`�L���X�g�\�P�b�g�n���h��
	/// </summary>
	socket_hndl m_MultiCastSoketHndl;
};

} } }
