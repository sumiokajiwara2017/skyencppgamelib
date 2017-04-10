#pragma once

namespace sky { namespace lib { namespace math {

class COBB2;
typedef SmartPointer< COBB2 > COBB2Ptr;

/// <summary>
/// ＯＢＢ（Oriented Bounding Box , 有向境界ボックス）クラス
/// </summary>
class COBB2 : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	COBB2();

	/// <summary>
	/// 空のＯＢＢか？
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// 作成
	/// </summary>
	void Create( const CBasicVector2Vector &points );

	/// <summary>
	/// OBBのマージ
	/// </summary>
	void Merge( const COBB2 &obb );

	/// <summary>
	/// インデックスで３軸を取得する
	/// </summary>
	CBasicVector2 GetAxis( eAxis axis ) const;

	/// <summary>
	/// インデックスで３軸の大きさの半分を得る
	/// </summary>
	dectype GetRadius( eAxis axis ) const;

	/// <summary>
	/// 点の最接近点を求める
	/// </summary>
	void ClosestPtPoint( const CBasicVector2 &point , CBasicVector2 &dest ) const;

	/// <summary>
	/// 点との距離の平方（２乗）を返す
	/// </summary>
	dectype SqDistPoint( const CBasicVector2 &point ) const;

	/// <summary>
	/// 大きさを適用したX軸の取得
	/// </summary>
	CBasicVector2 GetExtendedAxisX() const;

	/// <summary>
	/// 大きさを適用したY軸の取得
	/// </summary>
	CBasicVector2 GetExtendedAxisY() const;

	/// <summary>
	/// 実効直径の取得
	/// </summary>
	dectype GetEffectiveDiameter( const CBasicVector2& direction ) const;

	/// <summary>
	/// 実効半径の取得
	/// </summary>
	dectype GetEffectiveRadius( const CBasicVector2& direction ) const;

	/// <summary>
	/// コーナーの取得
	/// 
	/// 以下の図のインデックスに沿ってコーナーを取得します。
	/// 0が中心で4が大きさです。
    /// y+
    /// |
    /// 1-+--2
    /// |    |
    /// |    |
    /// 0----3-- x+	 
	/// </summary>
	CBasicVector2 GetCorner(int index) const;

	/// <summary>
	/// 
	/// コーナー配列の取得
	/// 以下の図のインデックスに沿ってコーナーを取得します。
	/// 0が中心で4が大きさです。
    /// y+
    /// |
    /// 1-+--2
    /// |    |
    /// |    |
    /// 0----3-- x+	 
	/// </summary>
	void GetCornerArray( CBasicVector2 corner[ 4 ] ) const;

	/// <summary>
	/// OBBに回転マトリックスを掛ける
	/// </summary>
	COBB2 Transform( const CMatrix3x2& matrix ) const;

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

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	COBB2& operator =( const COBB2& v )
	{
		m_Pos =  v.m_Pos;
		m_Sc =   v.m_Sc;
		m_Matrix = v.m_Matrix;
		return *this;
	}

	/// <summary>
	/// 中心座標
	/// </summary>
	CBasicVector2 m_Pos;

	/// <summary>
	/// ３軸方向の大きさ
	/// </summary>
	CBasicVector2 m_Sc;

	/// <summary>
	/// ３軸の方向（３軸の状態は行列の中から取り出せる）
	/// </summary>
	CMatrix3x2 m_Matrix;

};

} } }