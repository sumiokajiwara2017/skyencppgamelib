namespace sky { namespace lib {

template< typename T >
inline CStack< T >::CStack( s32 size )
{
	
	m_pData = NEW_ARRAY_() T[ size ]; //�X�^�b�N�̐[�����̈���m�ۂ��A���̐擪�|�C���^��ێ�
	m_Size = size;
	m_PushNum = 0;
}

template< typename T >
inline CStack< T >::~CStack()
{
	DEL_ARRAY m_pData; //@@SKYTODO size�̒l���傫���Ƃ������G���[����i�v�����j
}

template< typename T >
inline u32 CStack< T >::Push( const T &data )
{
    SKY_ASSERT_MSG( m_PushNum < m_Size , _T( "Stack size over. Size=[%d], PushNum=[%d]" ) , m_Size , m_PushNum );

	T* pCursor;

	//Push�����猻�݂̃J�[�\���g�b�v�A�h���X���擾
	_SetCursor( m_PushNum , pCursor );

	//�g�b�v�A�h���X�ɒl���R�s�[
	*pCursor = data;

	//Push��Up
	m_PushNum++;

	return ( m_PushNum-1 );
}

template< typename T >
inline u32 CStack< T >::Push()
{
    SKY_ASSERT_MSG( m_PushNum < m_Size , _T( "stack size over. Size=[%d], PushNum=[%d]" ) , m_Size , m_PushNum );

	//Push��Up
	m_PushNum++;

	return ( m_PushNum-1 );
}

template< typename T >
inline skyBool CStack< T >::Pop( T &data )
{
	SKY_ASSERT_MSG( m_PushNum > 0, _T( "The stack is empty." ) ); //The stack is empty.

	//Push��Donw
	--m_PushNum;

	T *pCur;

	//�J�[�\����i�߂�
	_SetCursor( m_PushNum , pCur );

	//�l��Ԃ��B�i�R�s�[�R���X�g���N�^�����j
	data = *pCur;

	return skyTrue;
}

template< typename T >
inline skyBool CStack< T >::Pop()
{
	SKY_ASSERT_MSG( m_PushNum > 0, _T( "The stack is empty." ) );

	//Push��Donw
	--m_PushNum;

	return skyTrue;
}

template< typename T >
inline skyBool CStack< T >::PopBottom( T &data )
{
	T *pCur;

	//�J�[�\�����擾
	_SetCursor( 0 , pCur );

	//�l��Ԃ��B�i�R�s�[�R���X�g���N�^�����j
	data = *pCur;

	return PopBottom();
}

template< typename T >
inline skyBool CStack< T >::PopBottom()
{
	T *pCur;

	//�J�[�\�����擾
	_SetCursor( 1 , pCur );

	//Push��Donw
	--m_PushNum;

	//�������S�̂��P���R�[�h���X�^�b�N�̉��ɂ��炷�i��Ԃ������Ԃ��j
	CMemoryUtil_::Copy( m_pData , pCur , sizeof( T ) * m_PushNum );

	return skyTrue;
}

template< typename T >
inline void CStack< T >::Set( const T &data )
{
	SKY_ASSERT_MSG( m_PushNum > 0, _T( "The stack is empty." ) );

	T *pCur;

	//�J�[�\����i�߂�
	_SetCursor( m_PushNum-1 , pCur );

	*pCur = data;
}


template< typename T >
inline void CStack< T >::Clear()
{
	m_PushNum = 0;
}

template< typename T >
inline u32 CStack< T >::GetNum() const { return m_PushNum; }

template< typename T >
inline u32 CStack< T >::GetSize() const	{ return m_Size; }

template< typename T >
inline T &CStack< T >::GetTop()
{
	T *pCur;

	//��ԏ�̃|�C���^���擾
	_SetCursor( m_PushNum-1 , pCur );

	return *pCur;
}

template< typename T >
inline T &CStack< T >::GetBottom()
{
	T *pCur;

	//��ԉ��̃|�C���^���擾
	_SetCursor( 0 , pCur );

	return *pCur;
}

template< typename T >
inline T &CStack< T >::GetItem( s32 idx ) const
{
	T *pCur;

	//�J�[�\����i�߂�
	_SetCursor( idx , pCur );

	return *pCur;
}
template< typename T >
inline skyBool CStack< T >::GetItem( s32 idx , T &data ) const
{
	T *pCur;

	//�J�[�\����i�߂�
	_SetCursor( idx , pCur );

	data = *pCur;

	return skyTrue;
}
template< typename T >
inline T &CStack< T >::operator[]( s32 idx ) const
{
	T *result;
	SetItCursor( idx , result );
	return *result;
}

template< typename T >
inline CStack< T >::iterator::iterator( CStack< T > *pStack, s32 cursor ) :
Iterator< CStack , T >( pStack , cursor )
{
}

template< typename T >
inline skyBool CStack< T >::IsBeginBefor( iterator &it ) const
{
	return ( it.GetIdx() < 0 );
}

template< typename T >
inline typename CStack< T >::iterator CStack< T >::Begin()
{
    return CStack< T >::iterator( this , 0 );
}

template< typename T >
inline typename CStack< T >::iterator CStack< T >::End()
{
    return CStack< T >::iterator( this , m_PushNum );
}

template< typename T >
inline typename CStack< T >::iterator CStack< T >::Erases( typename CStack< T >::iterator &it )
{
	s32 cursor = it.Delete();
    return CStack< T >::iterator( this , cursor );
}

template< typename T >
inline skyBool CStack< T >::SetItCursor( u32 idx , T* &data ) const
{
	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_PushNum == 0 ) return skyFalse;

    SKY_ASSERT_MSG( idx < m_PushNum , _T( "index over. idx=[%d], pushNum=[%d]" ) , idx , m_PushNum );

	//�I��
	_SetCursor( idx , data );

	return skyTrue;
}

template< typename T >
inline skyBool CStack< T >::DeleteIdx( u32 idx )
{
	idx;
	SKY_PANIC_MSG( _T( "This function is invalid." ) ); //���̊֐��͖����ł��B���̌Ă񂾂�ł����H

	return skyFalse;
}

template< typename T >
inline skyBool	CStack< T >::_SetCursor( u32 idx , T* &pCur ) const
{
	//�f�[�^�擪�A�h���X�𐔒l�ɂ��āA�P�̊i�[�T�C�Y�~�i�[�����|�������̂𐔒l�Ƃ��đ���
	pCur = ( T * )( ( u32 )( m_pData ) + ( idx * sizeof( T ) ) );

	return skyTrue;
}

} }
