#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ×未実装
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×未実装

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CookTorranceShaderObject;
typedef SmartPointer< CookTorranceShaderObject >		CookTorranceShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// CookTorranceモデルシェーダー
/// </summary>
class CookTorranceShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CookTorranceShaderObject(){};

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
	static CookTorranceShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CookTorranceShaderObjectPtr spBasicLightShaderObject( NEW__( CookTorranceShaderObject , CookTorranceShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CookTorranceShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CookTorranceShaderObjectCreate_()			 sky::lib::graphic::CookTorranceShaderObject::Create()
#define CookTorranceShaderObjectCreateName_( name ) sky::lib::graphic::CookTorranceShaderObject::Create( name )
