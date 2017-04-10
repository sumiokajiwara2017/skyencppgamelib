#pragma once

namespace sky { namespace lib {

/// <summary>
/// 関係性種別
/// </summary>
enum eRelationKind
{
	eRelationKind_Branch = 0 ,
	eRelationKind_Leaf ,
	eRelationKind_Child ,
	eRelationKind_Root ,
};

/// <summary>
/// Compositeパターンのテンプレート
/// このクラスを継承する場合、RTTIを実装してください。下記記述をcpp内に記述してください。
/// SkyImplementRTTI( sky::lib::CComposite< hoge > , sky::lib::base::SkyRefObject );
/// </summary>
template< class T , typename LIST_KEY_TYPE = u32 >
class CComposite : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CComposite();
	inline CComposite( T pParent );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CComposite();

	/// <summary>
	/// 子の関連付け
	/// </summary>
	inline T AttachChild( LIST_KEY_TYPE key, const T &child );

	/// <summary>
	/// 子の関連付け
	/// </summary>
	inline T AttachChild( const T &child );
	inline T AttachChildFront( const T &child );

	/// <summary>
	/// 子の関連付け解除
	/// </summary>
	inline void DetachChild( const T &child );

	/// <summary>
	/// 全ての子の関連付け解除
	/// </summary>
	inline void DetachAllChild();

	/// <summary>
	/// 葉の関連付け
	/// </summary>
	inline T AttachLeaf( LIST_KEY_TYPE key, const T &leaf );

	/// <summary>
	/// 葉の関連付け
	/// </summary>
	inline T AttachLeaf( const T &leaf );
	inline T AttachLeafFront( const T &leaf );

	/// <summary>
	/// 葉の関連付け解除
	/// </summary>
	inline void DetachLeaf( const T &leaf );

	/// <summary>
	/// 全ての葉の関連付け解除
	/// </summary>
	inline void DetachAllLeaf();

	/// <summary>
	/// 枝の関連付け
	/// </summary>
	inline T AttachBranch( const T &spLeaf );

	/// <summary>
	/// 枝の関連付け解除
	/// </summary>
	inline void DetachBranch();

	/// <summary>
	/// 枝の取得
	/// </summary>
	inline const T& GetBranch();

	/// <summary>
	/// 子の取得
	/// </summary>
	inline T Child( LIST_KEY_TYPE no );

	/// <summary>
	/// 親の取得
	/// </summary>
	inline T Parent();

	/// <summary>
	/// 親の関連付け
	/// </summary>
	inline void AttachParent( T pParent );

	/// <summary>
	/// 親の関連付け解除
	/// </summary>
	inline void DetachParent();

	/// <summary>
	/// 最上位かどうか判定する
	/// </summary>
	inline skyBool IsRoot();

	/// <summary>
	/// 子が存在するか確認する
	/// </summary>
	inline skyBool IsChildOn();

	/// <summary>
	/// 子のリストを返す
	/// </summary>
	CList< LIST_KEY_TYPE , T > &ChildList();

	/// <summary>
	/// 葉のリストを返す
	/// </summary>
	CList< LIST_KEY_TYPE , T > &LeafList();

protected:

	/// <summary>
	/// 親の参照
	/// </summary>
	T							m_pParent;
	skyBool						m_IsParentOn;

	/// <summary>
	/// 子の参照リスト
	/// </summary>
	typedef CList< LIST_KEY_TYPE , T > CCompositeList;
	CCompositeList	m_ChildList;

	/// <summary>
	/// 葉の参照リスト
	/// </summary>
	CCompositeList	m_LeafList;

	/// <summary>
	/// 枝
	/// </summary>
	T 	           m_Branch;
};

} }

#include "Composite.inl"