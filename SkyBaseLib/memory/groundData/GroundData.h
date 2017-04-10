#pragma once

namespace sky { namespace lib { namespace memory {

struct CGroundData;

//テンプレートTypedef郡
typedef DLinkList< CGroundData >					GroundDataPtrDList;	//双方向リスト定義

/// <summary>
/// グラウンドデータ
/// </summary>
struct CGroundData : public GroundDataPtrDList
{

public:

	/// <summary>
	/// メモリ破壊検知用ＩＤ
	/// </summary>
	u32						m_CheckBreakId;

	/// <summary>
	/// サイズ
	/// </summary>
	u32						m_Size;

	/// <summary>
	/// 開始アドレス
	/// </summary>
	u32						m_StartAdr;

	/// <summary>
	/// 終了アドレス
	/// </summary>
	u32						m_EndAdr;

	/// <summary>
	/// フロント使用済みサイズ
	/// </summary>
	u32						m_UsedSize;

	/// <summary>
	/// グラウンドバッファ
	/// </summary>
	void                   *m_pBuf;

	/// <summary>
	/// 削除リスト格納サイズ
	/// </summary>
	u32                     m_DeleteListAddSize;

	/// <summary>
	/// アリーナデータリスト(双方向リストなのでその型自体がルートになる）
	/// </summary>
	ArenaDataPtrDList      m_ArenaList;

	/// <summary>
	/// スタックアロケーター
	/// </summary>
	CStackAllocator         m_StackAllocator;

	/// <summary>
	/// アリーナの識別ID
	/// </summary>
	s32                     m_ArenaUniqueid;

	/// <summary>
	/// アロケーターID
	/// </summary>
	u32                     m_AllocatorId;

	/// <summary>
	/// アロケーターID
	/// </summary>
	u32                     m_BlockSize;

	/// <summary>
	/// アロックタイプ
	/// </summary>
	eAllocType             m_eAllocType;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGroundData();

	/// <summary>
	/// アリーナの作成
	/// </summary>
	inline u32	CreateArena( u32 size , const skyWString *pArenaName );

	/// <summary>
	/// アリーナの削除
	/// </summary>
	inline void DeleteArena( u32 arenaHndl );

	/// <summary>
	/// メモリ状況をプリントする
	/// </summary>
	inline void PrintMemory();

	/// <summary>
	/// メモリ破壊のチェック
	/// </summary>
	inline void CheckMemBreak();
};

} } }

#include "GroundData.inl"