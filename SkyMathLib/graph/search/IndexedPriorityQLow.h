#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �C���f�b�N�X�v���C�I���e�B�L���[
/// </summary>
template< class KeyType >
class CIndexedPriorityQLow
{

public:
  
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CIndexedPriorityQLow( CVectorPrimitive< KeyType >& keys , s32 MaxSize );
	CIndexedPriorityQLow& operator=( const CIndexedPriorityQLow& ); //�x�������̂��ߒ�`�B���ۂ��̃N���X�����ł܂킵����͂ł��Ȃ��B

	/// <summary>
	/// ���g�͋󂩁H
	/// </summary>
    skyBool IsEmpty() const;

	/// <summary>
	/// �l�̑}��
	/// </summary>
    void Insert( const s32 idx );

	/// <summary>
	/// �l�̍폜
	/// </summary>
    s32 Pop();

	/// <summary>
	/// �D�揇�ʂ̕ύX
	/// </summary>
    void ChangePriority( const s32 idx );

private:

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
    CVectorPrimitive< KeyType >& m_vecKeys;
    CVectorPrimitive< s32 >      m_Heap;
    CVectorPrimitive< s32 >      m_invHeap;
    s32                 m_iSize;
    s32                 m_iMaxSize;

	/// <summary>
	/// �l�̌���
	/// </summary>
    void Swap( s32 a, s32 b );
    void ReorderUpwards( s32 nd );
    void ReorderDownwards( s32 nd , s32 HeapSize );
};

} } }

#include "IndexedPriorityQLow.inl"