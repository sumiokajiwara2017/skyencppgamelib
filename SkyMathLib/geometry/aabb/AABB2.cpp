#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace math {

CAABB2::CAABB2() : CGeometryBase()
{
}

skyBool CAABB2::IsZero() const
{
	return ( m_Max.Length() == 0 && m_Min.Length() == 0 );
}

CBasicVector2 CAABB2::Corner( s32 i ) const
{

    SKY_ASSERT_MSG( i >= 0, _T( "index over. [%d]" ) , i );
    SKY_ASSERT_MSG( i <= 7, _T( "index over. [%d]" ) , i );

	// 指定のインデックスの座標を返す。
	return CBasicVector2(
		( i & 1 ) ? m_Max.X : m_Min.X ,
		( i & 2 ) ? m_Max.Y : m_Min.Y
	);
}

void CAABB2::Empty()
{
	//型最大値を設定することでボックスを空にする
	const dectype kBigNumber = 1e37f;
	m_Min.X = m_Min.Y = kBigNumber;
	m_Max.X = m_Max.Y = -kBigNumber;
}

void CAABB2::AddX( dectype x )
{
	m_Min.X += x;
	m_Max.X += x;
}

void CAABB2::AddY( dectype y )
{
	m_Min.Y += y;
	m_Max.Y += y;
}

void CAABB2::Merge( const CBasicVector2 &p )
{

	// この点を含むのに必要なだけこのボックスを大きくする

	if ( p.X < m_Min.X ) m_Min.X = p.X;
	if ( p.X > m_Max.X ) m_Max.X = p.X;
	if ( p.Y < m_Min.Y ) m_Min.Y = p.Y;
	if ( p.Y > m_Max.Y ) m_Max.Y = p.Y;
}

void CAABB2::Merge( const CAABB2 &box )
{
	// ボックスに必要なだけ大きくする

	if ( box.m_Min.X < m_Min.X ) m_Min.X = box.m_Min.X;
	if ( box.m_Max.X > m_Max.X ) m_Max.X = box.m_Max.X;
	if ( box.m_Min.Y < m_Min.Y ) m_Min.Y = box.m_Min.Y;
	if ( box.m_Max.Y > m_Max.Y ) m_Max.Y = box.m_Max.Y;
}

void CAABB2::SetToTransformedBox( const CAABB2 &box , const math::CMatrix3x2 &m )
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
}

skyBool	CAABB2::IsEmpty() const
{
	// どれかの軸が反転していないかをチェックする
	return ( m_Min.X > m_Max.X ) || ( m_Min.Y > m_Max.Y );
}

CBasicVector2	CAABB2::ClosestPointTo( const CBasicVector2 &p ) const
{
	// pを各実現上のボックスにプッシュする

	CBasicVector2 r;

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

	return r;
}

dectype	IntersectMovingAABB2 ( const CAABB2 &stationaryBox ,	const CAABB2 &movingBox , const CBasicVector2 &d )
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

	//ＯＫ，交差する
	//この交差が起こったときのパラメトリックな点を返す

	return tEnter;
}

} } }
