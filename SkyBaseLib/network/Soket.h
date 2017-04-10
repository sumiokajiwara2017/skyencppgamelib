#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �\�P�b�g�N���X
/// �G���[�������ɂ͗�O�𔭐������܂��B
/// �A�v���P�[�V�������̓G���[�𔻒肵�A�������s���Ă��������B
/// �� ��O����������
///	try
///	{
///    ����
/// }
///	catch( net::CSoketException &e )
/// {
///    ��O����
///	   e.Print();
/// }
/// catch( net::CSoketAppException &e )
/// {
///    ��O����
///	   e.Print();
/// }
///�� �����Ȃ���Ȃ�������A�f�[�^���ςɂȂ��Ă�����G���f�B�A�����^���܂��傤�B
///�� �l�b�g���[�N�̊e��d�l��RFC�𒲂ׂ�ƂłĂ���BICMP�Ȃ�RFC792���B�i��Fhttp://www5d.biglobe.ne.jp/~stssk/rfcjlist.html �j
/// </summary>
class CSoket : public base::SkyRefObject
{

public:

	/// <summary>
	/// �������i�S�̂ň�x�����j
	/// </summary>
	static void Initialize();

	/// <summary>
	/// �������i�S�̂ň�x�����j
	/// </summary>
	static void Dispose();

protected:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CSoket();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CSoket();

	/// <summary>
	/// �\�P�b�g���J��
	/// </summary>
	inline void Open(  socket_hndl &soketHndl , s32 AddressFamily , s32 soketType , s32 protcol );

	/// <summary>
	/// �\�P�b�g�����
	/// </summary>
	inline void Close( socket_hndl &soketHndl );

	/// <summary>
	/// �T�[�o�[�̃A�h���X���(IP�A�h���X�y�у|�[�g�ԍ�)���\�P�b�g�Ɍ��ѕt����B
	/// </summary>
	inline void Bind( socket_hndl &soketHndl , const CSoketAddressPtr &spAddr ); 

	/// <summary>
	/// �ڑ�
	/// </summary>
	inline void Connect( socket_hndl &soketHndl , const CSoketAddressPtr &spAddr );

	/// <summary>
	/// �ڑ�����
	// 0 ��M�𖳌��ɂ���
	// 1 ���M�𖳌��ɂ���
	// 2 ��M�A���M�𖳌��ɂ���
	/// </summary>
	inline void DisConnect( socket_hndl &soketHndl , s32 id );

	/// <summary>
	/// �ڑ����̎擾
	/// </summary>
	inline CSoketAddressPtr GetAddress();
	inline void             SetAddress( const CSoketAddressPtr &spSoketAddress );
};

} } }

#define CSoketInitialize_()	sky::lib::net::CSoket::Initialize()
#define CSoketDispose_()	sky::lib::net::CSoket::Dispose()

#include "Soket.inl"