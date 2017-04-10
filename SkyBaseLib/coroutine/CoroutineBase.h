#pragma once

namespace sky { namespace lib { namespace coroutine {

//�R���[�`���̏I���l
static const s32 COROUTINE_END_VALUE = 0;

/// <summary>
/// �R���[�`���x�[�X�N���X
/// �C���^�[�l�b�g�Ō������ȈՂȃR���[�`�������B
/// ���������[�������΃R���[�`���I�ȓ������ȒP�Ɏ����ł���B
/// �ڂ����̓e�X�g�R�[�h�Q�ƁB�{�i�I�Ȏ����͎����������肪�N���₷���̂ł��̃��C�u�����ł͂����܂łƂ���B
/// </summary>
class CCoroutineBase : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CCoroutineBase(){ Initialize(); }

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() { m_State = 0; }

	/// <summary>
	/// �R���[�`�������̎���
	/// </summary>
	virtual s32 vExecute( void *pParam ) = 0;

protected:

	/// <summary>
	/// ���݂̈ʒu
	/// </summary>
	s32 m_State;
};

#define COROUTINE_BEGIN_ switch( m_State ) { case 0:
#define COROUTINE_END_   default: break; }
#define YIELD_( ret ) { m_State = __LINE__; return ret; case __LINE__:; }

} } }