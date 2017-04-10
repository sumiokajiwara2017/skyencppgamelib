#pragma once

namespace sky { namespace lib { namespace net {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct  CResolverMember;

/// <summary>
/// IP�A�h���X�A�z�X�g������֘A�t������z�X�g���A�G�C���A�X���AIP�A�h���X�̂��ׂĂ��擾����
/// </summary>
class CResolver : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CResolver();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CResolver();
	
	/// <summary>
	/// �z�X�g������������i�w��l��"127.0.0.1"�Ȃǂ̃A�h���X��"localhost"���̃z�X�g��
	/// </summary>
	inline void Lookup( const char *pStr );

	/// <summary>
	/// �z�X�g�����擾����
	/// </summary>
	inline const string::CString& GetHostName();

	/// <summary>
	/// �z�X�g���Ɋ֘A����ʖ����擾����i��������\������j
	/// </summary>
	inline const string::CStringList& GetAliasesList();

	/// <summary>
	/// �z�X�g���Ɋ֘A����IP�A�h���X���擾����i��������\������j
	/// </summary>
	inline const string::CStringList& GetAddressList();

	/// <summary>
	/// �����o�͂���
	/// </summary>
	inline void Print();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	inline const CResolverMember *GetMember() const;

private:

	/// <summary>
	/// �����̃����o�ϐ�
	/// </summary>
	CResolverMember *m_pMember;

	/// <summary>
	/// �z�X�g��
	/// </summary>
	string::CString m_HostName;

	/// <summary>
	/// �G�C���A�X��
	/// </summary>
	string::CStringList m_AliasesList;

	/// <summary>
	/// �A�h���X���X�g�i�����j
	/// </summary>
	string::CStringList m_AddressList;
};

#ifdef SKYLIB_PLATFORM_WIN32

/// <summary>
/// WinSoket2�\�P�b�g�ڑ�����
/// </summary>
struct  CResolverMember
{
	/// <summary>
	/// �o�C�i���A�h���X�z��
	/// </summary>
	typedef CVectorPrimitive< ULONG > CBinAddressVector;
	CBinAddressVector m_BinAddress;

	/// <summary>
	/// �w��̃o�C�i���A�h���X���z��ɑ��݂��邩�`�F�b�N
	/// </summary>
	inline skyBool CheckBinAddress( ULONG address ) const;
};

#endif

} } }

#include "Resolver.inl"