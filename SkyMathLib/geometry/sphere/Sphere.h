#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 球情報クラス
/// </summary>
template< typename T >
class CSphere : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSphere();
	CSphere( T x , T y , T z , T radius );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CSphere();

	/// <summary>
	/// 空の球か？
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// 球と球の合成
	/// </summary>
	const CSphere< T > &Merge( const CSphere< T > &sphere );

	/// <summary>
	/// 球と点の合成
	/// 球の中心が補正される。appendに比べて球が大きくなってしまう。
	/// </summary>
	const CSphere< T > &Merge( const CVector3< T > &point );

	/// <summary>
	/// 球に点を追加
	/// 球の中心は動かない。
	/// </summary>
	const CSphere< T > &Append( const CVector3< T > &point );

	/// <summary>
	/// 頂点配列から適切な球を生成する
	/// </summary>
	const CSphere< T > &CreateFromPoints( const CVectorPrimitive< CVector3< T > > &points );

	/// <summary>
	/// directAccess
	/// </summary>
    T& X();
    const T& X() const;
    T& Y();
    const T& Y() const;
    T& Z();
    const T& Z() const;
    T& R();
    const T& R() const;

	/// <summary>
	/// getter/setter
	/// </summary>
	void			SetPos( const CVector3< T >& vec );
	void			AddPos( const CVector3< T >& vec );
	const CVector3< T >&	GetPos() const;
	void			SetRadius( dectype radius );
	void			AddRadius( dectype radius );
	T				GetRadius() const;

private:

	/// <summary>
	/// 中心座標
	/// </summary>
	CVector3< T >	m_Pos;

	/// <summary>
	/// 半径
	/// </summary>
	T			m_Radius;
};

//基本円型
typedef CSphere< dectype >           CBasicSphere;
typedef SmartPointer< CBasicSphere > CBasicSpherePtr;

} } }

#include "Sphere.inl"