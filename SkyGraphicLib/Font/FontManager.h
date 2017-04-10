#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �P�����f�[�^
/// </summary>
struct COneFontData
{
	//�������W
	math::CVector2< d64 > pos[ 4 ];

	//UV���W
	math::CVector2< d64 > uv[ 4 ];

	//�������͂ދ�`�̃T�C�Y�B�������Ɏ��K�v���Ȃ��ƍl���Ă������A�����^�C�v�̕����ł��A�S�p�Ɣ��p�ňႤ�ȂǁA�l�X�ȃp�^�[��������ƍl�����̂ŁB�i���ۂ�2��ނ����j
	math::CSize< d64 > size;
};

/// <summary>
/// �����Ǘ��N���X
/// </summary>
class CFontManager : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose();

	/// <summary>
	/// �����f�[�^�����[�h����i�r�b�g�}�b�v�A�t�u�o�C�i���j
	/// </summary>
	static void Load( font::eFontStyleType eStyle , const skyString *bmpPath , const skyString *uvPath );

	/// <summary>
	/// �����R�[�h�ƃX�^�C���ɑΉ��������_�����擾����
	/// </summary>
	static COneFontData &GetFont( u32 srcCode , font::eFontStyleType eStyle );

	/// <summary>
	/// �X�^�C���ɑΉ������e�N�X�`�������擾����
	/// </summary>
	static CTexturePtr &GetFontStyleTex( font::eFontStyleType eStyle );
};

//�C���X�^���X�A�N�Z�X������
#define CFontManagerInitialize_()	sky::lib::graphic::CFontManager::Initialize()
#define CFontManager_				sky::lib::graphic::CFontManager
#define CFontManagerDispose_()		sky::lib::graphic::CFontManager::Dispose()

} } }