#pragma once

namespace sky { namespace lib { namespace memory {

//�e���v���[�gTypedef�S
struct CAllocatorDeleteDataHeader;
typedef DLinkList< CAllocatorDeleteDataHeader >	CAllocatorDeleteDataHeaderPtrDList;	//�o�������X�g��`

/// <summary>
/// �A���P�[�g�̈�폜���X�g�w�b�_�[���
/// </summary>
struct CAllocatorDeleteDataHeader : public CAllocatorDeleteDataHeaderPtrDList{};

//�e���v���[�gTypedef�S
struct CAllocatorDataHeader;
typedef DLinkList< CAllocatorDataHeader >	CAllocatorDataHeaderPtrDList;	//�o�������X�g��`

/// <summary>
/// �A���P�[�g�̈惊�X�g�w�b�_�[���
/// IAllocator���p�����č쐬���邷�ׂẴA���P�[�^�[���m�ۂ���̈�̃w�b�_�[�ɂ͕K�����̍\���̂�t����
/// �������邱�Ƃŋ��ʏ�������������B�K�{�̕ϐ��͕K�����ׂẴA���P�[�^�[���K�؂Ȓl���i�[����B
/// �C�ӂ̗̈�͎g�p��������Ύg�p����B
/// </summary>
struct CAllocatorDataHeader : public CAllocatorDataHeaderPtrDList
{
	/// <summary>
	/// �Ǘ��̈�T�C�Y�i�K�{�j�i�w�b�_�[�E�t�b�^�[���܂܂Ȃ��j
	/// </summary>
	u32                         m_Size;

	/// <summary>
	/// �j�󌟒m�pID�i�K�{�j
	/// </summary>
	u32                         m_CheckBreakId;

	/// <summary>
	/// �A���P�[�^�[ID�i�K�{�j
	/// </summary>
	u32                         m_AllocatorId;

	/// <summary>
	/// �A���P�[�^�[�|�C���^�i�K�{�j
	/// </summary>
	IAllocator*                 m_pAllocator;

	/// <summary>
	/// �A���b�N�^�C�v�i�K�{�j
	/// </summary>
	eAllocType                  m_eAllocType;

	/// <summary>
	/// �����J�E���g�i�C�Ӂj
	/// </summary>
	u32                         m_MargeCount;

	/// <summary>
	/// �O�����h�n���h���i�C�Ӂj
	/// </summary>
	u32                         m_GroundHndl;

	/// <summary>
	/// �A���[�i�n���h��ID�i�C�Ӂj
	/// </summary>
	u32				            m_ArenaHndl;

	/// <summary>
	/// �A���P�[�g����ID�i�C�Ӂj
	/// </summary>
	eAllocDirectionType         m_eDirType;

	/// <summary>
	/// �A���P�[�g�T�C�YID�i�C�Ӂj
	/// </summary>
	eAllocSizeType              m_eSizeType;

	/// <summary>
	/// �g�p���t���O�i�C�Ӂj
	/// </summary>
	skyBool		                m_IsUsed;

	/// <summary>
	/// �X�^�b�N�ԍ��i�C�Ӂj
	/// </summary>
	u32                         m_StackNo;

#ifdef SW_SKYLIB_DEBUG_ON
	/// <summary>
	/// �A���b�N�t�@�C�����̈ꕔ�i�C�Ӂj
	/// </summary>
	skyMString                  m_Path[ DEBUG_FILE_NAME_LENGHT ];
#endif

	/// <summary>
	/// �A���b�N�t�@�C���s���i�C�Ӂj
	/// </summary>
	u32                         m_Line;

	/// <summary>
	/// �폜���X�g�o�^�p�w�b�_�[�i���ꂪ��ԍŌ�ɂ���̂ɂ͈Ӗ�������j
	/// </summary>
	CAllocatorDataHeaderPtrDList  m_DeleteList;

	/// <summary>
	/// �t�@�C�����̐ݒ�
	/// </summary>
	inline void SetFileName( const skyMString *pFname )
	{
		SKY_UNUSED_ARG( pFname );
#ifdef SW_SKYLIB_DEBUG_ON
		//\0������
		const char *end = strrchr( pFname , '\0' );

		//�������ׂ������邽�߂ɖ��O�̈ꕔ���L�^
		const char *start = end - ( DEBUG_FILE_NAME_LENGHT - 1 );
		memcpy( m_Path , start , DEBUG_FILE_NAME_LENGHT );
#endif
	}
};

} } }