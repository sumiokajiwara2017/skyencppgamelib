#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// レンダリングステップクラス
/// このクラスを継承してvRenderingを実装すれば描画をカスタマイズできる。
/// </summary>
class CRenderStep : public base::SkyRefObject
{
	friend class CRenderStepManager;
public:

	/// <summary>
	/// 名前
	/// </summary>
    CName   Name;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRenderStep(){}

    /// <summary>
	///  レンダリング実行
    /// </summary>
	virtual void Rendering( const fps::CFpsController &fps );

	/// <summary>
	/// ウィンドウの参照
	/// </summary>
	CWindowPtr             m_spWindow;

	/// <summary>
	/// レンダリングターゲット
	/// </summary>
	CRenderTargetPtr       m_spRenderTarget;

	/// <summary>
	/// 深度ステンシルバッファ
	/// </summary>
	CDepthStencilBufferPtr m_spDepthStencilBuffer;

	/// <summary>
	/// レンダーステート
	/// </summary>
	CRenderStatePtr        m_spRenderState;

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CRenderStepPtr Create( const hash::CHashStringPtr &name )
	{
		
		CRenderStepPtr spStep( NEW__( CRenderStep , CRenderStep() ) );
		if ( name.IsEmpty() == skyFalse ) spStep->Name.SetName( name );
		return spStep;
	}

private:

	/// <summary>
	/// 管理
	/// </summary>
	CRenderStepManagerPtr m_RenderStepManager;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderStep(){}
};

#define CRenderStepCreate_( name )	sky::lib::graphic::CRenderStep::Create( name );

} } }