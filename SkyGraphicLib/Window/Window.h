#pragma once

namespace sky { namespace lib { namespace graphic {

//===================================【Common】====================================================>

//プラットフォーム毎のメンバ変数(pimpl)
struct CWindowMember;

//テンプレートTypedef郡
class CWindow;
typedef SmartPointer< CWindow >				CWindowPtr;		//スマートポインタ版定義
typedef CList		< u32 , CWindowPtr >		CWindowPtrList;	//リスト定義
typedef CHash		< CWindowPtr >				CWindowPtrHash;	//Hash定義
typedef CHashMT		< CWindowPtr >				CWindowPtrHashMT;	//Hash定義

//-----------------------------------< 実装 >-------------------------------------------//

class CWindow : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CWindow();

	/// <summary>
	/// 幅と高さと位置を取得する
	/// </summary>
	math::CBasicRect&           GetRect			()										{ return m_WindowRect; }
	
	/// <summary>
	/// サイズ変更確認
	/// </summary>
	skyBool                     IsChangeSize    ( dectype width, dectype height )       { return ( m_WindowRect.W() != width || m_WindowRect.H() != height ); }

	/// <summary>
	/// サイズ変更
	/// </summary>
	void                        ReSize          ( dectype width, dectype height )       { m_WindowRect.W() = width; m_WindowRect.H() = height; }

	/// <summary>
	/// 使用準備をする
	/// </summary>
	void				        SetUp	        ( const math::CBasicRect &rect );
	void				        SetUp	        ( s32 hndl );

	/// <summary>
	/// 移動時デリゲート
	/// </summary>
	void Move( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );

	/// <summary>
	/// ウィンドウ識別IDの取得
	/// </summary>
	wndType GetWindowId();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	inline CWindowMember* GetMember(){ return m_pMember; }

//---Serialize

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr  Clone();
	serialize::CSerializeInterfacePtr  CloneSharing();

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
	/// インスタンスの作成
	/// </summary>
	static CWindowPtr			Create( const hash::CHashStringPtr &name )
	{
		
		CWindowPtr spWindow( NEW__( CWindow , CWindow() ) );
		if ( name.IsEmpty() == skyFalse ) spWindow->Name.SetName( name );
		return spWindow;
	}

protected:

	/// <summary>
	/// セットアップ種別
	/// </summary>
	enum eSetUpKind
	{
		eSetUpKind_Create = 0 ,
		eSetUpKind_Hndl ,
	} 
	m_SetUpKind;

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CWindowMember*      m_pMember;

//---Member

	/// <summary>
	/// Windowの縦横、座標
	/// </summary>
	math::CBasicRect	     m_WindowRect;

//---Member

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CWindow();
};

} } }

#define CWindowCreate_()	        sky::lib::graphic::CWindow::Create()
#define CWindowCreateName_( name )	sky::lib::graphic::CWindow::Create( name )

//SkyBaseLibで実装されている
LRESULT WINAPI SkyWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace sky { namespace lib { namespace graphic {

struct CWindowMember
{
	/// <summary>
	/// Windowハンドル
	/// </summary>
	HWND						m_nWindowHndl;

	/// <summary>
	/// Windowクラス
	/// </summary>
	WNDCLASSEX					m_kWc;

	/// <summary>
	/// 移動感知デリゲート
	/// </summary>
	delegate::IDelegateObjectPtr m_spMoveDelegate;

	/// <summary>
	/// サイズ変更感知デリゲート
	/// </summary>
	delegate::IDelegateObjectPtr m_spReSizeDelegate;

	/// <summary>
	/// Windowハンドルを取得する
	/// </summary>
	inline const HWND GetWindowHndl() const { return m_nWindowHndl; };
};

} } }
