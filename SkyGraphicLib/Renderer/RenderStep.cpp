#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >----------------------------------------------//

//描画に関するすべての処理
void CRenderStep::Rendering( const fps::CFpsController &fps )
{
	//レンダリングパラメーター
	CRenderingParam param;
	param.Fps = fps;

	//Windowを積む
	param.Window = m_spWindow;

	//スタックに積む
	param.RenderTargetStack.Push( m_spRenderTarget );
	param.DepthStencilBufferStack.Push( m_spDepthStencilBuffer );

	//レンダリングターゲットを設定
	CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );

	//カラーバッファをクリア
	CRenderCommand::ClearColorBuffer( param.RenderTargetStack.GetTop() );

	//深度ステンシルバッファのクリア
	CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );

	//スタックに積む
	param.RenderStateStack.Push( m_spRenderState );

	//レンダーステートを設定する
	CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );

	//このステップの機能として全シーンを標準的な描画をする
	CScenePtrList &kSceneList =  m_RenderStepManager->GetSceneList();

	//シーン毎の描画スタート
	FOREACH( CScenePtrList, it , kSceneList )
	{
		//シーンが描画オフか、シーングラフがないか、シーングラフのルートがカリングオフの場合はなにもせず次へ
		if (  ( * it )->m_IsOff == skyTrue || ( * it )->GetSceneGraphRoot().IsEmpty() || ( * it )->GetSceneGraphRoot()->IsAppCulling() ) continue;

		//シーン独自のレンダーステートを積む
		if ( ( * it )->GetRenderState( skyFalse ).IsEmpty() == skyFalse ) 
		{
			//スタックに積む
			param.RenderStateStack.Push( ( * it )->GetRenderState() );

			//設定する
			CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );
		}

		//シーン独自の深度ステンシルバッファを積む
		if ( ( * it )->GetDepthStencilBuffer( skyFalse ).IsEmpty() == skyFalse )
		{
			//スタックに積む
			param.DepthStencilBufferStack.Push( ( * it )->GetDepthStencilBuffer() );

			//設定する
			CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
		}

		//深度バッファのクリア指定があればクリア
		if ( ( * it )->m_IsDepthBufferClear == skyTrue )
		{
			CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//ステンシルバッファのクリア指定があればクリアする
		if ( ( * it )->m_IsStencilBufferClear == skyTrue )
		{
			CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//シーン独自のシザー矩形を積む
		if ( ( * it )->GetScissorRect( skyFalse ).IsEmpty() == skyFalse )
		{
			( * it )->GetScissorRect()->vRendering( param );
		}

		//ビューポートを積む
		( * it )->GetViewPort()->vRendering( param );

		//カメラ管理の更新
		( *it )->GetCameraManager()->Update( param.Fps );

		//現在のカメラを取得
		CameraPtr spCurrentCamera = ( *it )->GetCameraManager()->GetCurrentCamera();

		//カメラジョイントがあれば行列スタックにPushする
		if ( spCurrentCamera->IsJointUse() == skyTrue )
		{
			param.MatrixStack.Push();
			math::CJointPtr camerajoint = spCurrentCamera;
			if ( ( *it )->GetSceneGraphRoot()->IsJointUse() == skyTrue )
			{//カメラにジョイントがあった場合、カメラのジョイントを計算に入れる。
				math::CJointPtr spJoint = ( * it )->GetSceneGraphRoot();
				param.MatrixStack.LoadAndMultLocalBeforeMatrix( spJoint->ComputeLocalMatrix() * camerajoint->ComputeLocalMatrix() );
			}
			else
			{
				param.MatrixStack.LoadAndMultLocalBeforeMatrix( camerajoint->ComputeLocalMatrix() );
			}
		}

		//カメラを積む
		spCurrentCamera->vRendering( param );

		//ライトを積む
		( * it )->GetLight()->vRendering( param );

		//シーン独自のレンダリングターゲットを積む
		if ( ( * it )->GetRenderTarget( skyFalse ).IsEmpty() == skyFalse )
		{
			//スタックに積む
			param.RenderTargetStack.Push( ( * it )->GetRenderTarget() );

			//設定する
			CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );

			//レンダリングターゲット全体をクリア
			CRenderCommand::ClearColorBuffer( param.RenderTargetStack.GetTop() );
		}

		//シーンの描画開始
		CRenderCommand::BeginRenderingScene();

//--------------▽

		//シーングラフノードツリーのトラバースレンダリング
		( * it )->GetSceneGraphRoot()->vRendering( param );
		( * it )->GetSceneGraphRoot()->RenderingChild( param ); //再帰で子もすべてレンダリングされる。
		( * it )->GetSceneGraphRoot()->vPostRendering( param );

		//半透明レンダーデータの描画
		param.AlphaDataRendering();

//--------------△

		//シーンの描画終了
		CRenderCommand::EndRenderingScene();

		//ポストエフェクトの描画（マネージャーがあればポストエフェクトをするという意思とみなす）
		const CPostEffectManagerPtr &spPostEffectManager = ( *it )->GetPostEffectManager();
		if ( spPostEffectManager.IsEmpty() == skyFalse && ( * it )->GetRenderTarget( skyFalse ).IsEmpty() == skyFalse )
		{
			spPostEffectManager->Rendering( ( * it )->GetRenderTarget() , param.DepthStencilBufferStack.GetTop() );
		}

		//ライトを取る
		( * it )->GetLight()->vPostRendering( param );

		//カメラを取る
		spCurrentCamera->vPostRendering( param );

		//カメラジョイントジョイントがあれば行列スタックからPopする
		if ( spCurrentCamera->IsJointUse() == skyTrue )
		{
			param.MatrixStack.Pop();
		}

		//ビューポートを取る
		( * it )->GetViewPort()->vPostRendering( param );

		//シーン独自のシザー矩形を取る
		if ( ( * it )->GetScissorRect( skyFalse ).IsEmpty() == skyFalse )
		{
			( * it )->GetScissorRect( skyFalse )->vPostRendering( param );
		}

		//シーン独自の深度ステンシルバッファを取る
		if ( ( * it )->m_IsDepthBufferClear == skyFalse )
		{
			//スタックから取る
			param.DepthStencilBufferStack.Pop();

			//設定する
			CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
		}

		//シーン独自のレンダーターゲットを取る
		if ( ( * it )->GetRenderTarget( skyFalse ).IsEmpty() == skyFalse )
		{
			( * it )->GetRenderTarget()->vPostRendering( param );
		}

		//シーン独自のレンダーステートを取る
		if ( ( * it )->GetRenderState( skyFalse ).IsEmpty() == skyFalse )
		{
			//スタックから取る
			param.RenderStateStack.Pop();

			//設定する
			CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );
		}
	}

	//レンダリングターゲットをディスプレイに表示する
	CRenderCommand::SendDisplayRenderTarget( m_spRenderTarget );

	//スタックから取る
	param.RenderTargetStack.Pop();
	param.DepthStencilBufferStack.Pop();
	param.RenderStateStack.Pop();
}

} } }
