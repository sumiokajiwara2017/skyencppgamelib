#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �ȉ~�`���N���X(���W�͂����ĂR�����j
/// </summary>
template< typename T >
class CEllipse : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CEllipse();
	CEllipse( T x , T y, T radiusW , T radiusH , T rot );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CEllipse();

	/// <summary>
	/// directAccess
	/// </summary>
    T& X();
    const T& X() const;
    T& Y();
    const T& Y() const;
    T& RW();
    const T& RW() const;
    T& RH();
    const T& RH() const;
    T& ROT();
    const T& ROT() const;

	/// <summary>
	/// getter/setter
	/// </summary>
	void			SetPos( CVector2< T >& vec );
	CVector2< T >&	GetPos();
	void			SetRadius( dectype radiusW , dectype radiusH );
	CSize< T >&	    GetRadius();

private:

	/// <summary>
	/// ���W
	/// </summary>
	CVector2< T >	m_Pos;

	/// <summary>
	/// ���a
	/// </summary>
	CSize< T >	    m_Radius;

	/// <summary>
	/// ��]�p�x
	/// </summary>
	T               m_Rot;

};

//��{�~�^
typedef CEllipse< dectype >     CBasicEllipse;

} } }

#include "Ellipse.inl"