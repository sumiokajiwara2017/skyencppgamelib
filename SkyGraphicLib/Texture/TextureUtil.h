#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �e�N�X�`���֗������Q
/// </summary>
class CTextureUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �e�N�X�`���p�X����e�N�X�`�������擾����
	/// </summary>
	static void GetTextureInfo( const hash::CHashStringPtr &path , texture::CTextureInfo &dest );

	/// <summary>
	/// �e�N�X�`����dds�t�H�[�}�b�g�Ɉ��k����
	/// </summary>
	static void ddsConvert( const hash::CHashStringPtr &path , stream::IStreamPtr &dest );
};

} } }