#pragma once

/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CDirectionalLightShaderObject;
typedef SmartPointer< CDirectionalLightShaderObject >		CDirectionalLightShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// ���s���V�F�[�_�[
/// </summary>
class CDirectionalLightShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDirectionalLightShaderObject(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();

    /// <summary>
	///  ���̕���������
    /// </summary>
	void SetLightDirection( const math::CBasicVector3 &postion )
	{ 
		m_LightDirection = postion;
		m_LightDirection.Normalize();
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
	static CDirectionalLightShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CDirectionalLightShaderObjectPtr spBasicLightShaderObject( NEW__( CDirectionalLightShaderObject , CDirectionalLightShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// ���C�g�̕���
	/// </summary>
	math::CBasicVector3 m_LightDirection;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CDirectionalLightShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CDirectionalLightShaderObjectCreate_()			 sky::lib::graphic::CDirectionalLightShaderObject::Create()
#define CDirectionalLightShaderObjectCreateName_( name ) sky::lib::graphic::CDirectionalLightShaderObject::Create( name )
