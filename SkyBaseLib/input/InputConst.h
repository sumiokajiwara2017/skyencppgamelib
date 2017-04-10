#pragma once

/// <summary>
/// input機能共通定数
/// </summary>
namespace sky { namespace lib { namespace input {

	/// <summary>
	/// 端末のキーボード、マウスのプレイヤー番号(キーボード、マウスは基本1個しか接続されないはずで、他の番号でのキーボード、マウスの入力はネットワーク経由だろう多分）
	/// </summary>
	static const u32 CURRENT_PLAYNER_NO = 0;

	/// <summary>
	/// デフォルトプレイヤー数
	/// </summary>
	static const u32 DEFAULT_PLAYER_NUM = 1;

} } }
