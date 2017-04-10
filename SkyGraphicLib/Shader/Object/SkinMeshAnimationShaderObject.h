#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CSkinMeshAnimationShaderObject;
typedef SmartPointer< CSkinMeshAnimationShaderObject >		CSkinMeshAnimationShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �X�L�����b�V���A�j���[�V�����V�F�[�_�[
/// </summary>
class CSkinMeshAnimationShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CSkinMeshAnimationShaderObject(){};

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
	static CSkinMeshAnimationShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CSkinMeshAnimationShaderObjectPtr spBasicLightShaderObject( NEW__( CSkinMeshAnimationShaderObject , CSkinMeshAnimationShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSkinMeshAnimationShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CSkinMeshAnimationShaderObjectCreate_()				sky::lib::graphic::CSkinMeshAnimationShaderObject::Create()
#define CSkinMeshAnimationShaderObjectCreateName_( name )	sky::lib::graphic::CSkinMeshAnimationShaderObject::Create( name )
