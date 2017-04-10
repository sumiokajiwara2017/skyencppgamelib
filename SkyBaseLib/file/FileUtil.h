#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< ���� >---------------------------------------------//

//�񓯊��t�@�C���ǂݍ��ݏ����̖߂�l�^
typedef SmartPointer< FunctionCallBackAsync_r< stream::IStreamPtr > > FileLoadAsyncRs;

//�񓯊��t�@�C���������݂̖߂�l�^
typedef SmartPointer< FunctionCallBackAsync_v > FileSaveAsyncRs;

/// <summary>
/// �t�@�C���֘A�������ʏ���
/// �e�L�X�g�t�@�C����ǂޏꍇ�A�ǂݍ��ރe�L�X�g�t�@�C���̕����R�[�h���d�v�ɂȂ�B
/// �����������オ���C�h�����ŁA�}���`�o�C�g�̃e�L�X�g�t�@�C����ǂ񂾏ꍇ�A�ǂݍ��񂾒���͕����������Ă���B
/// </summary>
class CFileUtil : base::SkyObject
{
public :

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ���
	/// </summary>
	static void Dispose();

	/// <summary>
	/// �t�@�C���̍X�V���Ԃ��擾����
	/// </summary>
	static u32		GetFileUpdateTime		( const skyString *pPath );

	/// <summary>
	/// �t�@�C���̍X�V���Ԃ��n�����l�ƈႤ���Ƃ𔻒肷��
	/// </summary>
	static skyBool IstFileUpdateTimeChange	( u32 time , const skyString *pPath );

//---------------�������Ƀt�@�C�������؂�Ȃ��ꍇ�������g��---------------------��

	/// <summary>
	/// �t�@�C���T�C�Y�̎擾
	/// </summary>
	static skyBool GetSize					( IFileInfoPtr &pFileInfo );

	/// <summary>
	/// �t�@�C�����I�[�v������i�g���I���������Ă��������BpFileInfo�͓����Ŏ��̂���������܂��j
	/// </summary>
	static skyBool	Open					( const skyString *pPath , IFileInfoPtr &pFileInfo , eOpenMode eMode ); //�Ǎ���p�ŃI�[�v���i�t�@�C����������΃G���[�j

	/// <summary>
	/// �t�@�C���̔C�ӂ̈ʒu����C�ӂ̃T�C�Y�̃f�[�^�����o���ipBuf�͎g���I�������DEL���Ă��������j
	/// </summary>
	static u32 Read							( IFileInfoPtr &pFileInfo , u8 *pBuf , u32 bufSize , u32 readSize , u32 pos );

	/// <summary>
	/// �t�@�C���̔C�ӂ̈ʒu�ɔC�ӂ̃T�C�Y�̃f�[�^����������
	/// </summary>
	static u32 Write						( IFileInfoPtr &pFileInfo , u8* pBuf , u32 bufSize , u32 writeSize , u32 pos );

	/// <summary>
	/// �t�@�C�������ipFileInfo�͓����Ŏ��̂��폜����܂��j
	/// </summary>
	static skyBool Close					( IFileInfoPtr &pFileInfo );

//---------------�������Ƀt�@�C�������؂�Ȃ��ꍇ�������g��---------------------��

//---------------�������Ƀt�@�C��������ꍇ�������g��--------------------------��

	/// <summary>
	/// �t�@�C���̓����ǂݍ���(���񓯊��ɂ��g�����߂ɂ����ĎQ�Ɠn���ɂ��Ă��Ȃ��j
	/// </summary>
	static stream::IStreamPtr LoadFile      ( const hash::CHashStringPtr pPath );

	/// <summary>
	/// �t�@�C���̔񓯊��ǂݍ��݁i�I������͖߂�l�̃I�u�W�F�N�g�̃��\�b�hIsEnd()���g�p���Ă��������B�j
	/// ���������Ń��[�J�[�X���b�h�ɏ������Ϗ����Ă��܂��BCCThreadPoolManager�����������A���[�J�[�X���b�h���쐬���Ă���g�p���Ă��������B����
	/// </summary>
	static FileLoadAsyncRs LoadFileAsync    ( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// �t�@�C���̔񓯊��ǂݍ��݃L�����Z��
	/// </summary>
	static void LoadFileAsyncCancel         ( FileLoadAsyncRs &retObject );

	/// <summary>
	/// �t�@�C���̓�����������(���񓯊��ɂ��g�����߂ɂ����ĎQ�Ɠn���ɂ��Ă��Ȃ��j
	/// </summary>
	static void			SaveFile            ( stream::IStreamPtr spFile , skyBool isResume = skyFalse  );

	/// <summary>
	/// �t�@�C���̓����������݁i�I������͖߂�l�̃I�u�W�F�N�g�̃��\�b�hIsEnd()���g�p���Ă��������B�j
	/// ���������Ń��[�J�[�X���b�h�ɏ������Ϗ����Ă��܂��BCCThreadPoolManager�����������A���[�J�[�X���b�h���쐬���Ă���g�p���Ă��������B����
	/// </summary>
	static FileSaveAsyncRs SaveFileAsync    ( stream::IStreamPtr &spFile , skyBool isResume = skyFalse  );

	/// <summary>
	/// �t�@�C���̔񓯊��������݃L�����Z��
	/// </summary>
	static void FileSaveAsyncCancel         ( FileLoadAsyncRs &retObject );

	/// <summary>
	/// �e�L�X�g�t�@�C���̓ǂݍ��݁i�o�b�t�@�ւ̓ǂ݂���j�ipBuf�͎g���I�������DEL���Ă��������j
	/// </summary>
	static skyBool LoadTextFileFromDisc		( const skyString *pPath , u8 **ppBuf , u32 *pReadSize );

	/// <summary>
	/// �e�L�X�g�t�@�C���̏������݁i�o�b�t�@����̏������݁j
	/// </summary>
	static skyBool SaveLineTextFileForDisc	( const skyString *pPath , u8 *pBuf , skyBool isResume = skyFalse  );

	/// <summary>
	/// �t�@�C���̓ǂݍ��݁i�o�b�t�@�ւ̓ǂ݂���j�ipBuf�͎g���I�������DEL���Ă��������j
	/// </summary>
	static skyBool LoadFileFromDisc			( const skyString *pPath , u8 **ppBuf , u32 *pReadSize );

	/// <summary>
	/// �t�@�C���̏������݁i�o�b�t�@����̏������݁j
	/// </summary>
	static skyBool SaveBinFileForDisc			( const skyString *pPath , const u8 *pBuf , u32 bufSize , skyBool isResume = skyFalse  );

//---------------�������Ƀt�@�C��������ꍇ�������g��--------------------------��

private:

	//�e�@�\����
	CFileUtil(){};											//�R���X�g���N�^
	CFileUtil( const CFileUtil& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CFileUtil(){};									//�f�X�g���N�^
	CFileUtil &operator=(const CFileUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};


//�C���X�^���X�A�N�Z�X������
#define CFileUtilInitialize_()		sky::lib::file::CFileUtil::Initialize()
#define CFileUtil_					sky::lib::file::CFileUtil
#define CFileUtilDispose_()			sky::lib::file::CFileUtil::Dispose()

#ifdef SKYLIB_PLATFORM_WIN32

struct FileInfoWin32 : public IFileInfo
{
    SkyTypeDefRTTI;

	FILE*	pFp;
};

/// �l�r�c�m�ɂ���
/// �e�L�X�g���[�h�ŊJ���ꂽ�ꍇ�̓}���`�o�C�g������Ƃ��āA
/// �o�C�i�����[�h�ŊJ���ꂽ�ꍇ�̓��C�h������Ƃ��ēǂݏo���܂��B
/// ���Ȃ킿Unicode������ǂޏꍇ��rb�ɂ��Ȃ���΂Ȃ�Ȃ��B
namespace OpenMode
{
	//�e�L�X�g���[�h�ib���Ȃ��̂Łj
	static const skyString*	rp		= _T( "r+" );	//�ǂݍ��ݏ������݉\�i�t�@�C����������΃G���[�j
	static const skyString*	wp		= _T( "w+" ); 	//�������ݓǍ��\�i�㏑���j�i�t�@�C����������΍쐬�j
	static const skyString*	ap		= _T( "a+" );	//�������ݓǍ��\�i�ǋL�j�i�t�@�C����������΍쐬�j

	//�o�C�i�����[�h
	static const skyString*	rpb		= _T( "r+b" );	//�ǂݍ��ݏ������݉\�i�t�@�C����������΃G���[�j
	static const skyString*	wpb		= _T( "w+b" ); 	//�������ݓǍ��\�i�㏑���j�i�t�@�C����������΍쐬�j
	static const skyString*	apb		= _T( "a+b" );	//�������ݓǍ��\�i�ǋL�j�i�t�@�C����������΍쐬�j
}

#endif

/// <summary>
/// ���̃v���b�g�t�H�[���ōł��l�C�e�B�u�Ȋ֐����g�p���ăt�@�C�������̏�Ԃ̂܂ܑS����������ɓǂ݂���B
/// file�@�\������������Ă��Ȃ��Ă��A�t�@�C����ǂނ��Ƃ��ł��܂��B
/// </summary>
inline skyBool Native_FileRead( const skyString *pPath , u8 **ppBuf , u32 *pReadSize )
{

#ifdef SKYLIB_PLATFORM_WIN32 //Win32��̎���
	
	FILE*	pFp;

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , _T( "rb" ) ) ) != 0 ) //�o�C�i�����[�h�ŊJ��
#else
	if ( ( pFp = _tfopen( pPath , _T( "rb" ) ) ) == 0 ) //�o�C�i�����[�h�ŊJ��
#endif
	{
		return skyFalse;
	}

//-------�t�@�C���T�C�Y���擾

	u32 saveSeek = ftell( pFp );			//���݈ʒu��ۑ�
	u32 fileSize = 0;
	fseek( pFp, 0, SEEK_END );
	fileSize = ftell( pFp );				//�t�@�C���T�C�Y�̌v��
	fseek( pFp, saveSeek, SEEK_SET );		//���݈ʒu�̕���

//-------�t�@�C���o�b�t�@�̊m��

//	*ppBuf = ( u8 * )MALLOC__( sizeof( u8 ) * fileSize , sky::lib::memory::DEFAULT_ALIGN , __FILE__ , __LINE__ );
	*ppBuf = ( u8 * )MALLOC__( sizeof( u8 ) * fileSize , __FILE__ , __LINE__ );

//-------�t�@�C����ǂ�

	if ( ( *pReadSize = fread( *ppBuf , 1 , fileSize , pFp) ) == skyNull )
	{
		return skyFalse;
	}

//-------�t�@�C�������

	fclose( pFp );
//		_fcloseall(); //�Ȃ�Ƃ�����g���ƑS�Ẵt�@�C��������悤��

	return skyTrue;
#endif

}

} } }