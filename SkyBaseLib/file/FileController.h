#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// ファイル操作クラス
/// ファイル操作クラスはファイルに対する問い合わせに全て答える。
/// アプリケーションは、FileControllerに関してはLoadしか行わない。
/// アプリケーションは他に、BindFileManager、PoolFileManagerに対して
/// 事前にファイルの読み込みを依頼することができる。それをしておくことで、
/// FileControllerのLoadのレスポンスが高速になる。
/// アプリケーション側に無計画にファイルロードの処理負荷分散の処理を個々に入れるのは
/// 非効率でかつバグの温床になる。絶対に禁止である。アプリケーション側の制御処理が
/// アプリケーションの都合に合わせたタイミングでBindFileManager、PoolFileManager
/// に対して非同期のロード要求をだし、アプリケーションの制御以外の処理はCDiscFileController
/// のLoad処理を同期で読むべき。
///
/// +------------------+
/// |                  |
/// |  FileController  |－－－－－－＋－－－－－－－－－－－＋－－－－－－－－－－－－＋
/// |                  |　　　　　　｜　　　　　　　　　　　｜　　　　　　　　　　　  ｜
/// +------------------+　　　　　　｜                     ｜                      ｜
///         ｜①（高速※1）　　　　  ｜②（高速※1）         ｜③（低速※2）         ｜④（高速※1）
///         ▽　　　　　　　　　　   ▽					   ▽　　　　　　　　　　　▽
/// +------------------+    +------------------+　　+------------------+    +------------------+
/// |                  |	|                  |	|                  |    |                  |
/// |     BindFile     |	|     PoolFile     |	|    DiscFile      |    |    DefaultFile　 |
/// |     Manager      |	|     Manager      |	|                  |    |    Manager       |
/// |                  |	|                  |	|                  |    |                  |
/// +------------------+	+------------------+	+------------------+    +------------------+
///
/// ※1 事前ロードしておけば高速なレスポンスが得られる。
/// ※2 HDDやDVD等に必ずアクセスするので、絶対に遅い。本番では③と④に来ることは基本的に無い作りになるはず。
/// </summary>
class CDiscFileController : public base::SkyObject
{
    SkyTypeDefRTTI;

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
	/// 読み込み(アプリケーションはこれをただ呼ぶ。ポインタの中身がなければ(IsEmpty()がtrue)ファイルがどこにも存在しないことになる）
	/// 実務ではファイルの読み込みタイミングと使用タイミングが全く違うということがほとんどなので、ここでいう読み込みとは多くの場合、
	/// すでにメモリ上にあるファイルを受け取るという作業になることが望ましい。実際の読み込みは、BindFileManager、FilePoolContena、
	/// DefaultFileManagerに対して命令を出す。
	/// </summary>
	static const u8 *Load( const hash::CHashStringPtr &filePath );

	/// <summary>
	/// 削除
	/// </summary>
	static skyBool Delete( const hash::CHashStringPtr &filePath );

};

//インスタンスアクセス文字列
#define CDiscFileControllerInitialize_()     sky::lib::file::CDiscFileController::Initialize()
#define CDiscFileController_                 sky::lib::file::CDiscFileController
#define CDiscFileControllerDispose_()        sky::lib::file::CDiscFileController::Dispose()


} } }
