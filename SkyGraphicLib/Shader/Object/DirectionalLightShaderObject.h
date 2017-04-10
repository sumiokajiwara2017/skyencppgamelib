#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

class   CDirectionalLightShaderObject;
typedef SmartPointer< CDirectionalLightShaderObject >		CDirectionalLightShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// 平行光シェーダー
/// </summary>
class CDirectionalLightShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDirectionalLightShaderObject(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();

    /// <summary>
	///  光の方向を決定
    /// </summary>
	void SetLightDirection( const math::CBasicVector3 &postion )
	{ 
		m_LightDirection = postion;
		m_LightDirection.Normalize();
	}

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
	static CDirectionalLightShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CDirectionalLightShaderObjectPtr spBasicLightShaderObject( NEW__( CDirectionalLightShaderObject , CDirectionalLightShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// ライトの方向
	/// </summary>
	math::CBasicVector3 m_LightDirection;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDirectionalLightShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CDirectionalLightShaderObjectCreate_()			 sky::lib::graphic::CDirectionalLightShaderObject::Create()
#define CDirectionalLightShaderObjectCreateName_( name ) sky::lib::graphic::CDirectionalLightShaderObject::Create( name )
