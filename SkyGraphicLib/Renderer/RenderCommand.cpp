#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

void CRenderCommand::InitializeDevice( CDevicePtr pDevice , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::InitializeDevice , pDevice , skyFalse ) ) );
	}
	else
	{
		//デバイスの初期化
		pDevice->Initialize();
	}
}

void CRenderCommand::SetRadyWindow( const CWindowPtr &spWindow , const math::CBasicRect &rect , skyBool isCommand )
{
    SKY_ASSERT_MSG( spWindow.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyWindow , spWindow , rect , skyFalse ) ) );
	}
	else
	{
		//Windowの準備
		spWindow->SetUp( rect );
	}
}

void CRenderCommand::SetRadyFrameBufferRenderTarget( const CRenderTargetPtr &spRenderTarget , const CWindowPtr &spWindow  , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyFrameBufferRenderTarget , spRenderTarget , spWindow , skyFalse ) ) );
	}
	else
	{
		//フレームバッファの準備
		spRenderTarget->SetUpFrameBuffer( spWindow );
	}
}

inline void CRenderCommand::SetRadyTextureRenderTarget( const CRenderTargetPtr &spRenderTarget , const math::CBasicSize &size , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyTextureRenderTarget , spRenderTarget , size , skyFalse ) ) );
	}
	else
	{
		//テクスチャフレームバッファの準備
		spRenderTarget->SetUpTexture( size );
	}
}

void CRenderCommand::SetRadyDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , const CRenderTargetPtr &spRenderTarget , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyDepthStencilBuffer , spBuffer , spRenderTarget , skyFalse ) ) );
	}
	else
	{
		//深度ステンシルバッファの設定
		spBuffer->SetUp( spRenderTarget->GetSize() );
	}
}

void CRenderCommand::SetViewPort( const CViewPortPtr &spViewPort , skyBool isCommand )
{
    SKY_ASSERT_MSG( spViewPort.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetViewPort , spViewPort , skyFalse ) ) );
	}
	else
	{
		//ビューポートを設定
		spViewPort->SetRenderer();
	}
}

void CRenderCommand::SetScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand )
{
    SKY_ASSERT_MSG( spScissor.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetScissorRect , spScissor , skyFalse ) ) );
	}
	else
	{
		//シザリング矩形を設定
		spScissor->SetRenderer();
	}
}

void CRenderCommand::ClearScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand )
{
    SKY_ASSERT_MSG( spScissor.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearScissorRect , spScissor , skyFalse ) ) );
	}
	else
	{
		//シザリング矩形を設定
		spScissor->ClearRenderer();
	}
}

void CRenderCommand::SetRenderTarget( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderTarget , spRenderTarget , spBuffer , skyFalse ) ) );
	}
	else
	{
		//レンダリングターゲットに設定
		spRenderTarget->SetRenderer( spBuffer );
	}
}

void CRenderCommand::SetRenderState( const CRenderStatePtr &state , skyBool isCommand )
{
    SKY_ASSERT_MSG( state.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderState , state , skyFalse ) ) );
	}
	else
	{
		//レンダーステートの設定
		state->SetRenderer();
	}
}

void CRenderCommand::ClearColorBuffer( const CRenderTargetPtr &spRenderTarget , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearColorBuffer , spRenderTarget , skyFalse ) ) );
	}
	else
	{
		//カラーバッファのクリア
		spRenderTarget->ClearColorBuffer();
	}
}

void CRenderCommand::ReverseRenderState( const CRenderStatePtr &state , skyBool isCommand )
{
    SKY_ASSERT_MSG( state.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ReverseRenderState , state , skyFalse ) ) );
	}
	else
	{
		state->ReverseRenderer();
	}
}

void CRenderCommand::ClearDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearDepthStencilBuffer , spBuffer , skyFalse ) ) );
	}
	else
	{
		//レンダリングターゲットに設定
        spBuffer->ClearDepthStencilBuffer();
	}
}

void CRenderCommand::ClearDepthBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearDepthBuffer , spBuffer , skyFalse ) ) );
	}
	else
	{
		//レンダリングターゲットに設定
		spBuffer->ClearDepthBuffer();
	}
}

void CRenderCommand::ClearStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearStencilBuffer , spBuffer , skyFalse ) ) );
	}
	else
	{
		//レンダリングターゲットに設定
		spBuffer->ClearStencilBuffer();
	}
}


void CRenderCommand::SendDisplayRenderTarget( const CRenderTargetPtr &spRenderTarget , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SendDisplayRenderTarget , spRenderTarget , skyFalse ) ) );
	}
	else
	{
		//バックバッファからプライマリバッファへ転送
		spRenderTarget->Present();
	}
}

void CRenderCommand::BeginRenderingScene( skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::BeginRenderingScene , skyFalse ) ) );
	}
	else
	{
		//Beginに引数を渡すことで、ＧＰＵから値を取得したり、ＧＰＵの処理負荷を計測できる。
		CDevice_->BeginRendering();
	}
}
void CRenderCommand::EndRenderingScene( skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::EndRenderingScene , skyFalse ) ) );
	}
	else
	{
		//Beginに引数を渡すことで、ＧＰＵから値を取得したり、ＧＰＵの処理負荷を計測できる。
		CDevice_->EndRendering();
	}
}

void CRenderCommand::RenderingMesh( const RenderingMeshParam &param , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , param , skyFalse ) ) );
	}
	else
	{
        //描画領域サイズを設定
		param.m_spCamera->SetDisplaySize( param.m_spViewPort->GetRect().W() , param.m_spViewPort->GetRect().H() );

		//シェーダーにカメラを通知する（これで物体の見え方を決定できる。見え方とは描画結果そのもの）
		param.m_spShader->SetCamera( param.m_spCamera );

		//シェーダーに頂点座標変換行列を設定する（これで物体がどこに描画されるか決定できる）
		param.m_spShader->SetWorldMatrix( param.m_w );

		//シェーダーにメッシュの情報を渡す
		param.m_spShader->SetMesh( param.m_spMesh );

		//シェーダーにエキストラデータの情報を渡す
		param.m_spShader->SetExtraData( param.m_ExtraData );

		//パス番号設定
		param.m_spShader->SetRenderingPass( param.m_PassNo );

		//光を設定する
		param.m_spShader->SetLight( param.m_spLight );

		//シェーダーの描画
		param.m_spShader->vDraw();
	}
}

void CRenderCommand::RenderingPostEffectMesh( const RenderingPostEffectMeshParam &param , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingPostEffectMesh , param , skyFalse ) ) );
	}
	else
	{
		//描画
		param.m_spShader->vRendering( param.m_PassNo , param.m_spRenderTargetTexture , param.m_SamplerData , param.m_spMesh , param.m_spCamera->GetCameraMatrix() );
	}
}


void CRenderCommand::RenderingEffect( const RenderingEffectParam &param , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingEffect , param , skyFalse ) ) );
	}
	else
	{
        //描画領域サイズを設定
		param.m_spCamera->SetDisplaySize( param.m_spViewPort->GetRect().W() , param.m_spViewPort->GetRect().H() );

		//シェーダーにカメラを通知する（これで物体の見え方を決定できる。見え方とは描画結果そのもの）
		param.m_spEffect->SetCamera( param.m_spCamera );

		//シェーダーに頂点座標変換行列を設定する（これで物体がどこに描画されるか決定できる）
		param.m_spEffect->SetWorldMatrix( param.m_w );

		//シェーダーにエキストラデータの情報を渡す
		param.m_spEffect->SetExtraData( param.m_ExtraData );

		//パス番号設定
		param.m_spEffect->SetRenderingPass( param.m_PassNo );

		//光を設定する
		param.m_spEffect->SetLight( param.m_spLight );

		//シェーダーの描画
		param.m_spEffect->vDraw();
	}
}

void CRenderCommand::LoadTexture( const CTexturePtr &spTex , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::LoadTexture , spTex , skyFalse ) ) );
	}
	else
	{
		spTex->Load();
	}
}

void CRenderCommand::CreateMeshGpuMemory( const CMeshContenaPtr &spMesh , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::CreateMeshGpuMemory , spMesh , skyFalse ) ) );
	}
	else
	{
		spMesh->CreateGpuMemory();
	}
}

void CRenderCommand::WirteMeshGpuMemory( const CMeshContenaPtr &spMesh , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::WirteMeshGpuMemory , spMesh , skyFalse ) ) );
	}
	else
	{
		spMesh->WirteGpuMemory();
	}
}

void CRenderCommand::DeleteMeshCpuMemory( const CMeshContenaPtr &spMesh , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::DeleteMeshCpuMemory , spMesh , skyFalse ) ) );
	}
	else
	{
		spMesh->DeleteCpuMemory();
	}
}

/*
inline void CRenderCommand::Invoke( const CRenderingCommandPtr &command , skyBool isCommand )
{
	if ( isCommand )
	{
		//コマンドを登録
		CRenderCommandManager::InvokeCommand( command );
	}
	else
	{
		command->run();
	}
}
*/
} } }
