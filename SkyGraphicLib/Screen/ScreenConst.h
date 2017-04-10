#pragma once

namespace sky { namespace lib { namespace graphic { namespace screen {

/// <summary>
/// screen機能共通定数
/// </summary>
static const dectype VIRTUAL_SCREEN_W   = 1280.0;	                             //仮想解像度の幅
static const dectype VIRTUAL_SCREEN_H   = 720.0;	                             //仮想解像度の高さ
const static dectype SCREEN_ASPECT_RATE = VIRTUAL_SCREEN_W / VIRTUAL_SCREEN_H;	 //仮想解像度のアスペクト比
static const u32     LAYER_STACK_NUM    = 8;

/// <summary>
/// スクリーンハッシュサイズ
/// </summary>
static const u32    HASH_SIZE = 19;

/// <summary>
/// スクリーン分割種別
/// </summary>
enum eSplit
{
	eSplit_LR = 0 ,
	eSplit_TB ,
	eSplit_None 
};

} } } }