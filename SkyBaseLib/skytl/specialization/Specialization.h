#pragma once

/// <summary>
/// �e���v���[�g�̓��ꉻ�𗘗p������`�S
/// </summary>
namespace sky { namespace lib {

//---------------�e���v���[�g�ŏ���������g�p���邽�߂̓��ꉻskyBool��`--------------------��

/// <summary>
/// �e���v���[�gskyBool�\����
/// </summary>
template < skyBool b >
struct SKYTL_skyBool { typedef SKYTL_skyBool< b > ev; static const skyBool vl = b; };

//---------------�e���v���[�g�Ő��l�������g�p���邽�߂̓��ꉻ��`------------------------��

/// <summary>
/// �e���v���[�gs32�̍\����
/// </summary>
template < s32 n >
struct SKYTL_s32 { typedef SKYTL_s32< n > ev; static const s32 vl = n; };

/// <summary>
/// �e���v���[�gs32�̂��g�p���������Z
/// �y�g�p��z
/// s32 rs = sky::lib::SKYTL_s32_SUM< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_SUM { typedef SKYTL_s32< SKYTL_s32_1::ev::vl + SKYTL_s32_2::ev::vl > ev; };


/// <summary>
/// �e���v���[�gs32�̂��g�p���������Z
/// �y�g�p��z
/// s32 rs = sky::lib::SKYTL_s32_SUB< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_SUB { typedef SKYTL_s32< SKYTL_s32_1::ev::vl - SKYTL_s32_2::ev::vl > ev; };

/// <summary>
/// �e���v���[�gs32�̂��g�p�����|�Z
/// �y�g�p��z
/// s32 rs = sky::lib::SKYTL_s32_MUL< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_MUL { typedef SKYTL_s32< SKYTL_s32_1::ev::vl * SKYTL_s32_2::ev::vl > ev; };

/// <summary>
/// �e���v���[�gs32�̂��g�p�������Z
/// �y�g�p��z
/// s32 rs = sky::lib::SKYTL_s32_MOD< sky::lib::SKYTL_s32< 10 > , sky::lib::SKYTL_s32< 2 > >::ev::vl;
/// </summary>
template< class SKYTL_s32_1 , class SKYTL_s32_2 >
struct SKYTL_s32_MOD { typedef SKYTL_s32< SKYTL_s32_1::ev::vl / SKYTL_s32_2::ev::vl > ev; };

} }