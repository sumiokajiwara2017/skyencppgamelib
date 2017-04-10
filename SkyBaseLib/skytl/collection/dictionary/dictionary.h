#pragma once

namespace sky { namespace lib {

/// <summary>
/// ���I�Q�����z��R���e�i�N���X
/// </summary>
template< class T >
class CDictionary : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CDictionary();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CDictionary( u32 row = 0 , u32 col = 0 );

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	inline void Reset( u32 row , u32 col );

	/// <summary>
	/// �񐔂̎擾
	/// </summary>
	inline u32 GetColNum() const;

	/// <summary>
	/// �s���̎擾
	/// </summary>
	inline u32 GetRowNum() const;

	/// <summary>
	/// �l�̎Q�Ƃ̎擾
	/// </summary>
	inline T &Get( u32 row , u32 col ) const;

	/// <summary>
	/// �l�̎Q�Ƃ̎擾
	/// </summary>
	inline void Set( const T &val , u32 row , u32 col );

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	inline CDictionary& operator =( const CDictionary& src );

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	inline CDictionary( const CDictionary& obj );

protected:

	/// <summary>
	/// ��
	/// </summary>
	u32 m_Col;

	/// <summary>
	/// �s��
	/// </summary>
	u32 m_Row;

	/// <summary>
	/// �f�[�^
	/// </summary>
	T *m_pData;

	/// <summary>
	/// �T�C�Y�ύX
	/// </summary>
	inline void SizeChange( u32 row , u32 col );

};

/// <summary>
/// ���I�Q�����z��R���e�i�N���X
/// </summary>
template< class T >
class CDictionaryPrimitive : public CDictionary< T >
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CDictionaryPrimitive(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CDictionaryPrimitive( u32 row = 0 , u32 col = 0 );

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	inline CDictionaryPrimitive( const CDictionaryPrimitive& obj );

	/// <summary>
	/// �l�̎Q�Ƃ̎擾
	/// </summary>
	inline void Set( const T &val , u32 row , u32 col );

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	inline CDictionaryPrimitive& operator =( const CDictionaryPrimitive& src );

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// �f�o�b�O�v�����g
	/// </summary>
	inline void DebugPrint();

#endif

private:

	/// <summary>
	/// �T�C�Y�ύX
	/// </summary>
	inline void SizeChange( u32 row , u32 col );
};

typedef CDictionaryPrimitive< dectype > CDictionaryDec;

} }


#include "dictionary.inl"