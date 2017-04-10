#pragma once

/// <summary>
/// effect機能共通定数
/// </summary>
namespace sky { namespace lib { namespace graphic {

	static const s32 EFFECT_DESC_MAX_LEN = 64;

	//効果の組み合わせを示すBitフラグ
	static const u32 BIT_AMBIENT_LIGHT		= 1 << 0;
	static const u32 BIT_DIRECTIONAL_LIGHT	= 1 << 1;
	static const u32 BIT_SPOT_LIGHT_1		= 1 << 2;
	static const u32 BIT_SPOT_LIGHT_2		= 1 << 3;
	static const u32 BIT_SPOT_LIGHT_3		= 1 << 4;
	static const u32 BIT_POINT_LIGHT_1		= 1 << 5;
	static const u32 BIT_POINT_LIGHT_2		= 1 << 6;
	static const u32 BIT_POINT_LIGHT_3		= 1 << 7;
	static const u32 BIT_TEXTURE_MAPPING_1	= 1 << 8;
	static const u32 BIT_TEXTURE_MAPPING_2	= 1 << 9;
	static const u32 BIT_TEXTURE_MAPPING_3	= 1 << 10;

} } }
