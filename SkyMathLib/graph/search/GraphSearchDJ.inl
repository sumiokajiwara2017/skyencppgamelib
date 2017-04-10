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
	//�^�[�Q�b�g�����������ݒ肳��Ă��Ȃ������牽�������Ԃ�
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
	//�C���f�b�N�X�v���C�I���e�B�L���[���쐬����B
	//����́A���������̂��炨���������̂ց@�i�O������ցj�\�[�g����
	//�h�o�p�����v�f�̍ő吔�̓m�[�h���ł���B
	//����́A�m�[�h�����x���L���[��ŕ\������邱�Ƃ��Ȃ�����ł���
	CIndexedPriorityQLow< dectype > queue( m_CostToThisNode , m_Graph.GetAllNodeNum() );

	//�\�[�X�m�[�h���L���[�ɒu��
	queue.Insert( m_SourceIndex );

	//�L���[����łȂ��ԃ��[�v
	while( queue.IsEmpty() == skyFalse )
	{
		//�L���[����ł��R�X�g�̒Ⴂ�m�[�h�𓾂�
		//�ԋp�l�̓m�[�h�C���f�b�N�X�ł���A�m�[�h���g�ł͂Ȃ����Ƃ�Y��Ȃ����ƁB
		//���̃m�[�h�͂܂��r�o�s��ɂȂ��m�[�h�ł���A�\�[�X�m�[�h�ɍł��߂����̂ł���
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

			//���̃G�b�W�������m�[�h�ւ̃g�[�^���R�X�g�́A���݂̃m�[�h�ւ̃R�X�g��
			//����ɂȂ���G�b�W�̃R�X�g�����������̂ł���
			dectype newCost = m_CostToThisNode[ nextClosestNode ] + pEdge->GetCost();

			//���̃G�b�W�����T���̈��ɂȂ������ꍇ�A���̃G�b�W���w���m�[�h�ɓ��B����
			//�R�X�g���L�^���A���̃G�b�W�𖢒T���̈�ɉ����A�f�X�e�B�l�[�V�����m�[�h���o�p�ɉ�����
			if ( m_SearchFrontier[ pEdge->GetToIndex() ] == 0 )
			{
				m_CostToThisNode[ pEdge->GetToIndex() ] = newCost;
				queue.Insert( pEdge->GetToIndex() );
				m_SearchFrontier[ pEdge->GetToIndex() ] = pEdge;
			}
			//�����łȂ��ꍇ�́A���݂̃m�[�h����Ă��̃f�X�e�B�l�[�V�����m�[�h�ɓ��B����
			//�R�X�g������܂łɌ��������ł����Ȃ��R�X�g�������Ȃ����ǂ������e�X�g����B
			//���̌o�H�̃R�X�g�����Ȃ��ꍇ�A���̐V�����R�X�g���A���̐V�����R�X�g���o���e�B�l�[�V�����m�[�h�Ɋ��蓖�āA
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
