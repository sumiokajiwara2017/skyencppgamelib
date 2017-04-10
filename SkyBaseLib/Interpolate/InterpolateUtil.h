#pragma once

/// <summary>
/// 補間タイプ
/// </summary>
enum eInterpolateType
{
	eInterpolateType_Linear = 0 ,     //線形
	eInterpolateType_Acceleration ,   //加速
	eInterpolateType_Deceleration ,   //減速
	eInterpolateType_None ,           //補間無し（ワープ）
};

namespace sky { namespace lib { namespace interp {

/// <summary>
/// 補間無し補間（tが1.0fになったときendの値がdestにかえる）
/// </summary>
template< typename T >
inline T& None( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// 線形補間計算
/// </summary>
template< typename T >
inline T& Linear( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// 加速補間計算
/// </summary>
template< typename T >
inline T& Acceleration( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// 減速補間計算
/// </summary>
template< typename T >
inline T& Deceleration( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// 補間ロジック共通実行
/// </summary>
template< typename T >
inline T& InterpolateCommon( T& dest, const T& start, const T& end, f32 t , eInterpolateType eType );

/// <summary>
/// デグリーの球面線形補間計算
/// startとendは0～360である必要がある
/// </summary>
inline f32& LinearDegree( f32& dest, f32 start, f32 end, f32 t );
		
/// <summary>
/// エルミート補間計算
/// </summary>
template< typename T >
inline T& Hermite( T& dest, const T& start, const T& end, const T& tan0, const T& tan1, f32 t );
		
/// <summary>
/// デグリーの球面エルミート補間
/// startとendは0～360である必要がある
/// </summary>
inline f32& HermiteDegree( f32& dest, f32 start, f32 end, f32 tan0, f32 tan1, f32 t );

} } }

#include "InterpolateUtil.inl"