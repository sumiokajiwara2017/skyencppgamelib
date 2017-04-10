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
	// 相手のポインタをコピー
	m_pPtr				= src.m_pPtr;
	m_bIsNoDelete		= src.m_bIsNoDelete;

	if ( m_pPtr != skyNull )
	{
		// 自分自身の参照カウンタを増加
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
		//相手側をNULLにして終了
		m_pPtr = skyNull;
	}
	else
	{
		// 相手のポインタをコピー
	//	m_pPtr				= dynamic_cast< T * >( const_cast< T2 * >( src.GetPtr() ) );
		m_pPtr		        = SkyDynamicCast( T , src.GetPtrNoConst() ); //仮想親クラス（継承時にvirtualってかくことで多重同クラス継承を可能にするやつ）があるとここでコンパイルエラー
		m_bIsNoDelete		= src.IsNodelete();

		SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "It failed in dynamic Cast."  ) ); //ダイナミックキャストに失敗しました。キャストしようとした型ではないようです。

		// 自分自身の参照カウンタを増加
		m_pPtr->RefCntUp();
	}
}

template< class T ,	class ThreadingModel >
inline SmartPointer< T , ThreadingModel >::~SmartPointer()
{
	Release(); //参照ポインタを下げて、オブジェクトを破棄しにいく
}

template< class T ,	class ThreadingModel >
inline SmartPointer< T , ThreadingModel >& SmartPointer< T , ThreadingModel >::operator =( const SmartPointer< T , ThreadingModel > &src )
{
	// 自分自身への代入は不正で意味が無いので行わない。
	if( src.m_pPtr == m_pPtr ) return ( * this );

	// 自分は他人になってしまう（他のオブジェクトの管理に回るため現在のオブジェクトの管理を手放す）ので
	// 参照カウンタを1つ減少
	Release();

	// 相手のポインタをコピー
	m_pPtr		  = src.m_pPtr;
	m_bIsNoDelete = src.m_bIsNoDelete;

	if ( m_pPtr != skyNull )
	{
		// 新しい自分自身の参照カウンタを増加
		m_pPtr->RefCntUp();
	}

	return ( * this );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline SmartPointer< T , ThreadingModel >& SmartPointer< T , ThreadingModel >::operator =( const SmartPointer< T2 , ThreadingModel > &src ) //T2は親クラスを想定
{
    thread::CriticalLock Lock( this->m_pCs );

	// 自分自身への代入は不正で意味が無いので行わない。
	if( src.GetPtr() == m_pPtr ) return ( * this );

	// 自分は他人になってしまう（他のオブジェクトの管理に回るため現在のオブジェクトの管理を手放す）ので
	// 参照カウンタを1つ減少
	Release();

	// 相手のポインタをコピー
	m_pPtr		= ( T * )( const_cast< T2 * >( src.GetPtr() ) );
	m_bIsNoDelete = src.IsNodelete();

//	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "ダイナミックキャストに失敗しました。キャストしようとした型ではないようです。" )  );

	// 新しい自分自身の参照カウンタを増加
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

	//保持しているオブジェクトのポインタと対象とを比較
	return ( m_pPtr == val );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer< T , ThreadingModel >::operator ==( const SmartPointer< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//保持しているオブジェクトのポインタと対象とを比較
	return ( m_pPtr == src.GetPtr() );
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer< T , ThreadingModel >::operator !=( T *val )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//保持しているオブジェクトのポインタと対象とを比較
	return (m_pPtr != val);
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer< T , ThreadingModel >::operator !=( const SmartPointer< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//保持しているオブジェクトのポインタと対象とを比較
	return (m_pPtr != src.GetPtr());
}

template< class T ,	class ThreadingModel >
inline void SmartPointer< T , ThreadingModel >::SetPtr( T *src , skyBool bIsNoDelete ,  s32 add )
{
	thread::CriticalLock Lock( this->m_pCs );

	if( src == skyNull ) return;

	// 参照カウンタを減らした後に再初期化
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

	//参照カウンタは下げて
	Release();

	//自分の所有を放棄する
	m_pPtr = skyNull;
}

template< class T ,	class ThreadingModel >
inline void SmartPointer< T , ThreadingModel >::Release()
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( m_pPtr == skyNull ) return;

	SKY_ASSERT_MSG( m_pPtr->GetRefCnt() > 0 , _T( "The reference counter is already 0." ) ); //参照カウンタがすでに０です。０なのに参照カウンタを減らそうとしました

	//参照カウンタを下げる
	m_pPtr->RefCntDown();

	//参照カウンタが０だったら消す
	if( m_pPtr->GetRefCnt() == 0 )
	{
		if ( m_bIsNoDelete == skyFalse )
		{
			SAFE_DELETE( m_pPtr );			//オブジェクト消す
		}
		else
		{
			//この参照は意図的に破棄しない。
			m_pPtr = skyNull;
		}
	}
}

//-------------------マルチスレッド--------------▽

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
inline SmartPointerMT< T >::SmartPointerMT( SmartPointerMT< T2 > &src ) : //T2は親クラスを想定
SmartPointer< T , thread::ThreadingModels::CMultiThread >( src )
{
}

template< class T >
inline SmartPointerMT< T >::~SmartPointerMT()
{
}

//-------------------なんの型でも使えるスマートポインタ--------------▽

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >::SmartPointer2( T *src , s32 add )
{
	type        = 0;
	m_pRefCnt	= NEW__( u32 , u32 );
	*m_pRefCnt	= add;
	m_pPtr		= src;
	AddRef();	// 参照カウンタ増加
}

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >::SmartPointer2(const SmartPointer2< T , ThreadingModel > &src)
{
	// 相手のポインタをコピー
	type        = 0;
	m_pRefCnt	= src.m_pRefCnt;
	m_pPtr		= src.m_pPtr;

	// 自分自身の参照カウンタを増加
	AddRef();
}

template< class T , class ThreadingModel >
template< class T2 >
inline SmartPointer2< T , ThreadingModel >::SmartPointer2( const SmartPointer2< T2 , ThreadingModel > &src ) //T2は親クラスを想定
{
	// 相手のポインタをコピー
	m_pRefCnt	= src.GetRefPtr();
//	m_pPtr		= dynamic_cast< T * >( const_cast< T2 * >( src.GetPtr() ) );
	m_pPtr		= SkyDynamicCast( T , src.GetPtrNoConst() );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "It failed in dynamic Cast."  ) ); //ダイナミックキャストに失敗しました。キャストしようとした型ではないようです。

	// 自分自身の参照カウンタを増加
	AddRef();
}

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >::~SmartPointer2()
{
	if ( type != 0 ) return;
	Release(); //参照ポインタを下げて、オブジェクトを破棄しにいく
}

template< class T ,	class ThreadingModel >
inline SmartPointer2< T , ThreadingModel >& SmartPointer2< T , ThreadingModel >::operator =( const SmartPointer2< T , ThreadingModel > &src )
{
	// 自分自身への代入は不正で意味が無いので行わない。
	if( src.m_pPtr == m_pPtr ) return ( * this );

	// 自分は他人になってしまう（他のオブジェクトの管理に回るため現在のオブジェクトの管理を手放す）ので
	// 参照カウンタを1つ減少
	Release();

	// 相手のポインタをコピー
	m_pRefCnt	= src.m_pRefCnt;
	m_pPtr		= src.m_pPtr;

	// 新しい自分自身の参照カウンタを増加
	AddRef();

	return ( * this );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline SmartPointer2< T , ThreadingModel >& SmartPointer2< T , ThreadingModel >::operator =( const SmartPointer2< T2 , ThreadingModel > &src ) //T2は親クラスを想定
{
	thread::CriticalLock Lock( this->m_pCs );

	// 自分自身への代入は不正で意味が無いので行わない。
	if( src.GetPtr() == m_pPtr ) return (* this);

	// 自分は他人になってしまう（他のオブジェクトの管理に回るため現在のオブジェクトの管理を手放す）ので
	// 参照カウンタを1つ減少
	Release();

	// 相手のポインタをコピー
	m_pRefCnt	= src.GetRefPtr();
//	m_pPtr		= dynamic_cast< T * >( const_cast< T2 * >( src.GetPtr() ) ); //これだとDelegateが使えないのでコメントアウトしてdynamic_castを使用しない。
	m_pPtr		= ( T * )( const_cast< T2 * >( src.GetPtr() ) );

//	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "ダイナミックキャストに失敗しました。キャストしようとした型ではないようです。" ) );

	// 新しい自分自身の参照カウンタを増加
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

	//保持しているオブジェクトのポインタと対象とを比較
	return ( m_pPtr == val );
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer2< T , ThreadingModel >::operator ==( const SmartPointer2< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//保持しているオブジェクトのポインタと対象とを比較
	return ( m_pPtr == src.GetPtr() );
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointer2< T , ThreadingModel >::operator !=( T *val )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//保持しているオブジェクトのポインタと対象とを比較
	return (m_pPtr != val);
}

template< class T ,	class ThreadingModel >
template< class T2 >
inline skyBool SmartPointer2< T , ThreadingModel >::operator !=( const SmartPointer2< T2 , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( m_pPtr != skyNull , _T( "The pointer has not been set to this smart pointer yet. " ) );

	//保持しているオブジェクトのポインタと対象とを比較
	return (m_pPtr != src.GetPtr());
}

template< class T ,	class ThreadingModel >
inline void SmartPointer2< T , ThreadingModel >::SetPtr( T *src ,  s32 add )
{
	thread::CriticalLock Lock( this->m_pCs );

	if( src == skyNull ) return;

	// 参照カウンタを減らした後に再初期化
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

	//参照カウンタを減らして
	Release();

	//所有を放棄する
	m_pPtr = skyNull;
	m_pRefCnt = skyNull;
}

template< class T ,	class ThreadingModel >
void SmartPointer2< T , ThreadingModel >::Release()
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( m_pRefCnt == skyNull ) return;

	SKY_ASSERT_MSG( ( * m_pRefCnt ) > 0 , _T( "The reference counter is already 0." ) ); //参照カウンタがすでに０です。０なのに参照カウンタを減らそうとしました。

	//参照カウンタが０だったら消す
	if( m_pRefCnt != skyNull && --( * m_pRefCnt ) == 0 )
	{
		SAFE_DELETE( m_pPtr );			//オブジェクト消す
		SAFE_DELETE( m_pRefCnt );		//参照カウンタ消す
	}
}

//-------------------Com版--------------▽

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
inline SmartPointerCom< T , ThreadingModel >::SmartPointerCom( SmartPointerCom< T2 , ThreadingModel > &src ) : //T2は親クラスを想定
SmartPointer2< T , ThreadingModel >( src )
{
}

template< class T , class ThreadingModel >
inline SmartPointerCom< T , ThreadingModel >::~SmartPointerCom()
{
	Release(); //参照ポインタを下げて、オブジェクトを破棄しにいく
}

template< class T ,	class ThreadingModel >
inline void SmartPointerCom< T , ThreadingModel >::Release()
{
	thread::CriticalLock Lock( this->m_pCs );

	//参照カウンタが０だったら消す
        if( this->m_pRefCnt != skyNull && --( * this->m_pRefCnt ) == 0 )
	{
                SAFE_RELEASE( this->m_pPtr );			//オブジェクト消す
                SAFE_DELETE( this->m_pRefCnt );		//参照カウンタ消す
	}
}

} }
