#pragma once

#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#include "SkyMathLib/Collision.h"
#include "SkyFrameWorkLib/AI.h"

using namespace sky::lib;

//�ړ���Ԕ͈�
static math::CBasicVector3  g_SpaceMax( 40 , 40 , 40 );
static math::CBasicVector3  g_SpaceMin( -40 , -40 , -40 );

//�I�u�W�F�N�g���
enum eObjectKind
{
	eObjectKind_Player = 0 ,
	eObjectKind_Enemy ,
	eObjectKind_Box ,
	eObjectKind_Sphere ,
	eObjectKind_Edge ,
};

//�X�N���[���Ǘ�
extern graphic::CScreenObjectPtr    g_spScreen;

//�Փ˔���Ǘ�
extern math::CollisionManager g_CollisionManager;

//�����o��
extern graphic::CFontSimplePtr      g_spFont;
extern graphic::CFontPtr      g_spFontTitle;

class CPlayer;
class CPlayerAI;
class CEnemmy;

//�Q�[���Ǘ�
class GameManager
{
public:

	//��l�����̎Q��
	CPlayer *m_pPlayer;

	//��l�����`�h�L�����̎Q��
	CPlayerAI *m_pAIPlayer;

	//�G���̎Q��
	CEnemmy *m_pEnemy;

	//��Q��
	graphic::CModelPtr m_spWall;
	graphic::CModelPtr m_spSphere1;
	graphic::CModelPtr m_spSphere2;
	graphic::CModelPtr m_spSphere3;

	//�R���X�g���N�^
	GameManager() : m_pPlayer( skyNull ) , m_pEnemy( skyNull ){}

};
