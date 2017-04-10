#include "StdAfx.h"
#include "SkyBaseLib/File.h"
#include "SkyBaseLib/Directory.h"

namespace sky { namespace lib { namespace file {

hash::CHashStringPtrListPtrHashPtrHash	*CPropertyManager::m_pFileDataHash = skyNull;

void CPropertyManager::Initialize()
{
	//�t�@�C�����R���e�i���쐬
	m_pFileDataHash = NEW__( hash::CHashStringPtrListPtrHashPtrHash , hash::CHashStringPtrListPtrHashPtrHash( property::PROPERTY_FILE_HASH_SIZE ) );

	//Directory���̃t�@�C�����X�g�����
	skyString idir[ DIR_LEN_MAX ];
	skyString ifn[ FILE_LEN_MAX ];

	CPathUtil_::GetDirPath( property::PROPERTY_SEARCH_PATH , idir , DIR_LEN_MAX );
	CPathUtil_::GetFileName( property::PROPERTY_SEARCH_PATH , ifn , FILE_LEN_MAX );

	lib::directory::CDirectoryPtr dirPtr = CDirectoryFactory_::Create( idir , ifn );

	//�f�B���N�g���N���X����t�@�C�����X�g�����炤]
	lib::hash::CHashStringPtrList fileList;
	dirPtr->GetFilePathList( fileList ,skyFalse );

	//�t�@�C�����X�g�Əo�̓f�B���N�g���Ńt�@�C�����o�͂���
	FOREACH( lib::hash::CHashStringPtrList , it , fileList )
	{
		//�t�@�C�������ɃR���e�i�������ăf�[�^���i�[����
		hash::CHashStringPtrListPtrHashPtr propDataPtr( NEW__( hash::CHashStringPtrListPtrHash , hash::CHashStringPtrListPtrHash( property::DEFAULt_HASH_SIZE ) ) );

		//�t�@�C����ǂ�
		sky::lib::stream::IStreamPtr spSrcStream = CFileUtil_::LoadFile( *it );

		//�X�g���[�����J��
		spSrcStream->vOpen();

		//�v���p�e�B�t�@�C�������[�h
		CPropertyFileUtil_::Load( spSrcStream , *propDataPtr.GetPtrNoConst() );

		//�X�g���[�������
		spSrcStream->vClose();

		//�f�[�^���t�@�C�����R���e�i�ɓ����
		skyString key[ FILE_LEN_MAX ];
		CPathUtil_::GetFileName( ( *it )->GetString() , key , FILE_LEN_MAX );
		m_pFileDataHash->Set( CHS( key ) , propDataPtr );
	}
}

void CPropertyManager::Dispose()
{
	//�t�@�C�����R���e�i���폜
	SAFE_DELETE( m_pFileDataHash );
	m_pFileDataHash = skyNull;
}

const hash::CHashStringPtr &CPropertyManager::GetString( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index )
{
	const hash::CHashStringPtrListPtrHashPtr &hashPtr = ( *m_pFileDataHash )[ fileName ];
	return CPropertyFileUtil_::GetString( key , *hashPtr.GetPtrNoConst() , index );
}

s32 CPropertyManager::Gets32( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index )
{
	const hash::CHashStringPtrListPtrHashPtr &hashPtr = ( *m_pFileDataHash )[ fileName ];
	return CPropertyFileUtil_::Gets32( key , *hashPtr.GetPtrNoConst() , index );
}

d64 CPropertyManager::Getd64( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index )
{
	const hash::CHashStringPtrListPtrHashPtr &hashPtr = ( *m_pFileDataHash )[ fileName ];
	return CPropertyFileUtil_::Getd64( key , *hashPtr.GetPtrNoConst() , index );
}

void CPropertyManager::Dump( stream::IStreamPtr &dest )
{
	FOREACH( hash::CHashStringPtrListPtrHashPtrHash , it , *m_pFileDataHash )
	{
		CPropertyFileUtil_::Dump( *( *it ).GetPtrNoConst() , dest );
	}
}

} } }