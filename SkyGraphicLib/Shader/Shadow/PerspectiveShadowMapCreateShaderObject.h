#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×未実装

PerspectiveShadowMapを作成します。

PSMで影を創る手順はCPerspectiveShadowMapCreateShaderObjectでシーンのメッシュをシャドーマップにレンダリングし
CPerspectiveShadowMapDrawShaderObjectでメッシュを普通にレンダリングしつつ、シャドーマップを使用して影も同時にレンダリングする

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

static const dectype MIN_Z_NEAR  = 3.0f;
static const dectype MAX_Y_SCENE = 10.0f;
static const dectype SQRT2       = ( dectype )math::CMathUtil::Sqrt( 2.0 );
static const dectype SQRT3       = ( dectype )math::CMathUtil::Sqrt( 3.0 );

class   CPerspectiveShadowMapCreateShaderObject;
typedef SmartPointer< CPerspectiveShadowMapCreateShaderObject >		CPerspectiveShadowMapCreateShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// CookTorranceモデルシェーダー
/// </summary>
class CPerspectiveShadowMapCreateShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

	friend class CPerspectiveShadowMapDrawShaderObject;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPerspectiveShadowMapCreateShaderObject(){};

	/// <summary>
	/// ライトの位置を設定
	/// </summary>
	void SetShadowLightPosition( const math::CBasicVector4 &lightPos );

	/// <summary>
	/// PSMに用いる行列を更新する
	/// </summary>
	void UpdateShadowMatrix();

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();

//---Serialize

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---static

	/// <summary>
	/// 作成
	/// </summary>
	static CPerspectiveShadowMapCreateShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CPerspectiveShadowMapCreateShaderObjectPtr spBasicLightShaderObject( NEW__( CPerspectiveShadowMapCreateShaderObject , CPerspectiveShadowMapCreateShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// 必要パラメーター
	/// </summary>
	math::CMatrix4x3    m_LightView;
	math::CMatrix4x3    m_LightProjection;
	math::CMatrix4x3    m_LightViewProjection;
	math::CMatrix4x3    m_PsmMatrix;
	math::CBasicVector4 m_WorldSpaceLight;

	//以下は現在のカメラの情報に基本含まれているが、PSM用にやや違った値を算出する
	math::CMatrix4x3    m_View;
	math::CMatrix4x3    m_Projection;
	math::CMatrix4x3    m_ViewProjection;
	dectype             m_NearClip;
	dectype             m_FarClip;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPerspectiveShadowMapCreateShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();

	/// <summary>
	/// カメラの行列を求める
	/// </summary>
	/// <param name="wsLight">ワールド空間のライト</param>
	void CalcCameraMtx( const math::CBasicVector4 &wsLight );

	/// <summary>
	/// 平行光源の場合のライトの行列を求める
	/// </summary>
	/// <param name="ppsLight">透視変換後の空間のライト</param>
	void CalcLightMtx_ParalleLight( const math::CBasicVector4 &ppsLight );

	/// <summary>
	/// 点光源の場合のライトの行列を求める
	/// </summary>
	/// <param name="ppsLight">透視変換後の空間のライト</param>
	void CalcLightMtx_PointLight( const math::CBasicVector4 &ppsLight_ );
};

} } }

#define CPerspectiveShadowMapCreateShaderObjectCreate_()           sky::lib::graphic::CPerspectiveShadowMapCreateShaderObject::Create()
#define CPerspectiveShadowMapCreateShaderObjectCreateName_( name ) sky::lib::graphic::CPerspectiveShadowMapCreateShaderObject::Create( name )
