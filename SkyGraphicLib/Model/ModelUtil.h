#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// ���f���֗��֐��Q
/// </summary>
class CModelUtil
{

public:

	/// <summary>
	/// ���f���𐶐����܂��BMesh�͑��Ő����ς݂Ƃ��܂��B�V�F�[�_�[��PointLightShader��ݒ�B
	/// </summary>
	static CModelPtr CreateModelFromMesh( const CMeshContenaPtr &spMesh , CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// WavefrontOBJ�t�@�C�����烂�f���𐶐����܂��B�V�F�[�_�[��PointLightShader��ݒ�B
	/// </summary>
    static CModelPtr CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test�p������
	/// </summary>
    static CModelPtr CreateTestCube( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test�p�O�p����
	/// </summary>
    static CModelPtr CreateTestCone( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test���i�X�P�[�������̑傫����10x10��XZ���ʁj
	/// </summary>
    static CModelPtr CreateTestPlane( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test��
	/// </summary>
    static CModelPtr CreateTestSphere( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// �R�c�_���쐬
	/// </summary>
    static CModelPtr CreatePoint( const math::CBasicVector3 &point , CColor color = CColor_Gold , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// �R�c�̐��̍쐬
	/// </summary>
	static CModelPtr CreateLine3D( const math::CBasicVector3Vector &vector , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );
	static CModelPtr CreateLine3D( const math::CBasicLine3List &lineList , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// �C�ӂ̓_�W���Ń��f�����쐬����
	/// </summary>
	static CModelPtr Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CTextureMappingShaderObject::Create() );

	/// <summary>
	/// �R���̊��
	/// </summary>
	static CModelPtr CreateStandardLine3( dectype length = 20 );

	/// <summary>
	/// �w�y���ʊi�q
	/// </summary>
	static CModelPtr CreateGrid_XZ( dectype large = 1 , const CColor &color = CColor_Silver , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// �w�y���ʊi�q
	/// </summary>
	static CModelPtr CreateGrid_YX( dectype large = 1 , const CColor &color = CColor_Silver , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// �w�y���ʉ~��\������
	/// </summary>
	static CModelPtr CreateCircle_XZ( const math::CBasicCircle& circle , const CColor &color = CColor_Silver , u32 granularity = 32 );

	/// <summary>
	/// �w�x���ʋ�`���쐬����
	/// </summary>
	static CModelPtr CreateSquare( const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// �R�c�̐������̂̍쐬
	/// </summary>
	static CModelPtr CreateLine3DCube( const math::CBasicVector3Vector &vector , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );
	static CModelPtr CreateLine3DCube( const math::CMatrix4x3 &trans = math::CMatrix4x3_Identity , const CColor &color = CColor_Red );

	/// <summary>
	/// ����䗧���̂̃��f���̍쐬
	/// </summary>
	static CModelPtr CreateFrustum( const math::CBasicVector3 &pos , const CameraPtr &spCamera , const CColor &color = CColor_Red );

	/// <summary>
	/// �s���~�b�h�^�̐��̃��f�����쐬
	/// </summary>
	static CModelPtr CreateLinePyramid(  const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );
	static CModelPtr CreateLinePyramid( const math::CMatrix4x3 &trans = math::CMatrix4x3_Identity , const CColor &color = CColor_Red );

	/// <summary>
	/// �R�c�̐����̂̍쐬�i���_�����S�Ŏw��̔��a�̋����[���I�ɕ`�悷��
	/// </summary>
	static CModelPtr CreateLine3DSphere( const math::CBasicSphere &sphere , const CColor &color = CColor_Red , u32 granularity = 32 );

	/// <summary>
	/// �`�`�a�a����ŕ\�����郂�f�����쐬
	/// </summary>
	static CModelPtr CreateAABBDisp( const math::CAABB3 &aabb , const CColor &color );

	/// <summary>
	/// �n�a�a����ŕ\�����郂�f�����쐬
	/// </summary>
	static CModelPtr CreateOBBDisp( const math::COBB3 &obb , const CColor &color );
};

} } }