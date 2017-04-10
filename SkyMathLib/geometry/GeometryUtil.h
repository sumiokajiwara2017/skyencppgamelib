#pragma once

namespace sky { namespace lib { namespace math {


/// <summary>
/// 幾何学便利処理群
/// </summary>
namespace GeometryUtil {

/// <summary>
/// 中心座標の座標を左上原点座標に変換する（渡された２Ｄ座標が四角形ポリゴンの中心座標だと仮定した値を返す）
/// </summary>
inline void ConvertCPosToLTPos( CBasicVector2 &dest , const CBasicVector2 &src , const CBasicSize &srcScreenSize ); 

/// <summary>
/// 中心座標の座標を左上原点座標に変換する（渡された２Ｄ座標が四角形ポリゴンの左上頂点座標だと仮定した値を返す）
/// 上記関数との違いは、渡された座標を中心とした矩形の左上角を左上原点に合わせること。
/// </summary>
inline void ConvertCPosToLTPos(  CBasicVector2 &dest , const CBasicVector2 &src , const CBasicSize &srcPolSize , const CBasicSize &srcScreenSize ); 

/// <summary>
/// 指定の空間範囲内で存在する物体がループするように座標を変換する
/// 左に限界まで進んだら右に出る。上に限界まで進んだら下に出るような座標変換
/// </summary>
inline CBasicVector3 ConvertRoopPos( const CBasicVector3 &max , const CBasicVector3 &min , const CBasicVector3 &pos );

/// <summary>
/// ２Ｄベクトルを通常座標から同次座標に変換する
/// </summary>
inline CBasicVector3 ConvertToHomogeneous( const CBasicVector2 &src );

/// <summary>
/// ２Ｄベクトルを同次座標から通常座標に変換する
/// </summary>
inline CBasicVector2 ConvertFromHomogeneous( const CBasicVector3 &src );

/// <summary>
/// ３Ｄベクトルを通常座標から同次座標に変換する
/// </summary>
inline CBasicVector4 ConvertToHomogeneous( const CBasicVector3 &src );

/// <summary>
/// ３Ｄベクトルを同次座標から通常座標に変換する
/// </summary>
inline CBasicVector3 ConvertFromHomogeneous( const CBasicVector4 &src );

}

} } }

#include "GeometryUtil.inl"
