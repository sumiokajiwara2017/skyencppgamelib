#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

SqClassObject::SqClassObject() :
m_kName( NULL ) ,
m_ParentName( skyNull ) ,
m_MemberHash( sqclassobject::MEMBER_HASH_VALUE )
{
}

SqClassObject::SqClassObject( const hash::CHashStringPtr &pClassName ) :
m_kName( pClassName ) ,
m_ParentName( skyNull ) ,
m_MemberHash( sqclassobject::MEMBER_HASH_VALUE )
{
}

SqClassObject::SqClassObject( const hash::CHashStringPtr &pClassName , const hash::CHashStringPtr &parentName ) :
m_kName( pClassName ) ,
m_ParentName( parentName ) ,
m_MemberHash( sqclassobject::MEMBER_HASH_VALUE )
{
}


SqClassObject& SqClassObject::operator =( const SqClassObject& src )
{
	//クラス名のコピー
	m_kName = src.m_kName;

	//メンバ関数のコピー
	for( u32 i = 0 ; i < src.m_CppMethodList.GetNum() ; i++ )
	{
		AddMethod( src.m_CppMethodList[ i ].m_fname , src.m_CppMethodList[ i ].m_f );
	}

	//メンバ変数のコピー
	hash::CHashStringPtrList keyList;
	src.m_MemberHash.GetkeyList( keyList );
	FOREACH( hash::CHashStringPtrList , it , keyList )
	{
		AddMember( *it , src.m_MemberHash[ *it ] );
	}

	return *this;
}

void SqClassObject::AddMember( const hash::CHashStringPtr pMemberName , const SqParamObject &param )
{
	m_MemberHash.Set( pMemberName , param );
}

void SqClassObject::AddMethod( const hash::CHashStringPtr pMemberFunc , SQFUNCTION f , const hash::CHashStringPtr typemask )
{
	m_CppMethodList.AddTail( SqCppFunc( f , pMemberFunc , typemask ) );
}

SqClassObject::SqClassObject( const SqClassObject& src ) :
m_kName( skyNull ) ,
m_ParentName( skyNull ) ,
m_MemberHash( sqclassobject::MEMBER_HASH_VALUE )
{
	*this = src;
}


} } }

#endif