#pragma once

/// <summary>
/// 入力受付可能キーボードキーリスト
/// </summary>
enum eSkyKeybordKeyKind
{
	KEY_A = 0,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X ,
	KEY_Y,
	KEY_Z,
	KEY_ENTER,
	KEY_SHIFT,
	KEY_ESCAPE,
	KEY_HOME,
	KEY_LEFT,
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_ESC,
	KEY_TAB,

	eSkyKeybordKeyKind_Max ,
};

/// <summary>
/// マウスイベント種別（これはマウスデリゲートの引数でユーザーに渡されます）
/// </summary>
enum eSkyMouseKeyKind
{
	MOUSE_KEY_LEFT_BTN = 0 ,
	MOUSE_KEY_CENTER_BTN ,
	MOUSE_KEY_RIGHT_BTN ,

	eSkyMouseKeyKind_Max ,
};

/// <summary>
/// マウスイベント種別（これはマウスデリゲートの引数でユーザーに渡されます）
/// </summary>
enum eSkyMouseEventKind
{
    MOUSE_EVENT_LEFT_BTN_DOWN = 0 ,
    MOUSE_EVENT_LEFT_BTN_UP ,
	MOUSE_EVENT_RIGHT_BTN_DOWN ,
	MOUSE_EVENT_RIGHT_BTN_UP ,
	MOUSE_EVENT_CENTER_BTN_DOWN ,
	MOUSE_EVENT_CENTER_BTN_UP ,
	MOUSE_EVENT_WHEEL_ROTATION ,
	MOUSE_EVENT_MOVE ,

	eSkyMouseEventKind_Max ,
};

/// <summary>
/// ホイール押下時に同時に押されているボタン種別（これはマウスデリゲートの引数でユーザーに渡されます）
/// </summary>
#ifndef SKYLIB_PLATFORM_WIN32
#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010
#endif
enum eSkyMouseKeyWheelWithBtnKind
{
    MOUSE_WW_LEFT_BTN_DOWN   = MK_LBUTTON ,
	MOUSE_WW_RIGHT_BTN_DOWN  = MK_RBUTTON ,
	MOUSE_WW_CENTER_BTN_DOWN = MK_MBUTTON ,
	MOUSE_WW_CTRL_BTN_DOWN   = MK_CONTROL ,
	MOUSE_WW_SHIFT_DOWN      = MK_SHIFT ,

	eSkyMouseKeyWheelWithBtnKind_Max ,
};


/// <summary>
/// 入力受付可能パッドキーリスト
/// </summary>
enum eSkyPadKeyKind
{
	PAD_KEY_UP = 0 ,
	PAD_KEY_BOTTOM ,
	PAD_KEY_LEFT ,
	PAD_KEY_RIGHT ,

	PAD_BTN_UP ,
	PAD_BTN_BOTTOM,
	PAD_BTN_LEFT ,
	PAD_BTN_RIGHT ,

	PAD_TRIG_L1 ,
	PAD_TRIG_L2 ,
	PAD_TRIG_R1 ,
	PAD_TRIG_R2 ,

	eSkyPadKeyKind_Max ,
};