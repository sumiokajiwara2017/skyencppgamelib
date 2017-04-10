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

	//�L���[�Ƀ��[�g��ݒ肷��
	fifo.Push( &root );

	//�J�n�ʒu�͖K��ς݂ɂ���
	m_VisitedIndexVector[ m_SourceIndex ] = eMark_visited;

	//�L���[�ɃG�b�W�����݂���ԏ������Â���
	while( fifo.IsEmpty() == skyFalse )
	{
		//���̃G�b�W���擾����
		const Edge* pNextEdge;
		fifo.Pop( pNextEdge );

		//���̃m�[�h�̐e�Ɉ��t����
		m_RouteIndexVector[ pNextEdge->GetToIndex() ] = pNextEdge->GetFromIndex();

		//�^�[�Q�b�g������������I��
		if ( pNextEdge->GetToIndex() == m_TargetIndex )
		{
			result = skyTrue;
			break;
		}

		//���̃G�b�W�̏I�_�ɂ���m�[�h�Ɏ���G�b�W���L���[�u�Ƀv�b�V������
		const graph_type::EdgeTypeList& edgeList = m_Graph.GetEdgeList( pNextEdge->GetToIndex() );
		for ( u32 i = 0 ; i < edgeList.GetNum() ; i++ )
		{
			//�T���ς݂łȂ���΃v�b�V������
			const Edge *pEdge = &edgeList[ i ];
			if ( m_VisitedIndexVector[ pEdge->GetToIndex() ] == eMark_unvisited )
			{
				fifo.Push( pEdge );

				//���ׂ�O�ɂ��̃m�[�h�͖K��ς݂Ƃ��Ĉ󂪕t������
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