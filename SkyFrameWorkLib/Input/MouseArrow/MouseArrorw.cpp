#include "StdAfx.h"
#include "SkyFrameWorkLib/Input.h"

#include "SkyMathLib/Geometry.h"
#include "SkyGraphicLib/Device.h"
#include "SkyGraphicLib/Window.h"
#include "SkyGraphicLib/Renderer.h"


namespace sky { namespace lib { namespace framework {

static const skyString *POINT_IMG_FILE_PATH_ = _T( "data/lib/mouse/Mouse_Point.png" );
static const skyString *RESIZE_IMG_FILE_PATH_ = _T( "data/lib/mouse/Mouse_Resize.png" );

CMouseArrow::CMouseArrow(  wndType windowId , dectype prio )
{
	//�E�B���h�E����ID
	m_WindowId = windowId;

	//�}�E�X�̃C�x���g�o�^
	m_spMouseEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CMouseArrow > , delegate::CEventDelegateAdaptor< CMouseArrow >( this , &CMouseArrow::MouseEventDelegate ) ) );
	CMouse_->AttachInputListener( m_spMouseEventDelegate );

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = graphic::CTexture::Create( CHSC( POINT_IMG_FILE_PATH_ ) );

	//�e�N�X�`���ʂ�̋�`���쐬����
	m_spRootSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , math::CBasicVector2( 32.0f , 32.0f ) , prio , math::CBasicVector2( 1.0f , 1.0f ) , graphic::CColor_White );

	//CPU�������͏���
	graphic::CRenderCommand::DeleteMeshCpuMemory( m_spRootSprite->GetMeshContena() );

	//�������ł�
	m_spRootSprite->SetAlpha( skyTrue );

	//Windows�̃}�E�X�J�[�\���̔�\��
#ifdef SKYLIB_PLATFORM_WIN32
//	ShowCursor( FALSE );
#endif
}

CMouseArrow::~CMouseArrow( void )
{
	CMouse_->DetachInputListener( m_spMouseEventDelegate );

#ifdef SKYLIB_PLATFORM_WIN32
//	ShowCursor( TRUE );
#endif

}

void CMouseArrow::MouseEventDelegate ( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
    SKY_UNUSED_ARG( spSender );

	if ( args.Gets32( 3 ) == m_WindowId && args.Gets32( 0 ) == MOUSE_EVENT_MOVE && m_spRootSprite.IsEmpty() == skyFalse )
	{//�}�E�X�ړ��C�x���g�擾

		//������W�n�𒆐S���W�n�ɕϊ�����
		m_spRootSprite->SetPosition_X( args.Gets32( 1 ) );
		m_spRootSprite->SetPosition_Y( args.Gets32( 2 ) );
	}
}


} } }
