#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �A�h���X�����̒���
/// </summary>
static const u32 ADDRESS_STRING_LENGTH = 32;

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct  CSoketAddressMember;

class CSoketAddress;
typedef SmartPointer< CSoketAddress > CSoketAddressPtr;

/// <summary>
/// �\�P�b�g�ڑ�����N���X
/// </summary>
class CSoketAddress : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// address��IP���z�X�g�����g�p����( ��)"127.0.0.1" �܂��� "localhost" )
	/// </summary>
	inline CSoketAddress( s32 AddressFamily , u16 portNo , const char *address , skyBool isGetHostInfo = skyTrue );
	inline CSoketAddress( s32 AddressFamily , u16 portNo ); //�T�[�o�[�p�i�A�h���X�͗v��Ȃ��j
	inline CSoketAddress();                                 //��Ő���

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CSoketAddress();

	/// <summary>
	/// �A�h���X���擾
	/// </summary>
	inline const CResolver &GetResolver() const;

	/// <summary>
	/// �����o�ϐ��擾
	/// </summary>
	inline CSoketAddressMember* GetMember();

private:

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
	CSoketAddressMember* m_pMember;

	/// <summary>
	/// �A�h���X���ϊ���
	/// </summary>
	CResolver m_Resolver;
};

#ifdef SKYLIB_PLATFORM_WIN32

/// <summary>
/// WinSoket2�\�P�b�g�ڑ�����
/// </summary>
struct  CSoketAddressMember
{
	/// <summary>
	/// �ڑ����
	/// </summary>
	SOCKADDR_IN m_ConnectInfo;

	/// <summary>
	/// �ڑ��敶�����
	/// </summary>
	u8 m_Address[ ADDRESS_STRING_LENGTH ];
};

#endif

} } }

#include "SoketAddress.inl"