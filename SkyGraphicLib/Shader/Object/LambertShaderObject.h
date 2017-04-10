#pragma once

/*

ランバート照明は視線方向に依存しない光源の入射方向と面の法線だけで算出される陰影処理技法です。

【 実装状況 】
DirectX11 PC HLSL ×未実装
OpenGL    PC GLSL ○未実装
OpenGL    PC Cg   ×実装済

【 使用CMaterial値 】
・Diffuse
・Alpha

【 使用CLight値 】
//平行光のパラメーターを見ます。
・CDirectionalLightData::m_AmbientColor
・CDirectionalLightData::m_DiffuseColor
・CDirectionalLightData::m_Vec

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CLambertShaderObject;
typedef SmartPointer< CLambertShaderObject >		CLambertShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// Lambertモデルシェーダー
/// </summary>
class CLambertShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CLambertShaderObject(){};

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
	static CLambertShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CLambertShaderObjectPtr spBasicLightShaderObject( NEW__( CLambertShaderObject , CLambertShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CLambertShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CLambertShaderObjectCreate_()			 sky::lib::graphic::CLambertShaderObject::Create()
#define CLambertShaderObjectCreateName_( name ) sky::lib::graphic::CLambertShaderObject::Create( name )
