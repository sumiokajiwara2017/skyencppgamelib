#pragma once

namespace sky { namespace lib { namespace input {

//-------------------------------------< 実装 >---------------------------------------------//

class Ckeyboard;
typedef SmartPointer< Ckeyboard >	CkeyboardPtr;

/// <summary>
/// 入力受付管理
/// <summary>
class Ckeyboard  : public base::SkyRefObject
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
	/// プログラムキー入力（デバイスからの入力が無くても、キー入力があったことにする）
	/// <summary>
	void	KeyOn				( eSkyKeybordKeyKind key );

	/// <summary>
	/// キー押下中判定
	/// <summary>
	skyBool IsKeyOn				( eSkyKeybordKeyKind key );

	/// <summary>
	/// キー離し判定
	/// <summary>
	skyBool IsKeyRelease		( eSkyKeybordKeyKind key );

	/// <summary>
	/// キー押下直後判定
	/// <summary>
	skyBool IsKeyPress			( eSkyKeybordKeyKind key );

	/// <summary>
	/// キーリピート判定
	/// <summary>
	skyBool IsKeyRepeat			( eSkyKeybordKeyKind key );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Ckeyboard          (){};

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
	/// キーボード取得
	/// </summary>
	static CkeyboardPtr &Get    ( u32 playerNo = CURRENT_PLAYNER_NO );

protected:

	/// <summary>
	/// キー入力情報
	/// </summary>
	skyBool m_bKeybuf[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// １フレーム前のキー入力情報
	/// </summary>
	skyBool m_bOldKeybuf[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// プログラムキー入力情報
	/// </summary>
	skyBool m_bProgramKeybuf[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// １フレーム前のプログラムキー入力情報
	/// </summary>
	skyBool m_bOldProgramKeybuf[ eSkyKeybordKeyKind_Max ];


	/// <summary>
	/// リピートフラグ
	/// </summary>
	skyBool m_RepFlg[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// リピート間隔カウンター
	/// </summary>
	s32 m_RepIntvl[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// デリゲートハンドラ
	/// <summary>
	delegate::CDelegateHandler	        m_EventHandler;

	/// <summary>
	/// キー情報の更新
	/// <summary>
	static void _UpdateInputInfo();

	/// <summary>
	/// キー入力受付関数のコールバック更新
	/// <summary>
	static void _UpdateInputFunc( const fps::CFpsController &fps );

	/// <summary>
	/// コンストラクタ
	/// <summary>
	Ckeyboard(){};
	Ckeyboard( const Ckeyboard& ){};							//コピーコンストラクタ
	Ckeyboard &operator=(const Ckeyboard& ){ return *this; };	//代入演算子オーバーロード
};

} } }

#define CkeyboardInitialize_( playerNum ) sky::lib::input::Ckeyboard::Initialize( playerNum )
#define Ckeyboard_                        sky::lib::input::Ckeyboard::Get()
#define CkeyboardDispose_()               sky::lib::input::Ckeyboard::Dispose()
