
namespace sky { namespace lib {

template< class T , typename LIST_KEY_TYPE >
inline CComposite< T , LIST_KEY_TYPE >::CComposite() : m_IsParentOn( skyFalse )
{
}

template< class T , typename LIST_KEY_TYPE >
inline CComposite< T , LIST_KEY_TYPE >::CComposite( T pParent ) : m_IsParentOn( skyTrue )
{
	m_pParent = pParent;
}

template< class T , typename LIST_KEY_TYPE >
inline CComposite< T , LIST_KEY_TYPE >::~CComposite()
{
}

template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::AttachChild( LIST_KEY_TYPE key , const T &child )
{
	m_ChildList.AddTail( key, child );
	return child;
}
template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::AttachChild( const T &child )
{
	m_ChildList.AddTail( child );
	return child;
}

template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::AttachChildFront( const T &child )
{
	m_ChildList.AddFront( child );
	return child;
}

template< class T , typename LIST_KEY_TYPE >
inline void CComposite< T , LIST_KEY_TYPE >::DetachChild( const T &child )
{
	m_ChildList.Delete( child );
	child->DetachParent();
}

template< class T , typename LIST_KEY_TYPE >
inline void CComposite< T , LIST_KEY_TYPE >::DetachAllChild()
{
/*
    FOREACH( CCompositeList , it , m_ChildList )
	{
		m_ChildList.Delete( ( *it ) );
		( *it )->DetachParent();
    }
*/
	for( u32 i = 0 ; i < m_ChildList.GetNum() ; i++ )
	{
		m_ChildList.GetIdx( i )->DetachParent();
	}
	m_ChildList.DeleteAll();
}

template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::AttachLeaf( LIST_KEY_TYPE key , const T &leaf )
{
	m_LeafList.AddTail( key, leaf );
	return leaf;
}
template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::AttachLeaf( const T &leaf )
{
	m_LeafList.AddTail( leaf );
	return leaf;
}

template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::AttachLeafFront( const T &leaf )
{
	m_LeafList.AddFront( leaf );
	return leaf;
}

template< class T , typename LIST_KEY_TYPE >
inline void CComposite< T , LIST_KEY_TYPE >::DetachLeaf( const T &leaf )
{
	m_LeafList.Delete( leaf );
        leaf->DetachParent();
}

template< class T , typename LIST_KEY_TYPE >
inline void CComposite< T , LIST_KEY_TYPE >::DetachAllLeaf()
{
/*
    FOREACH( CCompositeList , it , m_LeafList )
	{
		m_LeafList.Delete( ( *it ) );
		( *it )->DetachParent();
    }
*/
	for( u32 i = 0 ; i < m_LeafList.GetNum() ; i++ )
	{
		m_LeafList.GetIdx( i )->DetachParent();
	}
	m_LeafList.DeleteAll();
}

template< class T , typename LIST_KEY_TYPE >
T CComposite< T , LIST_KEY_TYPE >::AttachBranch( const T &spBranch )
{
	m_Branch = spBranch;
}

template< class T , typename LIST_KEY_TYPE >
void CComposite< T , LIST_KEY_TYPE >::DetachBranch()
{
	m_Branch.Delete();
}

template< class T , typename LIST_KEY_TYPE >
const T &CComposite< T , LIST_KEY_TYPE >::GetBranch()
{
	return m_Branch;
}

template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::Child( LIST_KEY_TYPE no )
{
	SKY_ASSERT_MSG( m_ChildList.GetNum() > no , _T( "The given number is illegal." ) ); //与えられた番号が不正です。

	T p;

	m_ChildList.GetKey( no , p );

	return p;
}

template< class T , typename LIST_KEY_TYPE >
inline T CComposite< T , LIST_KEY_TYPE >::Parent()
{
	SKY_ASSERT_MSG( m_IsParentOn , _T( "The parent node is not set. " ) ); //親ノードは設定されていません。よって取得できません。

	return m_pParent;
}

template< class T , typename LIST_KEY_TYPE >
inline void CComposite< T , LIST_KEY_TYPE >::AttachParent( T pParent )
{
	m_pParent = pParent;
	m_IsParentOn = skyTrue;
}

template< class T , typename LIST_KEY_TYPE >
inline void CComposite< T , LIST_KEY_TYPE >::DetachParent()
{
	m_IsParentOn = skyFalse;
}

template< class T , typename LIST_KEY_TYPE >
inline skyBool CComposite< T , LIST_KEY_TYPE >::IsRoot()
{
	return !m_IsParentOn;
}

template< class T , typename LIST_KEY_TYPE >
inline skyBool CComposite< T , LIST_KEY_TYPE >::IsChildOn()
{
	return ( m_ChildList.GetNum() > 0 );
}

template< class T , typename LIST_KEY_TYPE >
inline CList< LIST_KEY_TYPE , T > &CComposite< T , LIST_KEY_TYPE >::ChildList()
{
	return m_ChildList;
}

template< class T , typename LIST_KEY_TYPE >
inline CList< LIST_KEY_TYPE , T > &CComposite< T , LIST_KEY_TYPE >::LeafList()
{
	return m_LeafList;
}

} }
