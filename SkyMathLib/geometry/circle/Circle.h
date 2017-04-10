#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 円形情報クラス
/// </summary>
template< typename T >
class CCircle : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CCircle();
	CCircle( T x , T y, T radius );

	/// <summary>
	/// デストラクタ
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
	/// 座標
	/// </summary>
	CVector2< T >	m_Pos;

	/// <summary>
	/// 半径
	/// </summary>
	T			m_Radius;
};

//基本円型
typedef CCircle< dectype >     CBasicCircle;

} } }

#include "Circle.inl"