#pragma once

/*

�y ������ �z
DirectX11 PC HLSL �~������
OpenGL    PC GLSL �~������
OpenGL    PC Cg   �~������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CookTorranceShaderObject;
typedef SmartPointer< CookTorranceShaderObject >		CookTorranceShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// CookTorrance���f���V�F�[�_�[
/// </summary>
class CookTorranceShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CookTorranceShaderObject(){};

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
	static CookTorranceShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CookTorranceShaderObjectPtr spBasicLightShaderObject( NEW__( CookTorranceShaderObject , CookTorranceShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CookTorranceShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CookTorranceShaderObjectCreate_()			 sky::lib::graphic::CookTorranceShaderObject::Create()
#define CookTorranceShaderObjectCreateName_( name ) sky::lib::graphic::CookTorranceShaderObject::Create( name )
