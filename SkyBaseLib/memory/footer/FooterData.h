#pragma once

namespace sky { namespace lib { namespace memory {

/// <summary>
/// �A���P�[�g�̈惊�X�g�t�b�^�[���
/// IAllocator���p�����č쐬���邷�ׂẴA���P�[�^�[���m�ۂ���̈�̃t�b�^�[�ɂ͕K�����̍\���̂�t����
/// �������邱�Ƃŋ��ʏ�������������
/// </summary>
struct CAllocatorDataFooter
{
	/// <summary>
	/// �������j�󌟒m�p�h�c
	/// </summary>
	u32          m_CheckBreakId;
};

} } }
