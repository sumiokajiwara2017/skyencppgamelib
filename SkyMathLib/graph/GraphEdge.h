#pragma once

namespace sky { namespace lib { namespace math {

class CGraphEdge;
typedef CList< u32 , CGraphEdge > CGraphEdgeList;
typedef CVectorPrimitive< CGraphEdgeList > CGraphEdgeListVector;

/// <summary>
/// �O���t�̃G�b�W�N���X
/// </summary>
class CGraphEdge : public base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CGraphEdge();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGraphEdge( graphIndex fromIndex , graphIndex toIndex , dectype cost );
	inline CGraphEdge( graphIndex fromIndex , graphIndex toIndex );
	inline CGraphEdge();

	/// <summary>
	/// �J�n�C���f�b�N�X�̎擾�^�ݒ�
	/// </summary>
	inline graphIndex GetFromIndex() const;
	inline void       SetFromIndex( graphIndex index );

	/// <summary>
	/// ���B�C���f�b�N�X�̎擾�^�ݒ�
	/// </summary>
	inline graphIndex GetToIndex() const;
	inline void       SetToIndex( graphIndex index );

	/// <summary>
	/// �R�X�g
	/// </summary>
	inline dectype GetCost() const;
	inline void    SetCost( dectype cost );

protected:

	/// <summary>
	/// �O���t�̊J�n�C���f�b�N�X
	/// </summary>
	graphIndex m_FromIndex;

	/// <summary>
	/// �O���t�̓��B�C���f�b�N�X
	/// </summary>
	graphIndex m_ToIndex;

	/// <summary>
	/// �R�X�g
	/// </summary>
	dectype m_Cost;

};

} } }

#include "GraphEdge.inl"