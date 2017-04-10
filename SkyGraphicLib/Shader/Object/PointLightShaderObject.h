#pragma once
/*

�y ������ �z
DirectX11 PC HLSL ��������
OpenGL    PC GLSL ��������
OpenGL    PC Cg   ��������

*/

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

class   CPointLightShaderObject;
typedef SmartPointer< CPointLightShaderObject >		CPointLightShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �_�����V�F�[�_�[
/// </summary>
class CPointLightShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPointLightShaderObject(){};

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();

    /// <summary>
	///  ���̈ʒu�ݒ�
    /// </summary>
	void SetLightPosition( const math::CBasicVector3 &postion ){ m_LightPosition = postion; }

    /// <summary>
	///  ���̋����ݒ�
    /// </summary>
	void SetLightPower( f32 power ){ m_LightPower = power; }

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
	static CPointLightShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CPointLightShaderObjectPtr spBasicLightShaderObject( NEW__( CPointLightShaderObject , CPointLightShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// ���C�g�̈ʒu
	/// </summary>
	math::CBasicVector3 m_LightPosition;

	/// <summary>
	/// ���C�g�̗�
	/// </summary>
	f32                 m_LightPower;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPointLightShaderObject();

	/// <summary>
	/// �p�X�P�`��i�����ƂɎ����j
	/// </summary>
	void Pass1Draw();
};

} } }

#define CPointLightShaderObjectCreate_()			sky::lib::graphic::CPointLightShaderObject::Create()
#define CPointLightShaderObjectCreateName_( name )	sky::lib::graphic::CPointLightShaderObject::Create( name )
