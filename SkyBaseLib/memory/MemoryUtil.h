#pragma once

#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
#include <crtdbg.h>
#endif

namespace sky { namespace lib { namespace memory {

//-------------------------------------< ���� >---------------------------------------------//

//���̂R�̃}�N���͊����ɒ�`����

#ifdef SKYLIB_IDE_VC10

/* �A���C�����g�t���������m�ۊ֐��BSIMD�Ή���������Ƃ��͂������������B
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
#define MALLOC__( size , aligne , pFname , line )		_aligned_malloc_dbg( size , aligne , pFname , line )
#define REALLOC__( p , size , aligne , pFname , line )	_aligned_realloc_dbg( p , aligne , size , pFname , line )
#define FREE__( p )								        _aligned_free_dbg( p )
#else
#define MALLOC__( size , aligne , pFname , line )		_aligned_malloc( size , aligne )
#define REALLOC__( p , size , aligne , pFname , line )	_aligned_realloc( p , size , aligne )
#define FREE__( p )								        _aligned_free( p )
#endif
*/
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
#define MALLOC__( size , pFname , line )		_malloc_dbg( size , _NORMAL_BLOCK , pFname , line )
#define REALLOC__( p , size , pFname , line )	_realloc_dbg( p , size , _NORMAL_BLOCK , pFname , line )
#define FREE__( p )								_free_dbg( p , _NORMAL_BLOCK )
#else
#define MALLOC__( size , pFname , line )		malloc( size )
#define REALLOC__( p , size , pFname , line )	realloc( p , size )
#define FREE__( p )								free( p )
#endif

#endif

#ifdef SKYLIB_IDE_QTCREATER

#define MALLOC__( size , pFname , line )	malloc( size )
#define REALLOC__( p , size , pFname , line )	realloc( p , size )
#define FREE__( p )					free( p )

#endif

class CMemoryUtil
{

public:

	/// <summary>
	/// �������m��
	/// </summary>
	static inline void *Malloc( u32 size , const skyMString *pFname , s32 line );

	/// <summary>
	/// �������Ċm��
	/// </summary>
	static inline void *Realloc( void *p , u32 size , const skyMString *pFname , s32 line );

	/// <summary>
	/// ���������
	/// </summary>
	static inline void Free( void *p );

	/// <summary>
	/// 0�N���A
	/// </summary>
	static inline void ClearZero( void* pData , u32 size );

	/// <summary>
	/// �w��̒l�ŃN���A
	/// </summary>
	static inline void ClearValue( void* pData , u32 value , u32 size );

	/// <summary>
	/// �������̒l���r
	/// </summary>
	static inline skyBool Cmp( const void *pData1 , const void *pData2 , u32 size );

	/// <summary>
	/// �R�s�[
	/// </summary>
	static inline void Copy( void* pDestData , const void* pSrcData ,  u32 srcSize );

	/// <summary>
	/// �w��̃|�C���^��擪�Ɏw��̃T�C�Y�������i�߂��|�C���^��Ԃ��B
	/// </summary>
	static inline void *MallocPointer( void *pSrc , u32 srcSize , u32 usedSize , u32 size );

	/// <summary>
	/// �w��̃|�C���^�𖖔��Ɏw��̃T�C�Y�������i�߂��|�C���^��Ԃ��B
	/// </summary>
	static inline void *BackMallocPointer( void *pSrc , u32 srcSize , u32 usedSize , u32 size );

	/// <summary>
	/// �w��̒l�ɋ߂��Q�̗ݏ�̒l�Ǝw����Ԃ�
	/// </summary>
	static inline void GetPowTow( u32 value , u32 *pOutValue , u32 *pOutIdx );

//-----�V�X�e���Ŏg�p----/

	/// <summary>
	/// �A���P�[�^�[ID�̔��s
	/// </summary>
	static u32  CreateAllocatorId();

	/// <summary>
	/// �A���P�[�^�[ID�̃��Z�b�g
	/// </summary>
	static void ResetAllocatorId();

};

//�C���X�^���X�A�N�Z�X������
#define CMemoryUtilInitialize_()		sky::lib::memory::CMemoryUtil::Initialize()
#define CMemoryUtil_					sky::lib::memory::CMemoryUtil
#define CMemoryUtilDispose_()			sky::lib::memory::CMemoryUtil::Dispose()

} } }

#include "MemoryUtil.inl"
