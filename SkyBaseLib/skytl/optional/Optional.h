#pragma once

namespace sky { namespace lib {

class COptional_empty {};

/// <summary>
/// �߂�l�N���X�x�[�X
/// </summary>
template < u64 size >
class COptionalBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    COptionalBase();
    COptionalBase( COptionalBase const & other );

	/// <summary>
	/// =���Z�q�̃I�[�o�[���C�h
	/// </summary>
    COptionalBase &operator = ( COptionalBase const & t );

	/// <summary>
	/// �L������
	/// </summary>
	skyBool const Valid() const;

	/// <summary>
	/// ��������
	/// </summary>
	skyBool const Invalid() const;

protected:

	/// <summary>
	/// �L���t���O
	/// </summary>
    bool m_bValid;

	/// <summary>
	/// �f�[�^
	/// </summary>
    u8 m_data[ size ];
};

/// <summary>
/// �߂�l�N���X
/// </summary>
template < class T >
class COptional : public COptionalBase< sizeof( T ) >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    COptional	();
    COptional	( T const & t );
	COptional	( COptional_empty const &);
    COptional	( COptional const & other );

	/// <summary>
	/// =���Z�q�̃I�[�o�[���C�h
	/// </summary>
    COptional & operator = ( T const & t );
    COptional & operator = ( COptional const & other );

	/// <summary>
	/// ==���Z�q�̃I�[�o�[���C�h
	/// </summary>
	skyBool const operator == ( COptional const & other ) const;

	/// <summary>
	/// <���Z�q�̃I�[�o�[���C�h
	/// </summary>
	skyBool const operator < ( COptional const & other) const;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
    virtual ~COptional();

	// Accessors.

	/// <summary>
	/// *���Z�q�̃I�[�o�[���C�h
	/// </summary>
	T const & operator * () const;
	T & operator * ();

	/// <summary>
	/// ->���Z�q�̃I�[�o�[���C�h
	/// </summary>
	T const * const operator -> () const;
	T		* const operator -> ();

	/// <summary>
	/// ������
	/// </summary>
	void Clear();

	/// <summary>
	/// �L�����H
	/// </summary>
	skyBool const Valid() const;

	/// <summary>
	/// �������H
	/// </summary>
	skyBool const Invalid() const;

private:

	/// <summary>
	/// �^�̎擾
	/// </summary>
    T const * const GetT		() const;
    T * const		GetT		();

	/// <summary>
	/// �R���X�g���N�^����
	/// </summary>
	void			Construct	( T const & t );

	/// <summary>
	/// �j������
	/// </summary>
    void			Destroy		();
};

} }

#include "Optional.inl"