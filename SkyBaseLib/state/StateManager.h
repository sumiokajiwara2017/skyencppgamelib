#pragma once

namespace sky { namespace lib { namespace state {

/// <summary>
/// �X�e�[�g�Ǘ��N���X
/// �A�v���P�[�V�����̍��{�̏�ԊǗ��͂���ōs���B
/// ��������N�����A�Q�[���N����̓X�N���v�g�ɐ����C������悵�B
/// ���̏ꍇ�̓X�N���v�g���Ǘ�����X�e�[�g���o�t�r�g���Ă����B
/// �X�e�[�g�؂�ւ��͊�{�L���[�Ɉ�U�o�^����`�ōs����B
/// �o�^�����̂łP�t���[������s�ƂȂ�B
/// �X�e�[�g�Ǘ��̓X�^�b�N�ōs����̂ŁA�X�e�[�g���ςݏオ���Ă����B
/// �Ȃ��L���[�Ȃ̂��B����̓X�e�[�g���g�����̃X�e�[�g��PUSH�����莩�����g��POP�����肷��̂ŁB
/// </summary>
class CStateManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CStateManager(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CStateManager();

	/// <summary>
	/// �X�e�[�g�̓o�^
	/// </summary>
	void Register( const hash::CHashStringPtr &name , const IStatePtr &spState );

	/// <summary>
	/// �X�e�[�g�̓o�^����
	/// </summary>
	void UnRegister( const hash::CHashStringPtr &name );

	/// <summary>
	/// �X�e�[�g�̂o�t�r�g
	/// </summary>
	void Push( const hash::CHashStringPtr &name );

	/// <summary>
	/// �X�e�[�g�̂o�n�o
	/// </summary>
	void Pop();

	/// <summary>
	/// �X�e�[�g�̐؂�ւ��i�����̃X�^�b�N���N���A���A�V�����X�e�[�g��ςށj
	/// </summary>
	void Change( const hash::CHashStringPtr &name );

	/// <summary>
	/// �X�e�[�g�̑S�j��
	/// </summary>
	void Clear();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// �w��̃X�e�[�g�̃X�^�b�N��̏�Ɏw��̃X�e�[�g������Ă��邩�`�F�b�N����
	/// </summary>
	skyBool IsUpState( const hash::CHashStringPtr &name , const hash::CHashStringPtr &upname );

	/// <summary>
	/// ������
	/// </summary>
	void Initialize	();

	/// <summary>
	/// ������
	/// </summary>
	void Dispose		();

//---static

	/// <summary>
	/// �C���X�^���X����
	/// EventManager�̓V�X�e����1���݂��邪�A�C���X�^���X���쐬���ă��[�J���Ŏg�p���邱�Ƃ��ł���
	/// </summary>
    static CStateManagerPtr Create();

	//�V���O���g���@�\��t��
	friend class SingletonPtr< CStateManager >;
	static SingletonPtr< CStateManager > Singleton;

private:

	/// <summary>
	/// ���ߎ��
	/// </summary>
	enum eOrder
	{
		eOrder_Push = 0,
		eOrder_Pop,
		eOrder_Change,
		eOrder_Clear,
	};

	/// <summary>
	/// ���ߏ��
	/// </summary>
	struct COrderInfo : public base::SkyRefObject
	{
		eOrder     eId;
		IStatePtr  spState;
	};
	typedef CMemoryPool< COrderInfo >   COrderInfoPool;
	typedef COrderInfo *				COrderInfoPtr;
	typedef CFifo< COrderInfoPtr >	    COrderInfoPtrFifo;

	/// <summary>
	/// �o�^�X�e�[�g�n�b�V��
	/// </summary>
	IStatePtrHash		m_RegisterStateHash;

	/// <summary>
	/// �X�e�[�g�X�^�b�N
	/// </summary>
	IStatePtrStack		m_ActiveStateStack;

	/// <summary>
	/// ���߃L���[
	/// </summary>
	COrderInfoPtrFifo   m_OrderFifo;

	/// <summary>
	/// ���߃L���[���v�[��
	/// </summary>
	COrderInfoPool		m_OrderInfoPool;

	/// <summary>
	/// ���߂��L���[�ɐς�
	/// </summary>
	void PushOrder( eOrder eId , const IStatePtr  &spState = IStatePtr() );

	/// <summary>
	/// ���߃L���[�����s����
	/// </summary>
	void ExecOrderFifo();

	/// <summary>
	/// ���߂����s����
	/// </summary>
	void ExecOrder( COrderInfoPtr pOrderInfo );
};

//�C���X�^���X�A�N�Z�X������
#define CStateManagerCreate_()	sky::lib::state::CStateManager::Singleton.CreateInstance()
#define CStateManager_			sky::lib::state::CStateManager::Singleton.GetInstance()
#define CStateManagerDelete_()	sky::lib::state::CStateManager::Singleton.DeleteInstance()

} } }
