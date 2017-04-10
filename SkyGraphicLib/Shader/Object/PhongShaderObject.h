#pragma once

/*

【 概要 】
光沢のあるプラスティックなどの鏡面反射の計算によく使われる。

【 実装状況 】
DirectX11 PC HLSL ○未実装
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×実装済

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

class   CPhongShaderObject;
typedef SmartPointer< CPhongShaderObject >		CPhongShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// Phongモデルシェーダー
/// </summary>
class CPhongShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPhongShaderObject(){};

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
	static CPhongShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CPhongShaderObjectPtr spBasicLightShaderObject( NEW__( CPhongShaderObject , CPhongShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPhongShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CPhongShaderObjectCreate_()			 sky::lib::graphic::CPhongShaderObject::Create()
#define CPhongShaderObjectCreateName_( name ) sky::lib::graphic::CPhongShaderObject::Create( name )
