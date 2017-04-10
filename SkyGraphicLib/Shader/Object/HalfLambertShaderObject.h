#pragma once

/*

�y �T�v �z
�n�[�t�E�����o�[�g���C�e�B���O��Valve��'98�N�́uHalf-Life�v����g�p���Ă���Valve�Ǝ��̋^�����W�I�V�e�B���C���e�B���O�Z�@�ł��邻���ł��B
�����o�[�g���A�e���͂����肵�Ă��炸�A���A�����Ƃ����]���B

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

class   CHalfLambertShaderObject;
typedef SmartPointer< CHalfLambertShaderObject >		CHalfLambertShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// Lambert���f���V�F�[�_�[
/// </summary>
class CHalfLambertShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CHalfLambertShaderObject(){};

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
	static CHalfLambertShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CHalfLambertShaderObjectPtr spBasicLightShaderObject( NEW__( CHalfLambertShaderObject , CHalfLambertShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CHalfLambertShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CHalfLambertShaderObjectCreate_()			 sky::lib::graphic::CHalfLambertShaderObject::Create()
#define CHalfLambertShaderObjectCreateName_( name ) sky::lib::graphic::CHalfLambertShaderObject::Create( name )
