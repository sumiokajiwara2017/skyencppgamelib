#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CSkinMeshAnimationShaderObject;
typedef SmartPointer< CSkinMeshAnimationShaderObject >		CSkinMeshAnimationShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// スキンメッシュアニメーションシェーダー
/// </summary>
class CSkinMeshAnimationShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CSkinMeshAnimationShaderObject(){};

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
	static CSkinMeshAnimationShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CSkinMeshAnimationShaderObjectPtr spBasicLightShaderObject( NEW__( CSkinMeshAnimationShaderObject , CSkinMeshAnimationShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSkinMeshAnimationShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CSkinMeshAnimationShaderObjectCreate_()				sky::lib::graphic::CSkinMeshAnimationShaderObject::Create()
#define CSkinMeshAnimationShaderObjectCreateName_( name )	sky::lib::graphic::CSkinMeshAnimationShaderObject::Create( name )
