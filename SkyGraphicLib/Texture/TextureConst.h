#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �e�N�X�`���@�\�̒萔
///
/// ���Ή��t�H�[�}�b�g�i�K�v�ɉ����đ��₷���A�ނ�݂ɐF�X�T�|�[�g����K�v�͂Ȃ��j
/// �Etag
/// �Ebmp256
/// �Edds
/// ���Ή��t�H�[�}�b�g�ł���΁A�p�X����e�N�X�`���̕��A�����A�t�H�[�}�b�g���̏����擾���鏈������������
/// 
/// ��dds�ւ̃R���o�[�g�Ή��^�C�v�i�R���o�[�g��NVIDIA�̃R���o�[�^�[���g�p����B�ǂ̌`���ɃR���o�[�g���������̓e�N�X�`�����Ŏw�肷��j
/// �EDXT1
/// �EDXT1a
/// �EDXT3
/// �EDXT5
/// �ERGBA
/// ���Ή����k�^�C�v�ł���΃p�X��n���ΔC�ӂ̃t�H���_��dds�o�͂��Ă����c�[����p�ӂ���
/// 
/// ���c�|�C���g�́A�p�X���炠�����񂪎擾�ł���悤�Ȏ����B�e�N�X�`���̏�����ł����Ȃ��Ƃ����Ȃ��Ȃ�Ď����͂m�f
/// </summary>
namespace texture
{
	/// <summary>
	/// �Ή����k�^�C�v�L�[���[�h
	/// �t�@�C�����Ɉ��k�`���������������܂܂�Ă���̂ŁA����ɂ���������dds�ɂ���B
	/// ��jhoge_dxt1_mip03.tga : dxt1���k�ŁAmipmap�͂R��
	/// </summary>
	static const skyString *s_eComp_dds_type_keyword[] =
	{ 
		_T( "dxt1" ) , 
		_T( "dxt1a" ) , 
		_T( "dxt3" ) ,
		_T( "dxt5" ) ,
		_T( "rgba" ) ,
	};
	/// <summary>
	/// �~�b�v�}�b�v���L�[���[�h
	/// </summary>
	static const skyString *mip_keyword = _T( "mip" );

	/// <summary>
	/// �Ή��t�@�C���t�H�[�}�b�g
	/// </summary>
	enum eTextureFileFormat
	{
		eTextureFileFormat_bmp = 0 ,
		eTextureFileFormat_jpg ,
		eTextureFileFormat_png ,
		eTextureFileFormat_dds ,
		eTextureFileFormat_tiff ,
		eTextureFileFormat_gip ,
		eTextureFileFormat_wmp
	};

	/// <summary>
	/// �Ή��t�H�[�}�b�g
	/// </summary>
	enum eTextureFormat
	{
		eTextureFormat_R8G8B8 = 0 ,
		eTextureFormat_A8R8G8B8 ,    //��ԗL���ǂ���ł��B1�s�N�Z��1byte(8bit)�Ń�������ɐF���| ABGR | ABGR |...�ƕ���ł���Ɖ��߂��܂�(���g���G���f�B�A���̂���)�B
		eTextureFormat_X8R8G8B8 ,
		eTextureFormat_A32R32G32B32 ,
		eTextureFormat_P8 ,              //8bit�J���[�C���f�b�N�X�t��
	};

	/// <summary>
	/// �Ή��e�N�X�`�����T�C�Y
	/// </summary>
	enum eTexture_with
	{
		eTexture_with_1    = 1 ,
		eTexture_with_2    = 2 ,
		eTexture_with_4    = 4 ,
		eTexture_with_8    = 8 ,
		eTexture_with_16   = 16 ,
		eTexture_with_32   = 32 ,
		eTexture_with_64   = 64 ,
		eTexture_with_128  = 128 ,
		eTexture_with_256  = 256 ,
		eTexture_with_512  = 512 ,
		eTexture_with_768  = 768 ,
		eTexture_with_1024 = 1024 ,
	};

	/// <summary>
	/// �Ή��e�N�X�`���c�T�C�Y
	/// </summary>
	enum eTexture_height
	{
		eTexture_height_1    = 1 ,
		eTexture_height_2    = 2 ,
		eTexture_height_4    = 4 ,
		eTexture_height_8    = 8 ,
		eTexture_height_16   = 16 ,
		eTexture_height_32   = 32 ,
		eTexture_height_64   = 64 ,
		eTexture_height_128  = 128 ,
		eTexture_height_256  = 256 ,
		eTexture_height_512  = 512 ,
		eTexture_height_768  = 768 ,
		eTexture_height_1024 = 1024 ,
	};

	/// <summary>
	/// SkyTexture���
	/// </summary>
	struct CTextureInfo
	{
		eTexture_with           Width;         //��
		eTexture_height         Height;        //����
		u32                     MipLevels;     //�~�b�v�}�b�v�̖���
		eTextureFormat          Format;        //�����t�H�[�}�b�g
	};
}

} } }