#pragma once
/*

【 実装状況 】
DirectX11 PC HLSL △実装済。バグあり。
OpenGL    PC GLSL ○実装済
OpenGL    PC Cg   ○実装済

*/

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)

struct CProjectionTextureShaderObjectMember;

class   CProjectionTextureShaderObject;
typedef SmartPointer< CProjectionTextureShaderObject >		CProjectionTextureShaderObjectPtr;			//スマートポインタ型定義

/// <summary>
/// 平行光シェーダー
/// </summary>
class CProjectionTextureShaderObject : public CShaderObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CProjectionTextureShaderObject(){};

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void              vDraw();

    /// <summary>
	///  テクスチャ光源の位置
    /// </summary>
	void SetTextureLightPosition( const math::CBasicVector3 &position )
	{
		m_TextureLightPosition = position;
	}

    /// <summary>
	///  テクスチャ光源の方向
    /// </summary>
	void SetTextureLightTag( const math::CBasicVector3 &tag )
	{
		m_TextureLightTag = tag;
	}

    /// <summary>
	///  テクスチャ光源の上方向ベクトル
    /// </summary>
	void SetTextureLightUp( const math::CBasicVector3 &up )
	{
		m_TextureLightUp = up;
	}

    /// <summary>
	///  アスペクト比
    /// </summary>
	void SetAspect( dectype aspect )
	{
		m_TextureLightAspect = aspect;
	}

    /// <summary>
	///  テクスチャ光源の画角設定
    /// </summary>
	void SetTextureLightFov( dectype fov )
	{
		m_TextureLightFov = fov;
	}

    /// <summary>
	///  テクスチャ光源の近接面距離設定
    /// </summary>
	void SetTextureLightNear( dectype near_ )
	{
		m_TextureLightNear = near_;
	}

    /// <summary>
	///  テクスチャ光源の遠方面距離設定
    /// </summary>
	void SetTextureLightFar( dectype far_ )
	{
		m_TextureLightFar = far_;
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
	static CProjectionTextureShaderObjectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CProjectionTextureShaderObjectPtr spBasicLightShaderObject( NEW__( CProjectionTextureShaderObject , CProjectionTextureShaderObject() ) );
		if ( name.IsEmpty() == skyFalse ) spBasicLightShaderObject->Name.SetName( name );
		return spBasicLightShaderObject;
	}

private:

	/// <summary>
	/// テクスチャライトの位置
	/// </summary>
	math::CBasicVector3 m_TextureLightPosition;

	/// <summary>
	/// テクスチャライトの上方向
	/// </summary>
	math::CBasicVector3 m_TextureLightUp;

	/// <summary>
	/// テクスチャライトのターゲット
	/// </summary>
	math::CBasicVector3 m_TextureLightTag;

    /// <summary>
	///  テクスチャ光源の画角
    /// </summary>
	dectype m_TextureLightFov;

    /// <summary>
	///  テクスチャ光源の画面比率
    /// </summary>
	dectype m_TextureLightAspect;

    /// <summary>
	///  テクスチャ光源の近接面距離
    /// </summary>
	dectype m_TextureLightNear;

    /// <summary>
	///  テクスチャ光源の遠方面距離
    /// </summary>
	dectype m_TextureLightFar;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CProjectionTextureShaderObject();

	/// <summary>
	/// パス１描画（環境ごとに実装）
	/// </summary>
	void Pass1Draw();
};

} } }

#define CProjectionTextureShaderObjectCreate_()			sky::lib::graphic::CProjectionTextureShaderObject::Create()
#define CProjectionTextureShaderObjectCreateName_( name )	sky::lib::graphic::CProjectionTextureShaderObject::Create( name )

