#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CTextureMappingShaderType2Object;
typedef SmartPointer< CTextureMappingShaderType2Object >		CTextureMappingShaderType2ObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �e�N�X�`���}�b�s���O������V�F�[�_�[
/// </summary>
class CTextureMappingShaderType2Object : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTextureMappingShaderType2Object(){};

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
	static CTextureMappingShaderType2ObjectPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CTextureMappingShaderType2ObjectPtr spBasicLightShaderObject( NEW__( CTextureMappingShaderType2Object , CTextureMappingShaderType2Object() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTextureMappingShaderType2Object();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CTextureMappingShaderType2ObjectCreate_()			sky::lib::graphic::CTextureMappingShaderType2Object::Create()
#define CTextureMappingShaderType2ObjectCreateName_( name )	sky::lib::graphic::CTextureMappingShaderType2Object::Create( name )

