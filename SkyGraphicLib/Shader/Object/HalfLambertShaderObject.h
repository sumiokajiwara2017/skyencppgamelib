#pragma once

/*

【 概要 】
ハーフ・ランバートライティングはValveが'98年の「Half-Life」から使用しているValve独自の疑似ラジオシティラインティング技法であるそうです。
ランバートより陰影がはっきりしておらず、リアルだという評判。

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

class   CHalfLambertShaderObject;
typedef SmartPointer< CHalfLambertShaderObject >		CHalfLambertShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// Lambertモデルシェーダー
/// </summary>
class CHalfLambertShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CHalfLambertShaderObject(){};

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
	static CHalfLambertShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CHalfLambertShaderObjectPtr spBasicLightShaderObject( NEW__( CHalfLambertShaderObject , CHalfLambertShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CHalfLambertShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CHalfLambertShaderObjectCreate_()			 sky::lib::graphic::CHalfLambertShaderObject::Create()
#define CHalfLambertShaderObjectCreateName_( name ) sky::lib::graphic::CHalfLambertShaderObject::Create( name )
