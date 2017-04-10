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

	//�X�^�b�N�Ƀ��[�g��ݒ肷��
	stack.Push( &root );

	while( stack.IsEmpty() == skyFalse )
	{
		//���̃G�b�W���擾����
		const Edge* pNextEdge;
		stack.Pop( pNextEdge );

		//���̃G�b�W�������e���L�^����
		m_RouteIndexVector[ pNextEdge->GetToIndex() ] = pNextEdge->GetFromIndex();

		//�K��ς݂̈��t����
		m_VisitedIndexVector[ pNextEdge->GetToIndex() ] = eMark_visited;

		//�^�[�Q�b�g������������I��
		if ( pNextEdge->GetToIndex() == m_TargetIndex )
		{
			result = skyTrue;
			break;
		}

		//���̃G�b�W���w���m�[�h����o�Ă���G�b�W�Q���X�^�b�N�Ƀv�b�V������
		//�i�������G�b�W���ȑO�K�ꂽ�m�[�h�������Ă��Ȃ��ꍇ�j
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
	//�p�X������Ώ�������
	if ( ( m_IsTargetFound == skyFalse || m_TargetIndex == GRAPH_INVALID_INDEX_NO ) == skyFalse )
	{
		//�^�[�Q�b�g���ŏ��ɒǉ�
		graphIndex nd = m_TargetIndex;
		dest.AddTail( nd );

		//���[�g��H��Ȃ���i�[���Ă���
		while( nd != m_SourceIndex )
		{
			nd = m_RouteIndexVector[ nd ];

			dest.AddTail( nd );
		}
	}

	return dest;
}

} } }