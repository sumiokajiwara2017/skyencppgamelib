#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Sprite.h"
#include "SkyGraphicLib/Display.h"

namespace sky { namespace lib { namespace graphic {

SingletonPtr< CFadeManager >	CFadeManager::Singleton;

void CFadeManager::CreateFadeSprite( dectype w , dectype h )
{
	if ( m_spSprite.IsEmpty() == skyTrue )
	{
		//�X�v���C�g���쐬
		m_spSprite = CSpriteUtil::CreateSquare();
		( ( CSpritePtr )m_spSprite )->DetachShader();
		( ( CSpritePtr )m_spSprite )->AttachShader( CMaterialDiffuseColorShaderObjectCreate_() );
		m_spSprite->SetScale_XY( math::CBasicVector2( w , h ) );
		m_spSprite->SetAlpha( skyTrue );
		SetFadeColor( CColor_White );

		//�V�[���O���t�̃��[�g���쐬
		m_spSceneGraphRoot = CSceneGraphNode::Create();

		//���[�g�̎q�ɂȂ�
		m_spSceneGraphRoot->AttachChild( m_spSprite );
	}
}

void CFadeManager::SetFadeSpeed( f32 speed )
{
	m_FadeSpeed = speed;
}

void CFadeManager::SetFadeColor( CColor color )
{
	if ( m_spSprite.IsEmpty() == skyFalse )
	{
		( ( CSpritePtr )m_spSprite )->GetMaterial()->Diffuse = color;
	}
}

void CFadeManager::ReSize( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );

	dectype w = ( dectype )args.Gets32( 0 );
	dectype h = ( dectype )args.Gets32( 1 );

	if ( m_spSprite.IsEmpty() == skyFalse )
	{
		m_spSprite->SetScale_XY( math::CBasicVector2( w , h ) );
	}
}

CFadeManager::CFadeManager() : m_FadeSpeed( 1.0f ) , m_FadeInFCurve( 1.0f ) , m_FadeOutFCurve( 0.0f )
{
	//�f���Q�[�g�ݒ�i���[�g�����f���Q�[�g�����B���[�g���番�����ꂽ�q�̓f���Q�[�g�������Ȃ��j
	m_spReSizeDelegate.SetPtr( NEW delegate::CEventDelegateAdaptor< CFadeManager >( this , &CFadeManager::ReSize ) );
	CEventManager_->AttachEventListener( CHS( event::SYSTEM_EVENT_WINDOW_RESIZE ) , m_spReSizeDelegate );

	//�l�擾�C�x���g�ݒ�
	m_Player.GetColValueEventHndl() += 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CFadeManager >( this, &CFadeManager::NewValueColFuncEvent ) );

	//�L�[�t���[���A�j���[�V�����\�z
	m_FadeInFCurve.AddKey( 1.0f , 0.0f , eInterpolateType_Linear );
	m_FadeOutFCurve.AddKey( 1.0f , 1.0f , eInterpolateType_Linear );
}

void CFadeManager::SetFadeInEnd()
{
	( ( CSpritePtr )m_spSprite )->GetMaterial()->Alpha = 1.0f;
	m_Player.CancelFCurveAll();
}

void CFadeManager::SetFadeOutEnd()
{
	( ( CSpritePtr )m_spSprite )->GetMaterial()->Alpha = 0.0f;
	m_Player.CancelFCurveAll();
}

eFadeStateKind CFadeManager::GetFadeState()
{
	//�t�F�[�h�l��1.0f��������t�F�[�h�A�E�g
	if( math::CMathUtil::NearyEq( ( ( CSpritePtr )m_spSprite )->GetMaterial()->Alpha , 1.0f , 0.00001f ) )
	{
		return eFadeStateKind_FadeOut;
	}
	//�t�F�[�h�l��0.0f��������t�F�[�h�C��
	else if( math::CMathUtil::NearyEq( ( ( CSpritePtr )m_spSprite )->GetMaterial()->Alpha , 0.0f , 0.00001f ) )
	{
		return eFadeStateKind_FadeIn;
	}

	return eFadeStateKind_FadePlaying;
}

void CFadeManager::ExecFadeIn()
{
	m_Player.PlayFCurve( &m_FadeInFCurve , 0.0f , 0.0f , m_FadeSpeed , 0.0f , kfm::ePlayEndType_End );
}

void CFadeManager::ExecFadeOut()
{
	m_Player.PlayFCurve( &m_FadeOutFCurve , 0.0f , 0.0f , m_FadeSpeed , 0.0f , kfm::ePlayEndType_End );
}

void CFadeManager::NewValueColFuncEvent ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );

	//�l�擾
	( ( CSpritePtr )m_spSprite )->GetMaterial()->Alpha = *( f32 * )args.Getu32( 1 );
}

void CFadeManager::Update( const fps::CFpsController &fps )
{
	//�L�[������΍Đ�����
	m_Player.Play( fps.GetPassageTime_Second() );
}

const CSceneGraphNodePtr &CFadeManager::GetSceneGraphRoot()
{
	return m_spSceneGraphRoot;
}

} } }
