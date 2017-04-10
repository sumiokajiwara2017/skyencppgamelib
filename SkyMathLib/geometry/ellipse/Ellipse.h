#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 楕円形情報クラス(座標はあえて３次元）
/// </summary>
template< typename T >
class CEllipse : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CEllipse();
	CEllipse( T x , T y, T radiusW , T radiusH , T rot );

	/// <summary>
	/// デストラクタ
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
	/// 座標
	/// </summary>
	CVector2< T >	m_Pos;

	/// <summary>
	/// 半径
	/// </summary>
	CSize< T >	    m_Radius;

	/// <summary>
	/// 回転角度
	/// </summary>
	T               m_Rot;

};

//基本円型
typedef CEllipse< dectype >     CBasicEllipse;

} } }

#include "Ellipse.inl"