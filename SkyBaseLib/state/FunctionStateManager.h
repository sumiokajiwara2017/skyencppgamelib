#pragma once

namespace sky { namespace lib { namespace state {

/// <summary>
/// �֐��X�e�[�g�}�V���i�P��^�p�j
/// </summary>
template< class T , typename ID >
class CFunctionStateManager
{

public:

	/// <summary>
	/// �֐��^
	/// </summary>
	typedef skyBool ( T::*FUNCTION )( const fps::CFpsController & );
	typedef void    ( T::*FUNCTIONSUB )();
	typedef skyBool ( T::*FUNCTIONLOAD )();
	typedef CFifo< ID >              StateQueue;
	typedef CList< u32 , ID >        StateList;

	/// <summary>
	/// �X�e�[�g�֐����
	/// </summary>
	struct FUNCTIONData
	{
		/// <summary>
		/// �X�e�[�g�֐��Q
		/// </summary>
		FUNCTIONLOAD m_Load;   //���[�h������������skyTrue��Ԃ�GetNextState�֐��Ŏ��̃X�e�[�g��ID�𓾂Ď��̃X�e�[�g��m_Load���Ă��skyTrue������܂ő҂��Č��݂̃X�e�[�g��`�悵�AskyTrue�����Ă���J�ڂ����烍�[�h���X�ɂȂ�
		FUNCTIONSUB  m_Init;   //���߂ăX�e�[�g�ɗ������Ɉ�񂾂�
		FUNCTIONSUB  m_Start;  //�X�e�[�g������x�Ɉ�񂾂�
		FUNCTION     m_Update; //���t���[���X�V
		FUNCTIONSUB  m_End;    //�X�e�[�g�������x�Ɉ�x����

		/// <summary>
		/// �t���O
		/// </summary>
		skyBool      m_IsInitExec;

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		void Constractor()
		{
			m_Load   = skyNull;
			m_Init   = skyNull;
			m_Start  = skyNull;
			m_Update = skyNull;
			m_End    = skyNull;
			m_IsInitExec = skyFalse;
		}
		FUNCTIONData()
		{
			Constractor();
		}
		FUNCTIONData( FUNCTIONLOAD load , FUNCTIONSUB init , FUNCTIONSUB start , FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Load  = load;
			m_Init  = init;
			m_Start  = start;
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTIONSUB init , FUNCTIONSUB start , FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Init  = init;
			m_Start  = start;
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTIONSUB start , FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Start  = start;
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTIONSUB start , FUNCTION update )
		{
			Constractor();
			m_Start  = start;
			m_Update = update;
		}
		FUNCTIONData( FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTION update )
		{
			Constractor();
			m_Update = update;
		}
	};

	typedef CList< ID , FUNCTIONData >  FUNCTIONList;

	/// <summary>
	/// �֐���o�^
	/// </summary>
	inline void RegisterState( ID id , FUNCTION func )
	{
		m_FunctionList.AddTail( id , FUNCTIONData( func ) );
	}
	inline void RegisterStateData( ID id , const FUNCTIONData &funcData )
	{
		m_FunctionList.AddTail( id , funcData );
	
	}

	/// <summary>
	/// ���݂̃t���[���J�E���^�[�̒l���擾
	/// </summary>
	inline u32 GetFrameCount()
	{
		return m_FrameCount;
	}

	/// <summary>
	/// ���݂̃X�e�[�g���擾
	/// </summary>
	inline ID GetState()
	{
		return m_CurrentState;
	}

	/// <summary>
	/// ���̃X�e�[�g���擾(skyFalse�Ŏ������݂��Ȃ��j
	/// </summary>
	inline skyBool GetNextState( ID &dest )
	{
		skyBool result = skyFalse;
		if ( m_StateQueue.GetNum() > 0 )
		{
			dest = m_StateQueue.GetFront();
		}
		return result;
	}

	/// <summary>
	/// ���݃X�e�[�g��m_Load���Ă�
	/// </summary>
	inline skyBool ExecCurrentStateLoad()
	{
		skyBool result = skyTrue; //�����Ȃ������犮���Ƃ���
		if( m_CurrentFunction.m_Load != skyNull )
		{
			result = ( m_pObject->*m_CurrentFunction.m_Load )();
		}
		return result;
	}

	/// <summary>
	/// ���̃X�e�[�g��m_Load���Ă�
	/// </summary>
	inline skyBool ExecNextStateLoad()
	{
		skyBool result = skyTrue; //�����Ȃ������犮���Ƃ���

		if ( m_StateQueue.GetNum() > 0 )
		{
			typename FUNCTIONData& next = m_FunctionList.GetIdx( m_StateQueue.GetFront() );

			if( next.m_Load != skyNull )
			{
			    result = ( m_pObject->*next.m_Load )();
			}
		}

		return result;
	}

	/// <summary>
	/// ���݃X�e�[�g�ɗ��܂��Ă���L���[�̐����擾����
	/// </summary>
	inline u32 GetQueueNum()
	{
		return m_StateQueue.GetNum();
	}

	/// <summary>
	/// ���݂̃X�e�[�g���I��������
	/// </summary>
	inline void EndCurrentState()
	{
		m_IsStateEnd = skyTrue;
	}

	/// <summary>
	/// �X�e�[�g�L���[�̓o�^
	/// </summary>
	inline void PushState( ID state )
	{
		m_StateQueue.Push( state );
	}

	/// <summary>
	/// �X�e�[�g�L���[�̃L�����Z��
	/// </summary>
	inline void CancelState( ID state )
	{
		m_StateQueue.SetCancelFlag( state , skyTrue );
	}

	/// <summary>
	/// �X�e�[�g�L���[�̑S�L�����Z��
	/// </summary>
	inline u32 CancelAllState()
	{
		m_StateQueue.AllPop();
	}

	/// <summary>
	/// �O���[�o���X�e�[�g�̒ǉ�
	/// </summary>
	inline void AddGlobalState( ID state )
	{
		m_GrobalStateList.AddTail( state );
	}

	/// <summary>
	/// �O���[�o���X�e�[�g�̍폜
	/// </summary>
	inline void DeleteGlobalState( ID state )
	{
		m_GrobalStateList.Delete( state );
	}

	/// <summary>
	/// �X�V
	/// </summary>
	inline skyBool Update( const fps::CFpsController &fps )
	{
		//���s���t���O
		skyBool IsExec = skyTrue;

		if ( m_IsStateEnd == skyFalse )
		{
			//�X�e�[�g�֐����s�iskyTrue���A���Ă����ꍇ�I������j
			m_IsStateEnd = ( m_pObject->*m_CurrentFunction.m_Update )( fps );

			//�t���[���J�E���^�[UP
			m_FrameCount++;
		}

		//��L��m_IsStateEnd��True�ɂȂ����ꍇ�Ԕ����ꂸ�Ɏ��̃X�e�[�g�𔭐i������B
		//1�t���󂭂����ŗl�X�Ȗ�肪��������ꍇ����������B
		if ( m_IsStateEnd == skyTrue )
		{
			//�L�����Z������Ă��Ȃ��X�e�[�g��擪�ɂ���
			skyBool isSeach = skyTrue;
			while( m_StateQueue.GetNum() > 0 && isSeach )
			{
				if ( m_StateQueue.IsFrontCancel() )
				{
					m_StateQueue.Pop();
				}
				else
				{
					isSeach = skyFalse;
				}
			}

			//�X�e�[�g�擾
			if ( m_StateQueue.GetNum() > 0 )
			{
				if ( m_CurrentFunction.m_End != skyNull )
				{
					( m_pObject->*m_CurrentFunction.m_End )();
				}

				//�O�̃X�e�[�g�h�c������Ă����i�����̔���Ɏg�p����j
				m_PrevState    = m_CurrentState;

				m_StateQueue.Pop( m_CurrentState );

                SKY_ASSERT_MSG( m_CurrentState < ( s32 )m_FunctionList.GetNum() , _T( "This has the possibility of the state not registered. Please used RegisterState().\n" ) );

				m_CurrentFunction = m_FunctionList.GetIdx( m_CurrentState );
				m_FrameCount  = 0;
				m_IsStateEnd   = skyFalse;

				if ( m_CurrentFunction.m_Init != skyNull && m_CurrentFunction.m_IsInitExec == skyFalse )
				{
					( m_pObject->*m_CurrentFunction.m_Init )();
					m_CurrentFunction.m_IsInitExec = skyTrue;
				}

				if ( m_CurrentFunction.m_Start != skyNull )
				{
					( m_pObject->*m_CurrentFunction.m_Start )();
				}
			}
			else
			{
				//�������s���Ă��Ȃ�
				IsExec = skyFalse;
			}
		}

		//�O���[�o���X�e�[�g�̎��s
		FOREACH( StateList , it , m_GrobalStateList )
		{
			typename FUNCTIONData& function = m_FunctionList.GetIdx( *it );
		    ( m_pObject->*function.m_Update )( fps );
		}

		return IsExec;
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFunctionStateManager( T *pParent , u32 queueSize = fifo::DEFAULT_FIFO_SIZE ) : 
	m_FrameCount( 0 ) , m_IsStateEnd( skyTrue ) , m_StateQueue( queueSize ) , m_CurrentFunction( skyNull )
	{
		m_pObject = pParent;
	}

private:
	
	/// <summary>
	/// �e�N���X
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// �֐����X�g
	/// </summary>
	FUNCTIONList m_FunctionList;

	/// <summary>
	/// �X�V
	/// </summary>
	u32          m_FrameCount;

	/// <summary>
	/// �X�e�[�g�I���t���O
	/// </summary>
	skyBool      m_IsStateEnd;

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	ID           m_PrevState;
	ID           m_CurrentState;
	FUNCTIONData m_CurrentFunction;

	/// <summary>
	/// �X�e�[�g�L���[
	/// </summary>
	StateQueue  m_StateQueue;

	/// <summary>
	/// �O���[�o���X�e�[�g�i�ʏ�̃X�e�[�g�̌�ɌĂ΂��j
	/// </summary>
	StateList m_GrobalStateList;
};

class FUNCTIONDelegateDataBase;
//typedef SmartPointer< FUNCTIONDelegateDataBase >	FUNCTIONDelegateDataBasePtr;				//�X�}�[�g�|�C���^�Œ�`
typedef FUNCTIONDelegateDataBase*	                FUNCTIONDelegateDataBasePtr;				//�|�C���^��`
typedef CList< u32 , FUNCTIONDelegateDataBasePtr >  FUNCTIONDelegateDataBasePtrList;

/// <summary>
/// �X�e�[�g�֐�����{�N���X
/// </summary>
struct FUNCTIONDelegateDataBase : public base::SkyRefObject
{
	/// <summary>
	/// ���[�h���Ă�
	/// </summary>
	virtual skyBool Load() = 0;

	/// <summary>
	/// ���������Ă�
	/// </summary>
	virtual void    Init() = 0; 

	/// <summary>
	/// �J�n���Ă�
	/// </summary>
	virtual void    Start() = 0; 

	/// <summary>
	/// �X�V���Ă�
	/// </summary>
	virtual skyBool Update( const fps::CFpsController & ) = 0; 

	/// <summary>
	/// �I�����Ă�
	/// </summary>
	virtual void    End() = 0; 
};

/// <summary>
/// �X�e�[�g�֐����
/// </summary>
template< class T >
struct FUNCTIONDelegateData : public FUNCTIONDelegateDataBase
{
	typedef skyBool ( T::*FUNCTION )( const fps::CFpsController & );
	typedef void    ( T::*FUNCTIONSUB )();
	typedef skyBool ( T::*FUNCTIONLOAD )();

	/// <summary>
	/// �f���Q�[�g�^
	/// </summary>
	T*           m_pObject;

	/// <summary>
	/// �t���O
	/// </summary>
	skyBool      m_IsInitExec;

	/// <summary>
	/// �X�e�[�g�֐��Q
	/// </summary>
	FUNCTIONLOAD m_Load;   //���[�h������������skyTrue��Ԃ�GetNextState�֐��Ŏ��̃X�e�[�g��ID�𓾂Ď��̃X�e�[�g��m_Load���Ă��skyTrue������܂ő҂��Č��݂̃X�e�[�g��`�悵�AskyTrue�����Ă���J�ڂ����烍�[�h���X�ɂȂ�
	FUNCTIONSUB  m_Init;   //���߂ăX�e�[�g�ɗ������Ɉ�񂾂�
	FUNCTIONSUB  m_Start;  //�X�e�[�g������x�Ɉ�񂾂�
	FUNCTION     m_Update; //���t���[���X�V
	FUNCTIONSUB  m_End;    //�X�e�[�g�������x�Ɉ�x����

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	void Constractor()
	{
		m_pObject = skyNull;
		m_Load    = skyNull;
		m_Init    = skyNull;
		m_Start   = skyNull;
		m_Update  = skyNull;
		m_End     = skyNull;
		m_IsInitExec = skyFalse;
	}
	FUNCTIONDelegateData()
	{
		Constractor();
	}
	FUNCTIONDelegateData( T* pObject , FUNCTION update , FUNCTIONSUB init = skyNull , FUNCTIONLOAD load = skyNull , FUNCTIONSUB start = skyNull , FUNCTIONSUB end = skyNull )
	{
		Constractor();
		m_pObject = pObject;
		m_Load    = load;
		m_Init    = init;
		m_Start   = start;
		m_Update  = update;
		m_End     = end;
	}

	/// <summary>
	/// ����
	/// </summary>
	skyBool Load()
	{
		if ( m_Load == skyNull ) return skyTrue;
		return ( m_pObject->*m_Load )();
	}
	void Init()
	{
		if ( m_Init == skyNull ) return;
		( m_pObject->*m_Init )();
	}
	void Start()
	{
		if ( m_Start == skyNull ) return;
		( m_pObject->*m_Start )();
	}
	skyBool Update( const fps::CFpsController &fps )
	{
		if ( m_Update == skyNull ) return skyTrue;
		return ( m_pObject->*m_Update )( fps );
	}
	void End()
	{
		if ( m_End == skyNull ) return;
		( m_pObject->*m_End )();
	}
};

template< typename ID >
class CFunctionDelgateStateManager
{

public:

	typedef CFifo< ID >              StateQueue;
	typedef CList< u32 , ID >        StateList;

	/// <summary>
	/// ���݃X�e�[�g��m_Load���Ă�
	/// </summary>
	inline skyBool ExecCurrentStateLoad()
	{
		if ( m_CurrentFunction != skyNull )
		{
			return m_CurrentFunction->Load();
		}
		else
		{
			return skyTrue;
		}
	}

	/// <summary>
	/// ���̃X�e�[�g��m_Load���Ă�
	/// </summary>
	inline skyBool ExecNextStateLoad()
	{
		skyBool result = skyTrue; //�����Ȃ������犮���Ƃ���

		if ( m_StateQueue.GetNum() > 0 )
		{
			typename FUNCTIONDelegateDataBasePtr &next = m_FunctionList.GetIdx( m_StateQueue.GetFront() );

		    result = next->Load();
		}

		return result;
	}

	/// <summary>
	/// �X�e�[�g�̓o�^
	/// </summary>
	inline void RegisterStateData( ID id , const FUNCTIONDelegateDataBasePtr &funcData )
	{
		m_FunctionList.AddTail( id , funcData );
	}

	/// <summary>
	/// ���݃X�e�[�g�ɗ��܂��Ă���L���[�̐����擾����
	/// </summary>
	inline u32 GetQueueNum()
	{
		return m_StateQueue.GetNum();
	}

	/// <summary>
	/// ���݂̃X�e�[�g���I��������
	/// </summary>
	inline void EndCurrentState()
	{
		m_IsStateEnd = skyTrue;
	}

	/// <summary>
	/// �X�e�[�g�L���[�̓o�^
	/// </summary>
	inline void PushState( ID state )
	{
		m_StateQueue.Push( state );
	}

	/// <summary>
	/// �X�e�[�g�L���[�̃L�����Z��
	/// </summary>
	inline void CancelState( ID state )
	{
		m_StateQueue.SetCancelFlag( state , skyTrue );
	}

	/// <summary>
	/// �X�e�[�g�L���[�̑S�L�����Z��
	/// </summary>
	inline u32 CancelAllState()
	{
		m_StateQueue.AllPop();
	}

	/// <summary>
	/// �O���[�o���X�e�[�g�̒ǉ�
	/// </summary>
	inline void AddGlobalState( ID state )
	{
		m_GrobalStateList.AddTail( state );
	}

	/// <summary>
	/// �O���[�o���X�e�[�g�̍폜
	/// </summary>
	inline void DeleteGlobalState( ID state )
	{
		m_GrobalStateList.Delete( state );
	}

	/// <summary>
	/// �X�V
	/// </summary>
	inline skyBool Update( const fps::CFpsController &fps )
	{
		//���s���t���O
		skyBool IsExec = skyTrue;

		if ( m_IsStateEnd == skyFalse )
		{
			//�X�e�[�g�֐����s�iskyTrue���A���Ă����ꍇ�I������j
			m_IsStateEnd = m_CurrentFunction->Update( fps );

			//�X�e�[�g���I�������I��
			if( m_IsStateEnd == skyTrue )
			{
				m_CurrentFunction->End();
			}

			//�t���[���J�E���^�[UP
			m_FrameCount++;
		}

		//��L��m_IsStateEnd��True�ɂȂ����ꍇ�Ԕ����ꂸ�Ɏ��̃X�e�[�g�𔭐i������B
		//1�t���󂭂����ŗl�X�Ȗ�肪��������ꍇ����������B
		if ( m_IsStateEnd == skyTrue )
		{
			//�L�����Z������Ă��Ȃ��X�e�[�g��擪�ɂ���
			skyBool isSeach = skyTrue;
			while( m_StateQueue.GetNum() > 0 && isSeach )
			{
				if ( m_StateQueue.IsFrontCancel() )
				{
					m_StateQueue.Pop();
				}
				else
				{
					isSeach = skyFalse;
				}
			}

			//�X�e�[�g�擾
			if ( m_StateQueue.GetNum() > 0 )
			{
				//�O�̃X�e�[�g�h�c������Ă����i�����̔���Ɏg�p����j
				m_PrevState    = m_CurrentState;

				m_StateQueue.Pop( m_CurrentState );

                SKY_ASSERT_MSG( m_CurrentState < ( s32 )m_FunctionList.GetNum() , _T( "This has the possibility of the state not registered. Please used RegisterState().\n" ) );

				m_CurrentFunction = m_FunctionList.GetIdx( m_CurrentState );
				m_FrameCount  = 0;
				m_IsStateEnd   = skyFalse;

				m_CurrentFunction->Init();

				m_CurrentFunction->Start();
			}
			else
			{
				//�������s���Ă��Ȃ�
				IsExec = skyFalse;
			}
		}

		//�O���[�o���X�e�[�g�̎��s
		FOREACH( StateList , it , m_GrobalStateList )
		{
			typename FUNCTIONDelegateDataBasePtr& function = m_FunctionList.GetIdx( *it );
			function->Update( fps );
		}

		return IsExec;
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFunctionDelgateStateManager( u32 queueSize = fifo::DEFAULT_FIFO_SIZE ) : 
	m_FrameCount( 0 ) , m_IsStateEnd( skyTrue ) , m_StateQueue( queueSize ) , m_CurrentFunction( skyNull )
	{
	}

private:
	
	/// <summary>
	/// �֐����X�g
	/// </summary>
	FUNCTIONDelegateDataBasePtrList m_FunctionList;

	/// <summary>
	/// �X�V
	/// </summary>
	u32          m_FrameCount;

	/// <summary>
	/// �X�e�[�g�I���t���O
	/// </summary>
	skyBool      m_IsStateEnd;

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	ID           m_PrevState;
	ID           m_CurrentState;
	FUNCTIONDelegateDataBasePtr m_CurrentFunction;

	/// <summary>
	/// �X�e�[�g�L���[
	/// </summary>
	StateQueue  m_StateQueue;

	/// <summary>
	/// �O���[�o���X�e�[�g�i�ʏ�̃X�e�[�g�̌�ɌĂ΂��j
	/// </summary>
	StateList m_GrobalStateList;
};

} } }
