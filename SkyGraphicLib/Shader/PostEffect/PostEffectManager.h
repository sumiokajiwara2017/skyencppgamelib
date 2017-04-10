#pragma once

namespace sky { namespace lib { namespace graphic {

class   CPostEffectManager;
typedef SmartPointer< CPostEffectManager > CPostEffectManagerPtr;			//スマートポインタ型定義

/// <summary>
/// ポストエフェクト管理
/// ポストエフェクトはスタック管理で、ポストエフェクト用のシェーダーをスタックの下から描画する
/// 【注意】ポストエフェクトを有効にするシーンは、必ずテクスチャレンダーターゲットを使用してください。
///         このクラスはCSceneクラスを通してのみ生成できます。CScene機能の一部として動作します。
/// </summary>
class CPostEffectManager : public base::SkyRefObject
{
	friend class CScene;

    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPostEffectManager(){};

	/// <summary>
	/// バッファ用レンダリングターゲット
	/// </summary>
	void SetBufferRenderTarget( const CRenderTargetPtr &spBufferRenderTarget );

	/// <summary>
	/// サンプラーの設定（無くてもいい）
	/// </summary>
	void SetSampler( const CSamplerData &sampler );

	/// <summary>
	/// ポストエフェクトを積む
	/// </summary>
	void PushPostEffect( const CPostEffectObjectPtr &spPostEffect );

	/// <summary>
	/// ポストエフェクトを取る
	/// </summary>
	void PopPostEffect();

	/// <summary>
	/// ポストエフェクトを有効／無効
	/// </summary>
	void SetEnablePostEffect( u32 stackNo , skyBool isEnable );

	/// <summary>
	/// ポストエフェクトを実行
	/// 引数は出力レンダリングターゲット。
	/// </summary>
	void Rendering( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spDepthStencilBuffer );

//---Serialize

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	skyBool LoadBinary( const stream::IStreamPtr& stream );

private:

	/// <summary>
	/// ポストエフェクト用スタック（型はリストだがスタックのように使用する）
	/// </summary>
	CPostEffectObjectPtrList m_PostEffectStack;

	/// <summary>
	/// 表示用スプライト
	/// </summary>
	CSceneGraphNodePtr m_BufferSizeSprite; //逆参照をするため型はあえてCSpritePtrではない

	/// <summary>
	/// 表示用カメラ
	/// </summary>
	CameraPtr m_spCamera;

	/// <summary>
	/// サンプラーデータ
	/// サンプラはポストエフェクトを実現させるための入力テクスチャデータ。
	/// 入力レンダリングターゲットだけでいい場合は使用しない。
	/// </summary>
	CSamplerData       m_SamplerData;

	/// <summary>
	/// バッファ用レンダリングターゲット
	/// </summary>
	CRenderTargetPtr   m_spBufferRenderTarget;

	/// <summary>
	/// バッファコピー用ポストエフェクト
	/// </summary>
	CPostEffectTextureMappingPtr m_spBufferCopyPostEffect;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPostEffectManager();

	/// <summary>
	/// 描画用スプライトの作成
	/// </summary>
	void CreateSprite(  dectype width , dectype height );

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CPostEffectManagerPtr Create()
	{
		CPostEffectManagerPtr spPostEffectManager( NEW__( CPostEffectManager , CPostEffectManager() ) );
		return spPostEffectManager;
	}
};

} } }

#define CPostEffectManagerCreate_()	 sky::lib::graphic::CPostEffectManager::Create()
