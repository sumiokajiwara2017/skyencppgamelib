namespace sky { namespace lib { namespace math {

inline CMatrix4x3Stack::CMatrix4x3Stack( u32 size ) :
m_Stack( size )
{
}

inline CMatrix4x3Stack::~CMatrix4x3Stack()
{
}

inline CMatrix4x3 &CMatrix4x3Stack::GetTop()
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) ); //�X�^�b�N����ł��B


	return m_Stack.GetTop();
}

inline void CMatrix4x3Stack::LoadIdentity()
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.GetTop().Identity();
}

inline void CMatrix4x3Stack::LoadMatrix( const CMatrix4x3 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.GetTop() = mat;
}

inline void CMatrix4x3Stack::Push()
{
	m_Stack.Push();
}

inline void CMatrix4x3Stack::Pop()
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.Pop();
}

inline void CMatrix4x3Stack::MultMatrix( const CMatrix4x3 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.Set( m_Stack.GetTop() * mat );
}

inline void CMatrix4x3Stack::MultMatrixLocal( const CMatrix4x3 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.Set( mat * m_Stack.GetTop() );
}

inline void CMatrix4x3Stack::LoadAndMultBeforeMatrix( const CMatrix4x3 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	if ( m_Stack.GetNum() == 1 )
	{ //��ԏ�̗v�f�̏ꍇ�A�������[�h
		LoadMatrix( mat );
	}
	else
	{ //��O�̗v�f�Ɗ|�����킹�ă��[�h
		LoadMatrix( m_Stack.GetItem( m_Stack.GetNum() - 2 ) * mat );
	}
}

inline void CMatrix4x3Stack::LoadAndMultLocalBeforeMatrix( const CMatrix4x3 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	if ( m_Stack.GetNum() == 1 )
	{ //��ԏ�̗v�f�̏ꍇ�A�������[�h
		LoadMatrix( mat );
	}
	else
	{ //��O�̗v�f�Ɗ|�����킹�ă��[�h
		LoadMatrix( mat * m_Stack.GetItem( m_Stack.GetNum() - 2 ) );
	}
}

inline void CMatrix4x3Stack::LoadAndMultLocalBeforeMatrixTransOnly( const CMatrix4x3 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	if ( m_Stack.GetNum() == 1 )
	{ //��ԏ�̗v�f�̏ꍇ�A�������[�h
		LoadMatrix( mat );
	}
	else
	{ //��O�̗v�f�Ɗ|�����킹�ă��[�h
		LoadMatrix( MulMatrix4x4TransOnly( mat , m_Stack.GetItem( m_Stack.GetNum() - 2 ) ) );
	}
}

} } }