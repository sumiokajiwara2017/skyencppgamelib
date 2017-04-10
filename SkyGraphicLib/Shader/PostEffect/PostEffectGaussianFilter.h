#pragma once
/*

【 概要 】
画面全体をぼかします。

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

namespace sky { namespace lib { namespace graphic {

class   CPostEffectGaussianFilter;
typedef SmartPointer< CPostEffectGaussianFilter >		CPostEffectGaussianFilterPtr;			//スマートポインタ型定義

/// <summary>
///  spRenderTargetTextureの内容をぼかしてレンダリングする
///  CSamplerData::m_Sampler0(使用しない）
///  CSamplerData::m_Sampler1(使用しない）
///  CSamplerData::m_Sampler2(使用しない）
/// </summary>
class CPostEffectGaussianFilter : public CPostEffectObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPostEffectGaussianFilter(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

	/// <summary>
	/// ぼかし度
	/// </summary>
	void SetDispersion( f32 value = 5.0f );

//---static

	/// <summary>
	/// 作成
	/// </summary>
	static CPostEffectGaussianFilterPtr   Create()
	{
		return CPostEffectGaussianFilterPtr( NEW__( CPostEffectGaussianFilter , CPostEffectGaussianFilter() ) );
	}

private:

	/// <summary>
	/// ウェイト
	/// </summary>
	f32 m_Weights[ 8 ];

	/// <summary>
	/// ぼかし度
	/// </summary>
	f32 m_Dispersion_sq;

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

	/// <summary>
	/// パス２描画（環境ごとに実装）
	/// </summary>
	void Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

	/// <summary>
	/// ウェイトを計算
	/// </summary>
	void CPostEffectGaussianFilter::ComputeGaussWeights( f32 dispersion );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPostEffectGaussianFilter();
};

} } }

#define CPostEffectGaussianFilterCreate_()	          sky::lib::graphic::CPostEffectGaussianFilter::Create()
