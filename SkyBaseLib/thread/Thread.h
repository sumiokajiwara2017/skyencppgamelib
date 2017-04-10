#pragma once

namespace sky { namespace lib { namespace thread {

class CThread;
typedef SmartPointer< CThread >	CThreadPtr;
typedef CList< u32 , CThreadPtr >	CThreadPtrList;

/// <summary>
/// �X���b�h�N���X
/// �g�����͂Q���
/// �@���̃N���X���p������ThreadMain�֐����I�[�o�[���C�h����B
/// �A�֐��I�u�W�F�N�g��o�^���Ă�����R�[������B
///
/// �X���b�h�N���X�̓V���v���ōŒ���֗��ȋ@�\����������B
/// �~���[�e�b�N�X�A�Z�}�t�H�A�C�x���g�͂����Ďg�p���Ȃ��B
/// �g�p�������ꍇ�͂��̎���������B�V���v���Ȃ�΂ǂ̊��ł��������ł���͂��B
/// �g�p���鎞���A���܂蕡�G�Ȗ��������N�����Ȃ��A�V���v���ȃV�`���G�[�V�����Ŏg�p���邱�ƁB
/// ��O�����ƃR���{����ƃJ�I�X�ɂȂ�̂łȂ�ׂ������邱�ƁB
/// </summary>
class CThread : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CThread();

	/// <summary>
	/// �X���b�h���C��
	/// </summary>
	virtual void ThreadMain();

	/// <summary>
	/// �X���b�h�J�n
	/// </summary>
	void Start();

	/// <summary>
	/// �X���b�h�I���i���̏����͊O���X���b�h����X���b�h��������~���邽�߁A���Ɋ댯�ł��B�j
	/// </summary>
	void End();

	/// <summary>
	/// �X���b�h�I������
	/// </summary>
	skyBool IsEnd();

	/// <summary>
	/// �X���b�h�I���҂�(�����Ń��[�v���ďI����҂��܂�)
	/// </summary>
	void EndWait();

	/// <summary>
	/// �X���b�h�ꎞ��~
	/// </summary>
	void Stop();

	/// <summary>
	/// �X���b�h�ꎞ��~�ĊJ
	/// </summary>
	void ReStart();

	/// <summary>
	/// �X���b�hID�擾
	/// </summary>
	tread_id GetId();

	/// <summary>
	/// �X���b�h�n���h���擾
	/// </summary>
	tread_hndl GetHndl();

	/// <summary>
	/// �X���b�h�쐬
	/// </summary>
	static CThreadPtr Create( u32 stackSize , const FunctionCallBackAsyncPtr &spFunction = FunctionCallBackAsyncPtr() );

protected:

	/// <summary>
	/// �X���b�h�X�e�[�g
	/// </summary>
	enum eState
	{
		eState_Exec = 0 ,
		eState_End ,
		eState_Suspend ,
		eState_Error ,
	} m_State;

	/// <summary>
	/// �t�@���N�V�����I�u�W�F�N�g
	/// </summary>
	FunctionCallBackAsyncPtr m_spFunction;

	/// <summary>
	/// �X���b�hID
	/// </summary>
	tread_id m_ThreadId;

	/// <summary>
	/// �X���b�h�n���h��
	/// </summary>
	tread_hndl m_ThreadHndl;

	/// <summary>
	/// �S�X���b�h���ʃG���g���[�|�C���g
	/// </summary>
	static unsigned __stdcall ThreadEntry_( void* pParam );

	/// <summary>
	/// �R���X�g���N�^�i�쐬�������_�ł̓X���b�h�͎��s����܂���B
	/// </summary>
	CThread( u32 stackSize );

private:

	CThread( u32 stackSize , const FunctionCallBackAsyncPtr &spFunction );
	void Constractor( u32 stackSize );
};

} } }