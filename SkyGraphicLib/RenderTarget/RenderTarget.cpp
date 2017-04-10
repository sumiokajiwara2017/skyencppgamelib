#include "StdAfx.h"
#include "SkyGraphicLib/RenderTarget.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Controller.h"

SkyImplementRTTI( sky::lib::graphic::CRenderTarget , sky::lib::graphic::CSceneGraphNode );

//===================================【Common】===================================================>

namespace sky { namespace lib { namespace graphic {

const serialize::CSerializeInterfacePtr& CRenderTarget::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CRenderTargetPtr )spContainer )->m_Size = m_Size;
	( ( CRenderTargetPtr )spContainer )->m_ClearColor = m_ClearColor;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CRenderTarget::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CRenderTargetPtr )spContainer )->m_pMember = m_pMember;
	( ( CRenderTargetPtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CRenderTarget::Clone()
{
	return Copy( CRenderTargetPtr( NEW__( CRenderTarget , CRenderTarget() ) ) );
}

serialize::CSerializeInterfacePtr CRenderTarget::CloneSharing()
{
	return CopySharing( Clone() );
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_Size.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Size.H , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.a , sizeof( f32 ) , isWrite ); \

skyBool CRenderTarget::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "rendertarget bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CRenderTarget::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "rendertarget bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

CRenderTarget::CRenderTarget() : 
m_Size( 0 , 0 ) , 
m_ClearColor( graphic::CColor_DeepBlue ) ,
m_pMember( NEW__( CRenderTargetMember , CRenderTargetMember ) ) ,
m_IsFrameBuffer( skyFalse )
{ 
}
 
CRenderTarget::~CRenderTarget()
{ 
	Release();
}

CTexturePtr CRenderTarget::GetColorBufferTexture()
{
    return m_spColorBufferTexture;
}

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

void CRenderTargetMember::CreateColorBufferFromSwapChain( CTexturePtr &spColorBufferTexture )
{
	ID3D11RenderTargetView  *pRenderTaget       = skyNull;

	//テクスチャがあったら開放する
	if ( spColorBufferTexture.IsEmpty() )
	{
		//テクスチャクラスのインスタンスを生成
		spColorBufferTexture = CTextureCreate_();
	}

//-------------バックバッファ関連（Windowに描画するためのもの）-------------------▽

	//スワップチェインのバックバッファを取得
	ID3D11Texture2D *pBackBuffer;
	HRESULT hr = m_spSwapChain->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( LPVOID * )&pBackBuffer );    
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "レンダリングターゲットの作成に失敗しました。\n" ) );
	}

	D3D11_TEXTURE2D_DESC backbufferDesc;
	pBackBuffer->GetDesc( &backbufferDesc );
	spColorBufferTexture->GetMember()->m_spTexture.SetPtr( pBackBuffer );
	spColorBufferTexture->GetInfo().Width = ( texture::eTexture_with )backbufferDesc.Width;
	spColorBufferTexture->GetInfo().Height = ( texture::eTexture_height )backbufferDesc.Height;

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;

	//レンダーターゲットの生成
	hr = CDevice_->GetMember()->m_spD3DDevice->CreateRenderTargetView( spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &rtvDesc, &pRenderTaget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "レンダリングターゲットの作成に失敗しました。\n" ) );
	}

	//スワップチェインのディスクリプタ取得
	DXGI_SWAP_CHAIN_DESC desc;
	m_spSwapChain->GetDesc( &desc );

	//レンダーターゲットをシェーダーでテクスチャとして使用するためのシェーダーリソースビューを生成
	ID3D11ShaderResourceView *pTextureResView_RenderTarget;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= desc.BufferDesc.Format;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &ShaderResourceViewDesc, &pTextureResView_RenderTarget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTargetMember::CreateColorBufferが失敗しました。\n" ) );
	}
	spColorBufferTexture->GetMember()->m_spShaderResourceView.SetPtr( pTextureResView_RenderTarget );

	//スマートポインタ管理にする
	m_spRenderTargetView.SetPtr( pRenderTaget );
}

 
void CRenderTarget::SetUpFrameBuffer( const CWindowPtr &spWindow )
{
	m_IsFrameBuffer = skyTrue;

	HRESULT hr = S_OK;

	//サイズの設定
	RECT rect;
	GetClientRect( spWindow->GetMember()->GetWindowHndl() , &rect );
	m_Size.W = ( dectype )( rect.right  - rect.left );
	m_Size.H = ( dectype )( rect.bottom - rect.top );

	IDXGISwapChain          *pSwapChain         = skyNull;

	//プレゼントパラメーターの初期化
	DXGI_SWAP_CHAIN_DESC d3DSwapChainDesc;
	CMemoryUtil_::ClearZero( &d3DSwapChainDesc , sizeof( d3DSwapChainDesc ) );

	//スワップチェーン用パラメーター生成
	d3DSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// バック バッファのフォーマット
	d3DSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			    // リフレッシュレートの分子
	d3DSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;				// リフレッシュレートの分母

	if ( SkyGraphicLibController_::IsFullScreen() )
	{//フルスクリーンモード

		d3DSwapChainDesc.Windowed = 0;											// ＷＩＮＤＯＷモードＯＦＦ
	}
	else
	{//Windowモード
		d3DSwapChainDesc.Windowed = 1;											// ＷＩＮＤＯＷモードＯＮ
	}

	d3DSwapChainDesc.BufferDesc.Width                  = ( UINT )m_Size.W;							        // スワップ チェーンのバック バッファの幅と高さ (ピクセル単位)
	d3DSwapChainDesc.BufferDesc.Height                 = ( UINT )m_Size.H;							        // スワップ チェーンのバック バッファの幅と高さ (ピクセル単位)
	d3DSwapChainDesc.BufferDesc.Format                 = DXGI_FORMAT_R8G8B8A8_UNORM;			// バックバッファのフォーマット
	d3DSwapChainDesc.BufferDesc.RefreshRate.Denominator= 1;										// リフレッシュレートの分子
	d3DSwapChainDesc.BufferDesc.RefreshRate.Numerator  = 60;										// リフレッシュレートの分母
	d3DSwapChainDesc.BufferDesc.Scaling                = DXGI_MODE_SCALING_CENTERED;			// スキャンラインのパラメーター
	d3DSwapChainDesc.BufferDesc.ScanlineOrdering       = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;		// スキャンラインのパラメーター
	d3DSwapChainDesc.SampleDesc.Count                  = 1;										// マルチサンプルのカウント
	d3DSwapChainDesc.SampleDesc.Quality                = 0;										// マルチサンプルの品質
	d3DSwapChainDesc.BufferUsage                       = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;			// バックバッファのフラグ
	d3DSwapChainDesc.BufferCount                       = 3;										// バックバッファ数
	d3DSwapChainDesc.OutputWindow						= spWindow->GetMember()->GetWindowHndl();     // 関連付けるウィンドウのハンドル
	d3DSwapChainDesc.Windowed							= 1;										// ウィンドウモードで起動するか否か
	d3DSwapChainDesc.SwapEffect                        = DXGI_SWAP_EFFECT_DISCARD;					// 画面更新の方法
	d3DSwapChainDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// スワップチェインのフラグ

	//スワップチェインの作成
	hr = CDevice_->GetMember()->m_spDXGIFactory->CreateSwapChain( CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst() , &d3DSwapChainDesc , &pSwapChain );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "レンダリングターゲットの作成に失敗しました。\n" ) );
	}
	m_pMember->m_spSwapChain.SetPtr( pSwapChain );

	//バッファ作成
	m_pMember->CreateColorBufferFromSwapChain( m_spColorBufferTexture );

	//フルスクリーン設定をする（一応）
	if ( SkyGraphicLibController_::IsFullScreen() )
	{
		m_pMember->m_spSwapChain->SetFullscreenState( TRUE , NULL );
	}
	else
	{
		m_pMember->m_spSwapChain->SetFullscreenState( FALSE , NULL );
	}
}

 
void CRenderTarget::SetUpTexture( const math::CBasicSize &size )
{
	m_IsFrameBuffer = skyFalse;

	ID3D11RenderTargetView  *pRenderTaget       = skyNull;

	//テクスチャクラスのインスタンスを生成
	m_spColorBufferTexture = CTextureCreate_();

	//サイズの設定
	m_spColorBufferTexture->m_Info.Width  = ( texture::eTexture_with )( u32 )size.W;
	m_spColorBufferTexture->m_Info.Height = ( texture::eTexture_height )( u32 )size.H;

	ID3D11Texture2D *pTexture = skyNull;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width              = ( UINT )size.W;
	desc.Height             = ( UINT )size.H;
	desc.MipLevels          = 1;
	desc.ArraySize          = 1;
	desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage              = D3D11_USAGE_DEFAULT;
	desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags     = 0; //ここが１だとエラー
	desc.MiscFlags          = 0;
	HRESULT	hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &desc, 0, &pTexture );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "CTexture::Create( const texture::CTextureInfo &texInfo )が失敗しました。\n" ) );
	}
	m_spColorBufferTexture->GetMember()->m_spTexture.OverWrite( pTexture ); //スマートポインタの中身のアドレスを上書きする

	//サイズはテクスチャのサイズを設定
	m_Size = size;

	//レンダーターゲットをシェーダーでテクスチャとして使用するためのシェーダーリソースビューを生成
	ID3D11ShaderResourceView *pTextureResView_RenderTarget;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= desc.Format;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= desc.MipLevels;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( m_spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &ShaderResourceViewDesc, &pTextureResView_RenderTarget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTargetMember::CreateColorBufferが失敗しました。\n" ) );
	}
	m_spColorBufferTexture->GetMember()->m_spShaderResourceView.OverWrite( pTextureResView_RenderTarget );

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;

	//レンダーターゲットビューの設定
	hr = CDevice_->GetMember()->m_spD3DDevice->CreateRenderTargetView( m_spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &rtvDesc, &pRenderTaget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "レンダリングターゲットの作成に失敗しました。\n" ) );
	}
	m_pMember->m_spRenderTargetView.SetPtr( pRenderTaget );
}
 
void CRenderTarget::ClearColorBuffer()
{
	//カラーバッファのクリア
	FLOAT clearColor[ 4 ] = { m_ClearColor.r , m_ClearColor.g , m_ClearColor.b , m_ClearColor.a };//RGBA
	CDevice_->GetMember()->m_spD3DDeviceContext->ClearRenderTargetView( m_pMember->m_spRenderTargetView.GetPtrNoConst() , clearColor );
}

 
void CRenderTarget::SetRenderer( const CDepthStencilBufferPtr &spBuffer )
{
	//初期化
	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 0 , NULL , NULL );

	//渡された値は内部で保持される。マイフレームやる必要はない。
	ID3D11RenderTargetView* pTbl[] = { m_pMember->m_spRenderTargetView.GetPtrNoConst() };

	//深度ステンシルバッファがあれば設定する。ない場合は前後関係がおかしくなる。
	ID3D11DepthStencilView *pDepthStencil = NULL;
	if ( spBuffer.IsEmpty() == skyFalse )
	{
		pDepthStencil = spBuffer->GetMember()->m_spDepthStencilView.GetPtrNoConst();
	}

	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 1, pTbl, pDepthStencil );
}

 
void CRenderTarget::Release()
{
	//消す
	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 0 , NULL , NULL );

	//リリース
	m_pMember->m_spSwapChain.Release();
	m_pMember->m_spRenderTargetView.Release();
	m_spColorBufferTexture.Delete();
}

 
void	CRenderTarget::Present()
{
	if ( m_pMember->m_spSwapChain.IsEmpty() )
	{
//		SKY_PANIC_MSG( _T( "テクスチャレンダリングターゲットは画面に直接転送することができません。シェーダーのパラメーターとして使用してください。\n" ) );
		return; //ただスキップする
	}

	HRESULT hr = m_pMember->m_spSwapChain->Present( skyNull , skyNull );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTarget::Presentに失敗しました。\n" ) );
	}
}

 
void CRenderTarget::ReSize( u32 newWidth, u32 newHeight )
{
	if ( m_pMember->m_spSwapChain.IsEmpty() )
	{
		SKY_PANIC_MSG( _T( "この処理はテクスチャレンダリングターゲットには対応していません。必要な場合実装してください。\n" ) );
		return;
	}

	//サイズの設定
	m_Size.W = ( dectype )newWidth;
	m_Size.H = ( dectype )newHeight;

	//レンダリングターゲットの破棄
	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 0 , NULL , NULL );
	m_pMember->m_spRenderTargetView.Release();
	m_spColorBufferTexture->GetMember()->m_spShaderResourceView.Delete();
	m_spColorBufferTexture->GetMember()->m_spTexture.Delete();

	//スワップチェインのリサイズ変更
	DXGI_SWAP_CHAIN_DESC desc;
	m_pMember->m_spSwapChain->GetDesc( &desc );
	m_pMember->m_spSwapChain->ResizeBuffers( desc.BufferCount , ( UINT )m_Size.W , ( UINT )m_Size.H , desc.BufferDesc.Format , 0 );

	//バッファ作成
	m_pMember->CreateColorBufferFromSwapChain( m_spColorBufferTexture );
}

#endif
//===================================【OpenGL】====================================================>

/*
（ちょこっとメモ

【バックバッファの値をテクスチャに書きこむ処理】
void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
描画対象になっているフレームバッファの内容の一部をテクスチャメモリにコピーします．target には GL_TEXTURE_2D を指定します．
level はミップマップの解像度レベルで，0 がベースの（最も解像度の高い）レベルで，ミップマップを使用しない時は 0 を指定しておきます．
xoffset と yoffset はコピー先のテクスチャ上の位置を指定します．x と y はコピー元のフレームバッファ上の位置です．width と height はコピーする領域の幅と高さです．

（使用例１
glReadBuffer( GL_BACK ); //入力元指定。GL_BACKはバックバッファ。glReadBuffer(GL_FRONT)ってやるとおそらくフロントバッファ
glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 10, 10, 0, 0, 310, 230 );//こうすることでテクスチャが設定されていることにもなる（おそらく）GL_TEXTURE_2Dは書き込み先を表すGL_TEXTURE_CUBE_MAP_POSITIVE_Xなどとするとキューブマップに使えたりする
glBegin( GL_QUADS );{
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -2.0f, -1.0f,  0.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -2.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  0.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  0.0f, -1.0f,  0.0f );
}glEnd();

（使用例２ 
glReadBuffer( GL_BACK ); //入力元指定。GL_BACKはバックバッファ。glReadBuffer(GL_FRONT)ってやるとおそらくフロントバッファ
glBindTexture( GL_TEXTURE_2D, *テクスチャID* );　//書き込み先にテクスチャを指定している
glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 10, 10, 0, 0, 310, 230 );//こうすることでテクスチャが設定されていることにもなる（おそらく）
glBegin( GL_QUADS );{
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -2.0f, -1.0f,  0.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -2.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  0.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  0.0f, -1.0f,  0.0f );
}glEnd();

*テクスチャID個*のテクスチャにバックバッファの値を書き込む

*/

#ifdef SKYLIB_RENDER_OPENGL4


void CRenderTarget::Release()
{
    if ( m_pMember->m_FOB.IsEmpty() == skyFalse )
    {
        m_pMember->m_FOB->Dispose();
		m_pMember->m_FOB.Delete();
    }

	if ( m_pMember->m_hDC != skyNull )
	{
		wglMakeCurrent( m_pMember->m_hDC , 0 );
		wglDeleteContext( m_pMember->m_hRC );
		ReleaseDC( m_pMember->m_nWindowHndl, m_pMember->m_hDC );

		if ( m_pMember->m_GlewC != skyNull )
		{
			CGlew::DeleteContext( m_pMember->m_GlewC );
		}
	}
}
 
void CRenderTarget::SetUpFrameBuffer( const CWindowPtr &spWindow )
{
	m_IsFrameBuffer = skyTrue;

	m_pMember->m_nWindowHndl = spWindow->GetMember()->GetWindowHndl(); 

	// デバイスコンテキストの取得、設定
	m_pMember->m_hDC = GetDC( m_pMember->m_nWindowHndl );

	if( NULL == m_pMember->m_hDC )
	{
        SKY_PANIC_MSG( _T( "It failed in making the device context. \n" ) );
	}

	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		24,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer //アキュムレーションバッファを使用するときはここを使用しよう！( 24 )?
		0, 0, 0, 0,            // accum bits ignored     //アキュムレーションバッファを使用するときはここを使用しよう！( 8 , 8 , 8 , 0 )?
		32,                    // 32-bit z-buffer 
		0,                     // no stencil buffer      //ステンシルバッファを使用するときはここを指定しよう！
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	}; 

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( m_pMember->m_hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( m_pMember->m_hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// レンダリングコンテキストの作成
	if( 0 == ( m_pMember->m_hRC = ::wglCreateContext( m_pMember->m_hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// レンダリングコンテキストをカレントのデバイスコンテキストに設定
	if( TRUE != ::wglMakeCurrent( m_pMember->m_hDC, m_pMember->m_hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

#ifndef GLEW_MX

   // GLEW初期化
   CGlew::Initialize();

#else

	m_pMember->m_GlewC = CGlew::CreateContext();
	CGlew::Initialize( m_pMember->m_GlewC );

#endif

	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *vender = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);

	SKY_PRINTF( "OpenGL   %s \n" , version );
	SKY_PRINTF( "Vender   %s \n" , vender );
	SKY_PRINTF( "Renderer %s \n" , renderer );

	m_pMember->m_IsFrameBuffer = skyTrue;
}

void CRenderTarget::SetUpTexture( const math::CBasicSize &size )
{
	m_IsFrameBuffer = skyFalse;

	m_pMember->m_IsFrameBuffer = skyFalse;

	m_pMember->m_FOB = CFramebufferObject::Create();
	m_pMember->m_FOB->Initialize( size );

	//テクスチャクラスのインスタンスを生成
	m_spColorBufferTexture = CTextureCreate_();
	m_spColorBufferTexture->GetMember()->texId[ 0 ] = m_pMember->m_FOB->GetTextureId();
	m_spColorBufferTexture->GetMember()->m_IsLoad = skyTrue;
	m_spColorBufferTexture->GetInfo().Width = ( texture::eTexture_with )( u32 )size.W;
	m_spColorBufferTexture->GetInfo().Height = ( texture::eTexture_height )( u32 )size.H;
}
 
void CRenderTarget::ClearColorBuffer()
{
	// クリアカラーの設定
	glClearColor( m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a );

	//カラーバッファとのクリア
	glClear( GL_COLOR_BUFFER_BIT );
}

 
void CRenderTarget::SetRenderer( const CDepthStencilBufferPtr &spBuffer )
{
	SKY_UNUSED_ARG( spBuffer );

	if ( m_pMember->m_IsFrameBuffer == skyTrue )
	{
		// ドライバにバグのあるグラフィックカードにおいてのSDIの最初の描画と、
		// すべてのグラフィックカードにおいてのMDI描画において
		// 以下の命令は、効果を発揮する。
		wglMakeCurrent( m_pMember->m_hDC, m_pMember->m_hRC );
		CGlew::SetCurrentContext( m_pMember->m_GlewC );
	}
	else
	{
		m_pMember->m_FOB->PreRender();

		//深度バッファのクリア
		glClear( GL_DEPTH_BUFFER_BIT );
	}
}

 
void CRenderTarget::Present()
{
	if ( m_pMember->m_IsFrameBuffer == skyTrue )
	{
		//バックバッファをフロントに
		SwapBuffers( m_pMember->m_hDC );
	}
	else
	{
		m_pMember->m_FOB->PostRender();
	}
}

void CRenderTarget::ReSize( u32 newWidth, u32 newHeight )
{
	SKY_UNUSED_ARG( newWidth );
	SKY_UNUSED_ARG( newHeight );
}

#endif

//===================================【Common】====================================================>

skyBool CRenderTarget::IsFrameBufferRenderTarget()
{
	return m_IsFrameBuffer;
}

void CRenderTarget::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This instance must use smartpointer. \n" ) );

    //半透明レンダーデータの描画（描画空間を分けるシーングラフノード要素が終わるときに半透明も出力する）
    param.AlphaDataRendering();

    //自分をスタックに積む
    param.RenderTargetStack.Push( thisPtr( CRenderTargetPtr ) ); //もともとこのthisが他のスマートポインタに管理されていた場合（RefCountが0以上の場合)この受け渡しをしても問題が発生しない。

    //自分のカラーバッファをクリア
    CRenderCommand::ClearColorBuffer( param.RenderTargetStack.GetTop() );

    //自分をレンダリングターゲットに適用する（ターゲットを一時置き換える）
    CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
}


void CRenderTarget::vPostRendering( CRenderingParam &param )
{
    //半透明レンダーデータの描画（描画空間を分けるシーングラフノード要素が終わるときに半透明も出力する）
    param.AlphaDataRendering();

    //レンダリングターゲットをディスプレイに表示する
    CRenderCommand::SendDisplayRenderTarget( thisPtr( CRenderTargetPtr ) );

    //自分をスタックから消す
    param.RenderTargetStack.Pop();

	if ( param.RenderTargetStack.GetNum() > 0 )
	{
		//一時置き換えたターゲットを元に戻す
		CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
	}
}

} } }
