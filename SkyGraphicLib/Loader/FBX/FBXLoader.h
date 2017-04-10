#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// FBX�`���̃f�[�^����O���t�B�b�N�f�[�^�����[�h����N���X
/// </summary>
class CFBXLoader : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// �t�@�C�����烍�[�h
	/// �p�X�̃R���g���[����CGraphicFilePathManager�ŕK���s���Ă��������B
	/// �ǂݍ��񂾌��ʂ̓f�[�^�x�[�X����擾�ł��܂��B
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );

};

} } }

#define CFBXLoaderInitialize_() sky::lib::graphic::CFBXLoader::Initialize()
#define CFBXLoaderLoader_	    sky::lib::graphic::CFBXLoader
#define CFBXLoaderDispose_()    sky::lib::graphic::CFBXLoader::Dispose()
