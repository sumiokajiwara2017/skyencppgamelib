#pragma once

#ifdef SW_SKYLIB_DEBUG_ON

//namespace�ň͂܂ꂽ�N���X�̐�s�錾
namespace sky { namespace lib { namespace thread { class CriticalSection; } } }

namespace sky { namespace lib { namespace debuglogic {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �f�t�H���g���O�J�e�S��
/// </summary>
static const u32 LOG_CATEGORY_ALL = 1 << 0;
static const u32 LOG_CATEGORY_LIB = 1 << 1;
//32bit��2bit��L�萔�Ŏg���Ă���̂ŁA����30��ނ̃t���O�����܂��B�e�A�v���P�[�V�����ōD���Ƀ��[���t�������Ďg���Ă��������B

/// <summary>
/// ���O���x��
/// </summary>
enum eLogLevel
{
	eLogLevel_Error = 0 ,
	eLogLevel_Warning ,
	eLogLevel_Dump 
};

/// <summary>
/// �f�o�b�O�����Q
/// �������iInitialize�j�͕K�����C���X���b�h�ōs���Ă��������B
/// </summary>
class CDebugUtil : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ���
	/// </summary>
	static void Dispose();

	/// <summary>
	/// ���̃��\�b�h���Ăяo�����X���b�h�����C���X���b�h���ǂ������肷��
	/// </summary>
	static skyBool IsMainThread();

	/// <summary>
	/// �o�͂��郍�O���x����ݒ肷��
	/// </summary>
	static void SetLogLevel( eLogLevel eLevel );

	/// <summary>
	/// �o�͂��郍�O�J�e�S���[��ݒ肷��icategory�̓r�b�g�t���O�Ƃ��Ĉ����Ă܂��B����������Ŏg���Ă��������B�j
	/// </summary>
	static void SetLogCategory( u32 category );

	/// <summary>
	/// �o�͂��郍�O�J�e�S���[��ݒ����������
	/// </summary>
	static void DeleteLogCategory( u32 category );

	/// <summary>
	/// �t�@�C���X�g���[����ݒ肷��
	/// </summary>
	static void OpenFileStream( const skyString *path );
	static void CloseFileStream();
	static void WriteFileStream( const skyString *format , ... );

	/// <summary>
	/// �������X�g���[����ݒ肷��(�����̓��O���o�͂���Ă��郁�����̃A�h���X���i�[����Ă���ϐ��̃A�h���X������܂��B�j
	/// </summary>
	static void OpenMemoryStream( const void **symbolPointAdr );
	static void CloseMemoryStream();
	static void WriteMemoryStream( const skyString *format , ... );

	/// <summary>
	/// ���O���o�͂���i�o�͐�͐ݒ肳�ꂽ�X�g���[���S�āj
	/// </summary>
	static void Printf( eLogLevel eLevel , u32 category , const skyWString *format , ... );
	static void Printf( eLogLevel eLevel , u32 category , const skyMString *format , ... );

	/// <summary>
	/// �C�ӂ̕�����ƃX�^�b�N�g���[�X��
	/// </summary>
	static void PrintfStackTrace( const skyString *format , ... );

	/// <summary>
	/// �����̕`��
	/// </summary>
	static void PrintParam( s32 argc , skyString *argv[] );

	/// <summary>
	/// ��~
	/// </summary>
	static void Panic( const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyWString *format , ... );
	static void Panic( const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyMString *format , ... );

	/// <summary>
	/// �A�T�[�g
	/// skyTrue�Ȃ�~�܂�Ȃ��BskyFalse�Ȃ�~�܂�B���X�ǂ������Y���I
	/// </summary>
	static void Assert( skyBool isSuccess , const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyWString *format , ... );
	static void Assert( skyBool isSuccess , const skyString *pFileName , const skyString *pFuncName , s32 lineNo , const skyMString *format , ... );

	/// <summary>
	/// ������~
	/// </summary>
	static void Exit(  s32 status );

	/// <summary>
	/// �������_���v
	/// </summary>
	static void MemDump( u8 *adr , u32 before , u32 after , u32 lineCharNum );

	/// <summary>
	/// �w��̐��̔��p�󔒂��v�����g����
	/// </summary>
	static void PrintIndent( u32 indentNum );

private:

	/// <summary>
	/// �N���e�B�J���Z�N�V����
	/// </summary>
	static thread::CriticalSection *m_pCs;

	/// <summary>
	/// ���O���x��
	/// </summary>
	static eLogLevel m_eLogLevel;

	/// <summary>
	/// ���O�J�e�S��
	/// </summary>
	static u32 m_nLogCategory;

	//�e�@�\����
	CDebugUtil				();										//�R���X�g���N�^
	CDebugUtil				( const CDebugUtil& ){};				//�R�s�[�R���X�g���N�^
	virtual ~CDebugUtil	();											//�f�X�g���N�^
	CDebugUtil &operator=	(const CDebugUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h
};

} } }

#endif

#ifdef SW_SKYLIB_DEBUG_ON
#define CDebugUtilInit_()											sky::lib::debuglogic::CDebugUtil::Initialize()
#define CDebugUtil_													sky::lib::debuglogic::CDebugUtil
#define CDebugUtilDispose_()										sky::lib::debuglogic::CDebugUtil::Dispose()
#define SKY_SET_LOG_LEVEL( eLevel )									CDebugUtil_::SetLogLevel( eLevel );
#define SKY_SET_LOG_CATEGORY( category )							CDebugUtil_::SetLogCategory( category );
#define SKY_DEL_LOG_CATEGORY( category )							CDebugUtil_::DeleteLogCategory( category );
#define SKY_OPEN_FILE_STREAM( path )								CDebugUtil_::OpenFileStream( path )
#define SKY_CLOSE_FILE_STREAM()										CDebugUtil_::CloseFileStream()
#define SKY_WRITE_FILE_STREAM( format, ... )						CDebugUtil_::WriteFileStream( format, ##__VA_ARGS__ )
#define SKY_OPEN_MEM_STREAM( symbolpa )								CDebugUtil_::OpenMemoryStream( symbolpa )
#define SKY_CLOSE_MEM_STREAM()										CDebugUtil_::CloseMemoryStream()
#define SKY_WRITE_MEM_STREAM( format, ... )							CDebugUtil_::WriteMemoryStream( format, ##__VA_ARGS__ )
#define SKY_PRINTF( format, ... )									CDebugUtil_::Printf( sky::lib::debuglogic::eLogLevel_Error , sky::lib::debuglogic::LOG_CATEGORY_ALL , format, ##__VA_ARGS__ )
#define SKY_PRINTF_CATEGORY( loglevel , logcategory , format, ... )	CDebugUtil_::Printf( loglevel , logcategory , format, ##__VA_ARGS__ )
#define SKY_ASSERT( isSuccess )										CDebugUtil_::Assert( isSuccess , T__FILE__, T__FUNCTION__, __LINE__, _T( "Assert!! \n" ) )
#define SKY_ASSERT_MSG(isSuccess , format, ... )					CDebugUtil_::Assert( isSuccess , T__FILE__, T__FUNCTION__, __LINE__, format, ##__VA_ARGS__ )
#define SKY_ASSERT_MAIN_THREAD()									CDebugUtil_::Assert( CDebugUtil_::IsMainThread() , T__FILE__, T__FUNCTION__, __LINE__, format, _T( "���̃X���b�h�̓��C���X���b�h�ł͂���܂���B���̏����̓��C���X���b�h�ōs���Ă��������B \n" ) )
#define SKY_PANIC()													CDebugUtil_::Panic( T__FILE__, T__FUNCTION__, __LINE__, _T( "Panic!! \n" ) )
#define SKY_PANIC_MSG( format, ... )								CDebugUtil_::Panic( T__FILE__, T__FUNCTION__, __LINE__, format, ##__VA_ARGS__ )
#define SKY_PRINT_STACK_TRACE_MSG( format, ... )					CDebugUtil_::PrintfStackTrace( format, ##__VA_ARGS__ )
#define SKY_MEM_DUMP( adr, before, after, lineCharNum )				CDebugUtil_::MemDump( adr , before , after , lineCharNum )
#define SKY_PRINT_INDENT( num )										CDebugUtil_::PrintIndent( num )
#else
#define CDebugUtilInit_()
#define CDebugUtil_
#define CDebugUtilDispose_()
#define SKY_SET_LOG_LEVEL( eLevel )									
#define SKY_SET_LOG_CATEGORY( category )							
#define SKY_DEL_LOG_CATEGORY( category )							
#define SKY_OPEN_FILE_STREAM( path )					
#define SKY_CLOSE_FILE_STREAM()						
#define SKY_WRITE_FILE_STREAM( format, ... )		
#define SKY_OPEN_MEM_STREAM( symbolpa )				
#define SKY_CLOSE_MEM_STREAM()						
#define SKY_WRITE_MEM_STREAM( format, ... )		
#define SKY_PRINTF( format, ... )					
#define SKY_PRINTF_CATEGORY( logcategory , format, ... )
#define SKY_ASSERT( isSuccess )						
#define SKY_ASSERT_MSG(isSuccess , format, ... )    
#define SKY_ASSERT_MAIN_THREAD()
#define SKY_PANIC()									
#define SKY_PANIC_MSG( format, ... )				
#define SKY_PRINT_STACK_TRACE_MSG( format, ... )				
#define SKY_MEM_DUMP( adr, before, after, lineCharNum )
#define SKY_PRINT_INDENT( num )
#endif
