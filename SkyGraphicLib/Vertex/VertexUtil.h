#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// ���_��񃆁[�e�B���e�B
/// </summary>
class CVertexUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �_�̍쐬
	/// </summary>
	static void CreatePoint( const math::CBasicVector3 &point , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// �Q�c�����Q�̍쐬
	/// </summary>
	static void CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector2Vector &vector , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// �Q�c���̍쐬
	/// </summary>
	static void CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &start = math::CBasicVector2( 0.0f , 0.0f ) , const math::CBasicVector2 &end = math::CBasicVector2( 0.0f , 5.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// �Q�c���̂̋�`�̍쐬
	/// </summary>
	static void CreateSquareLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , dectype width = 1.0f , dectype height = 1.0f , const CColor &color = CColor_Red );

	/// <summary>
	/// �R�c�����Q�̍쐬
	/// </summary>
	static void CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CMeshPtr &spMesh , const CColor &color = CColor_Red );
	static void CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicLine3List &lineList , const CMeshPtr &spMesh , const CColor &color = CColor_Red );
	static void CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &start = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const math::CBasicVector3 &end = math::CBasicVector3( 0.0f , 5.0f , 0.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// �R�c�G���e�B�e�B��ݒ肵�Ē��_���W�ϊ�����(���̏�����CPU���������̒��_��ύX���܂��B����GPU���ւ̔��f���K�v�ȏꍇ��spVertexBuffer->vWirteGpuMemory();�Ƃ������������Ĕ��f���Ă��������B�j
	/// </summary>
	static void ComputeEntity3D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity3DPtr &entity3d );

	/// <summary>
	/// �Q�c�G���e�B�e�B��ݒ肵�Ē��_���W�ϊ�����(���̏�����CPU���������̒��_��ύX���܂��B����GPU���ւ̔��f���K�v�ȏꍇ��spVertexBuffer->vWirteGpuMemory();�Ƃ������������Ĕ��f���Ă��������B�j
	/// </summary>
	static void ComputeEntity2D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity2DPtr &entity2d );


//---------------�֗��ȕ`�����������ǉ����Ă������i�ł���{�̓��f�����O�c�[���̃f�[�^�����ɂ���j-----------------��

	/// <summary>
	/// �����̍쐬
	/// </summary>
	static void CreateSphere( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , dectype sc = 1.0f , vrtxtype divNum = 12, const CColor &color = CColor_Red );

	/// <summary>
	/// ���̍쐬�i�t�u�l���ݒ肳��Ă��āA�n���̃e�N�X�`����\��ƒn���ɂȂ�j
	/// radius : ���a
	/// xnum   : �w�������̍דx�i���̐��l�������Ƃ���Y��ȋ��̂ɂȂ邪�A���_���͑�����j
	/// ynum   : �x�������̍דx�i���̐��l�������Ƃ���Y��ȋ��̂ɂȂ邪�A���_���͑�����j
	/// </summary>
	static void CreateSphereTexture( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype radius = 1.0f , vrtxtype xnum = 8 , vrtxtype ynum = 8 , const CColor &color = CColor_Red );

	/// <summary>
	/// ���̍쐬�i�S���_�Œ�B�t�u���ݒ肳��Ă��ăe�N�X�`���ƃe�N�X�`�����s�[�g�ݒ�Œn�ʂ��o����j
	/// size     : �T�C�Y
	/// uvrelnum : �t�u�J��Ԃ���
	/// </summary>
	static void CreateFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype size = 10.0f , dectype uvrelnum = 10.0f , const CColor &color = CColor_Red );

	/// <summary>
	/// �~���̍쐬�i�@���������ɐL�тĂ���B�t�u�l���ݒ肳��Ă��āA��̃e�N�X�`����\��Ƌ�ɂȂ�B�j
	/// ypolnum      : �c�����̃|���S����
	/// circlepolnum : �~����ŉ��̃|���S�����g����
	/// </summary>
	static void CreateCylinderRevers( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMaterial , dectype radius = 1.0f , dectype height = 1.0f , vrtxtype ypolnum = 1 , vrtxtype circlepolnum = 32 , const CColor &color = CColor_Red );

	/// <summary>
	/// �����̂̒��_���̍쐬(���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�͐������Ă����Ă��������j
	/// </summary>
	static void	CreateCube( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &scale = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// ��`�̍쐬
	/// </summary>
	static void CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &color = CColor_Red );
	static void CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype width , dectype height , const math::CBasicVector2 &uvLt , const math::CBasicVector2 &uvRt , const math::CBasicVector2 &uvLb , const math::CBasicVector2 &uvRb );

	/// <summary>
	/// XZ���ʂɃO���b�h��������
	/// </summary>
	static void CreateGrid_XZ( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// YX���ʂɃO���b�h��������
	/// </summary>
	static void CreateGrid_YX( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// �i�q��̒n�ʂ��쐬
	/// </summary>
	static void CreateGridFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// ���_���w��ł̃C���f�b�N�X�t�����_�o�b�t�@�̍쐬�i���W�A�C���f�b�N�X�͑S���O�ł��B��������Ŏg�p���Ă��������j
	/// </summary>
	static CVertexBufferPtr CreateVertexBuffer(  vrtxtype vertexNum = 4 , vrtxtype indexNum = 6 , vertex::ePrimitiveType ePrimType = vertex::ePrimitiveType_Trianglelist , const CIndexBufferPtr &spIndexBuffer = CIndexBufferPtr() ); 

//---------------�֗��ȕ`�����������ǉ����Ă������i�ł���{�̓��f�����O�c�[���̃f�[�^�����ɂ���j-----------------��

	/// <summary>
	/// ���_�X�g���[�����X�g�������_���[�ɐݒ肷��
	/// </summary>
	static void SetRenderBufferStream( const CVertexBufferStreamPtrList &list , const CIndexBufferPtr &spIndexBuffer , vertex::ePrimitiveType ePrimType = vertex::ePrimitiveType_None );

//---------------���ŗl�X�Ȍ`��`��-----------------��

	/// <summary>
	/// �W���_���w�肵�ė����̂̐��}�������B���ۂ͒��_�̈ʒu�ɂ���Č`�͌��肷��̂ŁA�����̂ɂȂ邩�ǂ����͒��_�̍��W�ɂ���Ă��܂�B
	/// </summary>
	static void CreateCubeLine( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMaterial , const CColor &color = CColor_Red );

	/// <summary>
	/// �T���_���w�肵�ăs���~�b�h�̐��}�������B���ۂ͒��_�̈ʒu�ɂ���Č`�͌��肷��̂ŁA�����̂ɂȂ邩�ǂ����͒��_�̍��W�ɂ���Ă��܂�B
	/// </summary>
	static void CreatePyramid( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMaterial , const CColor &color = CColor_Red );

//---------------���W���C���f�b�N�X���O���w��-----------------��

	/// <summary>
	/// �쐬
	/// </summary>
	static void Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh );
};

} } }

