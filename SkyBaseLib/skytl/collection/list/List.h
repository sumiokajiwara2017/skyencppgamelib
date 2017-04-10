#pragma once

namespace sky { namespace lib {

namespace clist
{
	static const u32 ITEM_POOL_DEFAULT_NUM = 8;
}

/// <summary>
/// �ėp�^�̃��X�g�N���X�i���̃N���X�͑���ɂ͑Ή����Ă��܂���B�������悤�Ȏg���������Ȃ��ł��������j
/// </summary>

template< typename KEY , typename DATA , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class CList : public IteratorIf< DATA > , public ThreadingModel
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CList( u32 poolNum = clist::ITEM_POOL_DEFAULT_NUM );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���Ή��j
	/// </summary>
	inline CList( const CList& src ) : m_kItemPool( src.m_kItemPool.GetPoolNum() ) { SKY_PANIC(); }

	/// <summary>
	/// ������Z�q�i���Ή��j
	/// </summary>
	inline CList& operator =( const CList& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CList();

	/// <summary>
	/// ���X�g�����ɒǉ�
	/// </summary>
	inline DATA &AddFront();
	inline void AddFront( const DATA &data );

	/// <summary>
	/// ���X�g�����ɒǉ�
	/// </summary>
	inline DATA &AddTail();
	inline void AddTail( const DATA &data );

	/// <summary>
	/// �w��̃f�[�^�̌�ɒǉ�
	/// </summary>
	inline void AddNext( const DATA &frontData , const DATA &data );

	/// <summary>
	/// id���ɓo�^����Bid���d�������ꍇid�S�̖����ɓo�^�����B�܂�㏑���͂���܂���
	/// </summary>
	inline void AddTail( KEY id , const DATA &data );

	/// <summary>
	/// �C���f�b�N�X�Ń��X�g�擾
	/// </summary>
	inline skyBool GetIdx( u32 idx , DATA &data ) const;
	inline DATA &GetIdx( u32 idx ) const;
	inline DATA &operator[]( s32 idx ) const;

	/// <summary>
	/// �����Ɠ����I�u�W�F�N�g�����݂��邩�ǂ����T���B����������߂�l�ɃC���f�b�N�X��Ԃ�
	/// </summary>
	inline KEY	Find( const DATA &data ) const;

	/// <summary>
	/// �L�[�Œl�擾
	/// </summary>
	inline skyBool GetKey( KEY id , DATA &data ) const;

	/// <summary>
	/// �l�Ń��X�g�폜
	/// </summary>
	inline skyBool Delete( const DATA &data );

	/// <summary>
	/// �����̗v�f���폜����
	/// </summary>
	inline skyBool DeleteTail();

	/// <summary>
	/// �S�폜
	/// </summary>
	inline void DeleteAll();

	/// <summary>
	/// ���݂̊i�[��
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// ���X�g���󂩂ǂ������肷��
	/// </summary>
	inline skyBool IsEmpty() const;

//---------------------�C�e���[�^�[�֘A-----------------------��

	/// <summary>
	/// �C�e���[�^�[
	/// </summary>
	class iterator : public Iterator< CList< KEY , DATA , ThreadingModel > , DATA >
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		inline iterator( CList< KEY , DATA , ThreadingModel > *pList , s32 cursor );
	};

	/// <summary>
	/// �J�[�\����Begin()����O�ɂ��鎞True�iFOREACH_REVERS�̂��߁j
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// �����ʒu�J�[�\�����w���C�e���[�^�[�擾
	/// </summary>
	inline iterator Begin() const;

	/// <summary>
	/// �C�e���[�^�[�擾
	/// </summary>
	inline iterator End() const;

	/// <summary>
	/// �C�e���[�^�[�̎w�������ʒu���폜
	/// </summary>
	inline iterator Erases( iterator &it );

	/// <summary>
	/// �w��̃C�e���[�^�[�̎��̃C�e���[�^�[���擾
	/// </summary>
	inline iterator GetNextPos( iterator &it ) const;

	/// <summary>
	/// �w��̃C���f�b�N�X�ɃJ�[�\���𓖂Ă�
	/// </summary>
	inline skyBool SetItCursor( u32 idx , DATA* &data ) const;

	/// <summary>
	/// �C���f�b�N�X�Ń��X�g�폜
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

//---------------------�C�e���[�^�[�֘A-----------------------��

protected:

	/// <summary>
	/// ���X�g��擪����T�����Ďw��̂h�c�ɃJ�[�\���𓖂Ă�(�Ȃ�������skyFalse��Ԃ��j
	/// </summary>
	inline DATA &SetCursorSearchStartHead( KEY id );

	/// <summary>
	/// ���X�g�𖖔�����T�����Ďw��̂h�c�ɃJ�[�\���𓖂Ă�(�Ȃ�������skyFalse��Ԃ��j
	/// </summary>
	inline DATA &SetCursorSearchStartTail( KEY id );

	/// <summary>
	/// ���X�g�̗v�f�̌^
	/// </summary>
	struct ListItem
	{
		DATA			data;
		KEY				id;
		ListItem*		pPrev;
		ListItem*		pNext;
		virtual ~ListItem(){}
	};

	/// <summary>
	/// �����o�[
	/// </summary>
	ListItem					*m_pHead;		//���X�g�̐擪
	ListItem					*m_pTail;		//���X�g�̖���
	u32							m_nAddNum;		//���X�g�̓o�^��
	CMemoryPool< ListItem >		m_kItemPool;	//���X�g�v�f�̃v�[��

	/// <summary>
	/// �V�����A�C�e�����쐻���ĕԂ�
	/// </summary>
	inline ListItem* _CreateItem( KEY id , const DATA &data );
	inline ListItem* _CreateItem( KEY id );
	inline ListItem* _CreateItem( const DATA &data );
	inline ListItem* _CreateItem();

	/// <summary>
	/// �w��̃C���f�b�N�X�̒l��擪������`�T�����ĒT��(���Ȃ炸������͂��j
	/// </summary>
	inline void _SetCursorSearchStartHeadIdx( u32 idx, ListItem* &pCur ) const;

	/// <summary>
	/// �w��̃C���f�b�N�X�̒l�𖖔�������`�T�����ĒT���i���Ȃ炸������͂��j
	/// </summary>
	inline void _SetCursorSearchStartTailIdx( u32 idx, ListItem* &pCur ) const;

	/// <summary>
	/// �w��̂h�c�̒l��擪������`�T�����ĒT���i������Ȃ����Ƃ��j
	/// </summary>
	inline void _SetCursorSearchStartHead( KEY id, ListItem* &pCur ) const;

	/// <summary>
	/// �w��̂h�c�̒l�𖖔�������`�T�����ĒT��
	/// </summary>
	inline void _SetCursorSearchStartTail( KEY id, ListItem* &pCur ) const;

	/// <summary>
	/// �w��̃|�C���^�̒l��擪������`�T�����ĒT���i������Ȃ����Ƃ��j
	/// </summary>
	inline skyBool _SetCursor( DATA p, ListItem* &pCur );

	/// <summary>
	/// ���݂̃J�[�\���l���J������
	/// </summary>
	inline void _DelCursor( ListItem* &pCur );
};

template< typename KEY, typename DATA >
class CListMT : public CList< KEY , DATA , thread::ThreadingModels::CMultiThread >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CListMT();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CListMT();
};

} }

#include "List.inl"