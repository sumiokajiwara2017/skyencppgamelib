#pragma once

namespace sky { namespace lib { namespace event {

//-------------------------------------< ���� >---------------------------------------------//

class CEventManager;
typedef SmartPointer< CEventManager >	CEventManagerPtr;
typedef CList< u32 , CEventManagerPtr >	CEventManagerPtrList;

/// <summary>
/// �C�x���g�Ǘ��N���X
/// </summary>
class CEventManager : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CEventManager();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CEventManager();

	//�V���O���g���@�\��t��
	friend class SingletonPtr< CEventManager >;
	static SingletonPtr< CEventManager > SingletonMember;

	/// <summary>
	/// �P�t���[���Ŏ��s����C�x���g�̐�
	/// </summary>
	void SetOneFrameEventExecNum( u32 eventExecNum );

	/// <summary>
	/// �C�x���g��o�^����
	/// </summary>
	void RegisterEvent		( const EventId &eventId );
	
	/// <summary>
	/// �C�x���g��o�^����������
	/// </summary>
	void UnRegisterEvent		( const EventId &eventId );

	/// <summary>
	/// �C�x���g��S�j������
	/// </summary>
	void DeleteAllEvent	();

	/// <summary>
	/// �C�x���g�𖳌��ɂ���
	/// </summary>
	void DisableEvent	( const EventId &eventId );

	/// <summary>
	/// �C�x���g��L���ɂ���
	/// </summary>
	void EnableEvent		( const EventId &eventId );

	/// <summary>
	/// �C�x���g���X�i�[��o�^
	/// </summary>
	void AttachEventListener( const EventId &eventId , const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// �C�x���g���X�i�[��o�^����
	/// </summary>
	void DetachEventListener( const EventId &eventId , const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// �C�x���g�𓯊��Ŕ���������
	/// </summary>
	void ExecuteEvent	( const EventId &eventId , const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// �C�x���g��x���Ŕ���������i���s�̃^�C�~���O�̓Q�[���t���[�����[�N��CEventManager::Update()���ǂ��ŌĂ΂��̂��Ō��܂�B�Q�[�����[�v�̍ŏ��ŌĂԂ̂�������₷���ƍl����j
	/// </summary>
	void ExecuteEventAsync	( const EventId &eventId , u32 uniqId = DEFAULT_UNIQ_ID , const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() , u32 startIntvl = 0 , u32 execCount = 1 , u32 execIntvl = 0 );

	/// <summary>
	/// �񓯊������������C�x���g������������
	/// </summary>
	void CanselExecuteEventAsync	( const EventId &eventId , u32 uniqId = DEFAULT_UNIQ_ID );

	/// <summary>
	/// �񓯊������������C�x���g��S����������
	/// </summary>
	void AllCanselExecuteEventAsync	();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// �C�x���g���s���O�o�̓X�g���[���ݒ�
	/// </summary>
	void SetOutLogStream		( stream::IStreamPtr &stream );

	/// <summary>
	/// �C�x���g���s���o�C�i���o�͂��邽�߂̃X�g���[����ݒ�
	/// </summary>
	void SetSaveExecBinary	( stream::IStreamPtr &stream );

	/// <summary>
	/// �C�x���g���s���O��ǂݍ��݃C�x���g���Đ�����
	/// </summary>
	void PlayExecBinary  	( stream::IStreamPtr &stream );

//Static

	/// <summary>
	/// �C���X�^���X����
	/// EventManager�̓V�X�e����1���݂��邪�A�C���X�^���X���쐬���ă��[�J���Ŏg�p���邱�Ƃ��ł���
	/// </summary>
    static CEventManagerPtr Create();

private:

	/// <summary>
	/// �C�x���g�o�^���
	/// </summary>
	struct CEventData
	{
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~CEventData(){}

		/// <summary>
		/// �C�x���gID
		/// </summary>
		EventId eventId;

		/// <summary>
		/// �L�������t���O
		/// </summary>
		skyBool IsEnable;

		/// <summary>
		/// �C�x���g�n���h��
		/// </summary>
		delegate::CDelegateHandler EventHandler;

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		CEventData() : IsEnable( skyTrue ) {}

	};
	typedef CMemoryPool< CEventData >			CEventDataPool;
	typedef CEventData *						CEventDataPtr;
	typedef CHash< CEventDataPtr >				CEventDataPtrHash;

	/// <summary>
	/// �C�x���g�Đ����
	/// </summary>
	struct CEventExecuteData
	{
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~CEventExecuteData(){}

		/// <summary>
		/// �C�x���g���茳
		/// </summary>
		CommonSmartPointer spSender;
			
		/// <summary>
		/// �f���Q�[�g�p�����[�^�[
		/// </summary>
		delegate::CDelegateArgs Args;

		/// <summary>
		/// �C�x���gID
		/// </summary>
		EventId eventId;

		/// <summary>
		/// ���j�[�NID
		/// </summary>
		u32 UniqId;

		/// <summary>
		/// �L�������t���O
		/// </summary>
		skyBool IsEnable;

		/// <summary>
		/// �L�����Z���t���O
		/// </summary>
		skyBool IsCancel;

		/// <summary>
		/// �J�n�҂��t���[����
		/// </summary>
		u32 StartIntvl;

		/// <summary>
		/// ���s��( -1 �͖����j
		/// </summary>
		s32 ExecCount;

		/// <summary>
		/// ���s�Ԋu�t���[����
		/// </summary>
		u32 ExecIntvl;

		/// <summary>
		/// ���s�Ԋu�t���[�����J�E���^�[
		/// </summary>
		u32 ExecIntvlCounter;

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		CEventExecuteData() :
			StartIntvl( 0 ) ,
			IsCancel( skyFalse ) ,
			IsEnable( skyTrue ) ,
			ExecCount( 1 ) ,
			ExecIntvl( 0 ) ,
			ExecIntvlCounter( 0 )
		{}

		/// <summary>
		/// ������Z�q�I�[�o�[���C�h
		/// </summary>
		virtual CEventExecuteData& operator =( const CEventExecuteData& src )
		{
			spSender        = src.spSender;
			Args            = src.Args;
			eventId         = src.eventId;
			IsEnable        = src.IsEnable;
			IsCancel        = src.IsCancel;
			StartIntvl      = src.StartIntvl;
			ExecCount       = src.ExecCount;
			ExecIntvl       = src.ExecIntvl;
			return *this;
		}

	};
	typedef CList< u32 , CEventExecuteData >		CEventExecuteDataFifo;

	/// <summary>
	/// �C�x���g���s��
	/// </summary>
	u32								m_EventExecNum;

	/// <summary>
	/// �C�x���g�o�^���e�[�u��
	/// </summary>
	CEventDataPtrHash				*m_pHandlerHash;

	/// <summary>
	/// �C�x���g�o�^���v�[��
	/// </summary>
	CEventDataPool					*m_pEventDataPool;

	/// <summary>
	/// �C�x���g���s���L���[
	/// </summary>
	CEventExecuteDataFifo			*m_pEventFifo;

	/// <summary>
	/// ���O�o�̓X�g���[��
	/// </summary>
	stream::IStreamPtr				*m_spOutLogStream;

	/// <summary>
	/// �Đ��o�C�i���o�̓X�g���[��
	/// </summary>
	stream::IStreamPtr				*m_spSaveExecuteBinaryStream;

};

//�C���X�^���X�A�N�Z�X������
#define CEventManagerCreate_()	sky::lib::event::CEventManager::SingletonMember.CreateInstance()
#define CEventManager_			sky::lib::event::CEventManager::SingletonMember.GetInstance()
#define CEventManagerDelete_()	sky::lib::event::CEventManager::SingletonMember.DeleteInstance()

} } }
