#pragma once

/*

�y �T�v �z
����̂���v���X�e�B�b�N�Ȃǂ̋��ʔ��˂̌v�Z�ɂ悭�g����B

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

�y �g�pCMaterial�l �z
�EAmbient
�EDiffuse
�ESpecular
�EPower
�EAlpha

�y �g�pCLight�l �z
//���s���̃p�����[�^�[�����܂��B
�ECDirectionalLightData::m_DiffuseColor
�ECDirectionalLightData::m_SpecularColor
�ECDirectionalLightData::m_Vec

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CPhongShaderObject;
typedef SmartPointer< CPhongShaderObject >		CPhongShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// Phong���f���V�F�[�_�[
/// </summary>
class CPhongShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPhongShaderObject(){};

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
	static CPhongShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CPhongShaderObjectPtr spBasicLightShaderObject( NEW__( CPhongShaderObject , CPhongShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPhongShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CPhongShaderObjectCreate_()			 sky::lib::graphic::CPhongShaderObject::Create()
#define CPhongShaderObjectCreateName_( name ) sky::lib::graphic::CPhongShaderObject::Create( name )
