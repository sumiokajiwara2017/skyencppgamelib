#pragma once

namespace sky { namespace lib { namespace graphic {

static const f32  FADE_TIME = 100.0f;

class   CFadeManager;
typedef SmartPointer< CFadeManager > CFadeManagerPtr; //スマートポインタ型定義
typedef kfm::CFCurve< f32 >          CFadeFCurve;
typedef kfm::CFCurvePlayer< f32 >    CFadeFCurvePlayer;

/// <summary>
/// フェード状態種別
/// </summary>
enum eFadeStateKind
{
	eFadeStateKind_FadeIn = 0 ,
	eFadeStateKind_FadePlaying ,
	eFadeStateKind_FadeOut ,
};

/// <summary>
/// フェード管理
/// 基本の白フェード、黒フェード、その他の色フェードのみ提供
/// 複雑なイメージフェードはスプライトとマテリアルアニメーションの組み合わせで
/// 独自に実装してください。
/// </summary>
class CFadeManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CFadeManager(){};

	/// <summary>
	/// フェードスプライトを作成
	/// </summary>
	void CreateFadeSprite( dectype w , dectype h );

	/// <summary>
	/// フェードスピードの設定
	/// </summary>
	void SetFadeSpeed( f32 speed );

	/// <summary>
	/// フェードカラーの設定
	/// </summary>
	void SetFadeColor( CColor color );

	/// <summary>
	/// フェードインする
	/// </summary>
	void ExecFadeIn();

	/// <summary>
	/// フェードアウトする
	/// </summary>
	void ExecFadeOut();

	/// <summary>
	/// フェードイン済み状態にする
	/// </summary>
	void SetFadeInEnd();

	/// <summary>
	/// フェードイン済み状態にする
	/// </summary>
	void SetFadeOutEnd();

	/// <summary>
	/// フェード状態取得
	/// </summary>
	eFadeStateKind GetFadeState();

	/// <summary>
	/// シーングラフのルートを取得
	/// フェードはフェード専用のシーンをさくせいして　２Ｄ作成してカメラはSetLeftTopCenter( skyFalse )にして、そこでフェードを描画する。
	/// </summary>
	const CSceneGraphNodePtr &GetSceneGraphRoot();

	/// <summary>
	/// 更新
	/// </summary>
	void Update( const fps::CFpsController &fps );

//----Static

	/// <summary>
	/// インスタンス生成
	/// EventManagerはシステムに1つ存在するが、インスタンスを作成してローカルで使用することもできる
	/// </summary>
    static CFadeManagerPtr Create();

	//シングルトン機能を付属
	friend class SingletonPtr< CFadeManager >;
	static SingletonPtr< CFadeManager > Singleton;

private:

	/// <summary>
	/// サイズ変更感知デリゲート
	/// </summary>
	delegate::IDelegateObjectPtr m_spReSizeDelegate;
    void ReSize( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );

	/// <summary>
	/// 新しい値を計算
	/// </summary>
	void NewValueColFuncEvent ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args );

	/// <summary>
	/// フェードスピード
	/// </summary>
	f32 m_FadeSpeed;

	/// <summary>
	/// フェードインファンクションカーブ
	/// </summary>
	CFadeFCurve  m_FadeInFCurve;

	/// <summary>
	/// フェードアウトファンクションカーブ
	/// </summary>
	CFadeFCurve  m_FadeOutFCurve;

	/// <summary>
	/// ファンクションカーブ再生機
	/// </summary>
	CFadeFCurvePlayer m_Player;

	/// <summary>
	/// フェード用スプライト
	/// </summary>
	CSceneGraphNodePtr m_spSprite;

	/// <summary>
	/// シーングラフのルート
	/// </summary>
	CSceneGraphNodePtr m_spSceneGraphRoot;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CFadeManager();
};

} } }

//インスタンスアクセス文字列
#define CFadeManagerCreate_()	sky::lib::graphic::CFadeManager::Singleton.CreateInstance()
#define CFadeManager_			sky::lib::graphic::CFadeManager::Singleton.GetInstance()
#define CFadeManagerDelete_()	sky::lib::graphic::CFadeManager::Singleton.DeleteInstance()
