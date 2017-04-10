#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// ���_�֘A��`
/// </summary>
namespace vertex {

	/// <summary>
	/// �v���~�e�B�u�̎�ށi���_�̃v���~�e�B�u�^�C�v�͈ȉ��̂����ꂩ��j
	/// </summary>
	enum ePrimitiveType
	{
		ePrimitiveType_Pointlist = 0,		//���W��_�ŕ`�悷��
		ePrimitiveType_Linelist,			//�e�_�������Ԃɐ��Ō���
		ePrimitiveType_Linestrip,			//�e�_��A���I�ɐ��Ō���
		ePrimitiveType_Trianglelist,		//�e�_�ŎO�p�`��`�悷��B3�̓_���ƂɎO�p�`�������B���_�̐�������Ȃ��ꍇ�͂��̒��_�͕`�悳��Ȃ��B
		ePrimitiveType_Meshtrip,		//�O�p�`���Ȃ����悤�Ȑ}�`�ŕ`��
		ePrimitiveType_Trianglefan,			//���_�����Ԃǂ���Ɍ���ł����A����Ɏn�_�Ɗe�_�����ԁB

		ePrimitiveType_None,
	};

	/// <summary>
	/// ���_�t�H�[�}�b�g�F���_���W���
	/// </summary>
	struct CVertexFmt_Pos
	{
		f32			x;       // �w���W
		f32			y;       // �x���W
		f32			z;       // �y���W

		//�R���X�g���N�^
		CVertexFmt_Pos(){}
		CVertexFmt_Pos( f32 x_ , f32 y_ , f32 z_ )
		{
			x = x_;
			y = y_;
			z = z_;
		}
		CVertexFmt_Pos( const math::CBasicVector3& src )
		{
			*this = src;
		}

		//����p�֐��i�\���̂̃T�C�Y�ɂ̓J�E���g����Ȃ��j
        inline CVertexFmt_Pos& operator =( const math::CBasicVector3& src )
		{
			x = ( f32 )src.X; 
			y = ( f32 )src.Y; 
			z = ( f32 )src.Z;
			return *this;
		}
		inline math::CBasicVector3 Get(){ return math::CBasicVector3( x , y , z ); }
	};

	/// <summary>
	/// ���_�t�H�[�}�b�g�F�c�R�c�@���������
	/// </summary>
	struct CVertexFmt_Normal
	{
		f32			nx;      // �@���w����
		f32			ny;      // �@���w����
		f32			nz;      // �@���w����

		//�R���X�g���N�^
		CVertexFmt_Normal(){}
		CVertexFmt_Normal( f32 x , f32 y , f32 z )
		{
			nx = x; 
			ny = y; 
			nz = z;
		}
		CVertexFmt_Normal( const math::CBasicVector3& src )
		{
			*this = src;
		}

		//����p�֐��i�\���̂̃T�C�Y�ɂ̓J�E���g����Ȃ��j
        inline CVertexFmt_Normal& operator =( const math::CBasicVector3& src )
		{
			nx = ( f32 )src.X; 
			ny = ( f32 )src.Y; 
			nz = ( f32 )src.Z;
			return *this;
		}
        inline CVertexFmt_Normal& operator +=( const math::CBasicVector3& src )
		{
			nx += ( f32 )src.X; 
			ny += ( f32 )src.Y; 
			nz += ( f32 )src.Z;
			return *this;
		}
        inline CVertexFmt_Normal& operator +=( const CVertexFmt_Normal& src )
		{
			nx += ( f32 )src.nx; 
			ny += ( f32 )src.ny; 
			nz += ( f32 )src.nz;
			return *this;
		}
		inline math::CBasicVector3 Get(){ return math::CBasicVector3( nx , ny , nz ); }
	};

	/// <summary>
	/// ���_�t�H�[�}�b�g�F�J���[�l���
	/// </summary>
	struct CVertexFmt_Color
	{
		f32         color[ 4 ]; //RGBA

		//�R���X�g���N�^
		CVertexFmt_Color(){}
		CVertexFmt_Color( const graphic::CColor& src )
		{
			*this = src;
		}

		//����p�֐��i�\���̂̃T�C�Y�ɂ̓J�E���g����Ȃ��j
        inline CVertexFmt_Color& operator =( const CColor& src )
		{
			color[ 0 ] = ( f32 )src.r;
			color[ 1 ] = ( f32 )src.g;
			color[ 2 ] = ( f32 )src.b;
			color[ 3 ] = ( f32 )src.a;
			return *this;
		}
		inline CColor Get(){ return CColor( color[ 0 ] , color[ 1 ] , color[ 2 ] , color[ 3 ] ); }
	};

	/// <summary>
	/// ���_�t�H�[�}�b�g�F�e�N�X�`�����W���
	/// </summary>
	struct CVertexFmt_UV
	{
		f32			tu;      // �e�N�X�`���t�u���W�t
		f32			tv;      // �e�N�X�`���t�u���W�u

		//�R���X�g���N�^
		CVertexFmt_UV(){}
		CVertexFmt_UV( const math::CBasicVector2& src )
		{
			*this = src;
		}
		CVertexFmt_UV( f32 tu_ , f32 tv_ )
		{
			tu = tu_;
			tv = tv_;
		}

		//����p�֐��i�\���̂̃T�C�Y�ɂ̓J�E���g����Ȃ��j
        inline CVertexFmt_UV& operator =( const math::CBasicVector2& src )
		{
			tu = ( f32 )src.X; 
			tv = ( f32 )src.Y; 
			return *this;
		}
		inline math::CBasicVector2 Get(){ return math::CBasicVector2( tu , tv ); }
	};

	/// <summary>
	/// �X�L�����b�V���p�֘A�s��C���f�b�N�X
	/// </summary>
	struct CVertexFmt_Index
	{
		CVertexFmt_Index( f32 initValue )
		{
			for( u32 i = 0 ; i < 4 ; i++ )
			{
				index[ i ] = initValue;
			}
		}
		f32 index[ 4 ];
	};

	/// <summary>
	/// �X�L�����b�V���p�֘A�s��E�F�C�g
	/// </summary>
	struct CVertexFmt_Weights
	{
		CVertexFmt_Weights( f32 initValue )
		{
			for( u32 i = 0 ; i < 4 ; i++ )
			{
				weights[ i ] = initValue;
			}
		}
		f32 weights[ 4 ];
	};

}

} } }