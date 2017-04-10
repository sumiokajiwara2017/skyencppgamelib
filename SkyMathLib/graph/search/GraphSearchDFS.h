#pragma once

namespace sky { namespace lib { namespace math {

static const u32 GRAPH_DFS_DEFAULT_SEARCH_USED_STACK_SIZE = 256;

/// <summary>
/// �[���D��T���iDepth First Search�j
/// </summary>
template< class graph_type >
class CGraphSearchDFS
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CGraphSearchDFS();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGraphSearchDFS( const graph_type& graph ,
		                    graphIndex        sourceIndex ,
							graphIndex        targetIndex = GRAPH_INVALID_INDEX_NO ,
		                    u32               useSearchStacSize = GRAPH_DFS_DEFAULT_SEARCH_USED_STACK_SIZE
							);
    CGraphSearchDFS& operator=( const CGraphSearchDFS& ); //�x�������̂��ߒ�`�B���ۂ��̃N���X�����ł܂킵����͂ł��Ȃ��B

	/// <summary>
	/// �^�[�Q�b�g������������True��Ԃ�
	/// </summary>
	skyBool Found() const;

	/// <summary>
	/// �\�[�X����^�[�Q�b�g�ւ̍ŒZ�p�X����Ȃ�m�[�h�C���f�b�N�X�̃��X�g��Ԃ�
	/// </summary>
	GraphIndexList &GetPathToTarget( GraphIndexList &dest ) const;

protected:

	/// <summary>
	/// �}�[�N���
	/// </summary>
	enum eMark
	{
		eMark_visited = 0 ,
		eMark_unvisited ,
		no_parent_assigned ,
	};
	typedef CVectorPrimitive< eMark > MarkVector;

	/// <summary>
	/// �G�b�W�ƃm�[�h�̌^���Ē�`����
	/// </summary>
	typedef typename graph_type::NodeType Node;
	typedef typename graph_type::EdgeType Edge;
	typedef CStack< const Edge * >        EdgePtrStack;

	/// <summary>
	/// �T�������O���t�ւ̎Q��
	/// </summary>
	const graph_type& m_Graph;

	/// <summary>
	/// �C���f�b�N�X�̖K��L�^��ۑ�����z��
	/// </summary>
	MarkVector m_VisitedIndexVector;

	/// <summary>
	/// �K��ς݂̃m�[�h�̃C���f�b�N�X���i�[�����z��
	/// </summary>
	GraphIndexVector m_RouteIndexVector;

	/// <summary>
	/// �o���C���f�b�N�X
	/// </summary>
	graphIndex m_SourceIndex;

	/// <summary>
	/// �ڕW�C���f�b�N�X
	/// </summary>
	graphIndex m_TargetIndex;

	/// <summary>
	/// �^�[�Q�b�g�܂ł̃p�X�����܂�����skyTrue
	/// </summary>
	skyBool m_IsTargetFound;

	/// <summary>
	/// ��������
	/// </summary>
	inline skyBool Search( u32 stacSize );
};

} } }

#include "GraphSearchDFS.inl"