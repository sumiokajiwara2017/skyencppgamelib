#pragma once

/// <summary>
/// 更新スレッド
/// </summary>
class DrawCommandManager
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DrawCommandManager( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DrawCommandManager( void );

	/// <summary>
	/// シングルトン機能付加
	/// <summary>
	friend class sky::lib::Singleton< DrawCommandManager >;
	static sky::lib::Singleton< DrawCommandManager > Singleton;

	/// <summary>
	/// スイッチリスト
	/// </summary>
	void SwitchList( void );

	/// <summary>
	/// データ更新リストの取得
	/// </summary>
	DrawCommandPtrList	*GetUpdateList( void );

	/// <summary>
	/// データ描画リストの取得
	/// </summary>
	DrawCommandPtrList	*GetDrawList( void );

private:

	/// <summary>
	/// コマンドリスト
	/// </summary>
	DrawCommandPtrList		m_CommandList[ 2 ];

	/// <summary>
	/// 更新リストインデックス
	/// </summary>
	u32						m_UpdateListIdx; // 値は0,1のどちらか

};

//シングルトンインスタンスアクセス文字列
#define DrawCommandManager_ DrawCommandManager::Singleton.GetInstance()