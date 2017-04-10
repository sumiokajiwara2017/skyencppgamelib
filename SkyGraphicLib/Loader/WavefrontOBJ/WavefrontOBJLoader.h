#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// WavefrontOBJ�`���̃f�[�^����O���t�B�b�N�f�[�^�����[�h����N���X
/// </summary>
class CWavefrontOBJLoader : public base::SkyObject
{

public:

	/// <summary>
	/// obj�t�@�C�������[�h����
	/// �p�X�̃R���g���[����CGraphicFilePathManager�ŕK���s���Ă��������B
    /// obj�t�@�C���Ɋ֘A����}�e���A���A�e�N�X�`�����ǂݍ���
	/// ���[�h���ꂽ���b�V����SetMesh�Ńf�[�^�x�[�X�Ɋi�[����ACreateMesh�ŃN���[�����ꂽ�l�����̊֐��ŕԂ��܂��B
	/// ���łɃ��[�h����Ă�����A�N���[�������l�𑦕Ԃ��܂��B
	/// </summary>
	static CMeshContenaPtr Load( const hash::CHashStringPtr &path , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex );
};

} } }

#define CWavefrontOBJLoader_	         sky::lib::graphic::CWavefrontOBJLoader

