#pragma once

namespace sky { namespace lib { namespace input {

//-------------------------------------< ���� >---------------------------------------------//

class Ckeyboard;
typedef SmartPointer< Ckeyboard >	CkeyboardPtr;

/// <summary>
/// ���͎�t�Ǘ�
/// <summary>
class Ckeyboard  : public base::SkyRefObject
{

public:

	/// <summary>
	/// �{�^�����s�[�g�Ԋu�̐ݒ�
	/// <summary>
	static void SetBtnRepeat	( s32 repIntvl , s32 onIntvl );

	/// <summary>
	/// ���͎�t���X�i�[��o�^
	/// </summary>
	void AttachInputListener	( const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// ���͎�t���X�i�[��o�^����
	/// </summary>
	void DetachInputListener	( const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// �v���O�����L�[���́i�f�o�C�X����̓��͂������Ă��A�L�[���͂����������Ƃɂ���j
	/// <summary>
	void	KeyOn				( eSkyKeybordKeyKind key );

	/// <summary>
	/// �L�[����������
	/// <summary>
	skyBool IsKeyOn				( eSkyKeybordKeyKind key );

	/// <summary>
	/// �L�[��������
	/// <summary>
	skyBool IsKeyRelease		( eSkyKeybordKeyKind key );

	/// <summary>
	/// �L�[�������㔻��
	/// <summary>
	skyBool IsKeyPress			( eSkyKeybordKeyKind key );

	/// <summary>
	/// �L�[���s�[�g����
	/// <summary>
	skyBool IsKeyRepeat			( eSkyKeybordKeyKind key );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Ckeyboard          (){};

//---------< static >----------

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize		( u32 playerNum );

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose			();

	/// <summary>
	/// �X�V
	/// </summary>
	static void Update			( const fps::CFpsController &fps );

	/// <summary>
	/// �L�[�{�[�h�擾
	/// </summary>
	static CkeyboardPtr &Get    ( u32 playerNo = CURRENT_PLAYNER_NO );

protected:

	/// <summary>
	/// �L�[���͏��
	/// </summary>
	skyBool m_bKeybuf[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// �P�t���[���O�̃L�[���͏��
	/// </summary>
	skyBool m_bOldKeybuf[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// �v���O�����L�[���͏��
	/// </summary>
	skyBool m_bProgramKeybuf[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// �P�t���[���O�̃v���O�����L�[���͏��
	/// </summary>
	skyBool m_bOldProgramKeybuf[ eSkyKeybordKeyKind_Max ];


	/// <summary>
	/// ���s�[�g�t���O
	/// </summary>
	skyBool m_RepFlg[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// ���s�[�g�Ԋu�J�E���^�[
	/// </summary>
	s32 m_RepIntvl[ eSkyKeybordKeyKind_Max ];

	/// <summary>
	/// �f���Q�[�g�n���h��
	/// <summary>
	delegate::CDelegateHandler	        m_EventHandler;

	/// <summary>
	/// �L�[���̍X�V
	/// <summary>
	static void _UpdateInputInfo();

	/// <summary>
	/// �L�[���͎�t�֐��̃R�[���o�b�N�X�V
	/// <summary>
	static void _UpdateInputFunc( const fps::CFpsController &fps );

	/// <summary>
	/// �R���X�g���N�^
	/// <summary>
	Ckeyboard(){};
	Ckeyboard( const Ckeyboard& ){};							//�R�s�[�R���X�g���N�^
	Ckeyboard &operator=(const Ckeyboard& ){ return *this; };	//������Z�q�I�[�o�[���[�h
};

} } }

#define CkeyboardInitialize_( playerNum ) sky::lib::input::Ckeyboard::Initialize( playerNum )
#define Ckeyboard_                        sky::lib::input::Ckeyboard::Get()
#define CkeyboardDispose_()               sky::lib::input::Ckeyboard::Dispose()
