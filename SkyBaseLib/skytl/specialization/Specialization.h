#pragma once

/// <summary>
/// テンプレートの特殊化を利用した定義郡
/// </summary>
namespace sky { namespace lib {

//---------------テンプレートで条件分岐を使用するための特殊化skyBool定義--------------------▽

/// <summary>
/// テンプレートskyBool構造体
/// </summary>
template < skyBool b >
struct SKYTL_skyBool { typedef SKYTL_skyBool< b > ev; static const skyBool vl = b; };

//---------------テンプレートで数値引数を使用するための特殊化定義------------------------▽

/// <summary>
/// テンプレートs32の構造体
/// </summary>
template < s32 n >
struct SKYTL_s32 { typedef SKYTL_s32< n > ev; static const s32 vl = n; };

/// <summary>
/// テンプレートs32のを使用した足し算
/// 【使用例】
/// s32 rs = sky::lib::SKYTL_s32_SUM< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_SUM { typedef SKYTL_s32< SKYTL_s32_1::ev::vl + SKYTL_s32_2::ev::vl > ev; };


/// <summary>
/// テンプレートs32のを使用した引き算
/// 【使用例】
/// s32 rs = sky::lib::SKYTL_s32_SUB< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_SUB { typedef SKYTL_s32< SKYTL_s32_1::ev::vl - SKYTL_s32_2::ev::vl > ev; };

/// <summary>
/// テンプレートs32のを使用した掛算
/// 【使用例】
/// s32 rs = sky::lib::SKYTL_s32_MUL< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_MUL { typedef SKYTL_s32< SKYTL_s32_1::ev::vl * SKYTL_s32_2::ev::vl > ev; };

/// <summary>
/// テンプレートs32のを使用した割算
/// 【使用例】
/// s32 rs = sky::lib::SKYTL_s32_MOD< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_MOD { typedef SKYTL_s32< SKYTL_s32_1::ev::vl / SKYTL_s32_2::ev::vl > ev; };

} }