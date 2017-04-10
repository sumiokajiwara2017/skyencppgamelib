#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// インデックスプライオリティキュー
/// </summary>
template< class KeyType >
class CIndexedPriorityQLow
{

public:
  
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CIndexedPriorityQLow( CVectorPrimitive< KeyType >& keys , s32 MaxSize );
	CIndexedPriorityQLow& operator=( const CIndexedPriorityQLow& ); //警告封じのため定義。実際このクラスを代入でまわしたりはできない。

	/// <summary>
	/// 中身は空か？
	/// </summary>
    skyBool IsEmpty() const;

	/// <summary>
	/// 値の挿入
	/// </summary>
    void Insert( const s32 idx );

	/// <summary>
	/// 値の削除
	/// </summary>
    s32 Pop();

	/// <summary>
	/// 優先順位の変更
	/// </summary>
    void ChangePriority( const s32 idx );

private:

	/// <summary>
	/// メンバ変数
	/// </summary>
    CVectorPrimitive< KeyType >& m_vecKeys;
    CVectorPrimitive< s32 >      m_Heap;
    CVectorPrimitive< s32 >      m_invHeap;
    s32                 m_iSize;
    s32                 m_iMaxSize;

	/// <summary>
	/// 値の交換
	/// </summary>
    void Swap( s32 a, s32 b );
    void ReorderUpwards( s32 nd );
    void ReorderDownwards( s32 nd , s32 HeapSize );
};

} } }

#include "IndexedPriorityQLow.inl"