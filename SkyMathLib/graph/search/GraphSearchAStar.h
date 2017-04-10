#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �`���T��
/// </summary>
template < class graph_type , class heuristic >
class CGraphSearchAStar
{

public:

	/// <summary>
	/// �^�̍Ē�`
	/// </summary>
	typedef typename graph_type::EdgeType    Edge;
	typedef CVectorPrimitive< const Edge * > EdgePtrVector;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CGraphSearchAStar();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGraphSearchAStar( const graph_type& graph ,
		                            graphIndex  sourceIndex ,
							        graphIndex  targetIndex = GRAPH_INVALID_INDEX_NO );
    CGraphSearchAStar& operator=( const CGraphSearchAStar& ); //�x�������̂��ߒ�`�B���ۂ��̃N���X�����ł܂킵����͂ł��Ȃ��B

	/// <summary>
	/// �r�o�s���`����G�b�W�̃x�N�^�[��Ԃ��B�R���X�g���N�^�Ƀ^�[�Q�b�g���^������ƁA
	/// ����́A�^�[�Q�b�g��������O�ɁA���ׂ�ꂽ���ׂẴm�[�h����\�������r�o�s�ƂȂ�B
	/// �����łȂ��ꍇ�́A���̃O���t���̂��ׂẴm�[�h���܂�
	/// </summary>
	EdgePtrVector &GetSPT( EdgePtrVector &vector ) const;

	/// <summary>
	/// �\�[�X����^�[�Q�b�g�ւ̍ŒZ�o�H���\������m�[�h�C���f�b�N�̃��X�g��Ԃ��B
	/// ����͂r�o�s���^�[�Q�b�g����������ɓ��삷�邱�ƂŌo�H���v�Z����
	/// </summary>
	GraphIndexList &GetPathToTarget( GraphIndexList &dest ) const;

	/// <summary>
	/// �^�[�Q�b�g�ւ̃g�[�^���R�X�g��Ԃ�
	/// </summary>
	dectype GetCostToTarget() const;

private:

	/// <summary>
	/// �T�������O���t�ւ̎Q��
	/// </summary>
	const graph_type& m_Graph;

	/// <summary>
	/// �m�[�h�ŃC���f�b�N�X�t�������B���̃m�[�h�ւ́u���ہv�̗ݐσR�X�g���܂�
	/// </summary>
	CostVector m_GCosts;

	///�m�[�h�ŃC���f�b�N�X�t�������Bm_GCosts[n]��n����^�[�Q�b�g�m�[�h�܂ł�
	///�q���[���X�e�B�b�N�ȃR�X�g���������R�X�g���܂�
	///����́AiPQ�ŃC���f�b�N�X�t�����x�N�^�[�ł���
	CostVector m_FCosts;

	/// <summary>
	/// �ŒZ�̌o�H�c���[( �r�o�s )
	/// ���̃O���t�ɂ����āA�r�o�s��̂��ׂẴm�[�h����\�[�X�d�x�܂ł̍ł��ǂ��o�H���J�v�Z����������������T�u�c���[
	/// </summary>
	EdgePtrVector m_ShortestPathTree;

	/// <summary>
	/// �r�o�s�ɐڑ����ꂽ�m�[�h�ɂȂ���u�e�v�G�b�W�́i�m�[�h�ɂ���āj�C���f�b�N�X�t��
	/// ���ꂽ�x�N�^�[�ł���B�������A����͂܂��r�o�s�ɒǉ�����Ă��Ȃ��B
	/// </summary>
	EdgePtrVector m_SearchFrontier;

	/// <summary>
	/// �o���C���f�b�N�X
	/// </summary>
	graphIndex m_SourceIndex;

	/// <summary>
	/// �ڕW�C���f�b�N�X
	/// </summary>
	graphIndex m_TargetIndex;

	/// <summary>
	/// ��������
	/// </summary>
	inline void Search();

};

} } }

#include "GraphSearchAStar.inl"
