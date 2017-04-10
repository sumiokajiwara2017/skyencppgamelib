#include "StdAfx.h"
#include "SkyBaseLib/Hash.h"

#include "SkyBaseLib/SKYTL.h"


namespace sky { namespace lib { namespace hash {

	/// <summary>
	/// Hash����Hash�e�[�u��
	/// </summary>
static hash::CHashStringPtrHash *m_pHashTable = skyNull;


void CHashStringManager::Initialize( u32 hashSize )
{
	SKY_ASSERT_MSG( m_pHashTable == skyNull , _T( "It tried to initialize it twice. " ) ); //�Q�x���������悤�Ƃ��܂����B

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
	{//������Ȃ������̂ō�낤

		//CHashString�𐶐�
		pHashSt.SetPtr( NEW__( CHashString , CHashString() ) );

		//CHashString��CRC�l�ƌ���������i�[����
		pHashSt->SetString( pKey , isNoCreateOn );

		//�e�[�u���Ɋi�[
		m_pHashTable->Set( pHashSt , pHashSt ); //�L�[���l�������B���ꂪ�R�c
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

		//�Q�ƃJ�E���^���P�A�܂肱�̃}�l�[�W���[�ȊO�Q�Ƃ��Ă��Ȃ���΁A�폜
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