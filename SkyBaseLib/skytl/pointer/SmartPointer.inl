namespace sky { namespace lib {

template< class T ,	class ThreadingModel >
inline SmartPointer< T , ThreadingModel >::SmartPointer( const T *src , skyBool bIsNoDelete , s32 add ) :
m_bIsNoDelete( bIsNoDelete )
{
	m_pPtr		= const_cast< T * >( src );
	if ( m_pPtr != skyNull )
	{
		m_pPtr->SetRefCnt( add );
		m_pPtr->RefCntUp();
	}
}

template< class T ,	class ThreadingModel >
inline SmartPointer< T , ThreadingModel >::SmartPointer( const SmartPointer< T , ThreadingModel > &src )
{
	// ����̃|�C���^���R�s�[
	m_pPtr				= src.m_pPtr;
	m_bIsNoDelete		= src.m_bIsNoDelete;

	if ( m_pPtr != skyNull )
	{
		// �������g�̎Q�ƃJ�E���^�𑝉�
		m_pPtr->RefCntUp();
	}
}

template< class T , class ThreadingModel >
template< class T2 >
inline SmartPointer< T , ThreadingModel >::SmartPointer( const SmartPointer< T2 , ThreadingModel > &src ) :
m_bIsNoDelete( skyFalse )
{
	if ( src.IsEmpty() == skyTrue )
	{
		//���葤��NULL�ɂ��ďI��
		m_pPtr = skyNull;
	}
	else
	{
		// ����̃|�C���^���R�s�[
	//	m_pPtr				= dynamic_cast< T * >( const_cast< T2 * >( src.GetPtr() ) );
		m_pPtr		        = SkyDynamicCast( T , src.GetPtrNoConst() ); //���z�e�N���X�i�p������virtual���Ă������Ƃő��d���N���X�p�����\�ɂ����j������Ƃ����ŃR���p�C���G���[
		m_bIsNoDelete		= src.IsNodelete();

		SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "It failed in dynamic Cast."  ) ); //�_�C�i�~�b�N�L���X�g�Ɏ��s���܂����B�L���X�g���悤�Ƃ����^�ł͂Ȃ��悤�ł��B

		// �������g�̎Q�ƃJ�E���^�𑝉�
		m_pPtr->RefCntUp();
	}
}

template< class T ,	class ThreadingModel >
inline SmartPointer< T , ThreadingModel >::~SmartPointer()
{
	Release(); //�Q�ƃ|�C���^�������āA�I�u�W�F�N�g��j�����ɂ���
}

template< class T ,	class ThreadingModel >
inline SmartPointer< T , ThreadingModel >& SmartPointer< T , ThreadingModel >::operator =( const SmartPointer< T , ThreadingModel > &src )
{
	// �������g�ւ̑���͕s���ňӖ��������̂ōs��Ȃ��B
	if( src.m_pPtr == m_pPtr ) return ( * this );

	// �����͑��l�ɂȂ��Ă��܂��i���̃I�u�W�F�N�g�̊Ǘ��ɉ�邽�ߌ��݂̃I�u�W�F�N�g�̊Ǘ���������j�̂�
	// �Q�ƃJ�E���^��1����
	Release();

	// ����̃|�C���^���R�s�[
	m_pPtr		  = src.m_pPtr;
	m_bIsNoDelete = src.m_bIsNoDelete;

	if ( m_pPtr != skyNull )
	{
		// �V�����������g�̎Q�ƃJ�E���^�𑝉�
		m_pPtr->RefCntUp();
	}

	return ( * this );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline SmartPointer< T , ThreadingModel >& SmartPointer< T , ThreadingModel >::operator =( const SmartPointer< T2 , ThreadingModel > &src ) //T2�͐e�N���X��z��
{
    thread::CriticalLock Lock( this->m_pCs );

	// �������g�ւ̑���͕s���ňӖ��������̂ōs��Ȃ��B
	if( src.GetPtr() == m_pPtr ) return ( * this );

	// �����͑��l�ɂȂ��Ă��܂��i���̃I�u�W�F�N�g�̊Ǘ��ɉ�邽�ߌ��݂̃I�u�W�F�N�g�̊Ǘ���������j�̂�
	// �Q�ƃJ�E���^��1����
	Release();

	// ����̃|�C���^���R�s�[
	m_pPtr		= ( T * )( const_cast< T2 * >( src.GetPtr() ) );
	m_bIsNoDelete = src.IsNodelete();

//	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "�_�C�i�~�b�N�L���X�g�Ɏ��s���܂����B�L���X�g���悤�Ƃ����^�ł͂Ȃ��悤�ł��B" )  );

	// �V�����������g�̎Q�ƃJ�E���^�𑝉�
	m_pPtr->RefCntUp();

	return ( * this );
}

template< class T ,	class ThreadingModel >
inline T* SmartPointer< T , ThreadingModel >::operator ->() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) ); //The pointer has not been set to this smart pointer yet. 

	return m_pPtr;
}

template< class T ,	class ThreadingModel >
inline T &SmartPointer< T , ThreadingModel >::operator*()
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) ); //The pointer has not been set to this smart pointer yet. 

	return *m_pPtr;
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer< T , ThreadingModel >::operator ==( T *val )
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( val == skyNull && m_pPtr == skyNull )
	{
		return skyTrue;
	}

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return ( m_pPtr == val );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer< T , ThreadingModel >::operator ==( const SmartPointer< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return ( m_pPtr == src.GetPtr() );
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer< T , ThreadingModel >::operator !=( T *val )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return (m_pPtr != val);
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer< T , ThreadingModel >::operator !=( const SmartPointer< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return (m_pPtr != src.GetPtr());
}

template< class T ,	class ThreadingModel >
inline void SmartPointer< T , ThreadingModel >::SetPtr( T *src , skyBool bIsNoDelete ,  s32 add )
{
	thread::CriticalLock Lock( this->m_pCs );

	if( src == skyNull ) return;

	// �Q�ƃJ�E���^�����炵����ɍď�����
	Release();
	m_pPtr = src;
	m_bIsNoDelete = bIsNoDelete;
	m_pPtr->SetRefCnt( add );
	m_pPtr->RefCntUp();
}

template< class T ,	class ThreadingModel >
inline void SmartPointer< T , ThreadingModel >::OverWrite( T *src )
{
	thread::CriticalLock Lock( this->m_pCs );

	m_pPtr = src;
}

template< class T ,	class ThreadingModel >
inline const T& SmartPointer< T , ThreadingModel >::GetIns() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return *m_pPtr;
}

template< class T ,	class ThreadingModel >
inline const T* SmartPointer< T , ThreadingModel >::GetPtr() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return m_pPtr;
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer< T , ThreadingModel >::IsNodelete() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return m_bIsNoDelete;
}

template< class T ,	class ThreadingModel >
inline T* SmartPointer< T , ThreadingModel >::GetPtrNoConst() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return m_pPtr;
}


template< class T ,	class ThreadingModel >
inline T& SmartPointer< T , ThreadingModel >::GetInsNoConst() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return *m_pPtr;
}

template< class T ,	class ThreadingModel >
inline const T** SmartPointer< T , ThreadingModel >::GetPtrPtr()
{
	thread::CriticalLock Lock( this->m_pCs );

//	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return ( const T ** )&m_pPtr;
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer< T , ThreadingModel >::IsEmpty() const 
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return ( m_pPtr == skyNull );
}

template< class T ,	class ThreadingModel >
inline void SmartPointer< T , ThreadingModel >::Delete()
{
	thread::CriticalLock Lock( this->m_pCs );

	//�Q�ƃJ�E���^�͉�����
	Release();

	//�����̏��L���������
	m_pPtr = skyNull;
}

template< class T ,	class ThreadingModel >
inline void SmartPointer< T , ThreadingModel >::Release()
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( m_pPtr == skyNull ) return;

	SKY_ASSERT_MSG( m_pPtr->GetRefCnt() > 0 , _T( "The reference counter is already 0." ) ); //�Q�ƃJ�E���^�����łɂO�ł��B�O�Ȃ̂ɎQ�ƃJ�E���^�����炻���Ƃ��܂���

	//�Q�ƃJ�E���^��������
	m_pPtr->RefCntDown();

	//�Q�ƃJ�E���^���O�����������
	if( m_pPtr->GetRefCnt() == 0 )
	{
		if ( m_bIsNoDelete == skyFalse )
		{
			SAFE_DELETE( m_pPtr );			//�I�u�W�F�N�g����
		}
		else
		{
			//���̎Q�Ƃ͈Ӑ}�I�ɔj�����Ȃ��B
			m_pPtr = skyNull;
		}
	}
}

//-------------------�}���`�X���b�h--------------��

template< class T >
inline SmartPointerMT< T >::SmartPointerMT( T *src , s32 add ) :
SmartPointer< T , thread::ThreadingModels::CMultiThread >( src , add )
{
}

template< class T >
inline SmartPointerMT< T >::SmartPointerMT( const SmartPointerMT< T > &src ) :
SmartPointer< T , thread::ThreadingModels::CMultiThread >( src )
{
}

template< class T >
template< class T2 >
inline SmartPointerMT< T >::SmartPointerMT( SmartPointerMT< T2 > &src ) : //T2�͐e�N���X��z��
SmartPointer< T , thread::ThreadingModels::CMultiThread >( src )
{
}

template< class T >
inline SmartPointerMT< T >::~SmartPointerMT()
{
}

//-------------------�Ȃ�̌^�ł��g����X�}�[�g�|�C���^--------------��

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >::SmartPointer2( T *src , s32 add )
{
	type        = 0;
	m_pRefCnt	= NEW__( u32 , u32 );
	*m_pRefCnt	= add;
	m_pPtr		= src;
	AddRef();	// �Q�ƃJ�E���^����
}

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >::SmartPointer2(const SmartPointer2< T , ThreadingModel > &src)
{
	// ����̃|�C���^���R�s�[
	type        = 0;
	m_pRefCnt	= src.m_pRefCnt;
	m_pPtr		= src.m_pPtr;

	// �������g�̎Q�ƃJ�E���^�𑝉�
	AddRef();
}

template< class T , class ThreadingModel >
template< class T2 >
inline SmartPointer2< T , ThreadingModel >::SmartPointer2( const SmartPointer2< T2 , ThreadingModel > &src ) //T2�͐e�N���X��z��
{
	// ����̃|�C���^���R�s�[
	m_pRefCnt	= src.GetRefPtr();
//	m_pPtr		= dynamic_cast< T * >( const_cast< T2 * >( src.GetPtr() ) );
	m_pPtr		= SkyDynamicCast( T , src.GetPtrNoConst() );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "It failed in dynamic Cast."  ) ); //�_�C�i�~�b�N�L���X�g�Ɏ��s���܂����B�L���X�g���悤�Ƃ����^�ł͂Ȃ��悤�ł��B

	// �������g�̎Q�ƃJ�E���^�𑝉�
	AddRef();
}

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >::~SmartPointer2()
{
	if ( type != 0 ) return;
	Release(); //�Q�ƃ|�C���^�������āA�I�u�W�F�N�g��j�����ɂ���
}

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >& SmartPointer2< T , ThreadingModel >::operator =( const SmartPointer2< T , ThreadingModel > &src )
{
	// �������g�ւ̑���͕s���ňӖ��������̂ōs��Ȃ��B
	if( src.m_pPtr == m_pPtr ) return ( * this );

	// �����͑��l�ɂȂ��Ă��܂��i���̃I�u�W�F�N�g�̊Ǘ��ɉ�邽�ߌ��݂̃I�u�W�F�N�g�̊Ǘ���������j�̂�
	// �Q�ƃJ�E���^��1����
	Release();

	// ����̃|�C���^���R�s�[
	m_pRefCnt	= src.m_pRefCnt;
	m_pPtr		= src.m_pPtr;

	// �V�����������g�̎Q�ƃJ�E���^�𑝉�
	AddRef();

	return ( * this );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline SmartPointer2< T , ThreadingModel >& SmartPointer2< T , ThreadingModel >::operator =( const SmartPointer2< T2 , ThreadingModel > &src ) //T2�͐e�N���X��z��
{
	thread::CriticalLock Lock( this->m_pCs );

	// �������g�ւ̑���͕s���ňӖ��������̂ōs��Ȃ��B
	if( src.GetPtr() == m_pPtr ) return (* this);

	// �����͑��l�ɂȂ��Ă��܂��i���̃I�u�W�F�N�g�̊Ǘ��ɉ�邽�ߌ��݂̃I�u�W�F�N�g�̊Ǘ���������j�̂�
	// �Q�ƃJ�E���^��1����
	Release();

	// ����̃|�C���^���R�s�[
	m_pRefCnt	= src.GetRefPtr();
//	m_pPtr		= dynamic_cast< T * >( const_cast< T2 * >( src.GetPtr() ) ); //���ꂾ��Delegate���g���Ȃ��̂ŃR�����g�A�E�g����dynamic_cast���g�p���Ȃ��B
	m_pPtr		= ( T * )( const_cast< T2 * >( src.GetPtr() ) );

//	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "�_�C�i�~�b�N�L���X�g�Ɏ��s���܂����B�L���X�g���悤�Ƃ����^�ł͂Ȃ��悤�ł��B" ) );

	// �V�����������g�̎Q�ƃJ�E���^�𑝉�
	AddRef();

	return ( * this );
}

template< class T ,	class ThreadingModel >
inline T* SmartPointer2< T , ThreadingModel >::operator ->() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return m_pPtr;
}

template< class T ,	class ThreadingModel >
inline T &SmartPointer2< T , ThreadingModel >::operator*()
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) ); //The pointer has not been set to this smart pointer yet. 

	return *m_pPtr;
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer2< T , ThreadingModel >::operator ==( T *val )
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( val == skyNull && m_pPtr == skyNull )
	{
		return skyTrue;
	}

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return ( m_pPtr == val );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer2< T , ThreadingModel >::operator ==( const SmartPointer2< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return ( m_pPtr == src.GetPtr() );
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer2< T , ThreadingModel >::operator !=( T *val )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return (m_pPtr != val);
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer2< T , ThreadingModel >::operator !=( const SmartPointer2< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
	return (m_pPtr != src.GetPtr());
}

template< class T ,	class ThreadingModel >
inline void SmartPointer2< T , ThreadingModel >::SetPtr( T *src ,  s32 add )
{
	thread::CriticalLock Lock( this->m_pCs );

	if( src == skyNull ) return;

	// �Q�ƃJ�E���^�����炵����ɍď�����
	Release();
	m_pRefCnt = NEW__( u32 , u32 );
	*m_pRefCnt = add;
	m_pPtr = src;
	AddRef();
}

template< class T ,	class ThreadingModel >
inline void SmartPointer2< T , ThreadingModel >::OverWrite( T *src )
{
	thread::CriticalLock Lock( this->m_pCs );

	m_pPtr = src;
}

template< class T ,	class ThreadingModel >
inline const T* SmartPointer2< T , ThreadingModel >::GetPtr() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return m_pPtr;
}

template< class T ,	class ThreadingModel >
inline T* SmartPointer2< T , ThreadingModel >::GetPtrNoConst() const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return m_pPtr;
}


template< class T ,	class ThreadingModel >
inline T** SmartPointer2< T , ThreadingModel >::GetPtrPtr()
{
	thread::CriticalLock Lock( this->m_pCs );

//	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	return ( T ** )&m_pPtr;
}

template< class T ,	class ThreadingModel >
inline u32* SmartPointer2< T , ThreadingModel >::GetRefPtr() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return m_pRefCnt; 
}

template< class T ,	class ThreadingModel >
inline u32 SmartPointer2< T , ThreadingModel >::GetRef() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return *m_pRefCnt; 
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer2< T , ThreadingModel >::IsEmpty() const 
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return ( m_pPtr == skyNull );
}

template< class T ,	class ThreadingModel >
inline void SmartPointer2< T , ThreadingModel >::AddRef()
{
	thread::CriticalLock Lock( this->m_pCs );

	( *m_pRefCnt )++;
}

template< class T ,	class ThreadingModel >
inline void SmartPointer2< T , ThreadingModel >::Delete()
{
	thread::CriticalLock Lock( this->m_pCs );

	//�Q�ƃJ�E���^�����炵��
	Release();

	//���L���������
	m_pPtr = skyNull;
	m_pRefCnt = skyNull;
}

template< class T ,	class ThreadingModel >
void SmartPointer2< T , ThreadingModel >::Release()
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( m_pRefCnt == skyNull ) return;

	SKY_ASSERT_MSG( ( * m_pRefCnt ) > 0 , _T( "The reference counter is already 0." ) ); //�Q�ƃJ�E���^�����łɂO�ł��B�O�Ȃ̂ɎQ�ƃJ�E���^�����炻���Ƃ��܂����B

	//�Q�ƃJ�E���^���O�����������
	if( m_pRefCnt != skyNull && --( * m_pRefCnt ) == 0 )
	{
		SAFE_DELETE( m_pPtr );			//�I�u�W�F�N�g����
		SAFE_DELETE( m_pRefCnt );		//�Q�ƃJ�E���^����
	}
}

//-------------------Com��--------------��

template< class T , class ThreadingModel >
inline SmartPointerCom< T , ThreadingModel >::SmartPointerCom( T *src , s32 add ) :
SmartPointer2< T , ThreadingModel >( src , add )
{
    this->type = 1;
}

template< class T , class ThreadingModel >
inline SmartPointerCom< T , ThreadingModel >::SmartPointerCom( const SmartPointerCom< T , ThreadingModel > &src ) :
SmartPointer2< T , ThreadingModel >( src )
{
    this->type = 1;
}

template< class T , class ThreadingModel >
template< class T2 >
inline SmartPointerCom< T , ThreadingModel >::SmartPointerCom( SmartPointerCom< T2 , ThreadingModel > &src ) : //T2�͐e�N���X��z��
SmartPointer2< T , ThreadingModel >( src )
{
}

template< class T , class ThreadingModel >
inline SmartPointerCom< T , ThreadingModel >::~SmartPointerCom()
{
	Release(); //�Q�ƃ|�C���^�������āA�I�u�W�F�N�g��j�����ɂ���
}

template< class T ,	class ThreadingModel >
inline void SmartPointerCom< T , ThreadingModel >::Release()
{
	thread::CriticalLock Lock( this->m_pCs );

	//�Q�ƃJ�E���^���O�����������
        if( this->m_pRefCnt != skyNull && --( * this->m_pRefCnt ) == 0 )
	{
                SAFE_RELEASE( this->m_pPtr );			//�I�u�W�F�N�g����
                SAFE_DELETE( this->m_pRefCnt );		//�Q�ƃJ�E���^����
	}
}

} }
