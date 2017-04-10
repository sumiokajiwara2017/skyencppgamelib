#pragma once

namespace sky { namespace lib {

/// <summary>
/// �ėp�^�̃X�^�b�N�N���X�i���̃N���X�͑���ɂ͑Ή����Ă��܂���B�������悤�Ȏg���������Ȃ��ł��������j
/// </summary>
template< typename T >
class CStack : public IteratorIf< T >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CStack( s32 size );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���Ή��j
	/// </summary>
	inline CStack( const CStack& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); }

	/// <summary>
	/// ������Z�q�i���Ή��j
	/// </summary>
	inline CStack& operator =( const CStack& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CStack();

	/// <summary>
	/// �X�^�b�N�ǉ�
	/// </summary>
	inline u32 Push( const T &data );
	inline u32 Push();

	/// <summary>
	/// �X�^�b�N�̈�ԏ�ɒl��ݒ肷��
	/// </summary>
	inline void Set( const T &data );

	/// <summary>
	/// �X�^�b�N����l�����o��
	/// </summary>
	inline skyBool Pop( T &data );
	inline skyBool Pop();
	inline skyBool PopBottom( T &data ); //�d���i���ʃX�^�b�N�ł���͂��Ȃ��B�j
	inline skyBool PopBottom();          //�d���i���ʃX�^�b�N�ł���͂��Ȃ��B�j

	/// <summary>
	/// �N���A����i�����I�ɂ̓J�E���^��0�ɖ߂������j
	/// </summary>
	inline void Clear();

	/// <summary>
	/// ���݂̊i�[�����擾����
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// �X�^�b�N�̃T�C�Y���擾����
	/// </summary>
	inline u32 GetSize() const;

	/// <summary>
	/// �X�^�b�N�̈�ԏ�̒l�̎Q�Ƃ��擾���܂��B�i���̎Q�Ƃ̓X�^�b�N�̏�Ԃ��ω�������l���ω�����B�����Ɏg�p���邱�Ɓj
	/// </summary>
	inline T &GetTop();

	/// <summary>
	/// �X�^�b�N�̈�ԉ��̒l�̎Q�Ƃ��擾���܂��B�i���̎Q�Ƃ̓X�^�b�N�̏�Ԃ��ω�������l���ω�����B�����Ɏg�p���邱�Ɓj
	/// </summary>
	inline T &GetBottom();

	/// <summary>
	/// �X�^�b�N�̒���O�Ƃ��A�C���f�b�N�X�w��Œl���擾���܂��B
	/// </summary>
	inline T &GetItem( s32 idx ) const;
	inline skyBool GetItem( s32 idx , T &data ) const;
	inline T &operator[]( s32 idx ) const;

	/// <summary>
	/// �󔻒�
	/// </summary>
	inline skyBool IsEmpty() const { return m_PushNum == 0; };

//---------------------�C�e���[�^�[�֘A-----------------------��

	/// <summary>
	/// �C�e���[�^�[
	/// </summary>
	class iterator : public Iterator<CStack< T >,T>
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		inline iterator( CStack< T > *pStack, s32 cursor );
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
	/// �w��̃C���f�b�N�X�ɃJ�[�\���𓖂Ă�
	/// </summary>
	inline skyBool SetItCursor( u32 idx , T* &data ) const;

	/// <summary>
	/// �C���f�b�N�X�Ń��X�g�폜
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

//---------------------�C�e���[�^�[�֘A-----------------------��

protected:

	/// <summary>
	/// �����o�[
	/// </summary>
	T*		m_pData;
	u32		m_Size;
	u32		m_PushNum;	//�X�^�b�N�̓o�^��

private:

	/// <summary>
	/// ���݂̃J�[�\���ʒu�A�h���X���擾
	/// </summary>
	inline skyBool	_SetCursor( u32 idx , T* &pCur ) const;

};

} }

#include "Stack.inl"