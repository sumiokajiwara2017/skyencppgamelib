#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CVertexDiffuseColorShaderObject;
typedef SmartPointer< CVertexDiffuseColorShaderObject >		CVertexDiffuseColorShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// ���_�J���[�̂ݏo���V�F�[�_�[�i���_���ɒ��_�J���[�����݂��邱�Ƃ�O��Ƃ���j
/// </summary>
class CVertexDiffuseColorShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CVertexDiffuseColorShaderObject(){};

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
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---static

	/// <summary>
	/// �쐬
	/// </summary>
	static CVertexDiffuseColorShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CVertexDiffuseColorShaderObjectPtr spBasicLightShaderObject( NEW__( CVertexDiffuseColorShaderObject , CVertexDiffuseColorShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CVertexDiffuseColorShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CVertexDiffuseColorShaderObjectCreate_()			sky::lib::graphic::CVertexDiffuseColorShaderObject::Create()
#define CVertexDiffuseColorShaderObjectCreateName_( name )	sky::lib::graphic::CVertexDiffuseColorShaderObject::Create( name )
