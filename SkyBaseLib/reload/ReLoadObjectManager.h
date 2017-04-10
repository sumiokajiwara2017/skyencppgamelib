#pragma once

namespace sky { namespace lib { namespace reload {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// リロード情報管理クラス
/// </summary>
class CReLoadObjectManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose();

	/// <summary>
	/// リロード情報登録
	/// </summary>
	static void Add( IReLoadObjectPtr &pObj );

	/// <summary>
	/// リロード情報全クリア
	/// </summary>
	static void Clear();

	/// <summary>
	/// 更新
	/// reloadIntvl:リロードチェック間隔(ミリ秒）
	/// </summary>
	static void Update( updateTime nowTime , updateTime reloadCheckIntvlTime = 2000.0 );

private:

	//リロード情報リスト
	static IReLoadObjectPtrList* m_pReLoadInfoList;

	//最後にリロードをしたフレーム数
	static updateTime            m_LastReloadTime;

	//各機能封印
	CReLoadObjectManager(){};													    //コンストラクタ
	CReLoadObjectManager( const CReLoadObjectManager& ){};							//コピーコンストラクタ
	virtual ~CReLoadObjectManager(){};											    //デストラクタ
	CReLoadObjectManager &operator=(const CReLoadObjectManager& ){ return *this; };	//代入演算子オーバーロード
};

//インスタンスアクセス文字列
#define CReLoadObjectManagerInitialize_()       sky::lib::reload::CReLoadObjectManager::Initialize()
#define CReLoadObjectManager_                   sky::lib::reload::CReLoadObjectManager
#define CReLoadObjectManagerDispose_()          sky::lib::reload::CReLoadObjectManager::Dispose()

} } }

