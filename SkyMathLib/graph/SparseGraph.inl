namespace sky { namespace lib { namespace math {

template < class node_type , class edge_type >
inline CSparseGraph< node_type , edge_type >::CSparseGraph( skyBool isDigraph ) : m_NextnodeIndex( 0 ) , m_IsDigraph( isDigraph )
{

}

template < class node_type , class edge_type >
inline u32 CSparseGraph< node_type , edge_type >::GetAllNodeNum() const
{
	return m_NodeVector.GetNum();
}

template < class node_type , class edge_type >
inline u32 CSparseGraph< node_type , edge_type >::GetActiveNodeNum() const
{
	u32 result = 0;
	for ( u32 i = 0 ; i < m_NodeVector.GetNum() ; i++ )
	{
		if ( m_NodeVector[ i ].GetIndex() != GRAPH_INVALID_INDEX_NO )
		{
			result++;
		}
	}
	return result;
}

template < class node_type , class edge_type >
inline graphIndex CSparseGraph< node_type , edge_type >::AddNode( const NodeType &node )
{
	if ( node.GetIndex() < ( graphIndex )m_NodeVector.GetNum() )
	{
		SKY_ASSERT( m_NodeVector[ node.GetIndex() ].GetIndex() == GRAPH_INVALID_INDEX_NO );
		m_NodeVector[ node.GetIndex() ] = node;
		return m_NextnodeIndex;
	}
  
	else
	{
		SKY_ASSERT( node.GetIndex() == m_NextnodeIndex );
		m_NodeVector.Add( node );
		m_EdgyListVector.Add( NEW EdgeTypeList() );
		return m_NextnodeIndex++;
	}
}

template < class node_type , class edge_type >
inline const typename CSparseGraph< node_type , edge_type >::NodeType& CSparseGraph< node_type , edge_type >::GetNode( graphIndex index ) const
{
	SKY_ASSERT( ( index < ( graphIndex )m_NodeVector.GetNum() ) && index >= 0 );

	return m_NodeVector[ index ];
}

template < class node_type , class edge_type >
inline typename CSparseGraph< node_type , edge_type >::NodeType& CSparseGraph< node_type , edge_type >::GetNode( graphIndex index )
{
	SKY_ASSERT( ( index < ( graphIndex )m_NodeVector.GetNum() ) && index >= 0 );

	return m_NodeVector[ index ];
}

template < class node_type , class edge_type >
inline void  CSparseGraph< node_type , edge_type >::CullInvalidEdges()
{
	FOREACH( EdgeTypeListPtrVector , it1 , m_EdgyListVector )
	{
		FOREACH_NOINC( EdgeTypeList , it2 , *( *it1 ) )
		{
			if ( m_NodeVector[ ( *it2 ).GetToIndex() ].GetIndex() == GRAPH_INVALID_INDEX_NO ||
				 m_NodeVector[ ( *it2 ).GetFromIndex() ].GetIndex() == GRAPH_INVALID_INDEX_NO )
			{
				it2 = ( *it1 )->Erases( it2 ); //注意
			}
			else
			{
				it2++;
			}
		}
	}
}

template < class node_type , class edge_type >
void CSparseGraph< node_type , edge_type >::RemoveNode( graphIndex node )                                   
{
	SKY_ASSERT( node < ( graphIndex )m_NodeVector.GetNum() );

	m_NodeVector[ node ].SetIndex( GRAPH_INVALID_INDEX_NO );

	if ( m_IsDigraph == skyFalse )
	{
		FOREACH( EdgeTypeList , it1 , *m_EdgyListVector[ node ] )
		{
			FOREACH_NOINC( EdgeTypeList , it2 , *m_EdgyListVector[ ( *it1 ).GetToIndex() ] )
			{
				if ( ( *it2 ).GetToIndex() == node )
				{
					it2 = m_EdgyListVector[ ( *it1 ).GetToIndex() ]->Erases( it2 ); //注意
					break;
				}
				else
				{
					it2++;
				}
			}
		}
	    m_EdgyListVector[ node ]->DeleteAll();
    }
	else
	{
		CullInvalidEdges();
	}
}

template < class node_type , class edge_type >
inline void CSparseGraph< node_type , edge_type >::AddEdge( EdgeType edge )
{
	SKY_ASSERT( ( edge.GetFromIndex() < ( graphIndex )m_NodeVector.GetNum() ) && ( edge.GetToIndex() < ( graphIndex )m_NodeVector.GetNum() ) );

	if ( m_NodeVector[ edge.GetFromIndex() ].GetIndex() != GRAPH_INVALID_INDEX_NO &&
         m_NodeVector[ edge.GetToIndex() ].GetIndex() != GRAPH_INVALID_INDEX_NO )
	{
		//まだこの経路が無かったら追加する
		if( IsUniqueEdge( edge.GetFromIndex() , edge.GetToIndex() ) == skyTrue )
		{
			m_EdgyListVector[ edge.GetFromIndex() ]->AddTail( edge );
		}

		if ( m_IsDigraph == skyFalse )
		{
			if( IsUniqueEdge( edge.GetToIndex() , edge.GetFromIndex() ) == skyTrue )
			{
				EdgeType newEdge = edge;

				newEdge.SetToIndex( edge.GetFromIndex() );
				newEdge.SetFromIndex( edge.GetToIndex() );

				m_EdgyListVector[ edge.GetToIndex() ]->AddTail( newEdge );
			}
		}
	}
}

template < class node_type , class edge_type >
inline const typename CSparseGraph< node_type , edge_type >::EdgeType& CSparseGraph< node_type , edge_type >::GetEdge( graphIndex fromIndex , graphIndex toIndex ) const
{
	SKY_ASSERT( ( fromIndex < ( graphIndex )m_NodeVector.GetNum() ) && fromIndex >= 0 );
	SKY_ASSERT( ( toIndex   < ( graphIndex )m_NodeVector.GetNum() ) && toIndex >= 0 );

	EdgeType *pResult = skyNull;

	FOREACH( EdgeTypeList , it , *m_EdgyListVector[ fromIndex ] )
	{
		if ( ( *it ).GetToIndex() == toIndex )
		{
			pResult = &( *it );
			break;
		}
	}

	SKY_ASSERT( pResult != skyNull );

	return *pResult;
}

template < class node_type , class edge_type >
inline const typename CSparseGraph< node_type , edge_type >::EdgeTypeList& CSparseGraph< node_type , edge_type >::GetEdgeList( graphIndex index ) const
{
	return *m_EdgyListVector[ index ];
}

template < class node_type , class edge_type >
inline typename CSparseGraph< node_type , edge_type >::EdgeType& CSparseGraph< node_type , edge_type >::GetEdge( graphIndex fromIndex , graphIndex toIndex )
{
	SKY_ASSERT( ( fromIndex < ( graphIndex )m_NodeVector.GetNum() ) && fromIndex >= 0 );
	SKY_ASSERT( ( toIndex   < ( graphIndex )m_NodeVector.GetNum() ) && toIndex >= 0 );

	EdgeType *pResult = skyNull;

	FOREACH( EdgeTypeList , it , *m_EdgyListVector[ fromIndex ] )
	{
		if ( ( *it ).GetToIndex() == toIndex )
		{
			pResult = &( *it );
			break;
		}
	}

	SKY_ASSERT( pResult != skyNull );

	return *pResult;
}

template < class node_type , class edge_type >
inline void CSparseGraph< node_type , edge_type >::RemoveEdge( graphIndex fromIndex , graphIndex toIndex )
{
	SKY_ASSERT( ( fromIndex < ( graphIndex )m_NodeVector.GetNum() ) && ( toIndex < ( graphIndex )m_NodeVector.GetNum() ) );

	if ( m_IsDigraph == skyFalse )
	{
		FOREACH( EdgeTypeList , it , *m_EdgyListVector[ toIndex ] )
		{
			if ( ( *it ).GetToIndex() == fromIndex )
			{
				it = m_EdgyListVector[ toIndex ]->Erase( it );
				break;
			}
		}
	}

	FOREACH( EdgeTypeList , it , *m_EdgyListVector[ fromIndex ] )
	{
		if ( ( *it ).GetToIndex() == toIndex )
		{
			it = m_EdgyListVector[ fromIndex ]->Erase( it );
			break;
		}
	}
}

template < class node_type , class edge_type >
inline void CSparseGraph< node_type , edge_type >::SetEdgeCost( graphIndex fromIndex , graphIndex toIndex , dectype newCost )
{
	SKY_ASSERT( ( fromIndex < ( graphIndex )m_NodeVector.GetNum() ) && ( toIndex < ( graphIndex )m_NodeVector.GetNum() ) );

	FOREACH( EdgeTypeList , it , *m_EdgyListVector[ fromIndex ] )
	{
		if ( ( *it ).GetToIndex() == toIndex )
		{
		  ( *it ).SetCost( newCost );
		  break;
		}
	}
}

template < class node_type , class edge_type >
skyBool CSparseGraph< node_type , edge_type >::IsUniqueEdge( graphIndex fromIndex , graphIndex toIndex ) const
{
	SKY_ASSERT( ( fromIndex < ( graphIndex )m_NodeVector.GetNum() ) && ( toIndex < ( graphIndex )m_NodeVector.GetNum() ) );

	skyBool result = skyTrue;

	FOREACH( EdgeTypeList , it , *m_EdgyListVector[ fromIndex ] )
	{
		if ( ( *it ).GetToIndex() == toIndex )
		{
			result = skyFalse;
			break;
		}
	}

	return result;
}

template < class node_type , class edge_type >
inline graphIndex CSparseGraph< node_type , edge_type >::GetNextFreeNodeIndex() const
{
	return m_NextnodeIndex;
}

template < class node_type , class edge_type >
inline skyBool CSparseGraph< node_type , edge_type >::IsDigraph() const
{
	return m_IsDigraph;
}

template < class node_type , class edge_type >
inline skyBool CSparseGraph< node_type , edge_type >::IsEmpty() const
{
	return ( m_NodeVector.GetNum() == 0 );
}

template < class node_type , class edge_type >
inline u32 CSparseGraph< node_type , edge_type >::GetEdgeNum() const
{
	u32 result = 0;
	for( u32 i = 0 ; i < m_EdgyListVector.GetNum() ; i++ )
	{
		result += m_EdgyListVector[ i ]->GetNum();
	}
	return result;
}

template < class node_type , class edge_type >
inline void CSparseGraph< node_type , edge_type >::RemoveAllEdge()
{
	//エッジの全削除
	FOREACH( EdgeTypeListPtrVector , it , m_EdgyListVector )
	{
		( *it ).DeleteAll();
	}
}

template < class node_type , class edge_type >
inline void CSparseGraph< node_type , edge_type >::Clear()
{
	m_NextnodeIndex = 0;

	//ノードの全削除
	m_NodeVector.Reset();

	//エッジの全削除
	FOREACH( EdgeTypeListPtrVector , it , m_EdgyListVector )
	{
		( *it )->DeleteAll();
		DEL *it;
	}
	m_EdgyListVector.Reset();
}

template < class node_type , class edge_type >
inline skyBool CSparseGraph< node_type , edge_type >::IsNodePresent( graphIndex index ) const
{
	if ( ( index >= ( graphIndex )m_NodeVector.GetNum() || ( m_NodeVector[ index ].GetIndex() == GRAPH_INVALID_INDEX_NO ) ) )
    {
		return skyFalse;
    }
    else
	{
		return skyTrue;
	}
}

template < class node_type , class edge_type >
inline skyBool CSparseGraph< node_type , edge_type >::IsEdgePresent( graphIndex fromIndex , graphIndex toIndex ) const
{
    if ( IsNodePresent( fromIndex ) && IsNodePresent( toIndex ) )
    {
		FOREACH( EdgeTypeList , it , *m_EdgyListVector[ fromIndex ] )
		{
			if ( ( *it ).GetToIndex() == toIndex )
			{
				return skyTrue;
			}
		}

        return skyFalse;
    }
    else
	{
		return skyFalse;
	}
}

} } }