#pragma once

namespace sky { namespace lib { namespace input {

//-------------------------------------< ���� >---------------------------------------------//

class CPad;
typedef SmartPointer< CPad >	CPadPtr;

/// <summary>
/// ���͎�t�Ǘ�
/// <summary>
class CPad  : public base::SkyRefObject
{

public:

	/// <summary>
	/// �{�^�����s�[�g�Ԋu�̐ݒ�
	/// <summary>
	void SetBtnRepeat			( s32 repIntvl , s32 onIntvl );

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
	void	KeyOn				( eSkyPadKeyKind key );

	/// <summary>
	/// �L�[����������
	/// <summary>
	skyBool IsKeyOn				( eSkyPadKeyKind key );

	/// <summary>
	/// �L�[��������
	/// <summary>
	skyBool IsKeyRelease		( eSkyPadKeyKind key );

	/// <summary>
	/// �L�[�������㔻��
	/// <summary>
	skyBool IsKeyPress			( eSkyPadKeyKind key );

	/// <summary>
	/// �L�[���s�[�g����
	/// <summary>
	skyBool IsKeyRepeat			( eSkyPadKeyKind key );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPad          (){};

//---------< static >----------

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize				( u32 playerNum );

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose				();

	/// <summary>
	/// �X�V
	/// </summary>
	static void Update			( updateTime time );

	/// <summary>
	/// �p�b�h�擾
	/// </summary>
	static CPadPtr &Get    ( u32 playerNo = CURRENT_PLAYNER_NO );

protected:

	/// <summary>
	/// �p�b�h�̃C���X�^���X
	/// </summary>
	CPadPtr m_KeyBord;

	/// <summary>
	/// �L�[���͏��
	/// </summary>
	skyBool m_bKeybuf[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// �P�t���[���O�̃L�[���͏��
	/// </summary>
	skyBool m_bOldKeybuf[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// ���s�[�g�t���O
	/// </summary>
	skyBool m_RepFlg[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// ���s�[�g�Ԋu�J�E���^�[
	/// </summary>
	s32 m_RepIntvl[ eSkyPadKeyKind_Max ];

	/// <summary>
	/// ���s�[�g�Ԋu
	/// </summary>
	s32 m_nRepIntvlNum;

	/// <summary>
	/// ���s�[�g�Ԋu
	/// </summary>
	s32 m_nOnIntvlNum;

	/// <summary>
	/// �R���g���[��(XInput)�̏��
	/// </summary>
#ifdef SKYLIB_IDE_VC10
    XINPUT_STATE    state;
#endif

	/// <summary>
	/// �R���g���[�����ڑ����Ă��邩�ǂ���
	/// </summary>
	skyBool            bConnected;

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
	static void _UpdateInputFunc( updateTime time );

	/// <summary>
	/// �R���X�g���N�^
	/// <summary>
	CPad()
	{
		m_nRepIntvlNum = C_S32(input_DEF_REPEATE_INTVL);
		m_nOnIntvlNum = C_S32(input_DEF_ON_INTVL);
	}
	CPad( const CPad& ){};							//�R�s�[�R���X�g���N�^
	CPad &operator=(const CPad& ){ return *this; };	//������Z�q�I�[�o�[���[�h
};

} } }

#define CPadInitialize_( playerNum ) sky::lib::input::CPad::Initialize( playerNum )
#define CPad_                        sky::lib::input::CPad::Get()
#define CPadDispose_()               sky::lib::input::CPad::Dispose()
