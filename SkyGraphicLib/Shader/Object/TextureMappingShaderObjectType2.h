#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CTextureMappingShaderType2Object;
typedef SmartPointer< CTextureMappingShaderType2Object >		CTextureMappingShaderType2ObjectPtr;			//スマートポインタ型定義

/// <summary>
/// テクスチャマッピングをするシェーダー
/// </summary>
class CTextureMappingShaderType2Object : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CTextureMappingShaderType2Object(){};

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
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---static

	/// <summary>
	/// 作成
	/// </summary>
	static CTextureMappingShaderType2ObjectPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CTextureMappingShaderType2ObjectPtr spBasicLightShaderObject( NEW__( CTextureMappingShaderType2Object , CTextureMappingShaderType2Object() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CTextureMappingShaderType2Object();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CTextureMappingShaderType2ObjectCreate_()			sky::lib::graphic::CTextureMappingShaderType2Object::Create()
#define CTextureMappingShaderType2ObjectCreateName_( name )	sky::lib::graphic::CTextureMappingShaderType2Object::Create( name )

