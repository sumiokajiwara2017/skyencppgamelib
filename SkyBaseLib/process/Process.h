#pragma once

namespace sky { namespace lib { namespace process {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

/// <summary>
/// �v���Z�X�N���X
/// �����t���[���Ԃɂ܂����鏈���͂��ׂăv���Z�X�Ƃ��Ĉ����B
/// �o�^�E���s�E���f�E�L�����Z�����ǂ�ȃ^�C�~���O�ł��\�B
/// </summary>
class IProcess : virtual public base::SkyRefObject
{
    SkyTypeDefRTTI;

friend class CProcessManager;

public:

	/// <summary>
	/// �X�V���ʎ�ʁi�}�l�[�W���[��������󂯎�菈���𕪂��܂��j
	/// </summary>
	enum eState
	{
		eState_Wait = 0,
		eState_Initialize,
		eState_Update,
		eState_Dispose, //�����܂ł�����v���Z�X�Ǘ����珟��Ɋ֘A�t����������܂��B
	};

	/// <summary>
	/// �X�V���ʎ�ʁi�}�l�[�W���[��������󂯎�菈���𕪂��܂��j
	/// </summary>
	enum eResult
	{
		eResult_Continue = 0,	//�p��
		eResult_End,			//�I��
	};

	/// <summary>
	/// �v���Z�X��Ԃ̎擾�^�ݒ�i�}�l�[�W���[������j
	/// </summary>
	virtual eState	vGetState() = 0;
	virtual void	vSetState( eState state ) = 0;

	/// <summary>
	/// ������
	/// </summary>
	virtual void	vInitialize() = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void	vDispose() = 0;

	/// <summary>
	/// �O�X�V
	/// </summary>
	virtual void	vPreUpdate( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual eResult vUpdate( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// ��X�V
	/// </summary>
	virtual void	vPostUpdate( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// ���O�擾
	/// </summary>
	virtual const hash::CHashStringPtr &vGetName() = 0;

	/// <summary>
	/// �D�揇�ʎ擾�^�ݒ�
	/// </summary>
	virtual void vSetPrio( u32 prio ) = 0;
	virtual u32	 vGetPrio() = 0;

	/// <summary>
	/// �Ǘ��̊֘A�t���^�֘A�t�������^�擾
	/// </summary>
	virtual void  vAttachManager( const CProcessManagerPtr &spManager ) = 0;
	virtual void  vDetachManager() = 0;
	virtual const CProcessManagerPtr &vGetManager() = 0;

	/// <summary>
	/// �q�v���Z�X�̊֘A�t���^�֘A�t�������^�ċA�������E�X�V�E�������i���̃v���Z�X���X�V���ꂽ��q�v���Z�X���X�V����܂��j
	/// </summary>
	virtual void vAttachChildProcess	( IProcessPtr &process ) = 0;
	virtual void vDetachChildProcess	( IProcessPtr &process ) = 0;
	virtual void vInitializeChildProcess() = 0;
	virtual void vDisposeChildProcess	() = 0;
	virtual void vPreUpdateChildProcess	( const fps::CFpsController &fps ) = 0;
	virtual void vUpdateChildProcess	( const fps::CFpsController &fps ) = 0;
	virtual void vPostUpdateChildProcess( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// ���̃v���Z�X�̊֘A�t���^�֘A�t�������i���̃v���Z�X���I���������̃t���[�����J�n����܂��j
	/// </summary>
	virtual void vAttachNextProcess( IProcessPtr &process ) = 0;
	virtual void vDetachnextProcess( IProcessPtr &process ) = 0;

private:

	/// <summary>
	/// ���̃v���Z�X���X�g�擾
	/// </summary>
	virtual IProcessPtrList	&GetNextProcessList() = 0;

};

//-----------------------------------< ���ێ��� >-------------------------------------------//

class AbsProcess : public IProcess
{

public:

	/// <summary>
	/// ����
	/// </summary>
	virtual void	vInitialize()                               {  }; ///�K�v�ɉ����ăI�[�o�[���C�h
	virtual void	vDispose()                                  {  }; //�K�v�ɉ����ăI�[�o�[���C�h
	virtual void	vPreUpdate( updateTime time )               { SKY_UNUSED_ARG( time ); }; //�K�v�ɉ����ăI�[�o�[���C�h
	virtual eResult vUpdate( updateTime time )                  { SKY_UNUSED_ARG( time ); return eResult_End;  }; //�K�v�ɉ����ăI�[�o�[���C�h
	virtual void	vPostUpdate( updateTime time )              { SKY_UNUSED_ARG( time );  }; //�K�v�ɉ����ăI�[�o�[���C�h

	void   vSetPrio( u32 prio )							        { m_Prio = prio; }
	u32    vGetPrio()										    { return m_Prio; }
	void   vAttachManager( const CProcessManagerPtr &spManager ){ m_spManager = spManager; }
	void   vDetachManager()                                     { m_spManager.Delete(); }
	const  CProcessManagerPtr &vGetManager()                    { return m_spManager; }

	eState vGetState()										    { return m_eState; }
	void   vSetState( eState state )						    { m_eState = state; }
	void   vAttachChildProcess	( IProcessPtr &process )        { m_ChildProcessList.AddTail( process ); };
	void   vDetachChildProcess	( IProcessPtr &process )        { m_ChildProcessList.Delete( process ); };
	void   vInitializeChildProcess()
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vInitialize();
			( *it )->vInitializeChildProcess();
		}
	}
	void   vDisposeChildProcess	()
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vDispose();
			( *it )->vDisposeChildProcess();
		}
	}
	void   vPreUpdateChildProcess  ( const fps::CFpsController &fps )		
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vPreUpdate( fps );
			( *it )->vPreUpdateChildProcess( fps );
		}
	}
	void   vUpdateChildProcess  ( const fps::CFpsController &fps )
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vUpdate( fps );
			( *it )->vUpdateChildProcess( fps );
		}
	}
	void   vPostUpdateChildProcess  ( const fps::CFpsController &fps )
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vPostUpdate( fps );
			( *it )->vPostUpdateChildProcess( fps );
		}
	}
	void   vAttachNextProcess	( IProcessPtr &process )	{ m_NextProcessList.AddTail( process ); };
	void   vDetachnextProcess	( IProcessPtr &process )	{ m_NextProcessList.Delete( process ); };
	IProcessPtrList	&GetNextProcessList(){ return m_NextProcessList; }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~AbsProcess(){};

	/// <summary>
	/// ���O�̎擾
	/// </summary>
	const hash::CHashStringPtr &vGetName(){ return m_Name; }

private:

	/// <summary>
	/// �����̊Ǘ���
	/// </summary>
	CProcessManagerPtr     m_spManager;

	/// <summary>
	/// �D�揇��
	/// </summary>
	u32						m_Prio;

	/// <summary>
	/// ���O
	/// </summary>
	hash::CHashStringPtr	m_Name;

	/// <summary>
	/// ���
	/// </summary>
	eState					m_eState;

	/// <summary>
	/// �q�v���Z�X
	/// </summary>
	IProcessPtrList			m_ChildProcessList;

	/// <summary>
	/// ���̃v���Z�X
	/// </summary>
	IProcessPtrList			m_NextProcessList;
};


} } }
