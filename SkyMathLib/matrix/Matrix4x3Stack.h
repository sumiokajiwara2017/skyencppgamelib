# pragma once


namespace sky { namespace lib { namespace math {

/// <summary>
/// �萔
/// <summary>
namespace matrix4x3stack_const
{
	static const u32 STACK_SIZE	= 32;
}

typedef CStack< CMatrix4x3 >	CMatrix4x3Stack_;

/// <summary>
/// �S���S�s��X�^�b�N
/// </summary>
class CMatrix4x3Stack : base::SkyObject
{

public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	inline CMatrix4x3Stack				( u32 size = matrix4x3stack_const::STACK_SIZE );

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
	inline virtual ~CMatrix4x3Stack		();

    /// <summary>
    /// �i�[����Ă��鐔��Ԃ�
    /// </summary>
	inline s32 GetNum				(){ return m_Stack.GetNum(); };

    /// <summary>
    /// �s��̈�ԏ��Ԃ�
    /// </summary>
	inline CMatrix4x3 &GetTop			();

    /// <summary>
    /// ��ԏ�̍s���P�ʍs��ɂ���
    /// </summary>
	inline void LoadIdentity			();

    /// <summary>
    /// ��ԏ�̍s����w��̍s��ɂ���
    /// </summary>
	inline void LoadMatrix				( const CMatrix4x3 &mat );

    /// <summary>
    /// �X�^�b�N�̈�ԏ�ɍs���ǉ�����
    /// </summary>
	inline void Push					();

    /// <summary>
    /// �X�^�b�N�̈�ԏ�̍s����폜����
    /// </summary>
	inline void Pop					();

    /// <summary>
    /// stacktop = stacktop * mat�@�̌v�Z���s���B
    /// </summary>
	inline void MultMatrix				( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = mat * stacktop �̌v�Z���s���B( mat���q��Loack�s��Astack���e�̃��[���h�s��A�Ƃ����ꍇ�̎q�̃��[���h���W��stack�ɎZ�o���铙�Ɏg���j
    /// </summary>
	inline void MultMatrixLocal		( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = stacktop - 1 * mat�̌v�Z���s���B
    /// </summary>
	inline void LoadAndMultBeforeMatrix( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = mat * stacktop - 1�̌v�Z���s���B
    /// </summary>
	inline void LoadAndMultLocalBeforeMatrix( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = mat * stacktop - 1�̌v�Z���s���B
    /// </summary>
	inline void LoadAndMultLocalBeforeMatrixTransOnly( const CMatrix4x3 &mat );

private:

    /// <summary>
    /// �X�^�b�N
    /// </summary>
	CMatrix4x3Stack_				m_Stack;

};

} } }

#include "Matrix4x3Stack.inl"