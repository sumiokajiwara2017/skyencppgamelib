#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CTextureMappingShaderObject;
typedef SmartPointer< CTextureMappingShaderObject >		CTextureMappingShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �e�N�X�`���}�b�s���O������V�F�[�_�[
/// </summary>
class CTextureMappingShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTextureMappingShaderObject(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();

//---Serialize

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---static

	/// <summary>
	/// �쐬
	/// </summary>
	static CTextureMappingShaderObjectPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CTextureMappingShaderObjectPtr spBasicLightShaderObject( NEW__( CTextureMappingShaderObject , CTextureMappingShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTextureMappingShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CTextureMappingShaderObjectCreate_()			sky::lib::graphic::CTextureMappingShaderObject::Create()
#define CTextureMappingShaderObjectCreateName_( name )	sky::lib::graphic::CTextureMappingShaderObject::Create( name )
