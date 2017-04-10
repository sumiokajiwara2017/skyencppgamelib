#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

namespace sky { namespace lib { namespace graphic {

class   CPostEffectTextureMapping;
typedef SmartPointer< CPostEffectTextureMapping >		CPostEffectTextureMappingPtr;			//スマートポインタ型定義

/// <summary>
///  spRenderTargetTextureの内容をレンダリングする
///  CSamplerData::m_Sampler0(使用しない）
///  CSamplerData::m_Sampler1(使用しない）
///  CSamplerData::m_Sampler2(使用しない）
/// </summary>
class CPostEffectTextureMapping : public CPostEffectObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPostEffectTextureMapping(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix );

//---static

	/// <summary>
	/// 作成
	/// </summary>
	static CPostEffectTextureMappingPtr   Create()
	{
		return CPostEffectTextureMappingPtr( NEW__( CPostEffectTextureMapping , CPostEffectTextureMapping() ) );
	}
};

} } }

#define CPostEffectTextureMappingCreate_()	          sky::lib::graphic::CPostEffectTextureMapping::Create()
