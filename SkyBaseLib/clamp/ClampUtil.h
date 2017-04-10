#pragma once

namespace sky { namespace lib {


//-----------------------------------< 関数群 >---------------------------------------------//

/// <summary>
/// クランプ種類
/// </summary>
enum eClampLoopType
{
	eClampLoopType_None ,		// ループ無し
	eClampLoopType_Clamp,		// 最大・最小値でループ
	eClampLoopType_Excess ,		// 超過分プラスでループ
};

/// <summary>
/// クランプ計算
/// </summary>
namespace ClampUtil
{
	/// <summary>
	/// 値のクランプをする
	/// </summary>
	template< typename T >
	inline skyBool ValueClamp	( T *pVal , T addVal , T clampMin, T clampMax , eClampLoopType loopType=eClampLoopType_Clamp );

	/// <summary>
	/// 値が行ったり来たりする
	/// </summary>
	template< typename T >
	inline void BtweenValueMove( T *pVal , T addVal , skyBool *pIsToUp , T clampMin, T clampMax );
}

} }

#include "ClampUtil.inl"