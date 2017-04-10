#pragma once

namespace sky { namespace lib { namespace file { 

/// <summary>
/// �o�C���h���f�[�^���X�g
/// </summary>
struct CBindSrcData : public base::SkyRefObject
{
	string::CString			path;
	u8						*pData;
	u32						dataSize;
};
typedef SmartPointer< CBindSrcData >		CBindSrcDataPtr;
typedef CList< u32 , CBindSrcDataPtr >		CBindSrcDataPtrPtrList;	//���X�g��`
typedef CHash< bind::DataHeader* >			CBindFilesHash;	//���X�g��`

/// <summary>
/// �o�C���h���[�e�B���e�B�֐��Q
/// </summary>
namespace CBindUtil
{
	/// <summary>
	/// bind::DataHeader�A�h���X����t�@�C���f�[�^���擾����
	/// </summary>
	inline u8 *GetFileData( bind::DataHeader *pData );

	/// <summary>
	/// �w��̃t�@�C���Q��A�����ăf�[�^��Ԃ�
	/// pCutPathFront���w�肷��ƁA�t�@�C���������ɕۑ������t�@�C���p�X�̐擪�Ǝw�肵�������񂪈�v�����ꍇ�A
	/// �t�@�C���p�X���������Č����t�@�C���ɕۑ������BWindows��̐�΃p�X��ێ�����Ă����邱�Ƃ̂ق��������͂��Ȃ̂ł��̋@�\�Ő�΃p�X�����J�b�g����
	/// </summary>
	inline skyBool BindFiles( string::CStringList &srcPathList , bind::FileHeader* &pDestBindData , skyBool isRepSlash = skyFalse , const skyString *pCutPathFront = _T( "" ) );

	/// <summary>
	/// �w��̃f�[�^�Q��A���B
	/// </summary>
	inline skyBool BindDatas( CBindSrcDataPtrPtrList &srcDataList , bind::FileHeader* &pDestBindData , skyBool isRepSlash = skyFalse , const skyString *pCutPathFront = _T( "" ) );

	/// <summary>
	/// �w��̘A���t�@�C������p�X���w�肵�ăt�@�C����ǂݍ���
	/// �ǂݍ��񂾃f�[�^�͂����܂Ō����t�@�C���̃f�[�^���̃|�C���^�Ȃ̂ŗ��p���I����Ă������Ȃ��ł��������B
	/// </summary>
	inline skyBool LoadFile( bind::FileHeader *pSrcBindData , const skyString *pSrcPath , bind::DataHeader* &pDestData );

	/// <summary>
	/// �w��̘A���t�@�C������t�@�C���p�X���L�[�ɂ���Hash�R���e�i���쐬����B�t�@�C���p�X�Ńt�@�C���f�[�^�ɍ����ɃA�N�Z�X�\
	/// </summary>
	inline void CreateBindFilesHash( bind::FileHeader *pSrcBindData , CBindFilesHash &destHash );
}

#define CBindUtil_					sky::lib::file::CBindUtil

//-------------------------------< ���� >----------------------------------------------

inline skyBool CBindUtil::BindFiles( string::CStringList &srcPathList , bind::FileHeader* &pDestBindData , skyBool isRepSlash , const skyString *pCutPathFront )
{
	//�f�[�^���X�g
	CBindSrcDataPtrPtrList srcDataList;

	//�p�X���X�g����f�[�^���X�g���쐬����
	FOREACH( string::CStringList , it , srcPathList )
	{
		u8 *bbuf;
		u32 readSize;
		if ( CFileUtil::LoadFileFromDisc( ( *it ) , &bbuf , &readSize ) == skyFalse )
		{
            SKY_PANIC_MSG( _T( "file can't open. %s \n" ) , ( *it ) );
			continue;
		}

		CBindSrcDataPtr dataPtr = CBindSrcDataPtr( NEW__( CBindSrcData , CBindSrcData() ) );
		dataPtr->path = ( *it );
		dataPtr->pData = bbuf;
		dataPtr->dataSize = readSize;

		srcDataList.AddTail( dataPtr );
	}

	//��������
	BindDatas( srcDataList , pDestBindData , isRepSlash , pCutPathFront );

	FOREACH( CBindSrcDataPtrPtrList , it , srcDataList )
	{
		DEL( ( *it )->pData ); //�q�[�v�Ɋm�ۂ����t�@�C���f�[�^������
	}

	return skyFalse;
}

inline u8 *CBindUtil::GetFileData( bind::DataHeader *pData )
{
	//�o�C���h����Ă���f�[�^�������ǂ��Ă���
	u8* pCursor = ( u8* )pData;

	//�w�b�_�[�T�C�Y���i�߂��A�h���X���t�@�C���f�[�^�A�h���X�ƂȂ�B
	return pCursor + sizeof ( bind::DataHeader );
}

inline skyBool CBindUtil::BindDatas( CBindSrcDataPtrPtrList &srcDataList , bind::FileHeader* &pDestBindData , skyBool isRepSlash , const skyString *pCutPathFront )
{
	//�܂��̓T�C�Y���v�Z�i�ŏ��Ƀt�@�C���w�b�_�[�̃T�C�Y��������j
	memSize bindDataSize = sizeof( bind::FileHeader );

	//�f�[�^�̃T�C�Y�ƃf�[�^�w�b�_�[�𑫂����T�C�Y��S�f�[�^�����Z����
	FOREACH( CBindSrcDataPtrPtrList , it , srcDataList )
	{
		//�f�[�^�w�b�_�[�ƃf�[�^�T�C�Y�����Z
		bindDataSize += sizeof ( bind::DataHeader ) + ( *it )->dataSize;
	}

	//�����t�@�C�����̗̈�m��
	pDestBindData = ( bind::FileHeader * )SkyMalloc( bindDataSize );
	u8* pCursor = ( u8 * )pDestBindData;

	//�S�̂�0�N���A
	CMemoryUtil_::ClearZero( pDestBindData, bindDataSize );

	//�w�b�_�[�����i�[
	CMemoryUtil_::Copy( pDestBindData->cCode , bind::g_cFileCode, sizeof( bind::g_cFileCode ) );
	pDestBindData->bIsBigEndian = endian::EndianUtil::GetEndian();
	pDestBindData->nFileBindCount = srcDataList.GetNum();
	pDestBindData->nVersion = bind::VERSION_NO;
	pDestBindData->nFileSize = bindDataSize;
	pCursor = pCursor + sizeof ( bind::FileHeader );

	bind::DataHeader *pData;
	FOREACH( CBindSrcDataPtrPtrList , it , srcDataList )
	{
		//�f�[�^�̐擪���f�[�^�w�b�_�[�ɂ���
		pData = ( bind::DataHeader * )pCursor;

		//�J�b�g�w�肪���Ă�����p�X���J�b�g����
		const skyString *savePath = ( *it )->path;
		if ( CStringUtil_::Len( pCutPathFront ) > 0 )
		{
			//�J�b�g�p�X�Ɠ��������񂪃p�X�̐擪�ɂ��邩�H�i�p�X�̐擪�ȊO�ɂ�������J�b�g�ΏۂƂ݂Ȃ��Ȃ��j
			if ( CStringUtil_::StrstrDistance( savePath , pCutPathFront ) == 0 )
			{//����̂ŃJ�b�g����i�|�C���^�̐擪���J�b�g�p�X���i�߂�j
				savePath = savePath + CStringUtil_::Len( pCutPathFront );
			}
		}

		u32 dataPathSize = CStringUtil_::LenByte( savePath );
        SKY_ASSERT_MSG( dataPathSize <= PATH_LEN_MAX , _T( "file length over. %s \n" ) , ( *it )->path );

		//�f�[�^�w�b�_�[���쐬����
		CMemoryUtil_::Copy( pData->cFilePath , savePath ,  dataPathSize );

		if ( isRepSlash == skyTrue )
		{
			//�X���b�V���u�������w�肪���Ă�����\�}�[�N���X���b�V���ɒu��������
			CStringUtil_::Replace( ( skyString *)pData->cFilePath , _T( "\\" ) , _T( "/" ) );
		}

		pData->fileSize = ( *it )->dataSize;
		pData->nCompressFlg = 0;

		//�J�[�\�����w�b�_�[����f�[�^����
		pCursor = pCursor + sizeof ( bind::DataHeader );

		//�f�[�^���R�s�[
		CMemoryUtil_::Copy( pCursor , ( *it )->pData , ( *it )->dataSize );

		//�J�[�\�������̃f�[�^�̐擪��
		pCursor = pCursor + ( *it )->dataSize;
	}

	return skyFalse;
}

inline skyBool CBindUtil::LoadFile( bind::FileHeader *pSrcBindData , const skyString *pSrcPath , bind::DataHeader* &pDestData )
{
	//�o�C���h����Ă���f�[�^�������ǂ��Ă���
	u8* pCursor = ( u8* )pSrcBindData;

	pCursor = pCursor + sizeof ( bind::FileHeader );
	bind::DataHeader *pData;
	for( u32 i = 0 ; i < pSrcBindData->nFileBindCount ; i++ )
	{
		//�f�[�^�̐擪���f�[�^�w�b�_�[�ɂ���
		pData = ( bind::DataHeader * )pCursor;

		if (  CStringUtil_::Comp( pSrcPath , ( skyString * )pData->cFilePath ) )
		{
			//�f�[�^�𔭌������̂ł��̃f�[�^��Ԃ�
			pDestData = pData;
			return skyTrue; //���������̂ŋ����I��
		}
		else
		{
			//���̃f�[�^��
			pCursor = pCursor + sizeof ( bind::DataHeader ) + pData->fileSize; 
		}
	}

	return skyFalse;
}

inline void CBindUtil::CreateBindFilesHash( bind::FileHeader *pSrcBindData , CBindFilesHash &destHash )
{
	//�o�C���h����Ă���f�[�^�������ǂ��Ă���
	u8* pCursor = ( u8* )pSrcBindData;

	pCursor = pCursor + sizeof ( bind::FileHeader );
	bind::DataHeader *pData;
	for( u32 i = 0 ; i < pSrcBindData->nFileBindCount ; i++ )
	{
		//�f�[�^�̐擪���f�[�^�w�b�_�[�ɂ���
		pData = ( bind::DataHeader * )pCursor;

		//Hash�Ɋi�[����
		destHash.Set( CHS( ( skyString * )pData->cFilePath ) , pData );

		//���̃f�[�^��
		pCursor = pCursor + sizeof ( bind::DataHeader ) + pData->fileSize; 
	}
}


} } }
