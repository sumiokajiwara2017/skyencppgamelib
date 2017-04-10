#pragma once

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)

struct CRenderTargetMember;
typedef SmartPointer2< CRenderTargetMember > CRenderTargetMemberPtr;		//スマートポインタ版定義

//テンプレートTypedef郡

class CRenderTarget;
typedef SmartPointer< CRenderTarget >			    CRenderTargetPtr;		//スマートポインタ版定義
typedef CList		< u32 , CRenderTargetPtr >	    CRenderTargetPtrList;	//リスト定義
typedef CHash		< CRenderTargetPtr >			CRenderTargetPtrHash;	//Hash定義
typedef CHashMT		< CRenderTargetPtr >			CRenderTargetPtrHashMT;	//Hash定義

/// <summary>
///  レンダリング対象バッファクラス。
///  レンダリングターゲットには二種類ある。
///  ①フレームバッファレンダリングターゲット（SetUpFrameBufferで作成、描画用）
///  ②テクスチャレンダリングターゲット（SetUpTextureで作成、オフライン描画用）
/// </summary>
class CRenderTarget : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  デストラクタ
    /// </summary>
	virtual                ~CRenderTarget();

	/// <summary>
	/// ウィンドウに書き出すためのレンダリングターゲットを作成する。
	/// </summary>
	void                   SetUpFrameBuffer( const CWindowPtr &spWindow );

	/// <summary>
	/// テクスチャに書き出すためのレンダリングターゲットを作成する。引数に渡されたテクスチャを使用してレンダリングすることができる）
	/// </summary>
	void                   SetUpTexture( const math::CBasicSize &size );

	/// <summary>
	/// 開放
	/// </summary>
	void                   Release();

	/// <summary>
	/// フレームバッファレンダリングターゲットか？
	/// </summary>
	skyBool                IsFrameBufferRenderTarget();

	/// <summary>
	/// サイズの取得／設定
	/// </summary>
	math::CBasicSize       GetSize()                      { return m_Size; }
	void                   SetSize( math::CBasicSize size ){ m_Size = size; }

	/// <summary>
	/// サイズの変更
	/// </summary>
	void                   ReSize          ( u32 newWidth , u32 newHeight );

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	void				  vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	void				  vPostRendering   ( CRenderingParam &param );

	/// <summary>
	/// カラーバッファをテクスチャとして取得する
	/// </summary>
	CTexturePtr            GetColorBufferTexture();

	/// <summary>
	/// カラーバッファクリアカラーを設定／取得する
	/// </summary>
	CColor&                GetClearColor()                      { return m_ClearColor; };
	void                   SetClearColor( const CColor &color ) { m_ClearColor = color; };

//-----描画の際使用-------▽

	/// <summary>
	/// カラーバッファのクリア
	/// </summary>
    void                   ClearColorBuffer();

	/// <summary>
	/// レンダラーにカラーバッファと深度ステンシルバッファを設定する（以降設定されたものにたいして描画が行われる）
	/// </summary>
    void                   SetRenderer( const CDepthStencilBufferPtr &spBuffer = CDepthStencilBufferPtr() );

	/// <summary>
	/// レンダリングターゲットをDisplayに送信
	/// </summary>
    void                    Present();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	inline const CRenderTargetMemberPtr &GetMember(){ return m_pMember; }

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
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr 		  Clone();
	serialize::CSerializeInterfacePtr         CloneSharing();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CRenderTargetPtr	   Create( const hash::CHashStringPtr &pName = hash::CHashStringPtr() )
	{
		CRenderTargetPtr spRenderTarget( NEW__( CRenderTarget , CRenderTarget() ) );
		spRenderTarget->Name.SetName( pName );
		return spRenderTarget;
	}

protected:

//---Member

	/// <summary>
	/// 画面縦横の長さ
	/// </summary>
	math::CBasicSize        m_Size;

	/// <summary>
	/// クリアカラー
	/// </summary>
	CColor                  m_ClearColor;

//---Member

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CRenderTargetMemberPtr  m_pMember;

	/// <summary>
	/// カラーバッファテクスチャ
	/// </summary>
	CTexturePtr             m_spColorBufferTexture;

	/// <summary>
	/// フレームバッファレンダリングターゲットフラグ
	/// </summary>
	skyBool                 m_IsFrameBuffer;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderTarget();
};

} } }

#define CRenderTargetCreate_()           sky::lib::graphic::CRenderTarget::Create()
#define CRenderTargetCreateName_( name )	sky::lib::graphic::CRenderTarget::Create( name )

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< IDXGISwapChain >	         D3DSwapChainComPtr;
typedef SmartPointerCom< ID3D11RenderTargetView >    D3D11RenderTargetViewPtr; 
typedef SmartPointerCom< ID3D11DepthStencilView >    D3D11DepthStencilViewPtr;

struct CRenderTargetMember
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRenderTargetMember()
	{
		if ( m_spSwapChain.IsEmpty() == skyFalse )
		{
			//フルスクリーンだったら戻す
			BOOL IsFullscreen;
			m_spSwapChain->GetFullscreenState( &IsFullscreen, NULL );
			if ( IsFullscreen )
			{
				m_spSwapChain->SetFullscreenState( FALSE, NULL );
			}
		}
	}

	/// <summary>
	/// スワップチェイン
	/// </summary>
	D3DSwapChainComPtr					m_spSwapChain;

	/// <summary>
	/// レンダリングターゲット
	/// </summary>
	D3D11RenderTargetViewPtr            m_spRenderTargetView;

	/// <summary>
	/// スワップチェインのバックバッファを利用したカラーバッファの構築
	/// </summary>
	void                               CreateColorBufferFromSwapChain( CTexturePtr &spColorBufferTexture );
};


} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

struct CRenderTargetMember
{
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRenderTargetMember()
	{
		if ( m_FOB.IsEmpty() == skyFalse )
		{
			m_FOB->Dispose();
		}
	}

	/// <summary>
	/// Windowハンドル
	/// </summary>
	HWND m_nWindowHndl;

	/// <summary>
	/// デバイスコンテキスト
	/// </summary>
    HDC   m_hDC;

	/// <summary>
	/// レンダリングコンテキスト
	/// </summary>
	HGLRC m_hRC;

	/// <summary>
	/// Glewコンテキスト
	/// </summary>
	GLEWContext *m_GlewC;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderTargetMember() : m_IsFrameBuffer( skyTrue ) , m_hDC( NULL ) , m_hRC( NULL ) , m_GlewC( NULL ){}

	/// <summary>
	/// フレームバッファかフレームバッファオブジェクトかの判定フラグ
	/// </summary>
	skyBool m_IsFrameBuffer;

	/// <summary>
	/// フレームバッファオブジェクト（フレームバッファの時は空）
	/// </summary>
	CFramebufferObjectPtr m_FOB;
};

} } }

#endif