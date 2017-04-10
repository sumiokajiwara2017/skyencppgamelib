#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file {

//�o�C�i���v���p�e�B�̉��
static void _ParseBinary( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest )
{
	

	//�t�@�C���w�b�_�[��ǂ�
	property::FileHeader fileheader;
	src->vReadStream( ( u8 * )&fileheader , sizeof( property::FileHeader ) );

	//�v���p�e�B�̐��Ń��[�v
	for ( u32 i = 0 ; i < fileheader.nPropertyCount ; i++ )
	{
		//�f�[�^�w�b�_�[��ǂ�
		property::DataHeader dataHeader;
		src->vReadStream( ( u8 * )&dataHeader , sizeof( property::DataHeader ) );

		u32 readSize = 0;

		//�L�[��ǂ�
		string::CString key = ( skyString * )src->vGetSeekData();
		readSize = key.GetSize();
		src->vAddSeek( key.GetSize() );//�L�[���V�[�N��i�߂�
//��1   hashtype key;
//      CMemoryUtil_::Copy( &key , src->vGetSeekData() , GET_HASH_VALUE_SIZE() );
//      src->vAddSeek( GET_HASH_VALUE_SIZE() );//�L�[���V�[�N��i�߂�

		hash::CHashStringPtrListPtr valueList( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
		while ( readSize < dataHeader.valueSize )
		{
			//�l��ǂ�
			string::CString value = ( skyString * )src->vGetSeekData();
			valueList->AddTail( CHS( value.Get() ) );
			readSize += value.GetSize();
			src->vAddSeek( value.GetSize() );//�l���V�[�N��i�߂�
		}

		dest.Set( key , valueList );
	}
}

//�e�L�X�g�v���p�e�B�f�[�^�̉��
static void _ParseText( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest )
{
	

#ifdef SKYLIB_PLATFORM_WIN32

	//�v���O������Unicode�̏ꍇ�t�@�C����Unicode�łȂ���΂Ȃ�Ȃ�
	if ( CStringUtil_::IsProgramUnicode() )
	{
		SKY_ASSERT_MSG( CMemoryUtil_::Cmp( src->vGetData() , &BOM , BOM_SIZE ) , _T( "Please make the file Unicode because the program is Unicode. " ) ); //�v���O������Unicode�Ȃ̂Ńt�@�C����Unicode�ɂ��Ă��������B
	}

#endif

	//BOM���������ꍇ�A�����ǂݔ�΂�
	if ( CMemoryUtil_::Cmp( src->vGetData() , &BOM , BOM_SIZE ) )
	{
		u16 bom;
		src->vReadStream( ( u8 *)&bom , BOM_SIZE );
	}

	//�X�g���[������ꕶ�����ǂݍ���ł���
    skyString c = 0;
	u32 mode = 0; //0��key�ǂݍ��݃��[�h�A1��value�ǂݍ��݃��[�h
	string::CStringBuffer key;
	string::CStringBuffer value;
	hash::CHashStringPtrListPtr valueList( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
	u32 lineNum = 0;
	while( src->vReadStream( ( u8* )&c , sizeof( skyString ) ) > 0 )
	{
		//�󔒂������炷���Ƃ΂�
		if ( c == EG || c == _T( ' ' ) || c == _T( '	' ) )
		{
			continue;
		}

		//�R�����g�������珈���������Ƃ΂�
		if ( c == _T( '#' ) )
		{
			//���̉��s�R�[�h�܂ň�C�ɓǂށB�ǂ񂾂珈����i�߂�B����if�X�e�[�g�����g���K��������߂܂���B
			while( c != EC )
			{
				if ( src->vReadStream( ( u8* )&c , sizeof( skyString ) ) == 0 )
				{
					c = EC;
					break;
				}
			}
		}

		if ( c == EC || c == EOF )
		{//key�ƒl���m�肵��

			lineNum++;

			//key���S���̋󔒂������ꍇ��s�������Ƒz�肵�Ď��ɍs��
            if ( key == _T( "" ) )
			{
				continue;
			}

			//�l������Ίi�[(�Ō�̂P�������J���}�̏ꍇ�A�l�͋󔒂ɂȂ��Ă�͂��j
            if ( value != _T( "" ) )
			{
				valueList->AddTail( CHS( value.Get() ) );
			}

			//value�ǂݍ��݃��[�h�ɂȂ��ĂȂ�������G���[
			SKY_ASSERT_MSG( mode == 1 , _T( "Property file error. line [ %d ] data [ %s ]." ) , lineNum , key.Get() ); //�v���p�e�B�t�@�C���G���[ %d�s��[ %s ] �L�q���s���ł��B�L�[�ƒl��=�ŋ�؂��Ă��������B
			
			//�l���󂾂�����G���[
			SKY_ASSERT_MSG( valueList->GetNum() > 0 , _T( "Property file error. line [ %d ] data [ %s ]." ) , lineNum , key.Get() ); //�v���p�e�B�t�@�C���G���[ %d�s��[ %s ] �L�q���s���ł��B �l����������ł��B

			//�L�[���d��������G���[
			SKY_ASSERT_MSG( !dest.IsGet( CHS( key.Get() ) ) , _T( "Property file error. line [ %d ] data [ %s ]." ) , lineNum , key.Get() ); //�v���p�e�B�t�@�C���G���[ %d�s��[ %s ] �L�[���d�����Ă��܂��B

			dest.Set( CHS( key.Get() ) , valueList );
			key.Free();
			value.Free();
			mode = 0;
			valueList.Delete(); //�Ǘ����Ă���A�h���X��j���Bdest���Ɉ����p����Ă��邩��Q�ƃJ�E���^�������邾��
			valueList.SetPtr( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
			continue;
		}

		if ( c == _T( '=' ) )
		{//�L�[���m�肵��

			mode = 1;
			continue;
		}

		if ( mode == 0 )
		{//�L�[���\�z
			key += c;
		}
		else
		{
			if ( c == _T( ',' ) )
			{//�l���m�肷��

				valueList->AddTail( CHS( value.Get() ) );
				value.Free();
			}
			else
			{//�l���\�z
				value += c;
			}
		}
	}

    if ( key != _T( "" ) )
	{
		//�l������Ίi�[(�Ō�̂P�������J���}�̏ꍇ�A�l�͋󔒂ɂȂ��Ă�͂��j
        if ( value != _T( "" ) )
		{
			valueList->AddTail( CHS( value.Get() ) );
		}
		dest.Set( CHS( key.Get() ) , valueList );
	}
}

void CPropertyFileUtil::Load( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest )
{
	//�o�C�i�����e�L�X�g���𔻒肷��
	if ( CMemoryUtil_::Cmp( src->vGetData() , property::g_cFileCode , sizeof( property::g_cFileCode ) ) )
	{//�o�C�i���̏ꍇ�o�C�i����͂���
		_ParseBinary( src , dest );
	}
	else
	{//�e�L�X�g�̏ꍇ�e�L�X�g��͂���
		_ParseText( src , dest );
	}
}

static skyString *FILE_HEADER_MARK = _T( "fileheadermark" );
static skyString *DATA_HEADER_MARK = _T( "dataheadermark" );
void CPropertyFileUtil::Compile( stream::IStreamPtr &src ,  stream::IStreamPtr &dest )
{
	//�f�[�^�̈ꎞ�i�[�̈�
	hash::CHashStringPtrListPtrHash propData( property::DEFAULt_HASH_SIZE );

	//�e�L�X�g�v���p�e�B�t�@�C������͂���
	_ParseText( src , propData );

	//�o�C�i���̃w�b�_�[�����
	property::FileHeader fileheader;
	CMemoryUtil_::Copy( fileheader.cCode , property::g_cFileCode , sizeof( property::g_cFileCode ) );
	fileheader.nVersion = property::VERSION_NO;
	fileheader.nPropertyCount = propData.GetNum();
	fileheader.bIsBigEndian = endian::EndianUtil::GetEndian();
	fileheader.nFileSize = sizeof( property::FileHeader );

	//�}�[�L���O
	dest->vMarkWrite( FILE_HEADER_MARK , sizeof( property::FileHeader ) );

	//��͌��ʃf�[�^����o�C�i���f�[�^���쐬����
	hash::CHashStringPtrList keyList;
	propData.GetkeyList( keyList );

	FOREACH( hash::CHashStringPtrList , keyit , keyList )
	{
		property::DataHeader dataHeader;

		//�}�[�L���O
		dest->vMarkWrite( DATA_HEADER_MARK , sizeof( property::DataHeader ) );

		//�L�[�̃T�C�Y�𑫂����ށi�����̒����o�C�g���ƕ����I�[���j
		dataHeader.valueSize = CStringUtil_::LenByte( ( *keyit )->GetString() ) + sizeof( skyString );
//��1   dataHeader.valueSize = GET_HASH_VALUE_SIZE();

		//�L�[����������
		dest->vWriteStream( ( u8 *)( *keyit )->GetString() , dataHeader.valueSize );
//��1   dest->vWriteStream( ( u8 *)( *keyit )->Get() , dataHeader.valueSize );

		FOREACH( hash::CHashStringPtrList , valueListIt , *propData[ *keyit ].GetPtrNoConst() )
		{
			u32 valueSize = CStringUtil_::LenByte( ( *valueListIt )->GetString() ) + sizeof( skyString );
			dest->vWriteStream( ( u8 *)( *valueListIt )->GetString() , valueSize );
			dataHeader.valueSize += valueSize;
		}

		//����������
		dest->vMarkReplace( DATA_HEADER_MARK , ( u8 *)&dataHeader , sizeof( property::DataHeader ) );

		//�t�@�C���T�C�Y�ɏ�������
		fileheader.nFileSize += dataHeader.valueSize;
	}

	//����������
	dest->vMarkReplace( FILE_HEADER_MARK , ( u8 *)&fileheader , sizeof( property::FileHeader ) );
}

const hash::CHashStringPtr &CPropertyFileUtil::GetString( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index )
{
	return dest[ key ]->GetIdx( index );
}

s32 CPropertyFileUtil::Gets32( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index )
{
	const hash::CHashStringPtr &value = dest[ key ]->GetIdx( index );

	return CStringUtil_::ConvStringTos32( value->GetString() );
}

d64 CPropertyFileUtil::Getd64( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index )
{
	const hash::CHashStringPtr &value = dest[ key ]->GetIdx( index );

	return CStringUtil_::ConvStringTod64( value->GetString() );
}

void CPropertyFileUtil::Dump( hash::CHashStringPtrListPtrHash &src , stream::IStreamPtr &dest )
{
	//��͌��ʃf�[�^����o�C�i���f�[�^���쐬����
	hash::CHashStringPtrList keyList;
	src.GetkeyList( keyList );

	FOREACH( hash::CHashStringPtrList , keyit , keyList )
	{
		//�L�[����������
		dest->vWriteStream( ( *keyit )->GetString() );
		dest->vWriteStream( _T( "=" ) );
		FOREACH( hash::CHashStringPtrList , valueListIt , *src[ *keyit ].GetPtrNoConst() )
		{
			//�l����������
			dest->vWriteStream( ( *valueListIt )->GetString() );
			dest->vWriteStream( _T( "," ) );
		}
		dest->vWriteStream( &EC );
	}
}

} } }
