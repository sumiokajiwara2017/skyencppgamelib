#pragma once

namespace sky { namespace lib { namespace input {

//-------------------------------------< 実装 >---------------------------------------------//

struct CMouseMember;

class CMouse;
typedef SmartPointer< CMouse >	CMousePtr;

/// <summary>
/// 入力受付管理
/// <summary>
class CMouse  : public base::SkyRefObject
{

public:

	/// <summary>
	/// ボタンリピート間隔の設定
	/// <summary>
	static void SetBtnRepeat	( s32 repIntvl , s32 onIntvl );

	/// <summary>
	/// 入力受付リスナーを登録
	/// </summary>
	void AttachInputListener	( const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// 入力受付リスナーを登録解除
	/// </summary>
	void DetachInputListener	( const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMouse            ();

//---------< static >----------

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize		( u32 playerNum );

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose			();

	/// <summary>
	/// 更新
	/// </summary>
	static void Update			( const fps::CFpsController &fps );

	/// <summary>
	/// マウス取得
	/// </summary>
	static CMousePtr& Get       ( u32 playerNo = CURRENT_PLAYNER_NO );

	/// <summary>
	/// プログラムキー入力（デバイスからの入力が無くても、キー入力があったことにする）
	/// <summary>
	void	KeyOn				( eSkyMouseKeyKind key );

	/// <summary>
	/// キー押下中判定
	/// <summary>
	skyBool IsKeyOn				( eSkyMouseKeyKind key );

	/// <summary>
	/// キー離し判定
	/// <summary>
	skyBool IsKeyRelease		( eSkyMouseKeyKind key );

	/// <summary>
	/// キー押下直後判定
	/// <summary>
	skyBool IsKeyPress			( eSkyMouseKeyKind key );

	/// <summary>
	/// キーリピート判定
	/// <summary>
	skyBool IsKeyRepeat			( eSkyMouseKeyKind key );

//---イベント発生検知は各プラットフォーム様々だろう多分。Windowsはウインドウプロシージャでやってます-----

	/// <summary>
	/// マウス左ボタンダウンイベント発生
	/// </summary>
	static void       EventExec_LBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// マウス左ボタンアップイベント発生
	/// </summary>
	static void       EventExec_LBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// マウス右ボタンダウンイベント発生
	/// </summary>
	static void       EventExec_RBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// マウス右ボタンアップイベント発生
	/// </summary>
	static void       EventExec_RBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// マウス中央ボタンダウンイベント発生
	/// </summary>
	static void       EventExec_CBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// マウス中央ボタンアップイベント発生
	/// </summary>
	static void       EventExec_CBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// ホイール回転イベント発生
	/// </summary>
	static void       EventExec_HWRotation( u32 playerNo , s32 x , s32 y , s32 rotVal , eSkyMouseKeyWheelWithBtnKind eWWKind , wndType windowId );

	/// <summary>
	/// マウス移動イベント発生
	/// </summary>
	static void       EventExec_Move( u32 playerNo , s32 x , s32 y , wndType windowId );

protected:

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CMouseMember *m_pMember;

	/// <summary>
	/// コンストラクタ
	/// <summary>
	CMouse();
	CMouse( const CMouse& ){};							//コピーコンストラクタ
	CMouse &operator=(const CMouse& ){ return *this; };	//代入演算子オーバーロード
};

} } }

#define CMouseInitialize_( playerNum ) sky::lib::input::CMouse::Initialize( playerNum )
#define CMouse_                        sky::lib::input::CMouse::Get()
#define CMouseDispose_()               sky::lib::input::CMouse::Dispose()
