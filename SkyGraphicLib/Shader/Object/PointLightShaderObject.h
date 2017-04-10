#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CPointLightShaderObject;
typedef SmartPointer< CPointLightShaderObject >		CPointLightShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// 点光源シェーダー
/// </summary>
class CPointLightShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPointLightShaderObject(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();

    /// <summary>
	///  光の位置設定
    /// </summary>
	void SetLightPosition( const math::CBasicVector3 &postion ){ m_LightPosition = postion; }

    /// <summary>
	///  光の強さ設定
    /// </summary>
	void SetLightPower( f32 power ){ m_LightPower = power; }

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
	static CPointLightShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CPointLightShaderObjectPtr spBasicLightShaderObject( NEW__( CPointLightShaderObject , CPointLightShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// ライトの位置
	/// </summary>
	math::CBasicVector3 m_LightPosition;

	/// <summary>
	/// ライトの力
	/// </summary>
	f32                 m_LightPower;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPointLightShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CPointLightShaderObjectCreate_()			sky::lib::graphic::CPointLightShaderObject::Create()
#define CPointLightShaderObjectCreateName_( name )	sky::lib::graphic::CPointLightShaderObject::Create( name )
