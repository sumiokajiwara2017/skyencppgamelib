#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CRenderState , sky::lib::graphic::CSceneGraphNode );

//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< ID3D11RasterizerState >   D3D11RasterizerStatePtr;
typedef SmartPointerCom< ID3D11DepthStencilState > D3D11DepthStencilStatePtr;
typedef SmartPointerCom< ID3D11BlendState >        D3D11BlendStatePtr;

//-------------------------------------< ���� >---------------------------------------------//

//�X�e�[�g���x�[�X�\����
struct D3DSetRenderStateInfo;
typedef SmartPointer< D3DSetRenderStateInfo >   D3DSetRenderStateInfoPtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< u32 , D3DSetRenderStateInfoPtr > D3DSetRenderStateInfoPtrList;		//�X�}�[�g�|�C���^���X�g�^
typedef void ( *SetterFunc )();
struct D3DSetRenderStateInfo : public base::SkyRefObject
{
    virtual void                   Save    () = 0;
    virtual void                   Reverse () = 0;
	SetterFunc	                   m_pSetValueFunc;
};

//���X�^���C�U�[�X�e�[�g���\����
struct D3DRasterizerStateInfo : public D3DSetRenderStateInfo
{
    virtual void Save    ()
	{
		ID3D11RasterizerState *pRasterizerState;
		CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->RSGetState( &pRasterizerState );
		m_Save.SetPtr( pRasterizerState );
	}
    virtual void Reverse ()
	{
		if ( m_Save.IsEmpty() ) return;
		CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->RSSetState( m_Save.GetPtrNoConst() );
	}

	D3D11RasterizerStatePtr m_Save;
};

//�k�x�X�e���V���X�e�[�g���\����
struct D3DDepthStencilStateInfo : public D3DSetRenderStateInfo
{
    virtual void Save    ()
	{
		ID3D11DepthStencilState *pDepthStencilState;
		CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMGetDepthStencilState( &pDepthStencilState , &m_SaveRefCount );
		m_Save.SetPtr( pDepthStencilState );
	}
    virtual void Reverse ()
	{
		if ( m_Save.IsEmpty() ) return;
		CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetDepthStencilState( m_Save.GetPtrNoConst() , m_SaveRefCount );
	}

	D3D11DepthStencilStatePtr m_Save;
	UINT                      m_SaveRefCount;
};

//�u�����h�X�e�[�g���\����
struct D3DBlendStateInfo : public D3DSetRenderStateInfo
{
    virtual void Save    ()
	{
		ID3D11BlendState *pBlendState = skyNull;
		CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMGetBlendState( &pBlendState , m_SaveBlendFactor , &m_SaveMask );
		m_Save.SetPtr( pBlendState );
	}
    virtual void Reverse ()
	{
		if ( m_Save.IsEmpty() )
		{
            float blendFactor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
			CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetBlendState( NULL , blendFactor , 0xffffffff ); //���S�ɏ������Ⴄ
		}
		else
		{
			CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetBlendState( m_Save.GetPtrNoConst() , m_SaveBlendFactor , m_SaveMask );
		}
	}

	D3D11BlendStatePtr m_Save;
	FLOAT m_SaveBlendFactor[ 4 ];
	UINT  m_SaveMask;
};

struct CRenderStateMember
{
	/// <summary>
	/// �����_�[�X�e�[�g�ݒ胊�X�g
	/// </summary>
	D3DSetRenderStateInfoPtrList  m_SettingList;
};

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

#define RENDER_STATE_SET_MAX_NUM ( 8 )

//�X�e�[�g���x�[�X�\����
typedef void ( *SetterFunc )();
struct GLSetRenderStateInfo : public base::SkyRefObject
{
	GLSetRenderStateInfo( const GLenum &eKind , u8 setNum = 1 )
	{
		m_SetNum = setNum;
		const GLenum *pKind = &eKind;
		for( u32 i = 0 ; i < m_SetNum ; i++ )
		{
			m_eKind[ i ] = pKind[ i ];
		}
	}
	void Save()
	{ 
		for( u32 i = 0 ; i < m_SetNum ; i++ )
		{
			m_SaveValue[ i ] = glIsEnabled( m_eKind[ i ] );
		}
	};
	void Reverse()
	{
		for( u32 i = 0 ; i < m_SetNum ; i++ )
		{
			if( m_SaveValue[ i ] == false )
			{
				glDisable( m_eKind[ i ] );
			}
		}
	};
	SetterFunc	m_pSetValueFunc;
	u32         m_SetNum;
	GLenum      m_eKind[ RENDER_STATE_SET_MAX_NUM ];
	GLboolean   m_SaveValue[ RENDER_STATE_SET_MAX_NUM ];
};
typedef SmartPointer< GLSetRenderStateInfo >   GLSetRenderStateInfoPtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< u32 , GLSetRenderStateInfoPtr > GLSetRenderStateInfoPtrList;		//�X�}�[�g�|�C���^���X�g�^

struct CRenderStateMember
{
	/// <summary>
	/// �����_�[�X�e�[�g�ݒ胊�X�g
	/// </summary>
	GLSetRenderStateInfoPtrList  m_SettingList;
};

} } }

#endif

namespace sky { namespace lib { namespace graphic {

//===================================�yCommon�z====================================================>

void CRenderState::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This instance must use smartpointer.\n" ) );

	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�������X�^�b�N�ɐς�
	param.RenderStateStack.Push( thisPtr( CRenderStatePtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B

	//�����_�[�X�e�[�g��L���ɂ���
	CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );
}

void CRenderState::vPostRendering( CRenderingParam &param )
{
	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�X�^�b�N������
	param.RenderStateStack.Pop();

	if ( param.RenderStateStack.GetNum() > 0 )
	{
		//��O�̃����_�[�X�e�[�g��L���ɂ���
		CRenderCommand::ReverseRenderState( param.RenderStateStack.GetTop() );
	}
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsEnableCulling , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsEnableScissorTest , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsEnableDepthBuffer , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsEnableStencilTest , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsEnableStencilMask , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsEnableTextureBlending , sizeof( s32 ) , isWrite ); \

skyBool CRenderState::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "renderstate bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}
			
skyBool CRenderState::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "renderstate bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CRenderState::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CRenderStatePtr )spContainer )->m_IsEnableCulling = m_IsEnableCulling;
	( ( CRenderStatePtr )spContainer )->m_IsEnableScissorTest = m_IsEnableScissorTest;
	( ( CRenderStatePtr )spContainer )->m_IsEnableDepthBuffer = m_IsEnableDepthBuffer;
	( ( CRenderStatePtr )spContainer )->m_IsEnableStencilTest = m_IsEnableStencilTest;
	( ( CRenderStatePtr )spContainer )->m_IsEnableStencilMask = m_IsEnableStencilMask;
	( ( CRenderStatePtr )spContainer )->m_IsEnableTextureBlending = m_IsEnableTextureBlending;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CRenderState::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CRenderState::Clone()
{
	return Copy( CRenderStatePtr( NEW__( CRenderState , CRenderState() ) ) );
}

serialize::CSerializeInterfacePtr CRenderState::CloneSharing()
{
	return Clone();
}

void CRenderState::Activate()
{
	ACTIVATE_COMMON();

	if ( m_IsEnableCulling == skyTrue )
	{
		EnableCulling();
	}
	if ( m_IsEnableScissorTest == skyTrue )
	{
		EnableScissorTest();
	}
	if ( m_IsEnableDepthBuffer == skyTrue )
	{
		EnableDepthBuffer();
	}
	if ( m_IsDisableDepthBuffer == skyTrue )
	{
		DisableDepthBuffer();
	}
	if ( m_IsEnableStencilTest == skyTrue )
	{
		EnableStencilTest();
	}
	if ( m_IsEnableStencilMask == skyTrue )
	{
		EnableStencilMask();
	}
	if ( m_IsEnableTextureBlending == skyTrue )
	{
		EnableTextureBlending();
	}
}

void CRenderState::Deactivate()
{
	DEACTIVATE_COMMON();
}

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

CRenderState::CRenderState()
{
	Constractor();
	m_pMember = NEW__( CRenderStateMember , CRenderStateMember );
}

CRenderState::~CRenderState()
{
	DEL m_pMember;
}

void CRenderState::EnableCulling ()
{
	
	m_IsEnableCulling = skyTrue;
}

void EnableScissorTestImple()
{
	//���X�^���C�U�X�e�[�g�̍쐬
	ID3D11RasterizerState *pState;
	D3D11_RASTERIZER_DESC RasterizerDesc =
	{
		D3D11_FILL_SOLID,	// D3D11_FILL_MODE FillMode;   //������؂�ւ���΃��C���[�t���[���\��( D3D11_FILL_WIREFRAME )�ɂł���
		D3D11_CULL_NONE,	// D3D11_CULL_MODE CullMode;   //������؂�ւ���΃J�����O�ł���
		TRUE,				// BOOL FrontCounterClockwise; //������ς���ƎO�p�`������肩�E��肩������ł���B
		0,					// INT DepthBias;
		0.0f,				// FLOAT DepthBiasClamp;
		FALSE,				// FLOAT SlopeScaledDepthBias;
		TRUE,				// BOOL DepthClipEnable;
		TRUE,				// BOOL ScissorEnable;
		FALSE,				// BOOL MultisampleEnable;
		TRUE,				// BOOL AntialiasedLineEnable;
	};
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateRasterizerState( &RasterizerDesc, &pState ); 
	if( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in making the state." ) ); //It failed in making the state.
	}

	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->RSSetState( pState );
}

void CRenderState::EnableScissorTest ()
{
	D3DRasterizerStateInfo* pInfo = NEW__( D3DRasterizerStateInfo , D3DRasterizerStateInfo() );
	pInfo->m_pSetValueFunc = &EnableScissorTestImple;
	m_pMember->m_SettingList.AddFront( D3DSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableScissorTest = skyTrue;
}

static void EnableDepthBufferImple()
{
	ID3D11DepthStencilState *pState = skyNull;

	//�[�x�^�X�e���V���̃X�e�[�g��ݒ�
    D3D11_DEPTH_STENCIL_DESC DepthDesc =
	{
        TRUE,                            // �[�x�o�b�t�@��L���ɂ���;
        D3D11_DEPTH_WRITE_MASK_ALL,      // D3D11_DEPTH_WRITE_MASK DepthWriteMask;
        D3D11_COMPARISON_LESS_EQUAL,     // D3D11_COMPARISON_FUNC DepthFunc;
    };
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice->CreateDepthStencilState( &DepthDesc, &pState );
	if( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in making the state." ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetDepthStencilState( pState , 0 );
}
void CRenderState::EnableDepthBuffer ()
{
	D3DDepthStencilStateInfo* pInfo = NEW__( D3DDepthStencilStateInfo , D3DDepthStencilStateInfo() );
	pInfo->m_pSetValueFunc = &EnableDepthBufferImple;
	m_pMember->m_SettingList.AddFront( D3DSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableDepthBuffer = skyTrue;
}

static void DisableDepthBufferImple()
{
	ID3D11DepthStencilState *pState = skyNull;

	//�[�x�^�X�e���V���̃X�e�[�g��ݒ�
    D3D11_DEPTH_STENCIL_DESC DepthDesc =
	{
        FALSE,                            // �[�x�o�b�t�@�𖳌��ɂ���;
        D3D11_DEPTH_WRITE_MASK_ALL,      // D3D11_DEPTH_WRITE_MASK DepthWriteMask;
        D3D11_COMPARISON_LESS_EQUAL,     // D3D11_COMPARISON_FUNC DepthFunc;
    };
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice->CreateDepthStencilState( &DepthDesc, &pState );
	if( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in making the state." ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetDepthStencilState( pState , 0 );
}
void CRenderState::DisableDepthBuffer ()
{
	D3DDepthStencilStateInfo* pInfo = NEW__( D3DDepthStencilStateInfo , D3DDepthStencilStateInfo() );
	pInfo->m_pSetValueFunc = &DisableDepthBufferImple;
	m_pMember->m_SettingList.AddFront( D3DSetRenderStateInfoPtr( pInfo ) );
	m_IsDisableDepthBuffer = skyTrue;
}

static void EnableStencilTestImple ()
{
	ID3D11DepthStencilState *pState = skyNull;

	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; // �[�x�e�X�g����
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO; // �������܂Ȃ�
	DepthStencil.DepthFunc        = D3D11_COMPARISON_LESS; // ��O�̕��̂�`��
	DepthStencil.StencilEnable    = TRUE; // �X�e���V���E�e�X�g����
	DepthStencil.StencilReadMask  = 0xff; // �X�e���V���ǂݍ��݃}�X�N�B
	DepthStencil.StencilWriteMask = 0xff; // �X�e���V���������݃}�X�N�B

	// �ʂ��\�������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ�
	DepthStencil.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR;  // �{�P
	DepthStencil.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // ��ɐ���

	// �ʂ����������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ�
	DepthStencil.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilPassOp      = D3D11_STENCIL_OP_INCR;   // �{�P //������D3D11_STENCIL_OP_DECR�ɂ��ăV���h�E�{�����[���������_�����O����Ɖe���ł���B
	DepthStencil.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // ��ɐ���
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice->CreateDepthStencilState(
		                                       &DepthStencil ,
											   &pState );
	if( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in making the state." ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetDepthStencilState( pState , 0 );
}

void CRenderState::EnableStencilTest ()
{
	D3DDepthStencilStateInfo* pInfo = NEW__( D3DDepthStencilStateInfo , D3DDepthStencilStateInfo() );
	pInfo->m_pSetValueFunc = &EnableStencilTestImple;
	m_pMember->m_SettingList.AddFront( D3DSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableStencilTest = skyTrue;
}

static void EnableStencilMaskImple ()
{
	ID3D11DepthStencilState *pState = skyNull;

	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�̍쐬
	// �[�x/�X�e���V���E�X�e�[�g�E�I�u�W�F�N�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; // �[�x�e�X�g����
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO; // �������܂Ȃ�
	DepthStencil.DepthFunc        = D3D11_COMPARISON_GREATER; // ���̕��̂�`��
	DepthStencil.StencilEnable    = TRUE;     // �X�e���V���E�e�X�g����
	DepthStencil.StencilReadMask  = 0xff;     // �X�e���V���ǂݍ��݃}�X�N�B
	DepthStencil.StencilWriteMask = 0;        // �X�e���V���������݃}�X�N�B

	// �ʂ��\�������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ�
	DepthStencil.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // �ێ�
	DepthStencil.FrontFace.StencilFunc        = D3D11_COMPARISON_NOT_EQUAL; // �����łȂ��ꍇ�A����

	// �ʂ����������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ�
	DepthStencil.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;   // �ێ�
	DepthStencil.BackFace.StencilFunc        = D3D11_COMPARISON_NEVER;  // ��Ɏ��s
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice->CreateDepthStencilState(&DepthStencil,
											   &pState);
	if( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in making the state." ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetDepthStencilState( pState , 0 );
}
void CRenderState::EnableStencilMask ()
{
	D3DDepthStencilStateInfo* pInfo = NEW__( D3DDepthStencilStateInfo , D3DDepthStencilStateInfo() );
	pInfo->m_pSetValueFunc = &EnableStencilMaskImple;
	m_pMember->m_SettingList.AddFront( D3DSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableStencilMask = skyTrue;
}

static void EnableTextureBlendingImple ()
{
	//�u�����h�X�e�[�g�̐ݒ�
	ID3D11BlendState *pState;
	D3D11_BLEND_DESC BlendDesc =
	{
		FALSE,			// BOOL AlphaToCoverageEnable;
		FALSE,			// BOOL IndependentBlendEnable;
	};

	BlendDesc.RenderTarget[ 0 ].BlendEnable           = TRUE;                         // BOOL BlendEnable;
	BlendDesc.RenderTarget[ 0 ].SrcBlend              = D3D11_BLEND_SRC_ALPHA;        // D3D11_BLEND SrcBlend;
	BlendDesc.RenderTarget[ 0 ].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;    // D3D11_BLEND DestBlend;
	BlendDesc.RenderTarget[ 0 ].BlendOp               = D3D11_BLEND_OP_ADD;           // D3D11_BLEND_OP BlendOp;
	BlendDesc.RenderTarget[ 0 ].SrcBlendAlpha         = D3D11_BLEND_ONE;              // D3D11_BLEND SrcBlendAlpha;
	BlendDesc.RenderTarget[ 0 ].DestBlendAlpha        = D3D11_BLEND_ZERO;             // D3D11_BLEND DestBlendAlpha;
	BlendDesc.RenderTarget[ 0 ].BlendOpAlpha          = D3D11_BLEND_OP_ADD;           // D3D11_BLEND_OP BlendOpAlpha;
	BlendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // UINT8 RenderTargetWriteMask;

	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateBlendState( &BlendDesc, &pState );
	if( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in making the state." ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetBlendState( pState , NULL , 0xFFFFFFFF );

}
void CRenderState::EnableTextureBlending ()
{
	D3DBlendStateInfo* pInfo = NEW__( D3DBlendStateInfo , D3DBlendStateInfo() );
	pInfo->m_pSetValueFunc = &EnableTextureBlendingImple;
	m_pMember->m_SettingList.AddFront( D3DSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableTextureBlending = skyTrue;
}

void CRenderState::SetRenderer()
{
	FOREACH( D3DSetRenderStateInfoPtrList , it , m_pMember->m_SettingList )
	{
		( *it )->Save();
		( *it )->m_pSetValueFunc();
	}
}

void CRenderState::ReverseRenderer()
{
	FOREACH( D3DSetRenderStateInfoPtrList , it , m_pMember->m_SettingList )
	{
		( *it )->Reverse();
	}
}

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

CRenderState::CRenderState()
{
	Constractor();
	m_pMember = NEW__( CRenderStateMember , CRenderStateMember );
}

CRenderState::~CRenderState()
{
	DEL m_pMember;
}

void EnableCullingImple ()
{

	//�J�����O��L���ɂ���
//	glEnable( GL_CULL_FACE );
//	glCullFace( GL_FRONT );
}
void CRenderState::EnableCulling ()
{
	GLSetRenderStateInfo* pInfo = NEW__( GLSetRenderStateInfo , GLSetRenderStateInfo( GLenum( GL_CULL_FACE ) ) );
	pInfo->m_pSetValueFunc = &EnableCullingImple;
	m_pMember->m_SettingList.AddFront( GLSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableCulling = skyTrue;
}

void EnableScissorTestImple ()
{
	//�V�U�[�e�X�g�L��
	glEnable( GL_SCISSOR_TEST );
}
void CRenderState::EnableScissorTest ()
{
	GLSetRenderStateInfo* pInfo = NEW__( GLSetRenderStateInfo , GLSetRenderStateInfo( GLenum( GL_SCISSOR_TEST ) ) );
	pInfo->m_pSetValueFunc = &EnableScissorTestImple;
	m_pMember->m_SettingList.AddFront( GLSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableScissorTest = skyTrue;
}

void EnableDepthBufferImple ()
{
	// �f�v�X�o�b�t�@�̃N���A
	glClearDepth( 1.0f );

	// �f�v�X�e�X�g��L���ɂ���
	glEnable( GL_DEPTH_TEST );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	glDepthFunc( GL_LEQUAL );
}

void CRenderState::EnableDepthBuffer ()
{
	GLSetRenderStateInfo* pInfo = NEW__( GLSetRenderStateInfo , GLSetRenderStateInfo( GLenum( GL_DEPTH_TEST ) ) );
	pInfo->m_pSetValueFunc = &EnableDepthBufferImple;
	m_pMember->m_SettingList.AddFront( GLSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableDepthBuffer = skyTrue;
}

void DisableDepthBufferImple ()
{
	// �f�v�X�e�X�g�𖳌��ɂ���
	glDisable( GL_DEPTH_TEST );
}

void CRenderState::DisableDepthBuffer ()
{
	GLSetRenderStateInfo* pInfo = NEW__( GLSetRenderStateInfo , GLSetRenderStateInfo( GLenum( GL_DEPTH_TEST ) ) );
	pInfo->m_pSetValueFunc = &EnableDepthBufferImple;
	m_pMember->m_SettingList.AddFront( GLSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableDepthBuffer = skyTrue;
}

void CRenderState::EnableStencilTest ()
{
	
	m_IsEnableStencilTest = skyTrue;
}

void CRenderState::EnableStencilMask ()
{
	
	m_IsEnableStencilMask = skyTrue;
}

void EnableTextureBlendingImple ()
{
	//�����ȕ��̂������łȂ����̂��B���iZ�����ŗL���ȏꍇ�j�̂�h���B
	glAlphaFunc( GL_GREATER , 0.1f );//�������̒l�𒴂��Ă�����\���iZ�o�b�t�@��ɒu���j
	glEnable( GL_ALPHA_TEST );

	//DirectX�Ƃ͈Ⴂ�e�N�X�`���u�����f�B���O�̋@�\�͎g�p�����A���t�@�u�����f�B���O�ŏ���
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
}

void CRenderState::EnableTextureBlending ()
{
	GLenum states[] = { GL_ALPHA_TEST , GL_BLEND };
	GLSetRenderStateInfo* pInfo = NEW__( GLSetRenderStateInfo , GLSetRenderStateInfo( *states , array_sizeof( states ) ) );
	pInfo->m_pSetValueFunc = &EnableTextureBlendingImple;
	m_pMember->m_SettingList.AddFront( GLSetRenderStateInfoPtr( pInfo ) );
	m_IsEnableTextureBlending = skyTrue;
}

void CRenderState::SetRenderer()
{
	FOREACH( GLSetRenderStateInfoPtrList , it , m_pMember->m_SettingList )
	{
		( *it )->Save();
		( *it )->m_pSetValueFunc();
	}
}

void CRenderState::ReverseRenderer()
{
	FOREACH( GLSetRenderStateInfoPtrList , it , m_pMember->m_SettingList )
	{
		( *it )->Reverse();
	}
}

#endif

} } }
