#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×未実装

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CPerspectiveShadowMapDrawShaderObject;
typedef SmartPointer< CPerspectiveShadowMapDrawShaderObject >		CPerspectiveShadowMapDrawShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// CookTorranceモデルシェーダー
/// </summary>
class CPerspectiveShadowMapDrawShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPerspectiveShadowMapDrawShaderObject(){};

	/// <summary>
	/// シャドウマップレンダーターゲットを設定する
	/// </summary>
	void AttachShadowMapRenderTarget( const CRenderTargetPtr &spShadowMap );

	/// <summary>
	/// CPerspectiveShadowMapCreateShaderObjectを設定する
	/// </summary>
	void AttachShadowMapCreater( const CPerspectiveShadowMapCreateShaderObjectPtr &spCreater );

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
	static CPerspectiveShadowMapDrawShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CPerspectiveShadowMapDrawShaderObjectPtr spBasicLightShaderObject( NEW__( CPerspectiveShadowMapDrawShaderObject , CPerspectiveShadowMapDrawShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPerspectiveShadowMapDrawShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();

	/// <summary>
	/// シャドウマップレンダーターゲット
	/// </summary>
	CRenderTargetPtr                           m_spShadowMapRenderTarget;

	/// <summary>
	/// シャドウマップクリエイターの参照
	/// </summary>
	CPerspectiveShadowMapCreateShaderObjectPtr m_spShadowMapCreate;
};

} } }

#define CPerspectiveShadowMapDrawShaderObjectCreate_()			 sky::lib::graphic::CPerspectiveShadowMapDrawShaderObject::Create()
#define CPerspectiveShadowMapDrawShaderObjectCreateName_( name ) sky::lib::graphic::CPerspectiveShadowMapDrawShaderObject::Create( name )
