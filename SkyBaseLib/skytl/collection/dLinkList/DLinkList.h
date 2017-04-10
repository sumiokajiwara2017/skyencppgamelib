#pragma once

namespace sky { namespace lib {

/// <summary>
/// �萔
/// <summary>
namespace dlinklist_const
{
	static const u32			DLINK_LIST_CREATE_MAX_NUM	= 512;					//DLinkList�C���X�^���X�쐬�ő吔
};
using namespace dlinklist_const;

/// <summary>
/// �ėp�^�̑o�������X�g
/// ���Ӂj���Ȃ����ȍ��̂��߁A��{�A���P�[�^�[�ȊO�Ɏg�p���Ȃ��B
/// </summary>
template< typename T >
class DLinkList
{

public:

	/// <summary>
	/// �����̑O�̒l
	/// </summary>
	T			*m_pPrev;

	/// <summary>
	/// �����̎��̒l
	/// </summary>
	T			*m_pNext;

	/// <summary>
	/// �C���X�^���X�ԍ�
	/// </summary>
	u32			m_InstanceNo;

	/// <summary>
	/// ����ID
	/// </summary>
	s32			m_Uniqueid;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DLinkList();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DLinkList();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �P���R�[�h�Ƃ��Ă̏�����
	/// </summary>
	void RecInit();

	/// <summary>
	/// ������
	/// </summary>
	void Term();

	/// <summary>
	/// ���ɘA��
	/// </summary>
	u32 Link( T *pItem );
	u32 LinkItemNext( T *pItem , T *pNewItem );
	u32 LinkItemPrev( T *pItem , T *pNewItem );

	/// <summary>
	/// �A������
	/// </summary>
	void UnLink( T *pItem );

	/// <summary>
	/// �C���f�b�N�X�Ŏ擾
	/// </summary>
	T* IdxFromHead( u32 idx );
	T* IdxFromTail( u32 idx );

	/// <summary>
	/// ����ID�Ŏ擾
	/// </summary>
	T* GetUniqueid( s32 uniqueid );

	/// <summary>
	/// �i�[���̎擾
	/// </summary>
	u32 GetNum() const;

	/// <summary>
	/// �f�o�b�O�p�������`�F�b�N
	/// </summary>
	void _CheckNumAndData();

	/// <summary>
	/// �쐬�C���X�^���X��
	/// </summary>
	static u32  m_CreateInstanceNum;

	/// <summary>
	/// �g�p���t���O
	/// </summary>
	static skyBool m_UsedFlg[ DLINK_LIST_CREATE_MAX_NUM ]; 

	/// <summary>
	/// �i�[��
	/// </summary>
	static u32	m_LinkNum[ DLINK_LIST_CREATE_MAX_NUM ];

	/// <summary>
	/// �o�������X�g�̍ŏ㕔�istatic�Ȃ̂ŗB��̑��݁j
	/// </summary>
	static T	*m_pHead[ DLINK_LIST_CREATE_MAX_NUM ];

	/// <summary>
	/// �o�������X�g�̍ŉ����istatic�Ȃ̂ŗB��̑��݁j
	/// </summary>
	static T	*m_pTail[ DLINK_LIST_CREATE_MAX_NUM ];

};

} }

#include "DLinkList.inl"