#pragma once

namespace sky { namespace lib { namespace graphic {

class   CEffectObject;
typedef SmartPointer< CEffectObject >		CEffectObjectPtr;			//スマートポインタ型定義

/// <summary>
/// エフェクトオブジェクトクラス
/// </summary>
class CEffectObject : public CSceneGraphNode
{

public:

    SkyTypeDefRTTI;

	/// <summary>
	/// パス数の取得
	/// </summary>
	inline u32 GetPassNum(){ return m_PassNum; };

	/// <summary>
	/// レンダリングパス番号設定
	/// </summary>
	inline void       SetRenderingPass( u32 passNo ){ m_RenderingPassNo = passNo; }

	/// <summary>
	/// 頂点座標変換行列を設定する（どのシェーダーにも頂点座標変換は必ず実装する）
	/// </summary>
	inline void       SetWorldMatrix ( const math::CMatrix4x3 &w )  { m_WorldMatrix = w; };

	/// <summary>
	/// カメラ情報を設定
	/// </summary>
	inline void       SetCamera      ( const CameraPtr &spCamera ) { m_spCamera = spCamera;}

	/// <summary>
	/// メッシュを設定
	/// </summary>
	inline void       SetExtraData   ( const CExtraDataPtr   &spExtra )   { m_spExtraData = spExtra; }

	/// <summary>
	/// ライトを設定
	/// </summary>
	inline void       SetLight   ( const CLightPtr   &spLight )   { m_spLight = spLight; }

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	virtual void vRendering	 ( CRenderingParam &param );

	/// <summary>
	/// 描画
	/// </summary>
	virtual void vDraw() = 0;

protected:

	/// <summary>
	/// パス数
	/// </summary>
	u32                       m_PassNum;

	/// <summary>
	/// パス数
	/// </summary>
	u32                       m_RenderingPassNo;

	/// <summary>
	/// ワールドマトリックス
	/// </summary>
	math::CMatrix4x3		  m_WorldMatrix;

	/// <summary>
	/// カメラ情報
	/// </summary>
    CameraPtr                 m_spCamera;

	/// <summary>
	/// エキストラデータ情報
	/// </summary>
    CExtraDataPtr             m_spExtraData;

	/// <summary>
	/// ライト情報
	/// </summary>
    CLightPtr                 m_spLight;

    /// <summary>
	///  現在使用しているシェーダーを示すＩＤ
    /// </summary>
	u32                       m_ShaderId;

    /// <summary>
	/// シェーダー情報
    /// </summary>
	CShaderInfoPtr            m_spShaderInfo;
};

} } }
