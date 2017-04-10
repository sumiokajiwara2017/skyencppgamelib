#pragma once

namespace sky { namespace lib {

namespace hashmap
{
	static const u32 ITEM_POOL_DEFAULT_NUM = 8;
}

/// <summary>
/// Hash�R���e�i�N���X�i���̃N���X�͑���ɂ͑Ή����Ă��܂���B�������悤�Ȏg���������Ȃ��ł��������j
/// </summary>
template< typename T , typename KEY = u32 , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class CHash : public IteratorIf< T > , public ThreadingModel
{
	
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CHash( s32 hashSize , u32 poolNum = hashmap::ITEM_POOL_DEFAULT_NUM );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���Ή��j
	/// </summary>
	inline CHash( const CHash& src ){ SKY_PANIC(); }

	/// <summary>
	/// ������Z�q�i���Ή��j
	/// </summary>
	inline CHash& operator =( const CHash& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CHash();

	/// <summary>
	/// �l��ݒ�
	/// </summary>
	inline T &Set( const hash::CHashStringPtr &hash );
	inline T &Set( const skyString *strId );
	inline T &Set( KEY id );
	inline void Set( const hash::CHashStringPtr &hash , const T &data );
	inline void Set( const skyString *strId , const T &data );
	inline void Set( KEY id , const T &data );

	/// <summary>
	/// �l�����݂��邩
	/// </summary>
	inline skyBool IsGet( KEY id );
	inline skyBool IsGet( const skyString *strId );
	inline skyBool IsGet( const hash::CHashStringPtr &hash );

	/// <summary>
	/// �l���擾
	/// </summary>
	inline skyBool Get( const skyString *strId , T &data ) const;
	inline T &Get( const skyString *strId ) const;
	inline T &operator[]( const skyString *strId ) const;

	inline skyBool Get( const hash::CHashStringPtr &hash , T &data ) const;
	inline T &Get( const hash::CHashStringPtr &hash ) const;
	inline T &operator[]( const hash::CHashStringPtr &hash ) const;

	inline skyBool Get( KEY id , T &data ) const;
	inline T &Get( KEY id ) const;
	inline T &operator[]( KEY id ) const;

	/// <summary>
	/// �l���폜
	/// </summary>
	inline skyBool Delete( const hash::CHashStringPtr &hash );
	inline skyBool Delete( const skyString *strId );
	inline skyBool Delete( KEY id );

	/// <summary>
	/// �l���폜
	/// </summary>
	inline void DeleteAll();

	/// <summary>
	/// ���݂̊i�[��
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// ���݂̊i�[��
	/// </summary>
	inline u32 GetHashSize() const;

	/// <summary>
	/// KeyList�̎擾
	/// </summary>
	inline void GetkeyList( hash::CHashStringPtrList &keyList ) const;

	/// <summary>
	/// ���X�g���󂩂ǂ������肷��
	/// </summary>
	inline skyBool IsEmpty() const;

//---------------------�C�e���[�^�[�֘A-----------------------��

	/// <summary>
	/// �C�e���[�^�[
	/// </summary>
	class iterator : public Iterator< CHash< T , KEY , ThreadingModel > , T >
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		inline iterator( CHash< T , KEY , ThreadingModel > *pHash , s32 cursor );
	};

	/// <summary>
	/// �J�[�\����Begin()����O�ɂ��鎞True�iFOREACH_REVERS�̂��߁j
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// �����ʒu�J�[�\�����w���C�e���[�^�[�擾
	/// </summary>
	inline iterator Begin();

	/// <summary>
	/// �C�e���[�^�[�擾
	/// </summary>
	inline iterator End();

	/// <summary>
	/// �C�e���[�^�[�̎w�������ʒu���폜
	/// </summary>
	inline iterator Erases( iterator &it );

	/// <summary>
	/// �w��̃J�[�\���̒l���擾����
	/// @@TODO���̃��W�b�N�͍������̂��߂ɍH�v�̗]�n���c���Ă���E�E�Ǝv���B
	/// </summary>
	inline skyBool SetItCursor( u32 idx , T* &data ) const;
	inline skyBool SetItCursor( u32 idx , T* &data , hash::CHashStringPtr &idst ) const;

	/// <summary>
	/// �w��̃J�[�\���̒l���폜����
	/// @@TODO���̃��W�b�N�͍������̂��߂ɍH�v�̗]�n���c���Ă���E�E�Ǝv���B
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

protected:

	struct _HashListTbl
	{
		KEY  					id;     					// ����ID�i�����j
		hash::CHashStringPtr	idst;						// ����ID�i�����j�������������Ə�񂪗򉻂��邽��
		T 						data;   					// �l
		struct _HashListTbl		*pNext; 					// ���̃f�[�^

		virtual ~_HashListTbl(){}
	};

	_HashListTbl				**m_ppListTbl;			//�n�b�V���{��
	u32							m_nSetNum;				//���݂̐ݒ萔
	u32							m_nHashSize;			//�n�b�V���L�[�͈́i�f���������炵���j
	CMemoryPool< _HashListTbl >	m_kItemPool;			//���X�g�v�f�̃v�[��

	/// <summary>
	/// Hash�l(���ۂ�Hash�e�[�u���̊i�[��j�v�Z
	/// </summary>
	inline u32 GetHashId( const skyString *pKeyStr ) const;

	/// <summary>
	/// ID��r
	/// </summary>
	inline skyBool EqId( KEY id , _HashListTbl *pList ) const;

	/// <summary>
	/// �l�̐ݒ�
	/// </summary>
	inline skyBool _Set( u32 id , u32 hashId , const T &data , const hash::CHashStringPtr &idst );

	/// <summary>
	/// �l���擾
	/// </summary>
	inline skyBool _Get( u32 id , u32 hashId , T* &data ) const;

	/// <summary>
	/// �l���폜
	/// </summary>
	inline skyBool _Delete( u32 id , u32 hashId );

	/// <summary>
	/// �w��̒l������
	/// </summary>
	inline void _DelList( _HashListTbl *pList );
};

template< typename T , typename KEY = s32 >
class CHashMT : public CHash< T , KEY , thread::ThreadingModels::CMultiThread >
{
	
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CHashMT( s32 hashSize );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CHashMT();
};

} }

#include "Hash.inl"