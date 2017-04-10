#pragma once

#ifdef SKYLIB_PLATFORM_WIN32

namespace sky { namespace lib { namespace dos {

//コマンドの長さの最大
static const u32 CMD_LEN_MAX = 1024;

/// <summary>
/// Ｄｏｓコマンドプロンプトをライブラリから実行、制御する
/// もちろんWindowsのみ
/// </summary>
class CDosUtil : public base::SkyObject
{

public:

	/// <summary>
	/// Ｄｏｓコマンドを同期実行し、結果を受け取る
	/// cmdにbatファイルのパスを渡すときは、必ず\\区切りにする/だと認識しない・・
	/// </summary>
	static u32 ExecDosCommand( const skyString *cmd , const skyString *outFilePath = skyNull , skyBool isDisplayWindow = skyTrue , const skyString *currentDir = skyNull );
};

//インスタンスアクセス文字列
#define CDosUtil_			sky::lib::dos::CDosUtil

} } }

#endif