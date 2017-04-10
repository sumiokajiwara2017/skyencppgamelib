#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �O���t�N���X�i���̃N���X�ɔC�ӂ̃m�[�h�A�C�ӂ̃G�b�W��t���Ďg�p����j
/// </summary>
template < class node_type , class edge_type >
class CSparseGraph : public base::SkyObject
{

public:

	/// <summary>
	/// �^�̍Ē�`
	/// </summary>
	typedef node_type NodeType;
	typedef edge_type EdgeType;

	typedef CVectorPrimitive< NodeType >       NodeTypeVector;
	typedef CList< u32 , EdgeType >   EdgeTypeList;
	typedef CVectorPrimitive< EdgeTypeList * > EdgeTypeListPtrVector;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CSparseGraph(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CSparseGraph( skyBool isDigraph );

	/// <summary>
	/// �w��̃C���f�b�N�X�̃m�[�h��Ԃ�
	/// </summary>
	inline const NodeType& GetNode( graphIndex index ) const;
	inline NodeType&       GetNode( graphIndex index );

	/// <summary>
	/// �w��̃C���f�b�N�X�̃G�b�W��Ԃ�
	/// </summary>
	inline const EdgeTypeList& GetEdgeList( graphIndex index ) const;
	inline const EdgeType&     GetEdge( graphIndex fromIndex , graphIndex toIndex ) const;
	inline EdgeType&           GetEdge( graphIndex fromIndex , graphIndex toIndex );

	/// <summary>
	/// ���̃t���[�m�[�h�C���f�b�N�X���擾����
	/// </summary>
	inline graphIndex GetNextFreeNodeIndex() const;

	/// <summary>
	/// ���̃O���t�Ƀm�[�h��ǉ����A�C���f�b�N�X��Ԃ�
	/// </summary>
	inline graphIndex AddNode( const NodeType &node );

	/// <summary>
	/// �m�[�h�𖳌�������
	/// </summary>
	inline void RemoveNode( graphIndex index );

	/// <summary>
	/// �G�b�W��ǉ��^�폜����
	/// </summary>
	inline void AddEdge( EdgeType edge );
	inline void RemoveEdge( graphIndex fromIndex , graphIndex toIndex );
	inline void RemoveAllEdge();

	/// <summary>
	/// ���̃O���t���̃A�N�e�B�u�{��A�N�e�B�u�ȃm�[�h�̌���Ԃ�
	/// </summary>
	inline u32 GetAllNodeNum() const;

	/// <summary>
	/// ���̃O���t���̃A�N�e�B�u�ȃm�[�h�̌���Ԃ�
	/// </summary>
	inline u32 GetActiveNodeNum() const;

	/// <summary>
	/// ���̃O���t���̃G�b�W�̌���Ԃ�
	/// </summary>
	inline u32 GetEdgeNum() const;

	/// <summary>
	/// ���̃O���t���L���O���t���H
	/// </summary>
	inline skyBool IsDigraph() const;

	/// <summary>
	/// �m�[�h���󂩁H
	/// </summary>
	inline skyBool IsEmpty() const;

	/// <summary>
	/// �O���t���N���A����
	/// </summary>
	inline void Clear();

	/// <summary>
	/// �w��̃C���f�b�N�X�̃m�[�h���L�������肷��
	/// </summary>
	inline skyBool IsNodePresent( graphIndex index ) const;

	/// <summary>
	/// �w��̃C���f�b�N�X�̃G�b�W���L�������肷��
	/// </summary>
	inline skyBool IsEdgePresent( graphIndex fromIndex , graphIndex toIndex ) const; 

	/// <summary>
	/// �����ȃG�b�W�����ׂď���
	/// </summary>
	inline void  CullInvalidEdges();

	/// <summary>
	/// �G�b�W�ɐV�����R�X�g��ݒ肷��
	/// </summary>
	inline void SetEdgeCost( graphIndex fromIndex , graphIndex toIndex , dectype newCost );

	/// <summary>
	/// �w��̌o�H�����݂��Ȃ����true��Ԃ�
	/// </summary>
	skyBool IsUniqueEdge( graphIndex fromIndex , graphIndex toIndex ) const;

protected:

	/// <summary>
	/// �m�[�h�Q
	/// </summary>
	NodeTypeVector m_NodeVector;

	/// <summary>
	/// �m�[�h�Ɋ֘A����G�b�W���X�g�z��
	/// �i�e�m�[�h�̃C���f�b�N�X�́A���̃m�[�h�Ɋ֘A�t����ꂽ�G�b�W�̃��X�g�̃L�[�ƂȂ�j
	/// </summary>
	EdgeTypeListPtrVector m_EdgyListVector;

	/// <summary>
	/// �w�����O���t�t���O
	/// </summary>
	skyBool m_IsDigraph;

	/// <summary>
	/// �ǉ��\��̃C���f�b�N�X
	/// </summary>
	graphIndex m_NextnodeIndex;
};

} } }

#include "SparseGraph.inl"