#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//#define SW_USE_SQ_STD_LIB

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// �萔
/// </summary>
namespace sq_const
{
	static const u32  VM_THREAD_STACK_SIZE		= 32;	//�ő哯���X���b�h�N����
	static const u32  VM_THREAD_FUNCNAME_LEN	= 64;	//�֐����̍ő�̒���
	static const u32  FUNC_PARAM_MAX_NUM		= 32;	//Squirrel�֐��p�����[�^�[���ő�
};

} } }

#endif