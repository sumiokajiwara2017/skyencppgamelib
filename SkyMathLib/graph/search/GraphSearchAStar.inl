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
	//�^�[�Q�b�g�����������ݒ肳��Ă��Ȃ������牽�������Ԃ�
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
	//�m�[�h�̃C���f�b�N�X�t���v���C�I���e�B�L���[���쐬����
	//���̃L���[�͒ႢF�R�X�g���m�[�h�ɗ^����i�e���f�{�g�j
	CIndexedPriorityQLow< dectype > queue( m_FCosts , m_Graph.GetAllNodeNum() );

	//�\�[�X�m�[�h���L���[�ɒu��
	queue.Insert( m_SourceIndex );

	//�L���[����łȂ���
	while( queue.IsEmpty() == skyFalse )
	{
		//�L���[����ł��R�X�g�̒Ⴂ�m�[�h�𓾂�
		s32 nextClosestNode = queue.Pop();

		//���̃G�b�W�𖖒T���̈悩��ŒZ�o�H�c���[�Ɉړ�����
		m_ShortestPathTree[ nextClosestNode ] = m_SearchFrontier[ nextClosestNode ];

		//���������^�[�Q�b�g�����݂���ꍇ
		if( nextClosestNode == m_TargetIndex )
		{
			break;
		}

		//�����ŃG�b�W���ɘa����
		//���ɍł��߂��m�[�h�ɐڑ�����Ă���e�G�b�W�Ɋւ���
		const graph_type::EdgeTypeList& edgeList = m_Graph.GetEdgeList( nextClosestNode );

		for ( u32 i = 0 ; i < edgeList.GetNum() ; i++ )
		{
			const Edge *pEdge = &edgeList[ i ];

			// ���̃m�[�h����^�[�Q�b�g�܂ł̃q���[���X�e�B�b�N�ȃR�X�g���v�Z����(H)
			dectype hCost = heuristic::Calculate( m_Graph , m_TargetIndex , pEdge->GetToIndex() );

			//���̃m�[�h����\�[�X�܂ł́u���ہv�c���Ƃ��v�Z����iG�j
			dectype gCost = m_GCosts[ nextClosestNode ] + pEdge->GetCost();

			//���̃m�[�h�����T���̈�ɒǉ����ꂽ���Ƃ��Ȃ���΁A�ǉ���G��F�R�X�g���X�V����
			if ( m_SearchFrontier[ pEdge->GetToIndex() ] == skyNull )
			{
				m_FCosts[ pEdge->GetToIndex() ] = gCost + hCost;
				m_GCosts[ pEdge->GetToIndex() ] = gCost;

				queue.Insert( pEdge->GetToIndex() );

				m_SearchFrontier[ pEdge->GetToIndex() ] = pEdge;
			}

			//���̃m�[�h�����łɖ��T���̈�ɂ���ꍇ�ŁA���̕��@�ł����܂Ŏ���R�X�g������܂łɌ�������
			//���̂������Ȃ��ꍇ�A�m�[�h�̃R�X�g�Ɩ��T���̈������ɏ]���čX�V����
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