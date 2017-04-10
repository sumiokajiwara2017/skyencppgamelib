#pragma once

namespace sky { namespace lib { namespace memory {

struct CArenaData;

//テンプレートTypedef郡
typedef DLinkList< CArenaData >	ArenaDataPtrDList;	//双方向リスト定義

/// <summary>
/// アリーナデータ
/// </summary>
struct CArenaData : public ArenaDataPtrDList
{

public:

	/// <summary>
	/// メモリ破壊検知用ＩＤ
	/// </summary>
	u32							m_CheckBreakId;

	/// <summary>
	/// アリーナ名
	/// </summary>
	skyWString					m_Name[ ARENA_NAME_MAX ];

	/// <summary>
	/// サイズ
	/// </summary>
	u32							m_Size;

	/// <summary>
	/// 開始アドレス
	/// </summary>
	u32							m_StartAdr;

	/// <summary>
	/// 終了アドレス
	/// </summary>
	u32							m_EndAdr;

	/// <summary>
	/// アリーナバッファ
	/// </summary>
	void						*m_pBuf;

	/// <summary>
	/// 所属するグラウンドハンドル（削除時に使用）
	/// </summary>
	u32							m_GroundHndl;

	/// <summary>
	/// スタックアロケーター
	/// </summary>
	CStackAllocator             m_StackAllocator;

	/// <summary>
	/// アロケーターID
	/// </summary>
	u32                         m_BlockSize;

	/// <summary>
	/// アロックタイプ
	/// </summary>
	eAllocType                  m_eAllocType;

	/// <summary>
	/// メモリ確保
	/// </summary>
	inline void* Malloc( u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line );

	/// <summary>
	/// メモリ再確保
	/// </summary>
	inline void* Realloc( void *ptr , u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line );

	/// <summary>
	/// 削除
	/// </summary>
	inline void	 Delete( CAllocatorDataHeader *pData );

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

#include "ArenaData.inl"