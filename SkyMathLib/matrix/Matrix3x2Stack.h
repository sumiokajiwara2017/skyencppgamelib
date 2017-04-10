# pragma once


namespace sky { namespace lib { namespace math {

/// <summary>
/// �萔
/// <summary>
namespace matrix3x2stack_const
{
	static const u32 STACK_SIZE	= 32;
}

typedef CStack< CMatrix3x2 >	CMatrix3x2Stack_;

/// <summary>
/// �S���S�s��X�^�b�N
/// </summary>
class CMatrix3x2Stack : base::SkyObject
{

public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	CMatrix3x2Stack				( u32 size = matrix3x2stack_const::STACK_SIZE );

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
	virtual ~CMatrix3x2Stack		();

    /// <summary>
    /// �i�[����Ă��鐔��Ԃ�
    /// </summary>
	s32 GetNum				(){ return m_Stack.GetNum(); };

    /// <summary>
    /// �s��̈�ԏ��Ԃ�
    /// </summary>
	CMatrix3x2 &GetTop			();

    /// <summary>
    /// ��ԏ�̍s���P�ʍs��ɂ���
    /// </summary>
	void LoadIdentity			();

    /// <summary>
    /// ��ԏ�̍s����w��̍s��ɂ���
    /// </summary>
	void LoadMatrix				( const CMatrix3x2 &mat );

    /// <summary>
    /// �X�^�b�N�̈�ԏ�ɍs���ǉ�����
    /// </summary>
	void Push					();

    /// <summary>
    /// �X�^�b�N�̈�ԏ�̍s����폜����
    /// </summary>
	void Pop					();

    /// <summary>
    /// stacktop = stacktop * mat�@�̌v�Z���s���B
    /// </summary>
	void MultMatrix				( const CMatrix3x2 &mat );

    /// <summary>
    /// stacktop = mat * stacktop �̌v�Z���s���B( mat���q��Loack�s��Astack���e�̃��[���h�s��A�Ƃ����ꍇ�̎q�̃��[���h���W��stack�ɎZ�o���铙�Ɏg���j
    /// </summary>
	void MultMatrixLocal		( const CMatrix3x2 &mat );

    /// <summary>
    /// stacktop = stacktop - 1 * mat�̌v�Z���s���B
    /// </summary>
	void LoadAndMultBeforeMatrix( const CMatrix3x2 &mat );

    /// <summary>
    /// stacktop = mat * stacktop - 1�̌v�Z���s���B
    /// </summary>
	void LoadAndMultLocalBeforeMatrix( const CMatrix3x2 &mat );

private:

    /// <summary>
    /// �X�^�b�N
    /// </summary>
	CMatrix3x2Stack_				m_Stack;

};

} } }