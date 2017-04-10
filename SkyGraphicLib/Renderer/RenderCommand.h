#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------< インターフェース >----------------------------------------//

/// <summary>
/// レンダリングコマンド処理群
/// </summary>
class CRenderCommand : public base::SkyObject
{

public:

	/// <summary>
	/// デバイスの初期化
	/// </summary>
	static void InitializeDevice( CDevicePtr pDevice , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ウィンドウの準備
	/// </summary>
	static void SetRadyWindow( const CWindowPtr &spWindow , const math::CBasicRect &rect , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// フレームバッファレンダリングターゲットの準備
	/// </summary>
	static void SetRadyFrameBufferRenderTarget( const CRenderTargetPtr &spRenderTarget , const CWindowPtr &spWindow  , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// テクスチャレンダリングターゲットの準備
	/// </summary>
	static void SetRadyTextureRenderTarget( const CRenderTargetPtr &spRenderTarget , const math::CBasicSize &size , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// 震度ステンシルバッファの準備
	/// </summary>
	static void SetRadyDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , const CRenderTargetPtr &spRenderTarget , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// 指定のレンダリングターゲットをレンダリングターゲットに設定する
	/// </summary>
	static void SetRenderTarget( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spBuffer/* = CDepthStencilBufferPtr() */ , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// レンダリングターゲットをプライマリバッファに転送する（表に出す）
	/// </summary>
	static void SendDisplayRenderTarget( const CRenderTargetPtr &spRenderTarget , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// レンダリングターゲットのカラーバッファをクリア
	/// </summary>
	static void ClearColorBuffer( const CRenderTargetPtr &spRenderTarget , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// 深度／ステンシルバッファのクリア
	/// </summary>
    static void ClearDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
    static void ClearDepthBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ステンシルバッファのクリア
	/// </summary>
    static void ClearStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ビューポートの設定
	/// </summary>
	static void SetViewPort( const CViewPortPtr &spViewPort , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// シザリング矩形の設定
	/// </summary>
	static void SetScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// シザリング矩形のクリア
	/// </summary>
	static void ClearScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// シーンレンダリング開始
	/// </summary>
	static void BeginRenderingScene( const skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// シーンレンダリング終了
	/// </summary>
	static void EndRenderingScene( const skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// レンダーステートの設定
	/// </summary>
	static void SetRenderState( const CRenderStatePtr &state , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// レンダーステートを戻す
	/// </summary>
	static void ReverseRenderState( const CRenderStatePtr &state , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// メッシュの描画
	/// </summary>
	struct RenderingMeshParam
	{
		u32                    m_PassNo;
		CViewPortPtr           m_spViewPort;
		CameraPtr              m_spCamera;
		CShaderObjectPtr       m_spShader;
		CLightPtr              m_spLight;
		math::CMatrix4x3       m_w;
		CExtraDataPtr          m_ExtraData;
		CMeshPtr               m_spMesh;
		math::CMatrix4x3Array  m_SkinMatrix;

		//コンストラクタ
		RenderingMeshParam( u32 passNo , const CViewPortPtr &spViewPort , const CameraPtr &spCamera , const CShaderObjectPtr &spShader , const CMeshPtr &spMesh , const math::CMatrix4x3 &w , const CExtraDataPtr &spExtra , const CLightPtr &spLight ) :
		m_PassNo( passNo ) ,
		m_spViewPort( spViewPort ) ,
		m_spCamera( spCamera ) ,
		m_spShader( spShader ) ,
		m_spMesh( spMesh ) ,
		m_w( w ) ,
		m_ExtraData( spExtra ) ,
		m_spLight( spLight )
		{}
	};
	static void RenderingMesh( const RenderingMeshParam &param , skyBool isCommand = IS_COMMAND );

	struct RenderingPostEffectMeshParam
	{
		u32                  m_PassNo;
		CTexturePtr			 m_spRenderTargetTexture;
		CSamplerData         m_SamplerData;
		CameraPtr            m_spCamera;
		CPostEffectObjectPtr m_spShader;
		CMeshContenaPtr      m_spMesh;
		CExtraDataPtr        m_ExtraData;

		//コンストラクタ
		RenderingPostEffectMeshParam( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData& sampler , const CameraPtr &spCamera , const CPostEffectObjectPtr &spShader , const CMeshContenaPtr &spMesh ) :
		m_PassNo( passNo ) ,
		m_spRenderTargetTexture( spRenderTargetTexture ) ,
		m_SamplerData( sampler ) ,
		m_spCamera( spCamera ) ,
		m_spShader( spShader ) ,
		m_spMesh( spMesh )
		{}
	};
	static void RenderingPostEffectMesh( const RenderingPostEffectMeshParam &param , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// エフェクトの描画
	/// </summary>
	struct RenderingEffectParam
	{
		u32                    m_PassNo;
		CViewPortPtr           m_spViewPort;
		CameraPtr              m_spCamera;
		CEffectObjectPtr       m_spEffect;
		CLightPtr              m_spLight;
		math::CMatrix4x3       m_w;
		CExtraDataPtr          m_ExtraData;
		CMeshPtr               m_spMesh;
		math::CMatrix4x3Array  m_SkinMatrix;

		//コンストラクタ
		RenderingEffectParam( u32 passNo , const CViewPortPtr &spViewPort , const CameraPtr &spCamera , const CEffectObjectPtr &spEffect , const math::CMatrix4x3 &w , const CExtraDataPtr &spExtra , const CLightPtr &spLight ) :
		m_PassNo( passNo ) ,
		m_spViewPort( spViewPort ) ,
		m_spCamera( spCamera ) ,
		m_spEffect( spEffect ) ,
		m_w( w ) ,
		m_ExtraData( spExtra ) ,
		m_spLight( spLight )
		{}
	};
	static void RenderingEffect( const RenderingEffectParam &param , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// テクスチャのロード
	/// </summary>
	static void LoadTexture( const CTexturePtr &spTex , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// メッシュの使用GPUメモリ作成
	/// </summary>
	static void CreateMeshGpuMemory( const CMeshContenaPtr &spVertex , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// メッシュの使用GPUメモリへの削除
	/// </summary>
	static void DeleteMeshCpuMemory( const CMeshContenaPtr &spVertex , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// メッシュの使用GPUメモリへの書き込み
	/// </summary>
	static void WirteMeshGpuMemory( const CMeshContenaPtr &spVertex , skyBool isCommand = IS_COMMAND );

/*
	/// <summary>
	/// 汎用コマンド発行（コマンド化したい処理を関数オブジェクトにして渡してください。）
	/// </summary>
	static void Invoke( const CRenderingCommandPtr &command , skyBool isCommand = IS_COMMAND );
*/
};

} } }

#define CRenderCommand_	sky::lib::graphic::CRenderCommand



