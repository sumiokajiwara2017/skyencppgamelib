#include "StdAfx.h"
#include "SkyBaseLib/File.h"

#include "SkyBaseLib/Directory.h"

SkyImplementRTTI( sky::lib::file::CDiscFileController , sky::lib::base::SkyObject );

namespace sky { namespace lib { namespace file { 

//-------------------------------------< ���� >---------------------------------------------//

void CDiscFileController::Initialize()
{
}

void CDiscFileController::Dispose()
{
}

const u8 *CDiscFileController::Load( const hash::CHashStringPtr &filePath )
{
	const u8 *result = skyNull;

	//�����t�@�C���ɖ₢���킹�ɂ���
	result = const_cast< u8 * >( CBindFileManager_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	//�v�[���t�@�C���ɖ₢���킹�ɂ���
	result = const_cast< u8 * >( CFilePoolContena_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	//�c�h�r�b�t�@�C����ǂށi�����Ƀt�@�C�����v�[������B�v�[�����Ȃ��Ɩ߂�l�������ɂȂ邽�߁B�j
	CFilePoolContena_::Load( filePath );
	result = const_cast< u8 * >( CFilePoolContena_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	//�f�t�H���g�t�@�C����ǂ�
	result = const_cast< u8 * >( CDefaultFileManager_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	return skyNull; //������Ȃ�����
}

skyBool CDiscFileController::Delete( const hash::CHashStringPtr &filePath )
{
	skyBool result = skyFalse;

	//�����t�@�C���������ɂ���
	result = CBindFileManager_::DeleteFile( filePath );
	if ( result ) return result;

	//�v�[���t�@�C���������ɂ���
	result = CFilePoolContena_::DeleteFile( filePath );
	if ( result ) return result;

	return skyFalse;
}

} } }