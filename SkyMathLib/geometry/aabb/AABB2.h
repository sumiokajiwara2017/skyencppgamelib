#pragma once

namespace sky { namespace lib { namespace math {

class CMatrix4x3;

class CAABB2;
typedef SmartPointer< CAABB2 > CAABB2Ptr;

/// <summary>
/// ＡＡＢＢ（Axis Aligned Bounding Box , 軸沿いボックス）クラス
///---------------------------------------------------------------------------
/// AABB3::corner
///
/// ８つの角点の１つを返す
/// これらの点は以下のように番号付けされる。
///
///  2                            3 
///   +----------------------------+
///   |                            |
///   |                            |      +Y
///   |                            |      |
///   |                            |      |
///   |                            |      |
///   |                            |      |
///   |                            |      |
///   |                            |      |     
///   |                            |      |    
///   |                            |      |   
///   |                            |      |  
///   |                            |      | 
///   |                            |      |
///   |                            |      ----------------- +X
///   ------------------------------
///  0                              1
///
/// Bit 0 は mini.X vs. max.X
/// Bit 1 は mini.Y vs. max.Y
/// </summary>
class CAABB2 : public CGeometryBase
{
public:

	/// <summary>
	/// 最大値と最小値（非常にシンプル）
	/// </summary>
	CBasicVector2 m_Min;
	CBasicVector2 m_Max;

	CBasicVector2 Size  () const { return m_Max - m_Min; }
	dectype       SizeX ()       { return m_Max.X - m_Min.X; }
	dectype       SizeY ()       { return m_Max.Y - m_Min.Y; }
	CBasicVector2 Center() const { return (m_Min + m_Max) * 0.5f;	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAABB2();

	/// <summary>
	/// 空のＡＡＢＢか？
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// 移動
	/// </summary>
	void AddX( dectype x );
	void AddY( dectype y );

	/// <summary>
	/// ４つの角の点の１つを返す
	/// </summary>
	CBasicVector2 Corner( s32 i ) const;

// ボックス操作

	/// <summary>
	/// これらの値に実際に大きい／小さい数字を設定することでボックスを空にする
	/// </summary>
	void Empty();

	/// <summary>
	/// ボックスに点を追加する
	/// </summary>
	void Merge( const CBasicVector2 &p );

	/// <summary>
	/// AABBをボックスに追加する
	/// </summary>
	void Merge( const CAABB2 &box );

	/// <summary>
	/// このボックスを座標変換し、新しいＡＡＢＢを計算する
	/// これは常に、少なくとも元のボックスと同じ大きさのＡＡＢＢとなることを忘れないように
	/// 相当大きくなる場合もある
	/// </summary>
	void SetToTransformedBox( const CAABB2 &box , const math::CMatrix3x2 &m );

// 内包／交差テスト

	/// <summary>
	/// このボックスが空の場合はskyTrueを返す
	/// </summary>
	skyBool IsEmpty() const;

	/// <summary>
	/// ある点に対してこのボックス上で最も近い点を返す
	/// </summary>
	CBasicVector2 ClosestPointTo( const CBasicVector2 &p ) const;

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	CAABB2& operator =( const CAABB2& v )
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
dectype		IntersectMovingAABB2( const CAABB2 &stationaryBox ,	const CAABB2 &movingBox ,	const CBasicVector2 &d );

} } }