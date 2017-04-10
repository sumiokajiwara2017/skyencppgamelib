#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CMaterialDiffuseColorShaderObject;
typedef SmartPointer< CMaterialDiffuseColorShaderObject >		CMaterialDiffuseColorShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �}�e���A����Diffuse�J���[�̐F�ł��̂܂܏o�͂���V�F�[�_�[
/// </summary>
class CMaterialDiffuseColorShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMaterialDiffuseColorShaderObject(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();

//---Serialize

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr 		  Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---static

	/// <summary>
	/// �쐬
	/// </summary>
	static CMaterialDiffuseColorShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMaterialDiffuseColorShaderObjectPtr spBasicLightShaderObject( NEW__( CMaterialDiffuseColorShaderObject , CMaterialDiffuseColorShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMaterialDiffuseColorShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CMaterialDiffuseColorShaderObjectCreate_()			    sky::lib::graphic::CMaterialDiffuseColorShaderObject::Create()
#define CMaterialDiffuseColorShaderObjectCreateName_( name )	sky::lib::graphic::CMaterialDiffuseColorShaderObject::Create( name )
