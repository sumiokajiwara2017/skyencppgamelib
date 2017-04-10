#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CVertexDiffuseColorShaderObject;
typedef SmartPointer< CVertexDiffuseColorShaderObject >		CVertexDiffuseColorShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// 頂点カラーのみ出すシェーダー（頂点情報に頂点カラーが存在することを前提とする）
/// </summary>
class CVertexDiffuseColorShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CVertexDiffuseColorShaderObject(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();

//---Serialize

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
	static CVertexDiffuseColorShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CVertexDiffuseColorShaderObjectPtr spBasicLightShaderObject( NEW__( CVertexDiffuseColorShaderObject , CVertexDiffuseColorShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CVertexDiffuseColorShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CVertexDiffuseColorShaderObjectCreate_()			sky::lib::graphic::CVertexDiffuseColorShaderObject::Create()
#define CVertexDiffuseColorShaderObjectCreateName_( name )	sky::lib::graphic::CVertexDiffuseColorShaderObject::Create( name )
