namespace sky { namespace lib { namespace math {

template< class graph_type >
inline CGraphSearchDFS< graph_type >::~CGraphSearchDFS()
{
}

template< class graph_type >
inline CGraphSearchDFS< graph_type >::CGraphSearchDFS(
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
CGraphSearchDFS< graph_type >& CGraphSearchDFS< graph_type >::operator=( const CGraphSearchDFS& )
{ 
	SKY_PANIC_MSG( "this proc is invalid." );
	return *this;
}	

template< class graph_type >
inline skyBool CGraphSearchDFS< graph_type >::Search( u32 stacSize )
{
	skyBool result = skyFalse;

	EdgePtrStack stack( stacSize );

	Edge root;
	root.SetFromIndex( m_SourceIndex );
	root.SetToIndex( m_SourceIndex );
	root.SetCost( 0 );

	//スタックにルートを設定する
	stack.Push( &root );

	while( stack.IsEmpty() == skyFalse )
	{
		//次のエッジを取得する
		const Edge* pNextEdge;
		stack.Pop( pNextEdge );

		//このエッジが示す親を記録する
		m_RouteIndexVector[ pNextEdge->GetToIndex() ] = pNextEdge->GetFromIndex();

		//訪問済みの印を付ける
		m_VisitedIndexVector[ pNextEdge->GetToIndex() ] = eMark_visited;

		//ターゲットが見つかったら終了
		if ( pNextEdge->GetToIndex() == m_TargetIndex )
		{
			result = skyTrue;
			break;
		}

		//このエッジが指すノードから出ているエッジ群をスタックにプッシュする
		//（ただしエッジが以前訪れたノードを示していない場合）
		const graph_type::EdgeTypeList& edgeList = m_Graph.GetEdgeList( pNextEdge->GetToIndex() );
		for ( u32 i = 0 ; i < edgeList.GetNum() ; i++ )
		{
			const Edge *pEdge = &edgeList[ i ];
			if ( m_VisitedIndexVector[ pEdge->GetToIndex() ] == eMark_unvisited )
			{
				stack.Push( pEdge );
			}
		}

	}

	return result;
}

template< class graph_type >
skyBool CGraphSearchDFS< graph_type >::Found() const
{
	return m_IsTargetFound;
}

template< class graph_type >
GraphIndexList &CGraphSearchDFS< graph_type >::GetPathToTarget( GraphIndexList &dest ) const
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