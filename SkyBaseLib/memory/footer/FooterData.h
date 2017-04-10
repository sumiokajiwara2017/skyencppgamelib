#pragma once

namespace sky { namespace lib { namespace memory {

/// <summary>
/// アロケート領域リストフッター情報
/// IAllocatorを継承して作成するすべてのアロケーターが確保する領域のフッターには必ずこの構造体を付ける
/// そうすることで共通処理を実現する
/// </summary>
struct CAllocatorDataFooter
{
	/// <summary>
	/// メモリ破壊検知用ＩＤ
	/// </summary>
	u32          m_CheckBreakId;
};

} } }
