#pragma once

#ifdef SW_SKYLIB_DEBUG_ON

//namespaceで囲まれたクラスの先行宣言
namespace sky { namespace lib { namespace thread { class CriticalSection; } } }

namespace sky { namespace lib { namespace debuglogic {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// デフォルトログカテゴリ
/// </summary>
static const u32 LOG_CATEGORY_ALL = 1 << 0;
static const u32 LOG_CATEGORY_LIB = 1 << 1;
//32bit中2bit上記定数で使っているので、あと30種類のフラグが作れます。各アプリケーションで好きにルール付けをして使ってください。

/// <summary>
/// ログレベル
/// </summary>
enum eLogLevel
{
	eLogLevel_Error = 0 ,
	eLogLevel_Warning ,
	eLogLevel_Dump 
};

/// <summary>
/// デバッグ処理群
/// 初期化（Initialize）は必ずメインスレッドで行ってください。
/// </summary>
class CDebugUtil : public base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放
	/// </summary>
	static void Dispose();

	/// <summary>
	/// このメソッドを呼び出したスレッドがメインスレッドかどうか判定する
	/// </summary>
	static skyBool IsMainThread();

	/// <summary>
	/// 出力するログレベルを設定する
	/// </summary>
	static void SetLogLevel( eLogLevel eLevel );

	/// <summary>
	/// 出力するログカテゴリーを設定する（categoryはビットフラグとして扱ってます。理解した上で使ってください。）
	/// </summary>
	static void SetLogCategory( u32 category );

	/// <summary>
	/// 出力するログカテゴリーを設定を解除する
	/// </summary>
	static void DeleteLogCategory( u32 category );

	/// <summary>
	/// ファイルストリームを設定する
	/// </summary>
	static void OpenFileStream( const skyString *path );
	static void CloseFileStream();
	static void WriteFileStream( const skyString *format , ... );

	/// <summary>
	/// メモリストリームを設定する(引数はログが出力されているメモリのアドレスが格納されている変数のアドレスが入ります。）
	/// </summary>
	static void OpenMemoryStream( const void **symbolPointAdr );
	static void CloseMemoryStream();
	static void WriteMemoryStream( const skyString *format , ... );

	/// <summary>
	/// ログを出力する（出力先は設定されたストリーム全て）
	/// </summary>
	static void Printf( eLogLevel eLevel , u32 category , const skyWString *format , ... );
	static void Printf( eLogLevel eLevel , u32 category , const skyMString *format , ... );

	/// <summary>
	/// 任意の文字列とスタックトレースを
	/// </summary>
	static void PrintfStackTrace( const skyString *format , ... );

	/// <summary>
	/// 引数の描画
	/// </summary>
	static void PrintParam( s32 argc , skyString *argv[] );

	/// <summary>
	/// 停止
	/// </summary>
	static void Panic( const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyWString *format , ... );
	static void Panic( const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyMString *format , ... );

	/// <summary>
	/// アサート
	/// skyTrueなら止まらない。skyFalseなら止まる。時々どっちか忘れる！
	/// </summary>
	static void Assert( skyBool isSuccess , const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyWString *format , ... );
	static void Assert( skyBool isSuccess , const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyMString *format , ... );

	/// <summary>
	/// 強制停止
	/// </summary>
	static void Exit(  s32 status );

	/// <summary>
	/// メモリダンプ
	/// </summary>
	static void MemDump( u8 *adr , u32 before , u32 after , u32 lineCharNum );

	/// <summary>
	/// 指定の数の半角空白をプリントする
	/// </summary>
	static void PrintIndent( u32 indentNum );

private:

	/// <summary>
	/// クリティカルセクション
	/// </summary>
	static thread::CriticalSection *m_pCs;

	/// <summary>
	/// ログレベル
	/// </summary>
	static eLogLevel m_eLogLevel;

	/// <summary>
	/// ログカテゴリ
	/// </summary>
	static u32 m_nLogCategory;

	//各機能封印
	CDebugUtil				();										//コンストラクタ
	CDebugUtil				( const CDebugUtil& ){};				//コピーコンストラクタ
	virtual ~CDebugUtil	();											//デストラクタ
	CDebugUtil &operator=	(const CDebugUtil& ){ return *this; };	//代入演算子オーバーロード
};

} } }

#endif

#ifdef SW_SKYLIB_DEBUG_ON
#define CDebugUtilInit_()											sky::lib::debuglogic::CDebugUtil::Initialize()
#define CDebugUtil_													sky::lib::debuglogic::CDebugUtil
#define CDebugUtilDispose_()										sky::lib::debuglogic::CDebugUtil::Dispose()
#define SKY_SET_LOG_LEVEL( eLevel )									CDebugUtil_::SetLogLevel( eLevel );
#define SKY_SET_LOG_CATEGORY( category )							CDebugUtil_::SetLogCategory( category );
#define SKY_DEL_LOG_CATEGORY( category )							CDebugUtil_::DeleteLogCategory( category );
#define SKY_OPEN_FILE_STREAM( path )								CDebugUtil_::OpenFileStream( path )
#define SKY_CLOSE_FILE_STREAM()										CDebugUtil_::CloseFileStream()
#define SKY_WRITE_FILE_STREAM( format, ... )						CDebugUtil_::WriteFileStream( format, ##__VA_ARGS__ )
#define SKY_OPEN_MEM_STREAM( symbolpa )								CDebugUtil_::OpenMemoryStream( symbolpa )
#define SKY_CLOSE_MEM_STREAM()										CDebugUtil_::CloseMemoryStream()
#define SKY_WRITE_MEM_STREAM( format, ... )							CDebugUtil_::WriteMemoryStream( format, ##__VA_ARGS__ )
#define SKY_PRINTF( format, ... )									CDebugUtil_::Printf( sky::lib::debuglogic::eLogLevel_Error , sky::lib::debuglogic::LOG_CATEGORY_ALL , format, ##__VA_ARGS__ )
#define SKY_PRINTF_CATEGORY( loglevel , logcategory , format, ... )	CDebugUtil_::Printf( loglevel , logcategory , format, ##__VA_ARGS__ )
#define SKY_ASSERT( isSuccess )										CDebugUtil_::Assert( isSuccess , T__FILE__, T__FUNCTION__, __LINE__, _T( "Assert!! \n" ) )
#define SKY_ASSERT_MSG(isSuccess , format, ... )					CDebugUtil_::Assert( isSuccess , T__FILE__, T__FUNCTION__, __LINE__, format, ##__VA_ARGS__ )
#define SKY_ASSERT_MAIN_THREAD()									CDebugUtil_::Assert( CDebugUtil_::IsMainThread() , T__FILE__, T__FUNCTION__, __LINE__, format, _T( "このスレッドはメインスレッドではありません。この処理はメインスレッドで行ってください。 \n" ) )
#define SKY_PANIC()													CDebugUtil_::Panic( T__FILE__, T__FUNCTION__, __LINE__, _T( "Panic!! \n" ) )
#define SKY_PANIC_MSG( format, ... )								CDebugUtil_::Panic( T__FILE__, T__FUNCTION__, __LINE__, format, ##__VA_ARGS__ )
#define SKY_PRINT_STACK_TRACE_MSG( format, ... )					CDebugUtil_::PrintfStackTrace( format, ##__VA_ARGS__ )
#define SKY_MEM_DUMP( adr, before, after, lineCharNum )				CDebugUtil_::MemDump( adr , before , after , lineCharNum )
#define SKY_PRINT_INDENT( num )										CDebugUtil_::PrintIndent( num )
#else
#define CDebugUtilInit_()
#define CDebugUtil_
#define CDebugUtilDispose_()
#define SKY_SET_LOG_LEVEL( eLevel )									
#define SKY_SET_LOG_CATEGORY( category )							
#define SKY_DEL_LOG_CATEGORY( category )							
#define SKY_OPEN_FILE_STREAM( path )					
#define SKY_CLOSE_FILE_STREAM()						
#define SKY_WRITE_FILE_STREAM( format, ... )		
#define SKY_OPEN_MEM_STREAM( symbolpa )				
#define SKY_CLOSE_MEM_STREAM()						
#define SKY_WRITE_MEM_STREAM( format, ... )		
#define SKY_PRINTF( format, ... )					
#define SKY_PRINTF_CATEGORY( logcategory , format, ... )
#define SKY_ASSERT( isSuccess )						
#define SKY_ASSERT_MSG(isSuccess , format, ... )    
#define SKY_ASSERT_MAIN_THREAD()
#define SKY_PANIC()									
#define SKY_PANIC_MSG( format, ... )				
#define SKY_PRINT_STACK_TRACE_MSG( format, ... )				
#define SKY_MEM_DUMP( adr, before, after, lineCharNum )
#define SKY_PRINT_INDENT( num )
#endif
