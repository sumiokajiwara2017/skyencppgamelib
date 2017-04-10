#pragma once

#ifdef SKY_GRAPHIC_LIB_OPENGL

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)
struct CCgLightShaderObjectMember;

class   CCgLightShaderObject;
typedef SmartPointer< CCgLightShaderObject >		CCgLightShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// 基本シェーダー
/// ライブラリがサポートしているライト（ShaderLightManager参照）をすべて使える
/// </summary>
class CCgLightShaderObject : public CShaderObject
{

public:

	/// <summary>
	/// 読み込み
	/// </summary>
	static void Load( CCgLightShaderObjectMember *dest );

	/// <summary>
	/// 破棄
	/// </summary>
	static void Destroy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CCgLightShaderObject();

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();
	CShaderObjectPtr  vCloneNode();

	/// <summary>
	/// 作成
	/// </summary>
	static CCgLightShaderObjectPtr Create()
    {
		return CCgLightShaderObjectPtr( NEW CCgLightShaderObject() );
    }

private:

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CCgLightShaderObjectMember*    m_pMember;

	/// <summary>
	/// コンストラクタ（名前必須）
	/// </summary>
	CCgLightShaderObject();

};

} } }

#endif

#endif