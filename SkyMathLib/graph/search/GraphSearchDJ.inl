namespace sky { namespace lib { namespace math {

template< class graph_type >
inline CGraphSearchDijkstra< graph_type >::~CGraphSearchDijkstra()
{

}

template< class graph_type >
inline CGraphSearchDijkstra< graph_type >::CGraphSearchDijkstra( const graph_type& graph ,
		                     graphIndex        sourceIndex ,
						     graphIndex        targetIndex ) :
                             m_Graph( graph ) , 
						     m_SourceIndex( sourceIndex ) , 
							 m_TargetIndex( targetIndex ) , 
							 m_ShortestPathTree( skyNull , graph.GetAllNodeNum() , 4 ) ,
							 m_CostToThisNode( 0.0f , graph.GetAllNodeNum() , 4 ) ,
							 m_SearchFrontier( skyNull , graph.GetAllNodeNum() , 4 ) 
{
	Search();
}

template< class graph_type >
CGraphSearchDijkstra< graph_type >& CGraphSearchDijkstra< graph_type >::operator=( const CGraphSearchDijkstra& )
{ 
	SKY_PANIC_MSG( "this proc is invalid." );
	return *this;
}	

template< class graph_type >
inline typename CGraphSearchDijkstra< graph_type >::EdgePtrVector &CGraphSearchDijkstra< graph_type >::GetSPT( EdgePtrVector &vector ) const
{
	for( u32 i = 0 ; m_ShortestPathTree.GetNum() ; i++ )
	{
		vector[ i ] = m_ShortestPathTree[ i ];
	}

	return vector;
}

template< class graph_type >
inline GraphIndexList &CGraphSearchDijkstra< graph_type >::GetPathToTarget( GraphIndexList &dest ) const
{
	//ターゲットがそもそも設定されていなかったら何もせず返す
	if ( m_TargetIndex < 0 )  return dest;

	s32 nd = m_TargetIndex;

	dest.AddFront( nd );

	while ( ( nd != m_SourceIndex ) && ( m_ShortestPathTree[ nd ] != 0 ) )
	{
		nd = m_ShortestPathTree[ nd ]->GetFromIndex();
		dest.AddFront( nd );
	}

	return dest;
}

template< class graph_type >
inline dectype CGraphSearchDijkstra< graph_type >::GetCostToTarget() const
{
	return m_CostToThisNode[ m_TargetIndex ];
}

template< class graph_type >
inline void CGraphSearchDijkstra< graph_type >::Search()
{
	//インデックスプライオリティキューを作成する。
	//これは、小さいものからおおきいものへ　（前から後ろへ）ソートする
	//ＩＰＱが持つ要素の最大数はノード数である。
	//これは、ノードが何度もキュー上で表現されることがないからである
	CIndexedPriorityQLow< dectype > queue( m_CostToThisNode , m_Graph.GetAllNodeNum() );

	//ソースノードをキューに置く
	queue.Insert( m_SourceIndex );

	//キューが空でない間ループ
	while( queue.IsEmpty() == skyFalse )
	{
		//キューから最もコストの低いノードを得る
		//返却値はノードインデックスであり、ノード自身ではないことを忘れないこと。
		//このノードはまだＳＰＴ上にないノードであり、ソースノードに最も近いものである
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

			//このエッジが示すノードへのトータルコストは、現在のノードへのコストに
			//それにつながるエッジのコストを加えたものである
			dectype newCost = m_CostToThisNode[ nextClosestNode ] + pEdge->GetCost();

			//このエッジが未探索領域上になかった場合、そのエッジが指すノードに到達する
			//コストを記録し、そのエッジを未探索領域に加え、デスティネーションノードをＰＱに加える
			if ( m_SearchFrontier[ pEdge->GetToIndex() ] == 0 )
			{
				m_CostToThisNode[ pEdge->GetToIndex() ] = newCost;
				queue.Insert( pEdge->GetToIndex() );
				m_SearchFrontier[ pEdge->GetToIndex() ] = pEdge;
			}
			//そうでない場合は、現在のノードを介してそのデスティネーションノードに到達する
			//コストがこれまでに見つかった最も少ないコストよりも少ないかどうかをテストする。
			//この経路のコストが少ない場合、その新しいコストを、その新しいコストを出ぅティネーションノードに割り当て、
			else if ( ( newCost < m_CostToThisNode[ pEdge->GetToIndex() ] ) &&
				      ( m_ShortestPathTree[ pEdge->GetToIndex() ] == 0 ) )
			{
				m_CostToThisNode[ pEdge->GetToIndex() ] = newCost;
				queue.ChangePriority( pEdge->GetToIndex() );
				m_SearchFrontier[ pEdge->GetToIndex() ] = pEdge;
			}
		}
	}
}

} } }
