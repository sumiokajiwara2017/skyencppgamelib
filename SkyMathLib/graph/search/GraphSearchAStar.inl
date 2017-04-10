namespace sky { namespace lib { namespace math {

template < class graph_type , class heuristic >
inline CGraphSearchAStar< graph_type , heuristic >::~CGraphSearchAStar()
{

}

template < class graph_type , class heuristic >
inline CGraphSearchAStar< graph_type , heuristic >::CGraphSearchAStar( const graph_type& graph ,
		                     graphIndex        sourceIndex ,
						     graphIndex        targetIndex ) :
                             m_Graph( graph ) , 
						     m_SourceIndex( sourceIndex ) , 
							 m_TargetIndex( targetIndex )
{

	m_ShortestPathTree.Assign( graph.GetAllNodeNum() , skyNull );
	m_GCosts.Assign( graph.GetAllNodeNum() , 0.0f );
	m_FCosts.Assign( graph.GetAllNodeNum() , 0.0f );
	m_SearchFrontier.Assign( graph.GetAllNodeNum() , skyNull );

	Search();
}

template < class graph_type , class heuristic >
inline typename CGraphSearchAStar< graph_type , heuristic >::EdgePtrVector &CGraphSearchAStar< graph_type , heuristic >::GetSPT( EdgePtrVector &vector ) const
{
	for( u32 i = 0 ; m_ShortestPathTree.GetNum() ; i++ )
	{
		vector[ i ] = m_ShortestPathTree[ i ];
	}

	return vector;
}

template < class graph_type , class heuristic >
inline GraphIndexList &CGraphSearchAStar< graph_type , heuristic >::GetPathToTarget( GraphIndexList &dest ) const
{
	//ターゲットがそもそも設定されていなかったら何もせず返す
	if ( m_TargetIndex == GRAPH_INVALID_INDEX_NO )  return dest;

	s32 nd = m_TargetIndex;

	dest.AddFront( nd );

	while ( ( nd != m_SourceIndex ) && ( m_ShortestPathTree[ nd ] != 0 ) )
	{
		nd = m_ShortestPathTree[ nd ]->GetFromIndex();
		dest.AddFront( nd );
	}

	return dest;
}

template < class graph_type , class heuristic >
dectype CGraphSearchAStar< graph_type , heuristic >::GetCostToTarget() const
{
	return m_GCosts[ m_TargetIndex ];
}

template < class graph_type , class heuristic >
inline void CGraphSearchAStar< graph_type , heuristic >::Search()
{
	//ノードのインデックス付きプライオリティキューを作成する
	//このキューは低いFコストをノードに与える（Ｆ＝Ｇ＋Ｈ）
	CIndexedPriorityQLow< dectype > queue( m_FCosts , m_Graph.GetAllNodeNum() );

	//ソースノードをキューに置く
	queue.Insert( m_SourceIndex );

	//キューが空でない間
	while( queue.IsEmpty() == skyFalse )
	{
		//キューから最もコストの低いノードを得る
		s32 nextClosestNode = queue.Pop();

		//このエッジを末探索領域から最短経路ツリーに移動する
		m_ShortestPathTree[ nextClosestNode ] = m_SearchFrontier[ nextClosestNode ];

		//見つかったターゲットが存在する場合
		if( nextClosestNode == m_TargetIndex )
		{
			break;
		}

		//ここでエッジを緩和する
		//次に最も近いノードに接続されている各エッジに関して
		const graph_type::EdgeTypeList& edgeList = m_Graph.GetEdgeList( nextClosestNode );

		for ( u32 i = 0 ; i < edgeList.GetNum() ; i++ )
		{
			const Edge *pEdge = &edgeList[ i ];

			// このノードからターゲットまでのヒューリスティックなコストを計算する(H)
			dectype hCost = heuristic::Calculate( m_Graph , m_TargetIndex , pEdge->GetToIndex() );

			//このノードからソースまでの「実際」残すとを計算する（G）
			dectype gCost = m_GCosts[ nextClosestNode ] + pEdge->GetCost();

			//このノードが末探索領域に追加されたことがなければ、追加しGとFコストを更新する
			if ( m_SearchFrontier[ pEdge->GetToIndex() ] == skyNull )
			{
				m_FCosts[ pEdge->GetToIndex() ] = gCost + hCost;
				m_GCosts[ pEdge->GetToIndex() ] = gCost;

				queue.Insert( pEdge->GetToIndex() );

				m_SearchFrontier[ pEdge->GetToIndex() ] = pEdge;
			}

			//このノードがすでに未探索領域にある場合で、この方法でここまで至るコストがこれまでに見つかった
			//ものよりも少ない場合、ノードのコストと未探索領域をそれに従って更新する
			else if ( ( gCost < m_GCosts[ pEdge->GetToIndex() ] ) &&
				      ( m_ShortestPathTree[ pEdge->GetToIndex() ] ) == skyNull )
			{
				m_FCosts[ pEdge->GetToIndex() ] = gCost + hCost;
				m_GCosts[ pEdge->GetToIndex() ] = gCost;

				queue.ChangePriority( pEdge->GetToIndex() );

				m_SearchFrontier[ pEdge->GetToIndex() ] = pEdge;
			}
		}
	}
}

} } }