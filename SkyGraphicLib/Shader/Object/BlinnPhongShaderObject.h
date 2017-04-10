#pragma once

/*

【 概要 】
フォンの鏡面反射モデルは反射ベクトルを計算しますが，一部の環境によって反射ベクトルを計算するのが大変なそうなので，
ライトベクトルLと視線ベクトルEの中間の角度にあるベクトルで計算するBlinn-Phong鏡面反射モデルというのが考え出されています。

【 実装状況 】
DirectX11 PC HLSL ×未実装
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ×未実装

【 使用CMaterial値 】
・Ambient
・Diffuse
・Specular
・Power
・Alpha

【 使用CLight値 】
//平行光のパラメーターを見ます。
・CDirectionalLightData::m_DiffuseColor
・CDirectionalLightData::m_SpecularColor
・CDirectionalLightData::m_Vec

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CBlinnPhongShaderObject;
typedef SmartPointer< CBlinnPhongShaderObject >		CBlinnPhongShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// BlinnPhongモデルシェーダー
/// </summary>
class CBlinnPhongShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CBlinnPhongShaderObject(){};

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
	static CBlinnPhongShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CBlinnPhongShaderObjectPtr spBasicLightShaderObject( NEW__( CBlinnPhongShaderObject , CBlinnPhongShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CBlinnPhongShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CBlinnPhongShaderObjectCreate_()			 sky::lib::graphic::CBlinnPhongShaderObject::Create()
#define CBlinnPhongShaderObjectCreateName_( name ) sky::lib::graphic::CBlinnPhongShaderObject::Create( name )
