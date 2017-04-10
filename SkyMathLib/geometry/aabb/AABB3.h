#pragma once

namespace sky { namespace lib { namespace math {

class CMatrix4x3;

class CAABB3;
typedef SmartPointer< CAABB3 > CAABB3Ptr;

/// <summary>
/// ＡＡＢＢ（Axis Aligned Bounding Box , 軸沿いボックス）クラス
///---------------------------------------------------------------------------
/// AABB3::corner
///
/// ８つの角点の１つを返す
/// これらの点は以下のように番号付けされる。
///
///            6                                7
///              ------------------------------
///             /|                           /|
///            / |                          / |
///           /  |                         /  |
///          /   |                        /   |
///         /    |                       /    |
///        /     |                      /     |
///       /      |                     /      |
///      /       |                    /       |
///     /        |                   /        |
///  2 /         |                3 /         |
///   /----------------------------/          |
///   |          |                 |          |
///   |          |                 |          |      +Y
///   |        4 |                 |          | 
///   |          |-----------------|----------|      |
///   |         /                  |         /  5    |
///   |        /                   |        /        |       +Z
///   |       /                    |       /         |
///   |      /                     |      /          |     /
///   |     /                      |     /           |    /
///   |    /                       |    /            |   /
///   |   /                        |   /             |  /
///   |  /                         |  /              | /
///   | /                          | /               |/
///   |/                           |/                ----------------- +X
///   ------------------------------
///  0                              1
///
/// Bit 0 は mini.X vs. max.X
/// Bit 1 は mini.Y vs. max.Y
/// Bit 2 は mini.Z vs. max.Z
/// </summary>
class CAABB3 : public CGeometryBase
{
public:

	/// <summary>
	/// 最大値と最小値（非常にシンプル）
	/// </summary>
	CBasicVector3 m_Min;
	CBasicVector3 m_Max;

	CBasicVector3 Size  () const { return m_Max - m_Min; }
	dectype       SizeX ()       { return m_Max.X - m_Min.X; }
	dectype       SizeY ()       { return m_Max.Y - m_Min.Y; }
	dectype       SizeZ ()       { return m_Max.Z - m_Min.Z; }
	CBasicVector3 Center() const { return (m_Min + m_Max) * 0.5f;	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAABB3();

	/// <summary>
	/// 空のＡＡＢＢか？
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// 移動
	/// </summary>
	void AddX( dectype x );
	void AddY( dectype y );
	void AddZ( dectype z );

	/// <summary>
	/// ８つの角の点の１つを返す
	/// </summary>
	CBasicVector3 Corner( s32 i ) const;

// ボックス操作

	/// <summary>
	/// これらの値に実際に大きい／小さい数字を設定することでボックスを空にする
	/// </summary>
	void Empty();

	/// <summary>
	/// ボックスに点を追加する
	/// </summary>
	void Merge( const CBasicVector3 &p );

	/// <summary>
	/// AABBをボックスに追加する
	/// </summary>
	void Merge( const CAABB3 &box );

	/// <summary>
	/// このボックスを座標変換し、新しいＡＡＢＢを計算する
	/// これは常に、少なくとも元のボックスと同じ大きさのＡＡＢＢとなることを忘れないように
	/// 相当大きくなる場合もある
	/// </summary>
	void SetToTransformedBox( const CAABB3 &box , const math::CMatrix4x3 &m );

// 内包／交差テスト

	/// <summary>
	/// このボックスが空の場合はskyTrueを返す
	/// </summary>
	skyBool IsEmpty() const;

	/// <summary>
	/// ある点に対してこのボックス上で最も近い点を返す
	/// </summary>
	CBasicVector3 ClosestPointTo( const CBasicVector3 &p ) const;

	/// <summary>
	/// ボックスが平面のどちら側にあるかを分類する
	/// </summary>
	s32 ClassifyPlane( const CBasicPlane &plane ) const;

	/// <summary>
	/// 平面との動的な交差
	/// 
	/// 動的なAABB― 平面の交差テストを実行する
	/// 
	/// nはこの平面の法線(正規化されているとする)
	/// planeDは、この平面の方程式p・n=dのD値である
	/// dirはAABBの移動方向
	/// 
	/// この平面は静止していると仮定する
	/// 
	/// 交差のパラメトリックな点(交差が起こる前に移動した距離)を返す
	/// 交差しない場合は、とても大きな数を返す
	/// 移動の長さに対してチェックする必要がある
	/// 
	/// この平面の前面との交差だけが検出される
	/// </summary>
	dectype IntersectPlane( const CBasicPlane &plane , const CBasicVector3 &dir ) const;

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	CAABB3& operator =( const CAABB3& v )
	{
		m_Min = v.m_Min;
		m_Max = v.m_Max;
		return *this;
	}
};

/// <summary>
/// 動いているＡＡＢＢが精子しているＡＡＢＢと交差しているときのパラメトリックな点を返す
/// 交差しない場合は＞１を返す
/// </summary>
dectype		IntersectMovingAABB3( const CAABB3 &stationaryBox ,	const CAABB3 &movingBox ,	const CBasicVector3 &d );

} } }