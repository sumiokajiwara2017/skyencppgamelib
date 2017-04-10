#include "StdAfx.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/Time.h"

#include "SkyBaseLib/SKYTL.h"

namespace sky { namespace lib { namespace memory {

CAllocator::CAllocator( u32 blockSize ) 
{
	m_bIsDeposed = skyFalse;
	GroundUniqueid = 0;
	m_pCs = skyNull;
	m_BlockSize = blockSize;
	m_AllocatorId = CMemoryUtil_::CreateAllocatorId();
	m_eAllocType = eAllocType_AllocatorClass_New;
}

CAllocator::CAllocator( u32 blockSize , u32 id ) 
{
	m_bIsDeposed = skyFalse;
	GroundUniqueid = 0;
	m_pCs = skyNull;
	m_BlockSize = blockSize;
	m_AllocatorId = id;
	m_eAllocType = eAllocType_AllocatorClass_New;
}

CAllocator::~CAllocator()
{
	if ( m_pCs != skyNull )
	{
		thread::CriticalSection::Delete( m_pCs );
		m_pCs = skyNull;
		m_bIsDeposed = skyTrue;
	}
}

thread::CriticalSection* CAllocator::GetCriticalSection()
{
	if ( m_pCs == skyNull && m_bIsDeposed == skyFalse )
	{
		m_pCs = thread::CriticalSection::Create();
	}
	return m_pCs;
}

u32 CAllocator::CreateGround( u32 size )
{
	u32 result = UINT_MAX;

	if ( m_BlockSize > size )
	{
		SKY_PANIC_MSG( _T( "The memory doesn't suffice. \n" ) );
	}
	else
	{
		//�Œ��̃A���[�i���O���E���h�Ɠ����̃T�C�Y�w��Ŋm�ۂł���悤��sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter )�����Z���Ă���
		u32 realSize = size 
			+ sizeof( CGroundData )          + sizeof( CAllocatorDataHeader ) + sizeof ( CArenaData )          + sizeof( CAllocatorDataHeader )
			+ sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter );

		//�m�ۂ����������̈�̐擪������GroundData�N���X�̌^�Ƃ݂Ȃ��B
		CGroundData *pGroundData = new( MALLOC__( realSize , __FILE__ , __LINE__ ) ) CGroundData();

		//�A���P�[�^�[ID��o�^
		pGroundData->m_AllocatorId = m_AllocatorId;

		//�O���E���h�ɏ���ݒ肷��
		pGroundData->m_CheckBreakId		 = ALLOCATE_DATA_ID;
		pGroundData->m_Size		 = size	+ sizeof( CAllocatorDataHeader ) + sizeof( CArenaData )           + sizeof( CAllocatorDataHeader )
									    + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter );
		pGroundData->m_StartAdr	 = ( u32 )pGroundData;
		pGroundData->m_EndAdr	 = pGroundData->m_StartAdr + pGroundData->m_Size;
		pGroundData->m_pBuf      = ( u8 * )pGroundData + sizeof ( CGroundData );
		pGroundData->m_UsedSize  = 0;
		pGroundData->m_BlockSize = m_BlockSize;
		pGroundData->m_eAllocType= m_eAllocType;
		pGroundData->m_StackAllocator.AttachMemory( pGroundData->m_pBuf , pGroundData->m_Size , pGroundData->m_BlockSize , pGroundData->m_BlockSize , m_eAllocType );

#ifdef SW_SKYLIB_DEBUG_ON
		//�Ǘ��̈���w��̒l�œh��Ԃ��i�������}�b�v�������Ƃ��ɁA�O���E���h�������A�O�����h�O���A�g�p�̈悩���g�p�̈悩�킩��j
		CMemoryUtil_::ClearValue( pGroundData->m_pBuf , MEMORY_INIT_VALUE , pGroundData->m_Size );
#endif
		//�t�b�^�[�̒l��ݒ�
		CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pGroundData + sizeof( CGroundData ) + pGroundData->m_Size );
		pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

		//����ID��t��
		pGroundData->m_Uniqueid = GroundUniqueid;
		GroundUniqueid++;

		//�O���E���h���X�g�ɓo�^
		m_pGroundDataDList.Link( pGroundData );

		//����ID��Ԃ�
		result = pGroundData->m_Uniqueid;
	}
	return result;
}

void CAllocator::DeleteGround( u32 groundHndl )
{
	//�Ώۂ̃O���E���h�̐擪�|�C���^�擾
	CGroundData *pGround = m_pGroundDataDList.GetUniqueid( groundHndl );

	SKY_ASSERT_MSG( pGround->m_ArenaList.GetNum() == 0 , _T( "The arena still remains." ) );

	//�j��`�F�b�N
	pGround->CheckMemBreak();

	//�A���P�[�^�[ID�`�F�b�N
	SKY_ASSERT_MSG( pGround->m_AllocatorId == m_AllocatorId , _T( "AllocatorId id invalid. \n" ) );

	//�O���E���h���X�g����j��
	m_pGroundDataDList.UnLink( pGround );

	//�f�X�g���N�^
	pGround->~CGroundData();

	//�������������
	FREE__( pGround );
}

u32 CAllocator::CreateArena( u32 groundHndl , u32 size , const skyWString *arenaName )
{
	u32 hndl = 0;

	//�O���E���h�n���h�����s���̏ꍇ��~
	SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )groundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //���݂��Ȃ��O���E���h�ɑ΂��ă��������m�ۂ��ɍs���܂����B

	//�O���E���h�̎擾
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

	//�O���E���h����A���[�i�𐶐�����
	hndl = pGroundData->CreateArena( size , arenaName );

	return hndl;
}

void CAllocator::DeleteArena( u32 groundHndl , u32 arenaHndl )
{
	//�O���E���h�̎擾
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

	//�A���[�i�̍폜
	pGroundData->DeleteArena( arenaHndl );
}

void *CAllocator::Malloc( u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line )
{
	thread::CriticalLock lock( GetCriticalSection() );

	//�O���E���h���ł��Ă��Ȃ�������f�t�H���g�l�Ŋm�ۂ���
	if ( m_pGroundDataDList.GetNum() == 0 )
	{
		//�A���P�[�^�[ID�̏�����
		CMemoryUtil_::ResetAllocatorId();

		//�f�t�H���g�̃O���E���h�m��
		m_DefaultGroundHndl = CreateGround( START_GROUND_SIZE );

		//�f�t�H���g�̃A���[�i�m��
		m_DefaultArenaHndl = CreateArena( m_DefaultGroundHndl , START_ARENA_SIZE , START_ARENA_NAME );
	}

	//�O���E���h�n���h�����s���̏ꍇ��~
	SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )groundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //���݂��Ȃ��O���E���h�ɑ΂��ă��������m�ۂ��ɍs���܂����B

	//�O���E���h�̎擾
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

	//�T�C�Y�`�F�b�N
	SKY_ASSERT_MSG( pGroundData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the ground. " ) ); //�m�ۂ��悤�Ƃ��Ă���T�C�Y���O���E���h�T�C�Y�𒴂��Ă��܂��B

	//�A���[�i�n���h�����s���̏ꍇ��~
	SKY_ASSERT_MSG( pGroundData->m_ArenaList.GetNum() >= ( u32 )arenaHndl , _T( "It went to secure the memory for the arena that did not exist. " ) ); //���݂��Ȃ��A���[�i�ɑ΂��ă��������m�ۂ��ɍs���܂����B

	//�A���[�i�̎擾
	CArenaData *pArenaData = pGroundData->m_ArenaList.GetUniqueid( arenaHndl );

	//�T�C�Y�`�F�b�N
	SKY_ASSERT_MSG( pArenaData != skyNull , _T( "The memory doesn't suffice. " ) ); //������������܂���I
	SKY_ASSERT_MSG( pArenaData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the arena. " ) ); //�m�ۂ��悤�Ƃ��Ă���T�C�Y���A���[�i�T�C�Y�𒴂��Ă��܂��B

	//�A���[�i����m��
	void *pResultAdr = pArenaData->Malloc( size , groundHndl , arenaHndl , pFname , line );

	return pResultAdr;
}

void *CAllocator::Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line )
{
	void *pResultAdr = skyNull;
	if ( pData == skyNull )
	{
		pResultAdr = Malloc( size , groundHndl , arenaHndl , eAreaType , pFname , line );
	}
	else
	{
		thread::CriticalLock lock( GetCriticalSection() );

		//�O���E���h�n���h�����s���̏ꍇ��~
		SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )groundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //���݂��Ȃ��O���E���h�ɑ΂��ă��������m�ۂ��ɍs���܂����B

		//�O���E���h�̎擾
		CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

		//�T�C�Y�`�F�b�N
		SKY_ASSERT_MSG( pGroundData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the ground. " ) ); //�m�ۂ��悤�Ƃ��Ă���T�C�Y���O���E���h�T�C�Y�𒴂��Ă��܂��B

		//�A���[�i�n���h�����s���̏ꍇ��~
		SKY_ASSERT_MSG( pGroundData->m_ArenaList.GetNum() >= ( u32 )arenaHndl , _T( "It went to secure the memory for the arena that did not exist. " ) ); //���݂��Ȃ��A���[�i�ɑ΂��ă��������m�ۂ��ɍs���܂����B

		//�A���[�i�̎擾
		CArenaData *pArenaData = pGroundData->m_ArenaList.GetUniqueid( arenaHndl );

		//�T�C�Y�`�F�b�N
		SKY_ASSERT_MSG( pArenaData != skyNull , _T( "The memory doesn't suffice. " ) ); //������������܂���I
		SKY_ASSERT_MSG( pArenaData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the arena. " ) ); //�m�ۂ��悤�Ƃ��Ă���T�C�Y���A���[�i�T�C�Y�𒴂��Ă��܂��B

		//�A���[�i����m��
		pResultAdr = pArenaData->Realloc( pData , size , groundHndl , arenaHndl , pFname , line );

	}
	return pResultAdr;
}

void CAllocator::Delete( void *pData )
{
//	TIMER_OBJCET( _T( "CAllocator::Delete" ) );

	thread::CriticalLock lock( GetCriticalSection() );

	//�w�b�_�[�����o��
	CAllocatorDataHeader *pAllocateData = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );

	//�t�b�^�[�����o��
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pData + pAllocateData->m_Size );

	//�j��`�F�b�N
	SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( pAllocateData->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j

	//�m�ی��O���E���h�𔻕�
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( pAllocateData->m_GroundHndl );

	//�O���E���h�n���h�����s���̏ꍇ��~
	SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )pAllocateData->m_GroundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //���݂��Ȃ��O���E���h�ɑ΂��ă��������m�ۂ��ɍs���܂����B

	//�m�ی��A���[�i�𔻕�
	CArenaData *pArenaData = pGroundData->m_ArenaList.GetUniqueid( pAllocateData->m_ArenaHndl );

	//�A���[�i����폜
	pArenaData->Delete( pAllocateData );
}

void CAllocator::PrintMemory()
{
	CGroundData* pData = skyNull;
	for ( u32 i = 0 ; i < m_pGroundDataDList.GetNum() ; i++ )
	{
		pData = m_pGroundDataDList.IdxFromHead( i );
		pData->PrintMemory();
	}
}

void CAllocator::CheckMemBreak()
{
	CGroundData* pData = skyNull;
	for ( u32 i = 0 ; i < m_pGroundDataDList.GetNum() ; i++ )
	{
		pData = m_pGroundDataDList.IdxFromHead( i );
		pData->CheckMemBreak();
	}
}

} } }

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

/// <summary>
/// �A���P�[�^�[
/// </summary>
static sky::lib::memory::CAllocator	s_CAllocator( sky::lib::memory::START_BLOCK_SIZE , sky::lib::memory::SYSTEM_ALLOCATOR_ID );

u32 CreateMemoryGround( u32 size )
{
	SKY_ASSERT_MSG( size >= sky::lib::memory::GROUND_DATA_MIN_SIZE , _T( "The size doesn't suffice." ) ); //�T�C�Y������Ȃ�

	return s_CAllocator.CreateGround( size );
}

void DeleteMemoryGround( u32 groundHndl )
{
	s_CAllocator.DeleteGround( groundHndl );
}

u32 CreateMemoryArena( u32 groundHndl , u32 size , const skyString *pArenaName )
{
	SKY_ASSERT_MSG( size >= sky::lib::memory::ARENA_DATA_MIN_SIZE , _T( "The size doesn't suffice." ) ); //�T�C�Y������Ȃ�

	return s_CAllocator.CreateArena( groundHndl , size , pArenaName );
}

void DeleteMemoryArena( u32 groundHndl , u32 arenaHndl )
{
	s_CAllocator.DeleteArena( groundHndl , arenaHndl );
}

void *SkyNewMalloc_( u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	if ( eType == sky::lib::memory::eAllocType_Native_New )
	{
		return MALLOC__( size , pFname , line );
	}
	else
	{
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
		eAreaType;
		arenaHndl;
		groundHndl;
		return MALLOC__( size , pFname , line );
#else
		s_CAllocator.SetAllocType( sky::lib::memory::eAllocType_SkyNew );
		return s_CAllocator.Malloc( size , groundHndl , arenaHndl , eAreaType , pFname , line );
#endif
	}
}

void *SkyNewRealloc_( void* pData , u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	if ( eType == sky::lib::memory::eAllocType_Native_New )
	{//Native��realloc�̓��A���b�N����f�[�^��Native�ł���Ƃ݂Ȃ��B

		//�w�b�_�[�����o��
		sky::lib::memory::CAllocatorDataHeader *pOldAllocateData = ( sky::lib::memory::CAllocatorDataHeader * )( ( u8 * )pData - sizeof( sky::lib::memory::CAllocatorDataHeader ) );

		SKY_ASSERT_MSG( pOldAllocateData->m_CheckBreakId == sky::lib::memory::ALLOCATE_DATA_ID , _T( "This memory cannot lock rear. " ) ); //CAllocator��malloc�������������l�C�e�B�u�ȃ��A���b�N�ɂ������B

		return REALLOC__( pData , size , pFname , line );
	}
	else
	{
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
		eAreaType;
		arenaHndl;
		groundHndl;
		return REALLOC__( pData , size , pFname , line );
#else
		return s_CAllocator.Realloc( pData , size , groundHndl , arenaHndl , eAreaType , pFname , line );
#endif
	}
}

void SkyNewDelete_( void* pData , sky::lib::memory::eAllocType eType )
{
	if ( pData == skyNull ) return;

	if ( eType == sky::lib::memory::eAllocType_Native_New )
	{
		FREE__( pData );
	}
	else
	{
		//�w�b�_�[�����o��
		sky::lib::memory::CAllocatorDataHeader *pAllocateDataHeader = ( sky::lib::memory::CAllocatorDataHeader * )( ( u8 * )pData - sizeof( sky::lib::memory::CAllocatorDataHeader ) );

		//���[�U�[�����������A���P�[�^�[�Ŋm�ۂ��ꂽ���H
		if( pAllocateDataHeader->m_CheckBreakId == sky::lib::memory::ALLOCATE_DATA_ID && 
			pAllocateDataHeader->m_eAllocType == sky::lib::memory::eAllocType_AllocatorClass_New )
		{
			if ( pAllocateDataHeader->m_pAllocator == skyNull )
			{
				SKY_PANIC_MSG( _T( "There is a possibility that this memory has already been annulled. " ) ); //���̃������͂��łɔj�����ꂽ�����Ă���\��������܂��B
			}
			else
			{
				SKY_ASSERT_MSG( pAllocateDataHeader->m_pAllocator->m_CheckBreakId == sky::lib::memory::ALLOCATE_DATA_ID  , _T( "This memory cannot lock rear. " ) );//���̃������͂��łɔj������Ă��邩�A�A���P�[�^�[�|�C���^�����Ă�\��������B
				pAllocateDataHeader->m_pAllocator->Delete( pData );
			}
		}
		//�V�X�e�����X�^�e�B�b�N�Ő��������A���P�[�^�[�Ő�������ꂽ�f�[�^
		else
		{
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
			FREE__( pData );
#else
			s_CAllocator.Delete( pData );
#endif
		}
	}
}

void PrintMemory()
{
	s_CAllocator.PrintMemory();
}

void CheckMemBreak()
{
	s_CAllocator.CheckMemBreak();
}

const u32 GetDefaultMemoryGroundHandl()
{
	return s_CAllocator.m_DefaultGroundHndl;
}

const sky::lib::memory::CGroundData *GetMemoryGroundData( s32 groundHndl )
{
	return s_CAllocator.m_pGroundDataDList.GetUniqueid( groundHndl );
}

const u32 GetDefaultMemoryArenaHandl()
{
	return s_CAllocator.m_DefaultArenaHndl;
}

const sky::lib::memory::CArenaData *GetMemoryArenaData( s32 groundHndl , u32 arenaHndl )
{
	sky::lib::memory::CGroundData *pGroundData = s_CAllocator.m_pGroundDataDList.GetUniqueid( groundHndl );
	return pGroundData->m_ArenaList.GetUniqueid( arenaHndl );
}

#else
void *SkyNewMalloc_( u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
    SKY_UNUSED_ARG( eType );
    SKY_UNUSED_ARG( groundHndl );
    SKY_UNUSED_ARG( eAreaType );
    return MALLOC__( size , pFname , line );
}

void *SkyNewRealloc_( void* pData , u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
    SKY_UNUSED_ARG( eType );
    SKY_UNUSED_ARG( groundHndl );
    SKY_UNUSED_ARG( eAreaType );
    return REALLOC__( pData , size , pFname , line );
}

void SkyNewDelete_( void* pData , sky::lib::memory::eAllocType eType )
{
    SKY_UNUSED_ARG( eType );
    FREE__( pData );
}
#endif

