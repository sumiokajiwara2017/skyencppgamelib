#pragma once

namespace sky { namespace lib { namespace graphic {

class CPostEffectObject;
typedef SmartPointer< CPostEffectObject >   CPostEffectObjectPtr;		    //スマートポインタ版定義
typedef CList< u32 , CPostEffectObjectPtr > CPostEffectObjectPtrList;		//スマートポインタリスト型
/// <summary>
/// ポストエフェクト基本クラス
/// </summary>
class CPostEffectObject : public base::SkyRefObject
{
    SkyTypeDefRTTI;

friend class PostEffectManager;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPostEffectObject(){};

	/// <summary>
	/// パス数の取得
	/// </summary>
	u32 GetPassNum();

	/// <summary>
	/// 有効設定（有効だとポストエフェクトがかかる。デフォルト有効）
	/// </summary>
	void SetEnable( skyBool isEnable );

	/// <summary>
	/// 描画
	/// </summary>
	virtual void vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix ) = 0;

	/// <summary>
	/// この実装は空実装にしておく
	/// </summary>
	void vDraw(){};

protected:

	/// <summary>
	/// パス数
	/// </summary>
	u32 m_PassNum;

	/// <summary>
	/// 有効フラグ
	/// </summary>
	skyBool m_IsEnable;

    /// <summary>
	///  現在使用しているシェーダーを示すＩＤ
    /// </summary>
	u32	m_ShaderId;

    /// <summary>
	/// シェーダー情報
    /// </summary>
	CShaderInfoPtr m_spShaderInfo;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPostEffectObject();
};

} } }
