#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �T�C�Y�N���X
/// </summary>
template< typename T >
class CSize : public base::SkyRefObject
{
public:

	/// <summary>
	/// �T�C�Y
	/// </summary>
	T		W;
	T		H;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSize();
	CSize( T w, T h );


	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CSize();

	/// <summary>
	/// �󔻒�
	/// </summary>
	skyBool IsEmpty();

	/// <summary>
	/// �ݒ�
	/// </summary>
	void Set( T w, T h );

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	CSize& operator =( const CSize& size );

	/// <summary>
	/// +���Z�q�I�[�o�[���C�h�i�v�f�̑����Z,�V�����C���X�^���X����������ĕԂ�܂��B�j
	/// </summary>
	CVector2< T > operator +( const CVector2< T >& v1 ) const;

};

//��{��`�^
typedef CSize< dectype >     CBasicSize;
typedef CSize< u32 >         CSize_u32;

} } }

#include "Size.inl"