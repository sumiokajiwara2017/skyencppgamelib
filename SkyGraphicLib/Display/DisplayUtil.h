#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// Display関連の便利処理群
/// </summary>
namespace DisplayUtil
{
	/// <summary>
	/// 画面の解像度を取得する
	/// </summary>
	inline math::CBasicSize	DisplayPixel();
}

} } }

#include "DisplayUtil.inl"