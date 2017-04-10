#include "StdAfx.h"
#include "SkyMathLib/Matrix.h"

namespace sky { namespace lib { namespace math {

CMatrix3x2Stack::CMatrix3x2Stack( u32 size ) :
m_Stack( size )
{
}

CMatrix3x2Stack::~CMatrix3x2Stack()
{
}

CMatrix3x2 &CMatrix3x2Stack::GetTop()
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );//�X�^�b�N����ł��B

	return m_Stack.GetTop();
}

void CMatrix3x2Stack::LoadIdentity()
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.GetTop().Identity();
}

void CMatrix3x2Stack::LoadMatrix( const CMatrix3x2 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.GetTop() = mat;
}

void CMatrix3x2Stack::Push()
{
	m_Stack.Push();
}

void CMatrix3x2Stack::Pop()
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.Pop();
}

void CMatrix3x2Stack::MultMatrix( const CMatrix3x2 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.Set( m_Stack.GetTop() * mat );
}

void CMatrix3x2Stack::MultMatrixLocal( const CMatrix3x2 &mat )
{
	SKY_ASSERT_MSG( m_Stack.GetNum() > 0 , _T( "The stack is empty." ) );

	m_Stack.Set( mat * m_Stack.GetTop() );
}

void CMatrix3x2Stack::LoadAndMultBeforeMatrix( const CMatrix3x2 &mat )
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

void CMatrix3x2Stack::LoadAndMultLocalBeforeMatrix( const CMatrix3x2 &mat )
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

} } }