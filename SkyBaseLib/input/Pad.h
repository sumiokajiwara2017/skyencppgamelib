#pragma once

namespace sky { namespace lib { namespace input {

//-------------------------------------< 実装 >---------------------------------------------//

class CPad;
typedef SmartPointer< CPad >	CPadPtr;

/// <summary>
/// 入力受付管理
/// <summary>
class CPad  : public base::SkyRefObject
{

public:

	/// <summary>
	/// ボタンリピート間隔の設定
	/// <summary>
	void SetBtnRepeat			( s32 repIntvl , s32 onIntvl );

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
	void	KeyOn				( eSkyPadKeyKind key );

	/// <summary>
	/// キー押下中判定
	/// <summary>
	skyBool IsKeyOn				( eSkyPadKeyKind key );

	/// <summary>
	/// キー離し判定
	/// <summary>
	skyBool IsKeyRelease		( eSkyPadKeyKind key );

	/// <summary>
	/// キー押下直後判定
	/// <summary>
	skyBool IsKeyPress			( eSkyPadKeyKind key );

	/// <summary>
	/// キーリピート判定
	/// <summary>
	skyBool IsKeyRepeat			( eSkyPadKeyKind key );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CPad          (){};

//---------< static >----------

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize				( u32 playerNum );

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose				();

	/// <summary>
	/// 更新
	/// </summary>
	static void Update			( updateTime time );

	/// <summary>
	/// パッド取得
	/// </summary>
	static CPadPtr &Get    ( u32 playerNo = CURRENT_PLAYNER_NO );

protected:

	/// <summary>
	/// パッドのインスタンス
	/// </summary>
	CPadPtr m_KeyBord;

	/// <summary>
	/// キー入力情報
	/// </summary>
	skyBool m_bKeybuf[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// １フレーム前のキー入力情報
	/// </summary>
	skyBool m_bOldKeybuf[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// リピートフラグ
	/// </summary>
	skyBool m_RepFlg[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// リピート間隔カウンター
	/// </summary>
	s32 m_RepIntvl[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// リピート間隔
	/// </summary>
	s32 m_nRepIntvlNum;

	/// <summary>
	/// リピート間隔
	/// </summary>
	s32 m_nOnIntvlNum;

	/// <summary>
	/// コントローラ(XInput)の情報
	/// </summary>
#ifdef SKYLIB_IDE_VC10
    XINPUT_STATE    state;
#endif

	/// <summary>
	/// コントローラが接続しているかどうか
	/// </summary>
	skyBool            bConnected;

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
	static void _UpdateInputFunc( updateTime time );

	/// <summary>
	/// コンストラクタ
	/// <summary>
	CPad()
	{
		m_nRepIntvlNum = C_S32(input_DEF_REPEATE_INTVL);
		m_nOnIntvlNum = C_S32(input_DEF_ON_INTVL);
	}
	CPad( const CPad& ){};							//コピーコンストラクタ
	CPad &operator=(const CPad& ){ return *this; };	//代入演算子オーバーロード
};

} } }

#define CPadInitialize_( playerNum ) sky::lib::input::CPad::Initialize( playerNum )
#define CPad_                        sky::lib::input::CPad::Get()
#define CPadDispose_()               sky::lib::input::CPad::Dispose()
