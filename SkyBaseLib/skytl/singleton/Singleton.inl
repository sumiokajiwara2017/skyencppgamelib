namespace sky { namespace lib {

template< typename T , class ThreadingModel  >
inline void Singleton< T , ThreadingModel >::CreateInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	//��x�������͋�����
	SKY_ASSERT_MSG( m_pInstance == skyNull , _T( "It has already been initialized." ) ); //���ł�CreateInstance���Ă��܂��B�Q�x�����������Ȃ��ł��������B

	//�쐬
    m_pInstance = NEW__( T , T() );
}

template< typename T , class ThreadingModel  >
inline T* Singleton< T , ThreadingModel >::GetInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	//2��ڈȍ~��skyNull��Ԃ�
	SKY_ASSERT_MSG( m_pInstance != skyNull , _T( "It has not been initialized yet." ) ); //����������Ă��܂���BCreateInstance�����Ă��������B

	return m_pInstance;
}

template< typename T , class ThreadingModel  >
inline void Singleton< T , ThreadingModel >::DeleteInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	SAFE_DELETE( m_pInstance );
	m_pInstance = skyNull;
}

template< typename T , typename PARAM , class ThreadingModel  >
inline void SingletonParam< T , PARAM , ThreadingModel >::CreateInstance( PARAM param )
{
	thread::CriticalLock Lock( this->m_pCs );

	//��x�������͋�����
	SKY_ASSERT_MSG( m_pInstance == skyNull , _T( "It has already been initialized." ) ); //���ł�CreateInstance���Ă��܂��B�Q�x�����������Ȃ��ł��������B

	//�쐬
	m_pInstance = NEW__( T , T( param ) );
}

template< typename T , typename PARAM , class ThreadingModel  >
inline T* SingletonParam< T , PARAM , ThreadingModel >::GetInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	//2��ڈȍ~��skyNull��Ԃ�
	SKY_ASSERT_MSG( m_pInstance != skyNull , _T( "It has not been initialized yet." ) ); //����������Ă��܂���BCreateInstance�����Ă��������B

	return m_pInstance;
}

template< typename T , typename PARAM , class ThreadingModel  >
inline void SingletonParam< T , PARAM , ThreadingModel >::DeleteInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	SAFE_DELETE( m_pInstance );
	m_pInstance = skyNull;
}


template< typename T , class ThreadingModel  >
inline void SingletonPtr< T , ThreadingModel >::CreateInstance()
{
	
	thread::CriticalLock Lock( this->m_pCs );

	//��x�������͋�����
	SKY_ASSERT_MSG( m_spInstance.IsEmpty() == skyTrue , _T( "It has already been initialized." ) ); //���ł�CreateInstance���Ă��܂��B�Q�x�����������Ȃ��ł��������B

	//�쐬
    m_spInstance.SetPtr( NEW__( T , T() ) );
}

template< typename T , class ThreadingModel  >
inline const SmartPointer< T > &SingletonPtr< T , ThreadingModel >::GetInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	//2��ڈȍ~��skyNull��Ԃ�
	SKY_ASSERT_MSG( m_spInstance.IsEmpty() == skyFalse , _T( "It has not been initialized yet." ) ); //����������Ă��܂���BCreateInstance�����Ă��������B

	return m_spInstance;
}

template< typename T , class ThreadingModel  >
inline void SingletonPtr< T , ThreadingModel >::DeleteInstance()
{
	thread::CriticalLock Lock( this->m_pCs );

	m_spInstance.Delete();
}

} }
