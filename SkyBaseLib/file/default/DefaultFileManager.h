#pragma once

namespace sky { namespace lib { namespace file { 

struct CDefaultFileManagerMember;

/// <summary>
/// デフォルトファイル管理
/// 事前に拡張子をキーにファイルをロードしておくと、ファイルパスを渡して
/// ロード要求をだした場合、そのファイルパスの拡張子を見てファイルデータを返す。
/// テクスチャ等はしばしばファイルが無くてもそこでエラーせずに動作を継続してほしい。
/// そのための機能。
/// </summary>
class CDefaultFileManager : public base::SkyObject
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
	/// ファイルを非同期読み込みする。
	/// 戻り値として返したオブジェクトのフラグを監視することで命令をだしたファイルの
	/// 読み込み終了判定と、ファイルのデータの受け取りができるが、終了判定は下記の終了
	/// 判定関数を使用する。なぜなら複数のファイルのロード要求を出すことが想定され、
	/// それを一括で終了判定したいという要件が実務ではほとんどだと思われるため。
    ///
	/// ファイルにはカテゴリ番号を付加でき、カテゴリ番号で削除できる。カテゴリ番号は
	/// 同じファイルでも複数もつことができ、カテゴリ毎の削除命令が来たとしても、まず
	/// ファイルの付加されているカテゴリが削除され、自分がどこのカテゴリでもなくなった
	/// 場合はファイルが削除される。
    ///
    /// すでに存在するファイルのロード要求が来たら、ファイルの参照カウンタをＵＰし、
	/// 新規のロードは行わない。削除時も参照カウンタを下げ、参照カウンタが０になったら
	/// 削除を行う
	/// 
	/// </summary>
	static void LoadAsync( const hash::CHashStringPtr &pPath , u32 category = pool::DEFAULT_CATEGORY_NO );

	/// <summary>
	/// 非同期ファイル読み込み要求を全てキャンセルする
	/// </summary>
	static void CancelAll();

	/// <summary>
	/// 非同期ファイル読み込み終了判定
	/// ★★非同期ファイル読み込みを行った場合、ここで必ず終了判定を行ってください。数分たったから、１時間たったから読まれているはずというのは絶対許されません★★
	/// 非同期読み込み処理の戻り値であるFileLoadAsyncRs型のオブジェクトは内部でリストで管理され、終了判定に使用される。終了したものはリストから削除される）
	/// </summary>
	static skyBool IsLoadEnd();

	/// <summary>
	/// 同期ファイル読み込み
	/// </summary>
	static void Load( const hash::CHashStringPtr &pPath , u32 category = pool::DEFAULT_CATEGORY_NO );

	/// <summary>
	/// ファイルをファイルパスで削除する（もし結合ファイルのなかのどれかのファイルを持ち出している場合、そのファイルも無効になります。この処理は慎重に使ってください）
	/// </summary>
	static void Delete( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// ファイルをカテゴリで削除する
	/// </summary>
	static void Delete( u32 category = pool::DEFAULT_CATEGORY_NO );

	/// <summary>
	/// 管理ファイルを同期読み込みする。（FileControllerは必要に応じて内部でこれを呼ぶ）
	/// 管理ファイルとはバインドされた１個１個のファイルのこと。ユーザーはこのファイルを読みたいはず。
	/// </summary>
	static const u8 *LoadFile( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// 管理ファイルを削除する。実際には削除しない。結合ファイル無いに指定パスのファイルが存在していたかどうかだけ返す（FileControllerは必要に応じて内部でこれを呼ぶ）
	/// </summary>
	static skyBool   DeleteFile( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// 残ファイル情報をストリームにダンプする
	/// </summary>
	static void Print();

private:

	/// <summary>
	/// メンバ情報
	/// </summary>
	static CDefaultFileManagerMember *m_pMember;

};

//インスタンスアクセス文字列
#define CDefaultFileManagerInitialize_() sky::lib::file::CDefaultFileManager::Initialize()
#define CDefaultFileManager_             sky::lib::file::CDefaultFileManager
#define CDefaultFileManagerDispose_()    sky::lib::file::CDefaultFileManager::Dispose()

} } }