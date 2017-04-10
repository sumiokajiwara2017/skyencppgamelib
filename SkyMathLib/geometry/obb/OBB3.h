#pragma once

namespace sky { namespace lib { namespace math {

class COBB3;
typedef SmartPointer< COBB3 > COBB3Ptr;

/// <summary>
/// ＯＢＢ（Oriented Bounding Box , 有向境界ボックス）クラス
/// </summary>
class COBB3 : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	COBB3();

	/// <summary>
	/// 空のＯＢＢか？
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// 作成
	/// </summary>
	void Create( const CBasicVector3Vector &points );

	/// <summary>
	/// OBBのマージ
	/// </summary>
	void Merge( const COBB3 &obb );

	/// <summary>
	/// インデックスで３軸を取得する
	/// </summary>
	CBasicVector3 GetAxis( eAxis axis ) const;

	/// <summary>
	/// インデックスで３軸の大きさの半分を得る
	/// </summary>
	dectype GetRadius( eAxis axis ) const;

	/// <summary>
	/// 点の最接近点を求める
	/// </summary>
	void ClosestPtPoint( const CBasicVector3 &point , CBasicVector3 &dest ) const;

	/// <summary>
	/// 点との距離の平方（２乗）を返す
	/// </summary>
	dectype SqDistPoint( const CBasicVector3 &point ) const;

	/// <summary>
	/// 大きさを適用したX軸の取得
	/// </summary>
	CBasicVector3 GetExtendedAxisX() const;

	/// <summary>
	/// 大きさを適用したY軸の取得
	/// </summary>
	CBasicVector3 GetExtendedAxisY() const;

	/// <summary>
	/// 大きさを適用したZ軸の取得
	/// </summary>
	CBasicVector3 GetExtendedAxisZ() const;

	/// <summary>
	/// 実効直径の取得
	/// </summary>
	dectype GetEffectiveDiameter( const CBasicVector3& direction ) const;

	/// <summary>
	/// 実効半径の取得
	/// </summary>
	dectype GetEffectiveRadius( const CBasicVector3& direction ) const;

	/// <summary>
	/// コーナーの取得
	/// 
	/// 以下の図のインデックスに沿ってコーナーを取得します。
	/// 0が中心で4が大きさです。
    ///     y+
    ///     |
    ///     1----2
    ///    /|   /|
    ///   5-+--4 |
    ///   | 0--+-3-- x+
    ///   |/   |/
    ///   6----7	 
	/// 
	/// </summary>
	CBasicVector3 GetCorner( u32 index ) const;

	/// <summary>
	/// 
	/// コーナー配列の取得
	/// 以下の図のインデックスに沿ってコーナーを取得します。
	/// 0が中心で4が大きさです。
	///    y+
	///    |
	///    1----2
	///   /|   /|
	///  5-+--4 |
	///  | 0--+-3-- x+
	///  |/   |/
	///  6----7
	/// /
	///z+
	/// 
	/// </summary>
	void GetCornerArray( CBasicVector3 corner[ 8 ] ) const;

	/// <summary>
	/// OBBに回転マトリックスを掛ける
	/// </summary>
	COBB3 Transform( const CRotationMatrix& matrix ) const;

	/// <summary>
	/// 平面との動的な交差
	/// </summary>
	dectype IntersectPlane( const CBasicPlane &plane ) const;

	/// <summary>
	/// directAccess
	/// </summary>
    dectype& X()             { return m_Pos.X; }
    const dectype& X() const { return m_Pos.X; };
    dectype& Y()             { return m_Pos.Y; };
    const dectype& Y() const { return m_Pos.Y; };
    dectype& Z()             { return m_Pos.Z; };
    const dectype& Z() const { return m_Pos.Z; };

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	COBB3& operator =( const COBB3& v )
	{
		m_Pos =  v.m_Pos;
		m_Sc =   v.m_Sc;
		m_Matrix = v.m_Matrix;
		return *this;
	}

	/// <summary>
	/// 中心座標
	/// </summary>
	CBasicVector3 m_Pos;

	/// <summary>
	/// ３軸方向の大きさ
	/// </summary>
	CBasicVector3 m_Sc;

	/// <summary>
	/// ３軸の方向（３軸の状態は行列の中から取り出せる）
	/// </summary>
	CRotationMatrix m_Matrix;

};

} } }