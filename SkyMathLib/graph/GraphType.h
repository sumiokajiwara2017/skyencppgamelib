#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �����O���t�C���f�b�N�X
/// </summary>
static const graphIndex GRAPH_INVALID_INDEX_NO = -1;

/// <summary>
/// �C���f�b�N�X�ۑ��p�z��
/// </summary>
typedef CVectorPrimitive< graphIndex >     GraphIndexVector;

/// <summary>
/// �C���f�b�N�X�ۑ��p���X�g
/// </summary>
typedef CList< u32 , graphIndex > GraphIndexList;

/// <summary>
/// �R�X�g�ۑ��p�z��
/// </summary>
typedef CVectorPrimitive< dectype >        CostVector;

} } }