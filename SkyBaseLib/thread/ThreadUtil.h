#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
/// �X���b�h�֗��֐��Q
/// </summary>
class CThreadUtil
{

public:

	/// <summary>
	/// ���݂̃X���b�h�̂h�c���擾����(���̊֐����Ă񂾃X���b�h��ID��Ԃ��j
	/// </summary>
	 static inline tread_id  GetCurrentThreadId();

	/// <summary>
	/// ���݂̃X���b�h�𖰂点��
	/// </summary>
	 static inline void Sleep( updateTime ms );

	/// <summary>
	/// ���݂̃X���b�h���I��������i������Ă񂾃X���b�h���I������j
	/// </summary>
	 static inline void End();
};

} } }

//�C���X�^���X�A�N�Z�X������
#define CThreadUtil_			sky::lib::thread::CThreadUtil

#include "ThreadUtil.inl"