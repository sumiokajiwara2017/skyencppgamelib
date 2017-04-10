#pragma once

namespace sky { namespace lib {

/// <summary>
/// 定数
/// <summary>
namespace dlinklist_const
{
	static const u32			DLINK_LIST_CREATE_MAX_NUM	= 512;					//DLinkListインスタンス作成最大数
};
using namespace dlinklist_const;

/// <summary>
/// 汎用型の双方向リスト
/// 注意）かなり特殊な作りのため、基本アロケーター以外に使用しない。
/// </summary>
template< typename T >
class DLinkList
{

public:

	/// <summary>
	/// 自分の前の値
	/// </summary>
	T			*m_pPrev;

	/// <summary>
	/// 自分の次の値
	/// </summary>
	T			*m_pNext;

	/// <summary>
	/// インスタンス番号
	/// </summary>
	u32			m_InstanceNo;

	/// <summary>
	/// 識別ID
	/// </summary>
	s32			m_Uniqueid;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DLinkList();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DLinkList();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// １レコードとしての初期化
	/// </summary>
	void RecInit();

	/// <summary>
	/// 末期化
	/// </summary>
	void Term();

	/// <summary>
	/// 次に連結
	/// </summary>
	u32 Link( T *pItem );
	u32 LinkItemNext( T *pItem , T *pNewItem );
	u32 LinkItemPrev( T *pItem , T *pNewItem );

	/// <summary>
	/// 連結解除
	/// </summary>
	void UnLink( T *pItem );

	/// <summary>
	/// インデックスで取得
	/// </summary>
	T* IdxFromHead( u32 idx );
	T* IdxFromTail( u32 idx );

	/// <summary>
	/// 識別IDで取得
	/// </summary>
	T* GetUniqueid( s32 uniqueid );

	/// <summary>
	/// 格納数の取得
	/// </summary>
	u32 GetNum() const;

	/// <summary>
	/// デバッグ用整合性チェック
	/// </summary>
	void _CheckNumAndData();

	/// <summary>
	/// 作成インスタンス数
	/// </summary>
	static u32  m_CreateInstanceNum;

	/// <summary>
	/// 使用中フラグ
	/// </summary>
	static skyBool m_UsedFlg[ DLINK_LIST_CREATE_MAX_NUM ]; 

	/// <summary>
	/// 格納数
	/// </summary>
	static u32	m_LinkNum[ DLINK_LIST_CREATE_MAX_NUM ];

	/// <summary>
	/// 双方向リストの最上部（staticなので唯一の存在）
	/// </summary>
	static T	*m_pHead[ DLINK_LIST_CREATE_MAX_NUM ];

	/// <summary>
	/// 双方向リストの最下部（staticなので唯一の存在）
	/// </summary>
	static T	*m_pTail[ DLINK_LIST_CREATE_MAX_NUM ];

};

} }

#include "DLinkList.inl"