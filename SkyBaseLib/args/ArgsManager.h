#pragma once

namespace sky { namespace lib { namespace args {

/// <summary>
/// 定数
/// </summary>
static const u32 OPTION_VALUE_HASH_SIZE = 31;
static const u32 ARGS_INFO_HASH_SIZE = 31;
	
/// <summary>
/// 引数の種類
/// </summary>
enum eOptionType
{
    eOptionType_Switch = 0, //On/Offの判定に使用する
    eOptionType_OneValue,   //引数が単数
    eOptionType_ListValue,  //引数が複数
};

/// <summary>
/// 引数管理情報
/// </summary>
class ArgsInfo : public base::SkyObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ArgsInfo()
	{
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ArgsInfo( eOptionType eType, const hash::CHashStringPtr &option, const hash::CHashStringPtr &info, skyBool isRequired , skyBool isEnvReplaceExec )
	{
		m_eType = eType;
		m_Option = option;
		m_Info = info;
		m_IsRequired = isRequired;
		m_IsEnvReplaceExec = isEnvReplaceExec;
	}

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	ArgsInfo		( const ArgsInfo& src )
	{
		m_eType = src.m_eType;
		m_Option = src.m_Option;
		m_Info = src.m_Info;
		m_IsRequired = src.m_IsRequired;
		m_IsEnvReplaceExec = src.m_IsEnvReplaceExec;
	}

	/// <summary>
	/// メンバ変数
	/// </summary>
	eOptionType				m_eType;
	hash::CHashStringPtr	m_Option;
	hash::CHashStringPtr	m_Info;
	skyBool					m_IsRequired;
	skyBool					m_IsEnvReplaceExec;

};

/// <summary>
/// 引数管理
/// ただの文字列の羅列の引数にも、-optionで値を任意の個数していして受け取る引数にも対応している。
/// 
/// 【ミニコラム 設定ファイルよりも引数が優れている点】
/// もし作成したアプリケーションが、複数の場所で違う呼ばれ方をした場合、設定ファイルの場合その箇所毎に
/// 設定ファイルを用意しないといけない。引数の場合は呼び出し処理に記述を加えるだけでＯＫ。
/// </summary>
class CArgsManager : public base::SkyRefObject
{

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
	CArgsManager();

	//シングルトン機能を付属
	friend class Singleton< CArgsManager >;
    static Singleton< CArgsManager > SingletonMember;

    /// <summary>
    /// 引数格納HashTable
    /// </summary>
    hash::CHashStringPtrList			m_List;
    hash::CHashStringPtrListPtrHash		m_OptionValueHash;
	typedef CHash< ArgsInfo >			ArgsInfoHash;
    ArgsInfoHash						m_ArgsInfoHash;

    /// <summary>
    /// 引数情報設定
    /// </summary>
    void AddArgsInfo( const ArgsInfo &argsInfo );

    /// <summary>
    /// 引数情報をコンソールに出力する
    /// </summary>
    void WriteConsoleArgsInfo();

    /// <summary>
    /// 引数解析
    /// </summary>
    void Parse( int argc , skyString * argv[] );

    /// <summary>
    /// 引数を追加
    /// </summary>
    void Add( const hash::CHashStringPtr &arg );

    /// <summary>
    /// 引数の存在チェック
    /// </summary>
    skyBool IsArgsOn( u32 index );

    /// <summary>
    /// 引数を文字として取得
    /// </summary>
    const hash::CHashStringPtr &GetString( s32 index );

    /// <summary>
    /// 引数をs32として取得
    /// </summary>
    s32 GetS32( s32 index );

    /// <summary>
    /// オプション存在確認
    /// </summary>
    skyBool IsOptionOn( const hash::CHashStringPtr &optionName);

    /// <summary>
    /// 指定のオプションの必須フラグを操作する
    /// </summary>
    void SetReqFlg( const hash::CHashStringPtr &optionName , skyBool isRequired );

    /// <summary>
    /// オプションをキーで様々な形式で取得
    /// </summary>
    const hash::CHashStringPtr &GetOptionValue( const hash::CHashStringPtr &optionName );
    s32 GetOptionValue_s32( const hash::CHashStringPtr &optionName);
    u32 GetOptionValue_u32( const hash::CHashStringPtr &optionName);
    d64 GetOptionValue_d64( const hash::CHashStringPtr &optionName );

    /// <summary>
    /// オプションをキーで文字リストとして取得
    /// </summary>
    hash::CHashStringPtrListPtr &GetOptionValues( const hash::CHashStringPtr &optionName);

    /// <summary>
    /// 必須チェック
    /// </summary>
    void AssertCheckReqOption();
};

} } }

//インスタンスアクセス文字列
#define CArgsManagerCreate_()			sky::lib::args::CArgsManager::SingletonMember.CreateInstance()
#define CArgsManager_			sky::lib::args::CArgsManager::SingletonMember.GetInstance()
#define CArgsManagerDelete_()			sky::lib::args::CArgsManager::SingletonMember.DeleteInstance()
