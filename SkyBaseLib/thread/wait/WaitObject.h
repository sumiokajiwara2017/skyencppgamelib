#pragma once

namespace sky { namespace lib { namespace thread {

class CWaitObject;
typedef SmartPointer< CWaitObject >	CWaitObjectPtr;

/// <summary>
/// WaitObject�N���X
/// �X���b�h���Ƃ���ʒu�Ŏ��s���~�������ꍇ�AWaitObject�������ɋN���A���̏ꏊ��
/// �V�O�i������������܂ő҂�������B�d�������Ă��Ȃ��X���b�h�����ʂȃA�C�h�����O
////�����Ȃ����߂̏��u�Ƃ��ĕK�{�ȊT�O�B
/// Windows��ł�Event�@�\�Ƃ��Ď�������邪�ALinux��ł�Event�Ƃ����T�O���������߁A
////Event�N���X�ł͂Ȃ�WaitObject�N���X�Ƃ����撣���Ē��ۉ��������O�̃N���X�Ŏ����B
/// </summary>
class CWaitObject : public base::SkyRefObject
{

public:

	/// <summary>
	///	�f�X�g���N�^
	/// </summary>
	virtual ~CWaitObject();

	/// <summary>
	///	�V�O�i����Ԃɂ���(���̏u�ԑ҂��Ă����X���b�h���N����j
	/// </summary>
	void SetSignal();

	/// <summary>
	///	�m���V�O�i����Ԃɂ���
	/// </summary>
	void SetUnSignal();

	/// <summary>
	///	�I�u�W�F�N�g���V�O�i����ԁi���p�ł����ԁj�ɂȂ�܂ő҂�
	/// time = -1�̏ꍇ�A�V�O�i��������܂Ŗ����ɑ҂B
	/// </summary>
	eWaitResultKind	Wait( s32 time= -1 );

	/// <summary>
	///	�V�O�i����Ԃ��ǂ������肷��
	/// </summary>
	skyBool	IsSignal() const;

	/// <summary>
	///	�쐬
	/// </summary>
	static CWaitObjectPtr Create();

private:

	/// <summary>
	/// �N���e�B�J���Z�N�V����
	/// </summary>
	CriticalSection *m_pCs;

	/// <summary>
	///�@�V�O�i����ԃt���O
	/// </summary>
	volatile skyBool	m_IsSignal;

#ifdef SKYLIB_PLATFORM_WIN32

	/// <summary>
	///	�C�x���g���ʃn���h��
	/// </summary>
	HANDLE	m_Handle;

#endif

	/// <summary>
	///	�R���X�g���N�^
	/// </summary>
	CWaitObject();
};

} } }