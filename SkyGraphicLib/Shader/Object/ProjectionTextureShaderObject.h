#pragma once
/*

�y ������ �z
DirectX11 PC HLSL �������ρB�o�O����B
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)

struct CProjectionTextureShaderObjectMember;

class   CProjectionTextureShaderObject;
typedef SmartPointer< CProjectionTextureShaderObject >		CProjectionTextureShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// ���s���V�F�[�_�[
/// </summary>
class CProjectionTextureShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CProjectionTextureShaderObject(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();

    /// <summary>
	///  �e�N�X�`�������̈ʒu
    /// </summary>
	void SetTextureLightPosition( const math::CBasicVector3 &position )
	{
		m_TextureLightPosition = position;
	}

    /// <summary>
	///  �e�N�X�`�������̕���
    /// </summary>
	void SetTextureLightTag( const math::CBasicVector3 &tag )
	{
		m_TextureLightTag = tag;
	}

    /// <summary>
	///  �e�N�X�`�������̏�����x�N�g��
    /// </summary>
	void SetTextureLightUp( const math::CBasicVector3 &up )
	{
		m_TextureLightUp = up;
	}

    /// <summary>
	///  �A�X�y�N�g��
    /// </summary>
	void SetAspect( dectype aspect )
	{
		m_TextureLightAspect = aspect;
	}

    /// <summary>
	///  �e�N�X�`�������̉�p�ݒ�
    /// </summary>
	void SetTextureLightFov( dectype fov )
	{
		m_TextureLightFov = fov;
	}

    /// <summary>
	///  �e�N�X�`�������̋ߐږʋ����ݒ�
    /// </summary>
	void SetTextureLightNear( dectype near_ )
	{
		m_TextureLightNear = near_;
	}

    /// <summary>
	///  �e�N�X�`�������̉����ʋ����ݒ�
    /// </summary>
	void SetTextureLightFar( dectype far_ )
	{
		m_TextureLightFar = far_;
	}

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
	static CProjectionTextureShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CProjectionTextureShaderObjectPtr spBasicLightShaderObject( NEW__( CProjectionTextureShaderObject , CProjectionTextureShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �e�N�X�`�����C�g�̈ʒu
	/// </summary>
	math::CBasicVector3 m_TextureLightPosition;

	/// <summary>
	/// �e�N�X�`�����C�g�̏����
	/// </summary>
	math::CBasicVector3 m_TextureLightUp;

	/// <summary>
	/// �e�N�X�`�����C�g�̃^�[�Q�b�g
	/// </summary>
	math::CBasicVector3 m_TextureLightTag;

    /// <summary>
	///  �e�N�X�`�������̉�p
    /// </summary>
	dectype m_TextureLightFov;

    /// <summary>
	///  �e�N�X�`�������̉�ʔ䗦
    /// </summary>
	dectype m_TextureLightAspect;

    /// <summary>
	///  �e�N�X�`�������̋ߐږʋ���
    /// </summary>
	dectype m_TextureLightNear;

    /// <summary>
	///  �e�N�X�`�������̉����ʋ���
    /// </summary>
	dectype m_TextureLightFar;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CProjectionTextureShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CProjectionTextureShaderObjectCreate_()			sky::lib::graphic::CProjectionTextureShaderObject::Create()
#define CProjectionTextureShaderObjectCreateName_( name )	sky::lib::graphic::CProjectionTextureShaderObject::Create( name )

