#pragma once

namespace sky { namespace lib { namespace file {

/// <summary>
/// �v���p�e�B�Ǘ��N���X
/// </summary>
class CPropertyManager : public base::SkyObject
{

public:

	/// <summary>
	/// �������idata/property�f�B���N�g���ȉ��̃t�@�C�������ׂēǂށj
	/// </summary>
	static void Initialize	();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose		();

	/// <summary>
	/// �t�@�C�����ƃL�[�ƃC���f�b�N�X�Œl�𕶎��Ŏ擾
	/// </summary>
	static const hash::CHashStringPtr &GetString( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index = 0 );

	/// <summary>
	/// �t�@�C�����ƃL�[�ƃC���f�b�N�X�Œl�𐮐��Ŏ擾
	/// </summary>
	static s32 Gets32( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index = 0 );

	/// <summary>
	/// �t�@�C�����ƃL�[�ƃC���f�b�N�X�Œl�������Ŏ擾
	/// </summary>
	static d64 Getd64( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index = 0 );

	/// <summary>
	/// �v���p�e�B�t�@�C���̓��e���_���v����
	/// </summary>
	static void Dump( stream::IStreamPtr &dest );

private:

	/// <summary>
	/// �t�@�C�����̃v���p�e�B�i�[�n�b�V��
	/// </summary>
	static hash::CHashStringPtrListPtrHashPtrHash	*m_pFileDataHash;
};

#define CPropertyManagerInitialize_()	sky::lib::file::CPropertyManager::Initialize()
#define CPropertyManager_				sky::lib::file::CPropertyManager
#define CPropertyManagerDispose_()		sky::lib::file::CPropertyManager::Dispose()

} } }