#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �~�`���N���X
/// </summary>
template< typename T >
class CCircle : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CCircle();
	CCircle( T x , T y, T radius );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CCircle();

	/// <summary>
	/// directAccess
	/// </summary>
    T& X();
    const T& X() const;
    T& Y();
    const T& Y() const;
    T& R();
    const T& R() const;

	/// <summary>
	/// getter/setter
	/// </summary>
	void			     SetPos( CVector2< T >& vec );
	const CVector2< T >& GetPos() const;
	void			     SetRadius( dectype radius );
	T				     GetRadius() const;

private:

	/// <summary>
	/// ���W
	/// </summary>
	CVector2< T >	m_Pos;

	/// <summary>
	/// ���a
	/// </summary>
	T			m_Radius;
};

//��{�~�^
typedef CCircle< dectype >     CBasicCircle;

} } }

#include "Circle.inl"