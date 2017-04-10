#pragma once

namespace sky { namespace lib {

/// <summary>
/// �C�e���[�^�[����������R���e�i�̃C���^�[�t�F�[�X
/// </summary>
template<typename ITEM>
class IteratorIf : public base::SkyRefObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline IteratorIf(){};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IteratorIf(){};

	/// <summary>
	/// �w��̃J�[�\���̒l���擾����
	/// </summary>
	virtual skyBool SetItCursor( u32 idx , ITEM* &data ) const = 0;

	/// <summary>
	/// �w��̃J�[�\���̒l���폜����
	/// </summary>
	virtual skyBool DeleteIdx( u32 idx ) = 0;

	/// <summary>
	/// �v�f�̐���Ԃ�
	/// </summary>
	virtual u32 GetNum() const = 0;

};

/// <summary>
/// �C�e���[�^�[
/// </summary>
template<typename CONTAINER, typename ITEM>
class Iterator : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline Iterator( CONTAINER* pContainer, s32 cursor )
	{
		m_pContainer = pContainer;
		m_Cursor = cursor;
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Iterator(){}

	/// <summary>
	/// ����������
	/// </summary>
	inline void Init()
	{
		m_Cursor = 0;
	}

	/// <summary>
	/// �l�擾
	/// </summary>
	inline skyBool GetValue( ITEM &item ) const
	{
		ITEM &data;
        skyBool result = skyFalse;
		if ( m_pContainer->SetItCursor( m_Cursor , data ) == skyTrue )
		{
			item = data;
            result = skyTrue;
		}

		return result;
	}

	/// <summary>
	/// ++���Z�q�I�[�o�[���C�h�i�O�u���j
	/// </summary>
	inline void operator ++()
	{
		m_Cursor = Next();
	}
	/// <summary>
	/// ++���Z�q�I�[�o�[���C�h�i��u���j
	/// </summary>
	inline void operator ++( int )
	{
		m_Cursor = Next();
	}

	/// <summary>
	/// --���Z�q�I�[�o�[���C�h�i�O�u���j
	/// </summary>
	inline void operator --()
	{
		m_Cursor = Prev();
	}

	/// <summary>
	/// --���Z�q�I�[�o�[���C�h�i��u���j
	/// </summary>
	inline void operator --( int )
	{
		m_Cursor = Prev();
	}

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h
	/// </summary>
	inline skyBool operator ==(const Iterator &it) const
	{
		return ( m_Cursor == it.m_Cursor );
	}

	/// <summary>
	/// !=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline skyBool operator !=(const Iterator &it) const
	{
		return ( m_Cursor != it.m_Cursor );
	}

	/// <summary>
	/// *���Z�q�I�[�o�[���C�h
	/// </summary>
	inline ITEM &operator *()
	{
		ITEM *data = skyNull;
		if ( m_pContainer->SetItCursor( m_Cursor , data ) == skyFalse )
		{
			SKY_PANIC_MSG( _T( "Data Access error occurred." ) );
		}
 		return *data;
	}

	/// <summary>
	/// ���݂̃J�[�\���ʒu�̒l���폜����
	/// </summary>
	inline s32 Delete()
	{
		m_pContainer->DeleteIdx( m_Cursor );
		return m_Cursor;
	}

	/// <summary>
	/// ���݂̃J�[�\���ʒu��Ԃ�
	/// </summary>
	inline s32 GetIdx()
	{
		return m_Cursor;
	}

	/// <summary>
	/// ���̃J�[�\���ʒu��Ԃ�
	/// </summary>
	inline s32 Next()
	{
		s32 next = m_Cursor + 1;

		//����𒴂������Ȃ�
		u32 itemNum = m_pContainer->GetNum();
		if ( itemNum < ( u32 )next )
		{
			next = itemNum;
		}

		return next;
	}

	/// <summary>
	/// �O�̃J�[�\���ʒu��Ԃ�
	/// </summary>
	inline s32 Prev()
	{
		s32 prev = m_Cursor - 1;

		return prev;
	}

protected:

	/// <summary>
	/// ���݂̃J�[�\���ʒu
	/// </summary>
	s32			m_Cursor;

	/// <summary>
	/// ����Ώۃ��X�g
	/// </summary>
	CONTAINER*	m_pContainer;

};

} }
