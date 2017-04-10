#pragma once

/*

�y �T�v �z
�t�H���̋��ʔ��˃��f���͔��˃x�N�g�����v�Z���܂����C�ꕔ�̊��ɂ���Ĕ��˃x�N�g�����v�Z����̂���ςȂ����Ȃ̂ŁC
���C�g�x�N�g��L�Ǝ����x�N�g��E�̒��Ԃ̊p�x�ɂ���x�N�g���Ōv�Z����Blinn-Phong���ʔ��˃��f���Ƃ����̂��l���o����Ă��܂��B

�y ������ �z
DirectX11 PC HLSL �~������
OpenGL    PC GLSL ��������
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

class   CBlinnPhongShaderObject;
typedef SmartPointer< CBlinnPhongShaderObject >		CBlinnPhongShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// BlinnPhong���f���V�F�[�_�[
/// </summary>
class CBlinnPhongShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CBlinnPhongShaderObject(){};

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
	static CBlinnPhongShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CBlinnPhongShaderObjectPtr spBasicLightShaderObject( NEW__( CBlinnPhongShaderObject , CBlinnPhongShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CBlinnPhongShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CBlinnPhongShaderObjectCreate_()			 sky::lib::graphic::CBlinnPhongShaderObject::Create()
#define CBlinnPhongShaderObjectCreateName_( name ) sky::lib::graphic::CBlinnPhongShaderObject::Create( name )
