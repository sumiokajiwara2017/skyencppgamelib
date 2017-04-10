#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CRenderState , sky::lib::graphic::CSceneGraphNode );

//===================================【DirectX】===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< ID3D11RasterizerState >   D3D11RasterizerStatePtr;
typedef SmartPointerCom< ID3D11DepthStencilState > D3D11DepthStencilStatePtr;
typedef SmartPointerCom< ID3D11BlendState >        D3D11BlendStatePtr;

//-------------------------------------< 実装 >---------------------------------------------//

//ステート情報ベース構造体
struct D3DSetRenderStateInfo;
typedef SmartPointer< D3DSetRenderStateInfo >   D3DSetRenderStateInfoPtr;			//スマートポインタ型定義
typedef CList< u32 , D3DSetRenderStateInfoPtr > D3DSetRenderStateInfoPtrList;		//スマートポインタリスト型
typedef void ( *SetterFunc )();
struct D3DSetRenderStateInfo : public base::SkyRefObject
{
    virtual void                   Save    () = 0;
    virtual void                   Reverse () = 0;
	SetterFunc	                   m_pSetValueFunc;
};

//ラスタライザーステート情報構造体
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

//震度ステンシルステート情報構造体
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

//ブレンドステート情報構造体
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
			CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->OMSetBlendState( NULL , blendFactor , 0xffffffff ); //完全に消しちゃう
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
	/// レンダーステート設定リスト
	/// </summary>
	D3DSetRenderStateInfoPtrList  m_SettingList;
};

} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

#define RENDER_STATE_SET_MAX_NUM ( 8 )

//ステート情報ベース構造体
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
typedef SmartPointer< GLSetRenderStateInfo >   GLSetRenderStateInfoPtr;			//スマートポインタ型定義
typedef CList< u32 , GLSetRenderStateInfoPtr > GLSetRenderStateInfoPtrList;		//スマートポインタリスト型

struct CRenderStateMember
{
	/// <summary>
	/// レンダーステート設定リスト
	/// </summary>
	GLSetRenderStateInfoPtrList  m_SettingList;
};

} } }

#endif

namespace sky { namespace lib { namespace graphic {

//===================================【Common】====================================================>

void CRenderState::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This instance must use smartpointer.\n" ) );

	//半透明レンダーデータの描画（描画空間を分けるシーングラフノード要素が終わるときに半透明も出力する）
	param.AlphaDataRendering();

	//自分をスタックに積む
	param.RenderStateStack.Push( thisPtr( CRenderStatePtr ) ); //もともとこのthisが他のスマートポインタに管理されていた場合（RefCountが0以上の場合)この受け渡しをしても問題が発生しない。

	//レンダーステートを有効にする
	CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );
}

void CRenderState::vPostRendering( CRenderingParam &param )
{
	//半透明レンダーデータの描画（描画空間を分けるシーングラフノード要素が終わるときに半透明も出力する）
	param.AlphaDataRendering();

	//スタックから取る
	param.RenderStateStack.Pop();

	if ( param.RenderStateStack.GetNum() > 0 )
	{
		//一つ前のレンダーステートを有効にする
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

//===================================【DirectX】===================================================>

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
	//ラスタライザステートの作成
	ID3D11RasterizerState *pState;
	D3D11_RASTERIZER_DESC RasterizerDesc =
	{
		D3D11_FILL_SOLID,	// D3D11_FILL_MODE FillMode;   //ここを切り替えればワイヤーフレーム表示( D3D11_FILL_WIREFRAME )にできる
		D3D11_CULL_NONE,	// D3D11_CULL_MODE CullMode;   //ここを切り替えればカリングできる
		TRUE,				// BOOL FrontCounterClockwise; //ここを変えると三角形が左回りか右回りかを決定できる。
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

	//深度／ステンシルのステートを設定
    D3D11_DEPTH_STENCIL_DESC DepthDesc =
	{
        TRUE,                            // 深度バッファを有効にする;
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

	//深度／ステンシルのステートを設定
    D3D11_DEPTH_STENCIL_DESC DepthDesc =
	{
        FALSE,                            // 深度バッファを無効にする;
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

	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; // 深度テストあり
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO; // 書き込まない
	DepthStencil.DepthFunc        = D3D11_COMPARISON_LESS; // 手前の物体を描画
	DepthStencil.StencilEnable    = TRUE; // ステンシル・テストあり
	DepthStencil.StencilReadMask  = 0xff; // ステンシル読み込みマスク。
	DepthStencil.StencilWriteMask = 0xff; // ステンシル書き込みマスク。

	// 面が表を向いている場合のステンシル・テストの設定
	DepthStencil.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR;  // ＋１
	DepthStencil.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // 常に成功

	// 面が裏を向いている場合のステンシル・テストの設定
	DepthStencil.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilPassOp      = D3D11_STENCIL_OP_INCR;   // ＋１ //ここをD3D11_STENCIL_OP_DECRにしてシャドウボリュームをレンダリングすると影ができる。
	DepthStencil.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // 常に成功
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

	// 深度/ステンシル・ステート・オブジェクトの作成
	// 深度/ステンシル・ステート・オブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable      = TRUE; // 深度テストあり
	DepthStencil.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO; // 書き込まない
	DepthStencil.DepthFunc        = D3D11_COMPARISON_GREATER; // 奥の物体を描画
	DepthStencil.StencilEnable    = TRUE;     // ステンシル・テストあり
	DepthStencil.StencilReadMask  = 0xff;     // ステンシル読み込みマスク。
	DepthStencil.StencilWriteMask = 0;        // ステンシル書き込みマスク。

	// 面が表を向いている場合のステンシル・テストの設定
	DepthStencil.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // 維持
	DepthStencil.FrontFace.StencilFunc        = D3D11_COMPARISON_NOT_EQUAL; // 同じでない場合、成功

	// 面が裏を向いている場合のステンシル・テストの設定
	DepthStencil.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;   // 維持
	DepthStencil.BackFace.StencilFunc        = D3D11_COMPARISON_NEVER;  // 常に失敗
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
	//ブレンドステートの設定
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

//===================================【OpenGL】====================================================>

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

	//カリングを有効にする
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
	//シザーテスト有効
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
	// デプスバッファのクリア
	glClearDepth( 1.0f );

	// デプステストを有効にする
	glEnable( GL_DEPTH_TEST );

	//デプスファンク（同じか、手前にあるもので上描いていく）
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
	// デプステストを無効にする
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
	//透明な物体が透明でない物体を隠す（Z方向で有利な場合）のを防ぐ。
	glAlphaFunc( GL_GREATER , 0.1f );//第二引数の値を超えていたら表示（Zバッファ上に置く）
	glEnable( GL_ALPHA_TEST );

	//DirectXとは違いテクスチャブレンディングの機能は使用せずアルファブレンディングで処理
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
