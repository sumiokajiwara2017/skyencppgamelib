#pragma once

namespace sky { namespace lib { namespace math {

class CGraphNode;
typedef CVectorPrimitive< CGraphNode > CGraphNodeVector;			//�z��

/// <summary>
/// �O���t�̃m�[�h�N���X
/// </summary>
class CGraphNode : public base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CGraphNode();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGraphNode();
	inline CGraphNode( graphIndex index );

	/// <summary>
	/// �C���f�b�N�X�̎擾�ݒ�
	/// </summary>
	inline graphIndex GetIndex() const;
	inline void SetIndex( graphIndex index );

protected:

	/// <summary>
	/// �C���f�b�N�X
	/// </summary>
	graphIndex m_Index;

};

} } }

#include "GraphNode.inl"