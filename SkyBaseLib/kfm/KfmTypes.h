#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// 再生終了タイプ
/// </summary>
enum ePlayEndType
{
	ePlayEndType_End = 0    , //再生終了
	ePlayEndType_Repetition , //頭から再生開始（次があっても無視）
	ePlayEndType_Back       , //終了状態から戻る（次があっても無視）
};

/// <summary>
/// 再生状態
/// </summary>
enum ePlayState
{
	ePlayState_Wait = 0 ,
	ePlayState_Start , //開始時、イベントハンドラをコールする時にイベントハンドラの引数にこのステータスを渡す。実際のステートはePlayState_Play。
	ePlayState_Play ,
	ePlayState_Play_Reverse ,
	ePlayState_Stop ,
	ePlayState_End ,   //終了時、イベントハンドラをコールする時にイベントハンドラの引数にこのステータスを渡す。実際のステートはePlayState_Wait。
};

/// <summary>
/// シーケンスハッシュサイズ
/// </summary>
const u32 SEQUENCE_HASH_SIZE = 16;

} } }