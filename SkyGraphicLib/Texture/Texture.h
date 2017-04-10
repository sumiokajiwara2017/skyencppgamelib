#pragma once

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)

struct CTextureMember;
typedef SmartPointer2< CTextureMember >	CTextureMemberPtr;		//�X�}�[�g�|�C���^�Œ�`

//�e���v���[�gTypedef�S
class CTexture;
typedef SmartPointer< CTexture >        CTexturePtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CTexturePtr >      CTexturePtrList;	//���X�g��`
typedef CHash< CTexturePtr >            CTexturePtrHash;	//Hash��`(�|�C���^�j

class CTexture : public CSceneGraphNode
{
	friend class CRenderTarget;
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTexture();

	/// <summary>
	/// ���[�h�ς݃t���O
	/// </summary>
	skyBool                     IsLoaded ();

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	void                         Load    ();
	void                         Load    ( const stream::IStreamPtr& stream );

	/// <summary>
	/// ��������
	/// </summary>
	void                         Save    ( texture::eTextureFileFormat eFileFormat ); //�����ێ����Ă���p�X�ŏo��
	void                         Save    ( texture::eTextureFileFormat eFileFormat , const hash::CHashStringPtr &path ); //�w��̃p�X�ŏo��

	/// <summary>
	/// �쐬
	/// </summary>
	void						 Create  ( const texture::CTextureInfo &texInfo );

	/// <summary>
	/// �j��
	/// </summary>
	void                         UnLoad  ();

	/// <summary>
	/// �e�N�X�`�����s�[�g�ݒ�
	/// </summary>
	void                         SetTextureWrapRepeat();  //�͂ݏo���������J��Ԃ�
	void                         SetTextureWrapClamp();   //�͂ݏo�����������E�̐F�Ŗ��߂�
	void                         SetTextureWrapEdge();    //�͂ݏo�����������E�̐F�Ŗ��߂�
	void                         SetTextureWrapBorder();  //�͂ݏo���������w��̋��E�̐F�Ŗ��߂�
	void                         SetTextureWrapBorderColor( CColor color );  //�͂ݏo�����������E�̐F�Ŗ��߂�

	/// <summary>
	/// �����i���ێ����Ŏ�������ꍇ�͕K��virtual�Ő錾����B�����ŃI�[�o�[���C�h����邩������Ȃ�����j
	/// </summary>
	const hash::CHashStringPtr	 GetPath () { return Name.GetName(); }

	/// <summary>
	/// �e�N�X�`�����擾
	/// </summary>
	texture::CTextureInfo&       GetInfo () { return m_Info;  }

	/// <summary>
	/// �e�N�X�`�����s�N�Z���V�F�[�_�[�ɐݒ肷��
	/// </summary>
	void SetShader();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	inline const CTextureMemberPtr& GetMember(){ return m_pMember; }

//---Serialize

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	void Deactivate();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// �ă��[�h��f�o�b�O���̂��߂Ƀp�X�͕K�������œn��
	/// </summary>
	static CTexturePtr	Create( const hash::CHashStringPtr &path = hash::CHashStringPtr() )
	{
		
		CTexturePtr spTexture( NEW__( CTexture , CTexture() ) );
		spTexture->Name.SetName( path );
		return spTexture;
	}

protected:

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CTextureMemberPtr   m_pMember;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// �e�N�X�`�����
	/// </summary>
	texture::CTextureInfo  m_Info;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTexture();
};

} } }

#define CTextureCreate_()           sky::lib::graphic::CTexture::Create()
#define CTextureCreateName_( path ) sky::lib::graphic::CTexture::Create( path )
//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//�^�̍Ē�`
typedef SmartPointerCom< ID3D11Resource >             D3D11ResourceComPtr;
typedef SmartPointerCom< ID3D11ShaderResourceView >   D3D11ShaderResourceViewPtr;
typedef SmartPointerCom< ID3D11SamplerState >         D3D11SamplerStatePtr;

struct CTextureMember
{
	/// <summary>
	/// D3D�e�N�X�`��
	/// </summary>
	D3D11ResourceComPtr		   m_spTexture;

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�𐶐�����
	/// ���e�N�X�`�����V�F�[�_�[�Ŏg��Ȃ��ꍇ�͎g�p���Ȃ�
	/// </summary>
	D3D11ShaderResourceViewPtr m_spShaderResourceView;

	/// <summary>
	/// �T���v���X�e�[�g�̌�
	/// </summary>
	D3D11_SAMPLER_DESC         m_SamplerStateDesc;

	/// <summary>
	/// �T���v���X�e�[�g
	/// </summary>
	D3D11SamplerStatePtr       m_spTextureSamplerState;

	/// <summary>
	/// �T���v���X�e�[�g�̍č쐬
	/// </summary>
	void ReCreateSamplerState();
};

} } }

#endif
//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

#include "GLTextureLoader.h"

namespace sky { namespace lib { namespace graphic {

struct CTextureMember
{
	/// <summary>
	/// GL�e�N�X�`���n���h��
	/// </summary>
    GLuint texId[ 1 ];
	GLTextureLoader m_TexObj;

	skyBool m_IsLoad;

	CTextureMember() : m_IsLoad( skyFalse )
	{
	}

	virtual ~CTextureMember()
	{
		glDeleteTextures( 1 , &texId[ 0 ] );
	}
};

} } }

#endif
