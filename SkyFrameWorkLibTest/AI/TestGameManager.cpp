#include "StdAfx.h"
#include "TestGameManager.h"

//衝突判定管理
math::CollisionManager g_CollisionManager;

//スクリーン管理
graphic::CScreenObjectPtr    g_spScreen;

//文字出力
graphic::CFontSimplePtr      g_spFont;
graphic::CFontPtr            g_spFontTitle;

