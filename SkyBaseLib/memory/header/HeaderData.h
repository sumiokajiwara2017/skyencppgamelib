#pragma once

namespace sky { namespace lib { namespace memory {

//テンプレートTypedef郡
struct CAllocatorDeleteDataHeader;
typedef DLinkList< CAllocatorDeleteDataHeader >	CAllocatorDeleteDataHeaderPtrDList;	//双方向リスト定義

/// <summary>
/// アロケート領域削除リストヘッダー情報
/// </summary>
struct CAllocatorDeleteDataHeader : public CAllocatorDeleteDataHeaderPtrDList{};

//テンプレートTypedef郡
struct CAllocatorDataHeader;
typedef DLinkList< CAllocatorDataHeader >	CAllocatorDataHeaderPtrDList;	//双方向リスト定義

/// <summary>
/// アロケート領域リストヘッダー情報
/// IAllocatorを継承して作成するすべてのアロケーターが確保する領域のヘッダーには必ずこの構造体を付ける
/// そうすることで共通処理を実現する。必須の変数は必ずすべてのアロケーターが適切な値を格納する。
/// 任意の領域は使用したければ使用する。
/// </summary>
struct CAllocatorDataHeader : public CAllocatorDataHeaderPtrDList
{
	/// <summary>
	/// 管理領域サイズ（必須）（ヘッダー・フッターを含まない）
	/// </summary>
	u32                         m_Size;

	/// <summary>
	/// 破壊検知用ID（必須）
	/// </summary>
	u32                         m_CheckBreakId;

	/// <summary>
	/// アロケーターID（必須）
	/// </summary>
	u32                         m_AllocatorId;

	/// <summary>
	/// アロケーターポインタ（必須）
	/// </summary>
	IAllocator*                 m_pAllocator;

	/// <summary>
	/// アロックタイプ（必須）
	/// </summary>
	eAllocType                  m_eAllocType;

	/// <summary>
	/// 結合カウント（任意）
	/// </summary>
	u32                         m_MargeCount;

	/// <summary>
	/// グランドハンドル（任意）
	/// </summary>
	u32                         m_GroundHndl;

	/// <summary>
	/// アリーナハンドルID（任意）
	/// </summary>
	u32				            m_ArenaHndl;

	/// <summary>
	/// アロケート方向ID（任意）
	/// </summary>
	eAllocDirectionType         m_eDirType;

	/// <summary>
	/// アロケートサイズID（任意）
	/// </summary>
	eAllocSizeType              m_eSizeType;

	/// <summary>
	/// 使用中フラグ（任意）
	/// </summary>
	skyBool		                m_IsUsed;

	/// <summary>
	/// スタック番号（任意）
	/// </summary>
	u32                         m_StackNo;

#ifdef SW_SKYLIB_DEBUG_ON
	/// <summary>
	/// アロックファイル名の一部（任意）
	/// </summary>
	skyMString                  m_Path[ DEBUG_FILE_NAME_LENGHT ];
#endif

	/// <summary>
	/// アロックファイル行数（任意）
	/// </summary>
	u32                         m_Line;

	/// <summary>
	/// 削除リスト登録用ヘッダー（これが一番最後にあるのには意味がある）
	/// </summary>
	CAllocatorDataHeaderPtrDList  m_DeleteList;

	/// <summary>
	/// ファイル名の設定
	/// </summary>
	inline void SetFileName( const skyMString *pFname )
	{
		SKY_UNUSED_ARG( pFname );
#ifdef SW_SKYLIB_DEBUG_ON
		//\0を検索
		const char *end = strrchr( pFname , '\0' );

		//処理負荷をさげるために名前の一部を記録
		const char *start = end - ( DEBUG_FILE_NAME_LENGHT - 1 );
		memcpy( m_Path , start , DEBUG_FILE_NAME_LENGHT );
#endif
	}
};

} } }