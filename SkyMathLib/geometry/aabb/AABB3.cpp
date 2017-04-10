#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace math {

CAABB3::CAABB3() : CGeometryBase()
{
}

skyBool CAABB3::IsZero() const
{
	return ( m_Max.Length() == 0 && m_Min.Length() == 0 );
}

CBasicVector3 CAABB3::Corner( s32 i ) const
{

    SKY_ASSERT_MSG( i >= 0, _T( "index over. [%d]" ) , i );
    SKY_ASSERT_MSG( i <= 7, _T( "index over. [%d]" ) , i );

	// 指定のインデックスの座標を返す。
	return CBasicVector3(
		( i & 1 ) ? m_Max.X : m_Min.X ,
		( i & 2 ) ? m_Max.Y : m_Min.Y,
		( i & 4 ) ? m_Max.Z : m_Min.Z
	);
}

void CAABB3::Empty()
{
	//型最大値を設定することでボックスを空にする
	const dectype kBigNumber = 1e37f;
	m_Min.X = m_Min.Y = m_Min.Z = kBigNumber;
	m_Max.X = m_Max.Y = m_Max.Z = -kBigNumber;
}

void CAABB3::AddX( dectype x )
{
	m_Min.X += x;
	m_Max.X += x;
}

void CAABB3::AddY( dectype y )
{
	m_Min.Y += y;
	m_Max.Y += y;
}

void CAABB3::AddZ( dectype z )
{
	m_Min.Z += z;
	m_Max.Z += z;
}

void CAABB3::Merge( const CBasicVector3 &p )
{

	// この点を含むのに必要なだけこのボックスを大きくする

	if ( p.X < m_Min.X ) m_Min.X = p.X;
	if ( p.X > m_Max.X ) m_Max.X = p.X;
	if ( p.Y < m_Min.Y ) m_Min.Y = p.Y;
	if ( p.Y > m_Max.Y ) m_Max.Y = p.Y;
	if ( p.Z < m_Min.Z ) m_Min.Z = p.Z;
	if ( p.Z > m_Max.Z ) m_Max.Z = p.Z;
}

void CAABB3::Merge( const CAABB3 &box )
{
	// ボックスに必要なだけ大きくする

	if ( box.m_Min.X < m_Min.X ) m_Min.X = box.m_Min.X;
	if ( box.m_Max.X > m_Max.X ) m_Max.X = box.m_Max.X;
	if ( box.m_Min.Y < m_Min.Y ) m_Min.Y = box.m_Min.Y;
	if ( box.m_Max.Y > m_Max.Y ) m_Max.Y = box.m_Max.Y;
	if ( box.m_Min.Z < m_Min.Z ) m_Min.Z = box.m_Min.Z;
	if ( box.m_Max.Z > m_Max.Z ) m_Max.Z = box.m_Max.Z;
}

void CAABB3::SetToTransformedBox( const CAABB3 &box , const math::CMatrix4x3 &m )
{
	// 空の場合は何もしない

	if (box.IsEmpty() )
	{
		Empty();
		return;
	}

	// 平行移動部分から始める

	m_Min = m_Max = GetTranslationVector(m);

	// ９つの行列の要素をそれぞれ調べ、新しいＡＡＢＢを計算する
	if ( m.M11 > 0.0f )
	{
		m_Min.X += m.M11 * box.m_Min.X; m_Max.X += m.M11 * box.m_Max.X;
	}
	else
	{
		m_Min.X += m.M11 * box.m_Max.X; m_Max.X += m.M11 * box.m_Min.X;
	}

	if ( m.M12 > 0.0f )
	{
		m_Min.Y += m.M12 * box.m_Min.X; m_Max.Y += m.M12 * box.m_Max.X;
	}
	else
	{
		m_Min.Y += m.M12 * box.m_Max.X; m_Max.Y += m.M12 * box.m_Min.X;
	}

	if ( m.M13 > 0.0f )
	{
		m_Min.Z += m.M13 * box.m_Min.X; m_Max.Z += m.M13 * box.m_Max.X;
	}
	else
	{
		m_Min.Z += m.M13 * box.m_Max.X; m_Max.Z += m.M13 * box.m_Min.X;
	}

	if ( m.M21 > 0.0f )
	{
		m_Min.X += m.M21 * box.m_Min.Y; m_Max.X += m.M21 * box.m_Max.Y;
	}
	else
	{
		m_Min.X += m.M21 * box.m_Max.Y; m_Max.X += m.M21 * box.m_Min.Y;
	}

	if ( m.M22 > 0.0f )
	{
		m_Min.Y += m.M22 * box.m_Min.Y; m_Max.Y += m.M22 * box.m_Max.Y;
	}
	else
	{
		m_Min.Y += m.M22 * box.m_Max.Y; m_Max.Y += m.M22 * box.m_Min.Y;
	}

	if ( m.M23 > 0.0f )
	{
		m_Min.Z += m.M23 * box.m_Min.Y; m_Max.Z += m.M23 * box.m_Max.Y;
	}
	else
	{
		m_Min.Z += m.M23 * box.m_Max.Y; m_Max.Z += m.M23 * box.m_Min.Y;
	}

	if ( m.M31 > 0.0f )
	{
		m_Min.X += m.M31 * box.m_Min.Z; m_Max.X += m.M31 * box.m_Max.Z;
	}
	else
	{
		m_Min.X += m.M31 * box.m_Max.Z; m_Max.X += m.M31 * box.m_Min.Z;
	}

	if ( m.M32 > 0.0f )
	{
		m_Min.Y += m.M32 * box.m_Min.Z; m_Max.Y += m.M32 * box.m_Max.Z;
	}
	else
	{
		m_Min.Y += m.M32 * box.m_Max.Z; m_Max.Y += m.M32 * box.m_Min.Z;
	}

	if ( m.M33 > 0.0f )
	{
		m_Min.Z += m.M33 * box.m_Min.Z; m_Max.Z += m.M33 * box.m_Max.Z;
	}
	else
	{
		m_Min.Z += m.M33 * box.m_Max.Z; m_Max.Z += m.M33 * box.m_Min.Z;
	}
}

skyBool	CAABB3::IsEmpty() const
{
	// どれかの軸が反転していないかをチェックする
	return ( m_Min.X > m_Max.X ) || ( m_Min.Y > m_Max.Y ) || ( m_Min.Z > m_Max.Z );
}

CBasicVector3	CAABB3::ClosestPointTo( const CBasicVector3 &p ) const
{
	// pを各実現上のボックスにプッシュする

	CBasicVector3 r;

	if ( p.X < m_Min.X )
	{
		r.X = m_Min.X;
	}
	else if ( p.X > m_Max.X )
	{
		r.X = m_Max.X;
	}
	else
	{
		r.X = p.X;
	}

	if ( p.Y < m_Min.Y )
	{
		r.Y = m_Min.Y;
	} 
	else if ( p.Y > m_Max.Y )
	{
		r.Y = m_Max.Y;
	}
	else
	{
		r.Y = p.Y;
	}

	if ( p.Z < m_Min.Z )
	{
		r.Z = m_Min.Z;
	}
	else if ( p.Z > m_Max.Z )
	{
		r.Z = m_Max.Z;
	}
	else
	{
		r.Z = p.Z;
	}

	return r;
}

s32	CAABB3::ClassifyPlane( const CBasicPlane &plane ) const
{
	//法線をチェックし、Ｄの値の最小値と最大値を計算する	
	CBasicVector3 n = plane.m_Normal;
	dectype       d = plane.d;

	dectype	m_MinD, m_MaxD;

	if ( n.X > 0.0f )
	{
		m_MinD = n.X * m_Min.X; m_MaxD = n.X * m_Max.X;
	}
	else
	{
		m_MinD = n.X * m_Max.X; m_MaxD = n.X * m_Min.X;
	}

	if ( n.Y > 0.0f )
	{
		m_MinD += n.Y * m_Min.Y; m_MaxD += n.Y * m_Max.Y;
	}
	else
	{
		m_MinD += n.Y * m_Max.Y; m_MaxD += n.Y * m_Min.Y;
	}

	if ( n.Z > 0.0f )
	{
		m_MinD += n.Z * m_Min.Z; m_MaxD += n.Z * m_Max.Z;
	}
	else
	{
		m_MinD += n.Z * m_Max.Z; m_MaxD += n.Z * m_Min.Z;
	}

	// 完全にこの平面の前面側にあるかをチェックする

	if ( m_MinD >= d )
	{
		return +1;
	}

	// 完全にこの平面の裏面側にあるかをチェックする

	if (m_MaxD <= d)
	{
		return -1;
	}

	// この平面と交差する

	return 0;
}

dectype	CAABB3::IntersectPlane( const CBasicPlane &plane , const CBasicVector3 &dir ) const
{

	CBasicVector3 n      = plane.m_Normal;
	dectype       planeD = plane.d;


	// 正規化されたベクトルが渡されているかを確認する

	SKY_ASSERT_MSG( CMathUtil::FAbs( n.Dot( n ) - 1.0f ) < 0.01f		, _T( "Argument vector n is not Normalize. " ) ); //引数ベクトルnが正規化されていません。
	SKY_ASSERT_MSG( CMathUtil::FAbs( dir.Dot( dir ) - 1.0f) < 0.01f	, _T( "Argument vector n is not Normalize. " ) );

	// 交差しない場合この巨大な数を返す

	const dectype kNoIntersection = 1e30f;

	// 視斜角を計算する（この平面の前面に向かって移動していることを確認する）

	dectype	dot = n.Dot( dir );
	if ( dot >= 0.0f )
	{
		return kNoIntersection;
	}

	// 法線をチェックし、Ｄの最小値と最大値を計算する
	// m_MiniDは最善面にある角の点のＤの値である

	dectype	minD, maxD;

	if ( n.X > 0.0f )
	{
		minD = n.X * m_Min.X; maxD = n.X * m_Max.X;
	}
	else
	{
		minD = n.X * m_Max.X; maxD = n.X * m_Min.X;
	}

	if (n.Y > 0.0f)
	{
		minD += n.Y * m_Min.Y; maxD += n.Y * m_Max.Y;
	}
	else
	{
		minD += n.Y * m_Max.Y; maxD += n.Y * m_Min.Y;
	}

	if (n.Z > 0.0f)
	{
		minD += n.Z * m_Min.Z; maxD += n.Z * m_Max.Z;
	}
	else
	{
		minD += n.Z * m_Max.Z; maxD += n.Z * m_Min.Z;
	}

	//完全にこの平面の他の側にすでにあるかをチェックする
	if ( maxD <= planeD )
	{
		return kNoIntersection;
	}

	//最前面の角の点を用いて標準の光線追跡の方程式を解く
	dectype	t = ( planeD - minD ) / dot;

	// すでに貫通しているか？
	if ( t < 0.0f )
	{
		return 0.0f;
	}

	// > 1の場合は、この時間内では衝突しなかった
	// これは呼び出し側がチェックすべき条件である。

	return t;
}

dectype	IntersectMovingAABB3 ( const CAABB3 &stationaryBox ,	const CAABB3 &movingBox , const CBasicVector3 &d )
{
	//交差しない場合はこの巨大な数を返す

	const dectype kNoIntersection = 1e30f;

	//対象となる時間すべてを含むようにインターバルを初期化する
	dectype	tEnter = 0.0f;
	dectype	tLeave = 1.0f;

	//
	// 各次元の重なりのインターバルを計算し、これまでに累積したインターバルと
	// このインターバルの共通部分を計算する
	// 空のインターバルが検出されるとすぐに、負の結果（交差しない）を返す
	// それぞれの場合で、各次元で、空または無限のインターバルに注意する
	//

	// X軸をチェックする

	if ( d.X == 0.0f )
	{

		// Xのインターバルは空か無限である

		if (
			( stationaryBox.m_Min.X >= movingBox.m_Max.X ) ||
			( stationaryBox.m_Max.X <= movingBox.m_Min.X )
		) {

			// 時間インターバルが空なので交差しない

			return kNoIntersection;
		}

		// 時間インターバルが無限 - 更新は必要ない

	} else {

		// 除算を１回で済ませる

		dectype	oneOverD = 1.0f / d.X;

		// 開始と終了が重なっている場合、時間の値を計算する

		dectype	xEnter = ( stationaryBox.m_Min.X - movingBox.m_Max.X ) * oneOverD;
		dectype	xLeave = ( stationaryBox.m_Max.X - movingBox.m_Min.X ) * oneOverD;

		// インターバルの順番をチェックする

		if (xEnter > xLeave)
		{
			TemplateUtil::Swap( xEnter, xLeave );
		}

		// インターバルを更新する

		if ( xEnter > tEnter ) tEnter = xEnter;
		if ( xLeave < tLeave ) tLeave = xLeave;

		// この結果が空かチェックする

		if ( tEnter > tLeave )
		{
			return kNoIntersection;
		}
	}
	
	// Y軸をチェックする

	if (d.Y == 0.0f) {

		// Yのインターバルは空か無限である

		if (
			( stationaryBox.m_Min.Y >= movingBox.m_Max.Y ) ||
			( stationaryBox.m_Max.Y <= movingBox.m_Min.Y )
		)
		{

			// 時間インターバルが空なので交差しない

			return kNoIntersection;
		}

		// 時間インターバルが無限 - 更新は必要ない

	}
	else
	{

		// 除算を１回で済ませる

		dectype	oneOverD = 1.0f / d.Y;

		// 開始と終了が重なっている場合、時間の値を計算する

		dectype	yEnter = ( stationaryBox.m_Min.Y - movingBox.m_Max.Y ) * oneOverD;
		dectype	yLeave = ( stationaryBox.m_Max.Y - movingBox.m_Min.Y ) * oneOverD;

		// インターバルの順番をチェックする

		if ( yEnter > yLeave )
		{
			TemplateUtil::Swap( yEnter , yLeave );
		}

		// インターバルを更新する

		if ( yEnter > tEnter ) tEnter = yEnter;
		if ( yLeave < tLeave ) tLeave = yLeave;

		// この結果が空かチェックする

		if ( tEnter > tLeave )
		{
			return kNoIntersection;
		}
	}
	
	// Z軸をチェック

	if ( d.Z == 0.0f )
	{

		// Zのインターバルは空か無限である

		if ( ( stationaryBox.m_Min.Z >= movingBox.m_Max.Z ) ||
			( stationaryBox.m_Max.Z <= movingBox.m_Min.Z ) )
		{
			// 時間インターバルが空なので交差しない

			return kNoIntersection;
		}

		// 時間インターバルが無限 － 更新は必要ない

	}
	else
	{

		// 除算を１回で済ませる

		dectype	oneOverD = 1.0f / d.Z;

		// 開始と終了が重なっている場合、時間の値を計算する

		dectype	zEnter = ( stationaryBox.m_Min.Z - movingBox.m_Max.Z ) * oneOverD;
		dectype	zLeave = ( stationaryBox.m_Max.Z - movingBox.m_Min.Z ) * oneOverD;

		// インターバルの順番をチェックする

		if ( zEnter > zLeave )
		{
			TemplateUtil::Swap( zEnter , zLeave );
		}

		// インターバルを更新する

		if ( zEnter > tEnter ) tEnter = zEnter;
		if ( zLeave < tLeave ) tLeave = zLeave;

		// この結果が空かチェックする

		if ( tEnter > tLeave )
		{
			return kNoIntersection;
		}
	}

	//ＯＫ，交差する
	//この交差が起こったときのパラメトリックな点を返す

	return tEnter;
}

} } }
