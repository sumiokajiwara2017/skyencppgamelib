#pragma once

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)

struct CRenderTargetMember;
typedef SmartPointer2< CRenderTargetMember > CRenderTargetMemberPtr;		//�X�}�[�g�|�C���^�Œ�`

//�e���v���[�gTypedef�S

class CRenderTarget;
typedef SmartPointer< CRenderTarget >			    CRenderTargetPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList		< u32 , CRenderTargetPtr >	    CRenderTargetPtrList;	//���X�g��`
typedef CHash		< CRenderTargetPtr >			CRenderTargetPtrHash;	//Hash��`
typedef CHashMT		< CRenderTargetPtr >			CRenderTargetPtrHashMT;	//Hash��`

/// <summary>
///  �����_�����O�Ώۃo�b�t�@�N���X�B
///  �����_�����O�^�[�Q�b�g�ɂ͓��ނ���B
///  �@�t���[���o�b�t�@�����_�����O�^�[�Q�b�g�iSetUpFrameBuffer�ō쐬�A�`��p�j
///  �A�e�N�X�`�������_�����O�^�[�Q�b�g�iSetUpTexture�ō쐬�A�I�t���C���`��p�j
/// </summary>
class CRenderTarget : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  �f�X�g���N�^
    /// </summary>
	virtual                ~CRenderTarget();

	/// <summary>
	/// �E�B���h�E�ɏ����o�����߂̃����_�����O�^�[�Q�b�g���쐬����B
	/// </summary>
	void                   SetUpFrameBuffer( const CWindowPtr &spWindow );

	/// <summary>
	/// �e�N�X�`���ɏ����o�����߂̃����_�����O�^�[�Q�b�g���쐬����B�����ɓn���ꂽ�e�N�X�`�����g�p���ă����_�����O���邱�Ƃ��ł���j
	/// </summary>
	void                   SetUpTexture( const math::CBasicSize &size );

	/// <summary>
	/// �J��
	/// </summary>
	void                   Release();

	/// <summary>
	/// �t���[���o�b�t�@�����_�����O�^�[�Q�b�g���H
	/// </summary>
	skyBool                IsFrameBufferRenderTarget();

	/// <summary>
	/// �T�C�Y�̎擾�^�ݒ�
	/// </summary>
	math::CBasicSize       GetSize()                      { return m_Size; }
	void                   SetSize( math::CBasicSize size ){ m_Size = size; }

	/// <summary>
	/// �T�C�Y�̕ύX
	/// </summary>
	void                   ReSize          ( u32 newWidth , u32 newHeight );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	void				  vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	void				  vPostRendering   ( CRenderingParam &param );

	/// <summary>
	/// �J���[�o�b�t�@���e�N�X�`���Ƃ��Ď擾����
	/// </summary>
	CTexturePtr            GetColorBufferTexture();

	/// <summary>
	/// �J���[�o�b�t�@�N���A�J���[��ݒ�^�擾����
	/// </summary>
	CColor&                GetClearColor()                      { return m_ClearColor; };
	void                   SetClearColor( const CColor &color ) { m_ClearColor = color; };

//-----�`��̍ێg�p-------��

	/// <summary>
	/// �J���[�o�b�t�@�̃N���A
	/// </summary>
    void                   ClearColorBuffer();

	/// <summary>
	/// �����_���[�ɃJ���[�o�b�t�@�Ɛ[�x�X�e���V���o�b�t�@��ݒ肷��i�ȍ~�ݒ肳�ꂽ���̂ɂ������ĕ`�悪�s����j
	/// </summary>
    void                   SetRenderer( const CDepthStencilBufferPtr &spBuffer = CDepthStencilBufferPtr() );

	/// <summary>
	/// �����_�����O�^�[�Q�b�g��Display�ɑ��M
	/// </summary>
    void                    Present();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	inline const CRenderTargetMemberPtr &GetMember(){ return m_pMember; }

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
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr 		  Clone();
	serialize::CSerializeInterfacePtr         CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CRenderTargetPtr	   Create( const hash::CHashStringPtr &pName = hash::CHashStringPtr() )
	{
		CRenderTargetPtr spRenderTarget( NEW__( CRenderTarget , CRenderTarget() ) );
		spRenderTarget->Name.SetName( pName );
		return spRenderTarget;
	}

protected:

//---Member

	/// <summary>
	/// ��ʏc���̒���
	/// </summary>
	math::CBasicSize        m_Size;

	/// <summary>
	/// �N���A�J���[
	/// </summary>
	CColor                  m_ClearColor;

//---Member

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CRenderTargetMemberPtr  m_pMember;

	/// <summary>
	/// �J���[�o�b�t�@�e�N�X�`��
	/// </summary>
	CTexturePtr             m_spColorBufferTexture;

	/// <summary>
	/// �t���[���o�b�t�@�����_�����O�^�[�Q�b�g�t���O
	/// </summary>
	skyBool                 m_IsFrameBuffer;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderTarget();
};

} } }

#define CRenderTargetCreate_()           sky::lib::graphic::CRenderTarget::Create()
#define CRenderTargetCreateName_( name )	sky::lib::graphic::CRenderTarget::Create( name )

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< IDXGISwapChain >	         D3DSwapChainComPtr;
typedef SmartPointerCom< ID3D11RenderTargetView >    D3D11RenderTargetViewPtr; 
typedef SmartPointerCom< ID3D11DepthStencilView >    D3D11DepthStencilViewPtr;

struct CRenderTargetMember
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CRenderTargetMember()
	{
		if ( m_spSwapChain.IsEmpty() == skyFalse )
		{
			//�t���X�N���[����������߂�
			BOOL IsFullscreen;
			m_spSwapChain->GetFullscreenState( &IsFullscreen, NULL );
			if ( IsFullscreen )
			{
				m_spSwapChain->SetFullscreenState( FALSE, NULL );
			}
		}
	}

	/// <summary>
	/// �X���b�v�`�F�C��
	/// </summary>
	D3DSwapChainComPtr					m_spSwapChain;

	/// <summary>
	/// �����_�����O�^�[�Q�b�g
	/// </summary>
	D3D11RenderTargetViewPtr            m_spRenderTargetView;

	/// <summary>
	/// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�𗘗p�����J���[�o�b�t�@�̍\�z
	/// </summary>
	void                               CreateColorBufferFromSwapChain( CTexturePtr &spColorBufferTexture );
};


} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

struct CRenderTargetMember
{
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CRenderTargetMember()
	{
		if ( m_FOB.IsEmpty() == skyFalse )
		{
			m_FOB->Dispose();
		}
	}

	/// <summary>
	/// Window�n���h��
	/// </summary>
	HWND m_nWindowHndl;

	/// <summary>
	/// �f�o�C�X�R���e�L�X�g
	/// </summary>
    HDC   m_hDC;

	/// <summary>
	/// �����_�����O�R���e�L�X�g
	/// </summary>
	HGLRC m_hRC;

	/// <summary>
	/// Glew�R���e�L�X�g
	/// </summary>
	GLEWContext *m_GlewC;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderTargetMember() : m_IsFrameBuffer( skyTrue ) , m_hDC( NULL ) , m_hRC( NULL ) , m_GlewC( NULL ){}

	/// <summary>
	/// �t���[���o�b�t�@���t���[���o�b�t�@�I�u�W�F�N�g���̔���t���O
	/// </summary>
	skyBool m_IsFrameBuffer;

	/// <summary>
	/// �t���[���o�b�t�@�I�u�W�F�N�g�i�t���[���o�b�t�@�̎��͋�j
	/// </summary>
	CFramebufferObjectPtr m_FOB;
};

} } }

#endif