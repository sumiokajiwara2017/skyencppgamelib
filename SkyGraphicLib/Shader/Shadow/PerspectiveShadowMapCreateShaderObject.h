#pragma once

/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

PerspectiveShadowMap���쐬���܂��B

PSM�ŉe��n��菇��CPerspectiveShadowMapCreateShaderObject�ŃV�[���̃��b�V�����V���h�[�}�b�v�Ƀ����_�����O��
CPerspectiveShadowMapDrawShaderObject�Ń��b�V���𕁒ʂɃ����_�����O���A�V���h�[�}�b�v���g�p���ĉe�������Ƀ����_�����O����

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

static const dectype MIN_Z_NEAR  = 3.0f;
static const dectype MAX_Y_SCENE = 10.0f;
static const dectype SQRT2       = ( dectype )math::CMathUtil::Sqrt( 2.0 );
static const dectype SQRT3       = ( dectype )math::CMathUtil::Sqrt( 3.0 );

class   CPerspectiveShadowMapCreateShaderObject;
typedef SmartPointer< CPerspectiveShadowMapCreateShaderObject >		CPerspectiveShadowMapCreateShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// CookTorrance���f���V�F�[�_�[
/// </summary>
class CPerspectiveShadowMapCreateShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

	friend class CPerspectiveShadowMapDrawShaderObject;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPerspectiveShadowMapCreateShaderObject(){};

	/// <summary>
	/// ���C�g�̈ʒu��ݒ�
	/// </summary>
	void SetShadowLightPosition( const math::CBasicVector4 &lightPos );

	/// <summary>
	/// PSM�ɗp����s����X�V����
	/// </summary>
	void UpdateShadowMatrix();

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();

//---Serialize

	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---static

	/// <summary>
	/// �쐬
	/// </summary>
	static CPerspectiveShadowMapCreateShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CPerspectiveShadowMapCreateShaderObjectPtr spBasicLightShaderObject( NEW__( CPerspectiveShadowMapCreateShaderObject , CPerspectiveShadowMapCreateShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �K�v�p�����[�^�[
	/// </summary>
	math::CMatrix4x3    m_LightView;
	math::CMatrix4x3    m_LightProjection;
	math::CMatrix4x3    m_LightViewProjection;
	math::CMatrix4x3    m_PsmMatrix;
	math::CBasicVector4 m_WorldSpaceLight;

	//�ȉ��͌��݂̃J�����̏��Ɋ�{�܂܂�Ă��邪�APSM�p�ɂ�������l���Z�o����
	math::CMatrix4x3    m_View;
	math::CMatrix4x3    m_Projection;
	math::CMatrix4x3    m_ViewProjection;
	dectype             m_NearClip;
	dectype             m_FarClip;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPerspectiveShadowMapCreateShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();

	/// <summary>
	/// �J�����̍s������߂�
	/// </summary>
	/// <param name="wsLight">���[���h��Ԃ̃��C�g</param>
	void CalcCameraMtx( const math::CBasicVector4 &wsLight );

	/// <summary>
	/// ���s�����̏ꍇ�̃��C�g�̍s������߂�
	/// </summary>
	/// <param name="ppsLight">�����ϊ���̋�Ԃ̃��C�g</param>
	void CalcLightMtx_ParalleLight( const math::CBasicVector4 &ppsLight );

	/// <summary>
	/// �_�����̏ꍇ�̃��C�g�̍s������߂�
	/// </summary>
	/// <param name="ppsLight">�����ϊ���̋�Ԃ̃��C�g</param>
	void CalcLightMtx_PointLight( const math::CBasicVector4 &ppsLight_ );
};

} } }

#define CPerspectiveShadowMapCreateShaderObjectCreate_()           sky::lib::graphic::CPerspectiveShadowMapCreateShaderObject::Create()
#define CPerspectiveShadowMapCreateShaderObjectCreateName_( name ) sky::lib::graphic::CPerspectiveShadowMapCreateShaderObject::Create( name )
