#pragma once

/*

�����o�[�g�Ɩ��͎��������Ɉˑ����Ȃ������̓��˕����Ɩʂ̖@�������ŎZ�o�����A�e�����Z�@�ł��B

�y ������ �z
DirectX11 PC HLSL �~������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   �~������

�y �g�pCMaterial�l �z
�EDiffuse
�EAlpha

�y �g�pCLight�l �z
//���s���̃p�����[�^�[�����܂��B
�ECDirectionalLightData::m_AmbientColor
�ECDirectionalLightData::m_DiffuseColor
�ECDirectionalLightData::m_Vec

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CLambertShaderObject;
typedef SmartPointer< CLambertShaderObject >		CLambertShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// Lambert���f���V�F�[�_�[
/// </summary>
class CLambertShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CLambertShaderObject(){};

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
	static CLambertShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CLambertShaderObjectPtr spBasicLightShaderObject( NEW__( CLambertShaderObject , CLambertShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CLambertShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CLambertShaderObjectCreate_()			 sky::lib::graphic::CLambertShaderObject::Create()
#define CLambertShaderObjectCreateName_( name ) sky::lib::graphic::CLambertShaderObject::Create( name )
