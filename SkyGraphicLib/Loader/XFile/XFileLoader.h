#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// XFile�`���̃f�[�^����O���t�B�b�N�f�[�^�����[�h����N���X
/// </summary>
class CXFileLoader : public base::SkyObject
{

public:

	/// <summary>
	/// �t�@�C�����烍�[�h
	/// �p�X�̃R���g���[����CGraphicFilePathManager�ŕK���s���Ă��������B
	/// �ǂݍ��񂾌��ʂ̓f�[�^�x�[�X����擾�ł��܂��B
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );
};

} } }

#define CXFileLoaderLoader_	    sky::lib::graphic::CXFileLoader
