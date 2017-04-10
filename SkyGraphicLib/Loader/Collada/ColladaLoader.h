#pragma once

/// <summary>
/// COLLADA�̎���LIB�ւ̃����N
/// </summary>
#ifdef SW_SKYLIB_USE_COLLADA_DOM

#ifdef _DEBUG

#ifdef SW_SKYLIB_USE_COLLADA_14
#pragma comment( lib, "libcollada14dom22-sd.lib" )
#else
#pragma comment( lib, "libcollada15dom22-sd.lib" )
#endif

#pragma comment( lib, "pcrecppd.lib" )
#pragma comment( lib, "pcred.lib" )
#pragma comment( lib, "libxml2-vc100-mt.lib" )
#else

#ifdef SW_SKYLIB_USE_COLLADA_14
#pragma comment( lib, "libcollada14dom22-s.lib" )
#else
#pragma comment( lib, "libcollada15dom22-s.lib" )
#endif

#pragma comment( lib, "pcrecpp.lib" )
#pragma comment( lib, "pcre.lib" )
#pragma comment( lib, "libxml2-vc100-mt.lib" )
#endif

#endif

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// Collada�`���̃f�[�^����O���t�B�b�N�f�[�^�����[�h����N���X
/// ���ӁI�ǂݍ��ރ��f���͈ȉ��̏����𖞂����Ă�������
/// �E�O�p�`�|���S���ł��邱��
/// �E�}�e���A�����t�����Ă��邱��
/// 
/// Blender��SkyLib(DirectX11)�f�[�^�̂��߂Ɉȉ��̃R���p�[�g������
/// �ÓIMesh
/// �E���_��y��z�����ւ���B
/// �A�j���[�V�����f�[�^
/// �EY���g�k��Z���g�k��
/// �EZ���g�k��Y���g�k��
/// �EY����]��Z����]��
/// �EZ����]��Y����]��
/// �E��]�l�͂��ׂă}�C�i�X��
/// �EY���ړ���Z���ړ���
/// �EZ���ړ���Y���ړ���
/// �E�s����ȉ��̂悤�ɃR���o�[�g����
///  +-----------+    +-----------+
///  |00,01,02,03|    |00,08,04,12|
///  |04,05,06,07| �� |02,10,06,14|
///  |08,09,10,11| �� |01,09,05,13|
///  |12,13,14,15|    |03,11,07,15|
///  ------------+    +-----------+
/// ���R�[�h�ɂ����
/// skylib[ 00 ] = blender[ 00 ];
/// skylib[ 08 ] = blender[ 01 ];
/// skylib[ 04 ] = blender[ 02 ];
/// skylib[ 12 ] = blender[ 03 ] * scaling;
/// skylib[ 02 ] = blender[ 04 ];
/// skylib[ 10 ] = blender[ 05 ];
/// skylib[ 06 ] = blender[ 06 ];
/// skylib[ 14 ] = blender[ 07 ] * scaling;
/// skylib[ 01 ] = blender[ 08 ];
/// skylib[ 09 ] = blender[ 09 ];
/// skylib[ 05 ] = blender[ 10 ];
/// skylib[ 13 ] = blender[ 11 ] * scaling;
///	skylib[ 03 ] = blender[ 12 ];
///	skylib[ 11 ] = blender[ 13 ];
///	skylib[ 07 ] = blender[ 14 ];
///	skylib[ 15 ] = blender[ 15 ];
/// </summary>
class ColladaLoader : public base::SkyObject
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

#define ColladaLoaderLoader_	    sky::lib::graphic::ColladaLoader
