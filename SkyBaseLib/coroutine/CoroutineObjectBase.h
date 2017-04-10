#pragma once

namespace sky { namespace lib { namespace coroutine {

/// <summary>
/// �R���[�`���I�u�W�F�N�g�x�[�X
/// </summary>
struct CCoroutineObjectBase : public base::SkyRefObject
{
    SkyTypeDefRTTI;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() { m_State = 0; }

	/// <summary>
	/// ���݂̈ʒu
	/// </summary>
	s32 m_State;
};

#define COROUTINE_OBJ_BEGIN_( co ) switch( co.m_State ) { case 0:
#define COROUTINE_OBJ_END_   default: break; }
#define YIELD_OBJ_( co , ret ) { co.m_State = __LINE__; return ret; case __LINE__:; }

} } }