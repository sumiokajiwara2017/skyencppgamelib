#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CMaterialDiffuseColorShaderObject;
typedef SmartPointer< CMaterialDiffuseColorShaderObject >		CMaterialDiffuseColorShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// マテリアルのDiffuseカラーの色でそのまま出力するシェーダー
/// </summary>
class CMaterialDiffuseColorShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMaterialDiffuseColorShaderObject(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();

//---Serialize

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr 		  Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---static

	/// <summary>
	/// 作成
	/// </summary>
	static CMaterialDiffuseColorShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMaterialDiffuseColorShaderObjectPtr spBasicLightShaderObject( NEW__( CMaterialDiffuseColorShaderObject , CMaterialDiffuseColorShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMaterialDiffuseColorShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CMaterialDiffuseColorShaderObjectCreate_()			    sky::lib::graphic::CMaterialDiffuseColorShaderObject::Create()
#define CMaterialDiffuseColorShaderObjectCreateName_( name )	sky::lib::graphic::CMaterialDiffuseColorShaderObject::Create( name )
