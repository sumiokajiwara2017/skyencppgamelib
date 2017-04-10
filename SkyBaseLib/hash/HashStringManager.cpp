#include "StdAfx.h"
#include "SkyBaseLib/Hash.h"

#include "SkyBaseLib/SKYTL.h"


namespace sky { namespace lib { namespace hash {

	/// <summary>
	/// Hash文字Hashテーブル
	/// </summary>
static hash::CHashStringPtrHash *m_pHashTable = skyNull;


void CHashStringManager::Initialize( u32 hashSize )
{
	SKY_ASSERT_MSG( m_pHashTable == skyNull , _T( "It tried to initialize it twice. " ) ); //２度初期化しようとしました。

	m_pHashTable = NEW__( hash::CHashStringPtrHash , hash::CHashStringPtrHash( hashSize ) );
}

void CHashStringManager::Dispose()
{
	SKY_ASSERT_MSG( m_pHashTable != skyNull , _T( "It started making it to the end. " ) );

	SAFE_DELETE( m_pHashTable );
}

hash::CHashStringPtr CHashStringManager::GetPtr( const skyString *pKey , skyBool isNoCreateOn )
{
	hash::CHashStringPtr pHashSt;
	if ( m_pHashTable != NULL && m_pHashTable->Get( pKey , pHashSt ) == skyFalse )
	{//見つからなかったので作ろう

		//CHashStringを生成
		pHashSt.SetPtr( NEW__( CHashString , CHashString() ) );

		//CHashStringにCRC値と元文字列を格納する
		pHashSt->SetString( pKey , isNoCreateOn );

		//テーブルに格納
		m_pHashTable->Set( pHashSt , pHashSt ); //キーも値も同じ。これがコツ
	}

	return pHashSt;
}

hash::CHashStringPtr CHashStringManager::Get( const skyWString *pKey , skyBool isNoCreateOn )
{

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE

	return GetPtr( pKey , isNoCreateOn );

#else

	skyMString *pKeyConv = CStringUtil_::CreateMString( pKey );
	hash::CHashStringPtr result = GetPtr( pKeyConv , isNoCreateOn );
	CStringUtil_::DeleteMString( pKeyConv );
	return result;

#endif
	
}

hash::CHashStringPtr CHashStringManager::Get( const skyMString *pKey , skyBool isNoCreateOn )
{

#ifndef SW_SKYLIB_MEMORY_TEXT_UNICODE

	return GetPtr( pKey , isNoCreateOn );

#else

	skyWString *pKeyConv = CStringUtil_::CreateWString( pKey );
	hash::CHashStringPtr result = GetPtr( pKeyConv , isNoCreateOn );
	CStringUtil_::DeleteWString( pKeyConv );
	return result;

#endif

}

void CHashStringManager::Clear()
{
	m_pHashTable->DeleteAll();
}

void CHashStringManager::GC()
{
	FOREACH_NOINC( hash::CHashStringPtrHash , it , *m_pHashTable )
	{
		SKY_PRINTF( _T( "%s %d \n" ) , ( *it )->GetString() , ( *it )->GetRefCnt() );

		//参照カウンタが１、つまりこのマネージャー以外参照していなければ、削除
		if ( ( *it )->GetRefCnt() == 1 )
		{
			it = m_pHashTable->Erases( it );
		}
		else
		{
			it++;
		}
	}
}

} } }