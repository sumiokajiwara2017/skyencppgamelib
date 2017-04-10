#pragma once

/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CPerspectiveShadowMapDrawShaderObject;
typedef SmartPointer< CPerspectiveShadowMapDrawShaderObject >		CPerspectiveShadowMapDrawShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// CookTorrance���f���V�F�[�_�[
/// </summary>
class CPerspectiveShadowMapDrawShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPerspectiveShadowMapDrawShaderObject(){};

	/// <summary>
	/// �V���h�E�}�b�v�����_�[�^�[�Q�b�g��ݒ肷��
	/// </summary>
	void AttachShadowMapRenderTarget( const CRenderTargetPtr &spShadowMap );

	/// <summary>
	/// CPerspectiveShadowMapCreateShaderObject��ݒ肷��
	/// </summary>
	void AttachShadowMapCreater( const CPerspectiveShadowMapCreateShaderObjectPtr &spCreater );

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
	static CPerspectiveShadowMapDrawShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CPerspectiveShadowMapDrawShaderObjectPtr spBasicLightShaderObject( NEW__( CPerspectiveShadowMapDrawShaderObject , CPerspectiveShadowMapDrawShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPerspectiveShadowMapDrawShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();

	/// <summary>
	/// �V���h�E�}�b�v�����_�[�^�[�Q�b�g
	/// </summary>
	CRenderTargetPtr                           m_spShadowMapRenderTarget;

	/// <summary>
	/// �V���h�E�}�b�v�N���G�C�^�[�̎Q��
	/// </summary>
	CPerspectiveShadowMapCreateShaderObjectPtr m_spShadowMapCreate;
};

} } }

#define CPerspectiveShadowMapDrawShaderObjectCreate_()			 sky::lib::graphic::CPerspectiveShadowMapDrawShaderObject::Create()
#define CPerspectiveShadowMapDrawShaderObjectCreateName_( name ) sky::lib::graphic::CPerspectiveShadowMapDrawShaderObject::Create( name )
