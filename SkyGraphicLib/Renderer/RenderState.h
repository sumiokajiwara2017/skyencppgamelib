#pragma once

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)
struct CRenderStateMember;

/// <summary>
/// 定数
/// </summary>
namespace renderstate
{
	//同時に設定できるステートの最大数
	static const u32 STATE_STACK_MAX_NUM = 32;
}

//テンプレートTypedef郡
class CRenderState;
typedef SmartPointer< CRenderState >	CRenderStatePtr;      //スマートポインタ版定義
typedef CStack< CRenderStatePtr >		CRenderStatePtrStack;

/// <summary>
/// レンダーステートクラス（内部でスタック管理）
/// このクラスを使用すれば設定したステートを描画後にすべて戻すことができるが、多用はしないこと。地味にパフォーマンスを落とすらしい。
/// </summary>
class CRenderState : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRenderState();

	/// <summary>
	/// 初期化
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	virtual void   vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	virtual void   vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// カリングの有効設定
	/// </summary>
	void    EnableCulling ();

	/// <summary>
	/// シザーの有効設定
	/// </summary>
	void    EnableScissorTest ();

	/// <summary>
	/// 深度バッファ有効
	/// </summary>
	void    EnableDepthBuffer ();

	/// <summary>
	/// 深度バッファ有効
	/// </summary>
	void    DisableDepthBuffer ();

	/// <summary>
	/// ステンシルテスト有効
	/// </summary>
	void    EnableStencilTest ();

	/// <summary>
	/// ステンシルマスク有効
	/// </summary>
	void    EnableStencilMask ();

	/// <summary>
	/// ブレンドステート設定
	/// </summary>
	void    EnableTextureBlending ();

	/// <summary>
	/// レンダラーにステートを設定する
	/// </summary>
	void    SetRenderer();

	/// <summary>
	/// レンダラーにステートを設定する
	/// </summary>
	void    ReverseRenderer();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	CRenderStateMember* GetMember(){ return m_pMember; }

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
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	void Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	void Deactivate();

//---Static

	/// <summary>
	/// インスタンス作成
	/// </summary>
	static CRenderStatePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CRenderStatePtr spRenderState( NEW__( CRenderState , CRenderState() ) );
		if ( name.IsEmpty() == skyFalse ) spRenderState->Name.SetName( name );
		return spRenderState;
	}

protected:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderState();
	void Constractor()
	{
		m_IsEnableCulling = skyFalse;
		m_IsEnableScissorTest = skyFalse;
		m_IsEnableDepthBuffer = skyFalse;
		m_IsEnableStencilTest = skyFalse;
		m_IsEnableStencilMask = skyFalse;
		m_IsEnableTextureBlending = skyFalse;
	}

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CRenderStateMember*   m_pMember;

//---Member

	/// <summary>
	/// カリングの有効設定フラグ
	/// </summary>
	skyBool  m_IsEnableCulling;

	/// <summary>
	/// シザーの有効設定フラグ
	/// </summary>
	skyBool  m_IsEnableScissorTest;

	/// <summary>
	/// 深度バッファ有効フラグ
	/// </summary>
	skyBool  m_IsEnableDepthBuffer;

	/// <summary>
	/// 深度バッファ有効フラグ
	/// </summary>
	skyBool  m_IsDisableDepthBuffer;

	/// <summary>
	/// ステンシルテスト有効フラグ
	/// </summary>
	skyBool  m_IsEnableStencilTest;

	/// <summary>
	/// ステンシルマスク有効フラグ
	/// </summary>
	skyBool  m_IsEnableStencilMask;

	/// <summary>
	/// ブレンドステート設定フラグ
	/// </summary>
	skyBool  m_IsEnableTextureBlending;

//---Member

};

} } }

#define CRenderStateCreate_()			sky::lib::graphic::CRenderState::Create()
#define CRenderStateCreateName_( name )	sky::lib::graphic::CRenderState::Create( name )