namespace sky { namespace lib { namespace math {

template< class graph_type >
inline CGraphSearchBFS< graph_type >::~CGraphSearchBFS()
{
}

template< class graph_type >
inline CGraphSearchBFS< graph_type >::CGraphSearchBFS(
	                    const graph_type& graph ,
		                graphIndex        sourceIndex ,
						graphIndex        targetIndex ,
	                    u32               useSearchStacSize
						) : m_Graph( graph ) , 
						    m_SourceIndex( sourceIndex ) , 
							m_TargetIndex( targetIndex ) , 
							m_IsTargetFound( skyFalse ) ,
							m_VisitedIndexVector( eMark_unvisited , graph.GetAllNodeNum() , 4 ) ,
							m_RouteIndexVector( no_parent_assigned , graph.GetAllNodeNum() , 4 )
{
	m_IsTargetFound = Search( useSearchStacSize );
}

template< class graph_type >
CGraphSearchBFS< graph_type >& CGraphSearchBFS< graph_type >::operator=( const CGraphSearchBFS& )
{ 
	SKY_PANIC_MSG( "this proc is invalid." );
	return *this;
}	

template< class graph_type >
inline skyBool CGraphSearchBFS< graph_type >::Search( u32 fifoSize )
{
	skyBool result = skyFalse;

	EdgePtrFifo fifo( fifoSize );

	Edge root;
	root.SetFromIndex( m_SourceIndex );
	root.SetToIndex( m_SourceIndex );
	root.SetCost( 0 );

	//キューにルートを設定する
	fifo.Push( &root );

	//開始位置は訪問済みにする
	m_VisitedIndexVector[ m_SourceIndex ] = eMark_visited;

	//キューにエッジが存在する間処理しつづける
	while( fifo.IsEmpty() == skyFalse )
	{
		//次のエッジを取得する
		const Edge* pNextEdge;
		fifo.Pop( pNextEdge );

		//このノードの親に印を付ける
		m_RouteIndexVector[ pNextEdge->GetToIndex() ] = pNextEdge->GetFromIndex();

		//ターゲットが見つかったら終了
		if ( pNextEdge->GetToIndex() == m_TargetIndex )
		{
			result = skyTrue;
			break;
		}

		//このエッジの終点にあるノードに至るエッジをキューブにプッシュする
		const graph_type::EdgeTypeList& edgeList = m_Graph.GetEdgeList( pNextEdge->GetToIndex() );
		for ( u32 i = 0 ; i < edgeList.GetNum() ; i++ )
		{
			//探索済みでなければプッシュする
			const Edge *pEdge = &edgeList[ i ];
			if ( m_VisitedIndexVector[ pEdge->GetToIndex() ] == eMark_unvisited )
			{
				fifo.Push( pEdge );

				//調べる前にこのノードは訪問済みとして印が付けられる
				m_VisitedIndexVector[ pEdge->GetToIndex() ] = eMark_visited;
			}
		}

	}

	return result;
}

template< class graph_type >
skyBool CGraphSearchBFS< graph_type >::Found() const
{
	return m_IsTargetFound;
}

template< class graph_type >
GraphIndexList &CGraphSearchBFS< graph_type >::GetPathToTarget( GraphIndexList &dest ) const
{
	//パスがあれば処理する
	if ( ( m_IsTargetFound == skyFalse || m_TargetIndex == GRAPH_INVALID_INDEX_NO ) == skyFalse )
	{
		//ターゲットを最初に追加
		graphIndex nd = m_TargetIndex;
		dest.AddTail( nd );

		//ルートを辿りながら格納していく
		while( nd != m_SourceIndex )
		{
			nd = m_RouteIndexVector[ nd ];

			dest.AddTail( nd );
		}
	}

	return dest;
}

} } }