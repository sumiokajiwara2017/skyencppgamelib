#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// SkyGraphicBinary�`���̃f�[�^����O���t�B�b�N�f�[�^�����[�h����N���X
/// </summary>
class CSkyGraphicBinaryLoader : public base::SkyObject
{

public:

	/// <summary>
	/// �t�@�C�����烍�[�h
	/// �p�X�̃R���g���[����CGraphicFilePathManager�ŕK���s���Ă��������B
	/// �ǂݍ��񂾌��ʂ̓f�[�^�x�[�X����擾�ł��܂��B
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );

	/// <summary>
	/// SceneGraphNode���t�@�C���ɃZ�[�u
	/// </summary>
	static void SaveFile( const hash::CHashStringPtr &path , const CSceneGraphNodePtr &spSceneGraphNode );
};

} } }

#define CSkyGraphicBinaryLoader_	sky::lib::graphic::CSkyGraphicBinaryLoader
