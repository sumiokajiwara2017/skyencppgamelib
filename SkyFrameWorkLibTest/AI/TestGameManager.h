#pragma once

#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#include "SkyMathLib/Collision.h"
#include "SkyFrameWorkLib/AI.h"

using namespace sky::lib;

//移動空間範囲
static math::CBasicVector3  g_SpaceMax( 40 , 40 , 40 );
static math::CBasicVector3  g_SpaceMin( -40 , -40 , -40 );

//オブジェクト種別
enum eObjectKind
{
	eObjectKind_Player = 0 ,
	eObjectKind_Enemy ,
	eObjectKind_Box ,
	eObjectKind_Sphere ,
	eObjectKind_Edge ,
};

//スクリーン管理
extern graphic::CScreenObjectPtr    g_spScreen;

//衝突判定管理
extern math::CollisionManager g_CollisionManager;

//文字出力
extern graphic::CFontSimplePtr      g_spFont;
extern graphic::CFontPtr      g_spFontTitle;

class CPlayer;
class CPlayerAI;
class CEnemmy;

//ゲーム管理
class GameManager
{
public:

	//主人公情報の参照
	CPlayer *m_pPlayer;

	//主人公側ＡＩキャラの参照
	CPlayerAI *m_pAIPlayer;

	//敵情報の参照
	CEnemmy *m_pEnemy;

	//障害物
	graphic::CModelPtr m_spWall;
	graphic::CModelPtr m_spSphere1;
	graphic::CModelPtr m_spSphere2;
	graphic::CModelPtr m_spSphere3;

	//コンストラクタ
	GameManager() : m_pPlayer( skyNull ) , m_pEnemy( skyNull ){}

};
