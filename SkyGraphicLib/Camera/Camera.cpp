#include "StdAfx.h"
#include "SkyGraphicLib/Camera.h"

SkyImplementRTTI( sky::lib::graphic::Camera , sky::lib::graphic::CSceneGraphNode ); //最終的にはこっち

namespace sky { namespace lib { namespace graphic {

Camera::Camera( eCameraKind ekind ) : 
m_eKind( ekind ) , 
m_IsOrthoOn( skyFalse ) ,
m_IsCurrentOn( skyFalse ) ,
m_IsDefaultInputOn( skyFalse ) ,
m_IsMouseLBtnClick( skyFalse ) ,
m_IsMouseRBtnClick( skyFalse ) ,
m_MouseClickPos( 0 , 0 )
{
	m_Data.m_Eye = CAMERA_DEFAULT_EYE;
	m_InitData.m_Eye =  CAMERA_DEFAULT_EYE;
	m_Data.m_Tag = CAMERA_DEFAULT_TAG;
	m_InitData.m_Tag = CAMERA_DEFAULT_TAG;
	m_Data.m_UpVec = CAMERA_DEFAULT_UP_VECTOR;
	m_InitData.m_UpVec = CAMERA_DEFAULT_UP_VECTOR;
	m_Data.m_RightVec = CAMERA_DEFAULT_RIGHT_VECTOR;
	m_InitData.m_RightVec = CAMERA_DEFAULT_RIGHT_VECTOR;
	m_Data.m_Near = CAMERA_DEFAULT_NEAR;
	m_InitData.m_Near = CAMERA_DEFAULT_NEAR;
	m_Data.m_Far = CAMERA_DEFAULT_FAR;
	m_InitData.m_Far = CAMERA_DEFAULT_FAR;
	m_Data.m_Fov = CAMERA_DEFAULT_FOV;
	m_InitData.m_Fov = CAMERA_DEFAULT_FOV;
	m_Data.m_Zoom = CAMERA_DEFAULT_ZOOM;
	m_InitData.m_Zoom = CAMERA_DEFAULT_ZOOM;
	m_Data.m_Aspect = CAMERA_DEFAULT_ASPECT;
	m_InitData.m_Aspect = CAMERA_DEFAULT_ASPECT;
	m_Data.m_Distance = CAMERA_DEFAULT_DISTANCE;
	m_InitData.m_Distance = CAMERA_DEFAULT_DISTANCE;
	m_Data.m_Angle = math::CBasicAngleVector3( 0 , 0 , 0 , 0 );
	m_InitData.m_Angle = math::CBasicAngleVector3( 0 , 0 , 0 , 0 );
	m_Data.m_ViewSize = math::CBasicSize( CAMERA_DEFAULT_SCREEN_WIDTH , CAMERA_DEFAULT_SCREEN_HEIGHT ) ,
	m_InitData.m_ViewSize = math::CBasicSize( CAMERA_DEFAULT_SCREEN_WIDTH , CAMERA_DEFAULT_SCREEN_HEIGHT ) ,


	m_WorldMatrix.Identity();
	m_ViewMatrix.Identity();
    m_ProjectionMatrix.Identity();
    m_ViewProjectionMatrix.Identity();
    m_OrthoMatrix.Identity();

	//マウスのイベント登録
	m_spMouseEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< Camera > , delegate::CEventDelegateAdaptor< Camera >( this , &Camera::MouseEventDelegate ) ) );
	CMouse_->AttachInputListener( m_spMouseEventDelegate );

	//キーボードイベント登録
	m_spKeyBoradEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< Camera > , delegate::CEventDelegateAdaptor< Camera >( this , &Camera::KeyBoradEventDelegate ) ) );
	Ckeyboard_->AttachInputListener( m_spKeyBoradEventDelegate );

	//行列も全て計算しなおし
	UpdateViewInfo( m_Data.m_Eye , m_Data.m_Tag , m_Data.m_UpVec );
	UpdateProjectionInfo( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
	UpdateOrthoInfo( m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
}

void Camera::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This object used smart pointer.\n" ) );//このオブジェクトは必ずスマートポインタで扱ってね

	//内部情報を更新
	Update( param.Fps );

	//自分をスタックに積む
	param.CameraStack.Push( thisPtr( CSceneGraphNodePtr ) ); //もともとこのthisが他のスマートポインタに管理されていた場合（RefCountが0以上の場合)この受け渡しをしても問題が発生しない。
}

void Camera::vPostRendering( CRenderingParam &param )
{
	//スタックから取る
	param.CameraStack.Pop();
}

void Camera::Update( const fps::CFpsController &fps )
{
	SKY_UNUSED_ARG( fps );

	switch( m_eKind )
	{
	case eCameraKind_Satellite: // ターゲット　と　回転角度が決まっている
		m_Data.m_Angle.ToVector( m_Data.m_Eye, m_Data.m_Distance );
		m_Data.m_Eye += m_Data.m_Tag;
		break;
	case eCameraKind_Fps:       // 回転角度　と　カメラ位置　が決まっている
		m_Data.m_Angle.ToVector( m_Data.m_Tag, m_Data.m_Distance );
		m_Data.m_Tag *= -1.0f;
		m_Data.m_Tag += m_Data.m_Eye;
		break;
	case eCameraKind_Fix:       // ターゲット　と　カメラ位置　が決まっている
		m_Data.m_Angle    = math::FaceFromTo( m_Data.m_Tag , m_Data.m_Eye );
		m_Data.m_Distance = ( m_Data.m_Tag - m_Data.m_Eye ).Length();
		break;
	}

	m_Data.m_Angle.ToMatrixZXY( m_WorldMatrix );  //向きから回転行列を算出
	m_WorldMatrix.SetCol3( m_Data.m_Eye , 1.0f ); //位置を行列に組み込む

	//ビュー行列の更新
	UpdateViewInfo( m_Data.m_Eye , m_Data.m_Tag , m_Data.m_UpVec );
}

void Camera::SetInitInfo( const math::CBasicVector3 &eye , const math::CBasicVector3 &tag , const math::CBasicVector3 &upVec , const math::CBasicVector3 &rightVec , dectype fov , dectype nearValue , dectype farValue , dectype width , dectype height , const math::CBasicAngleVector3 &angle , const math::CBasicAngleVector3 &inclinationangle , dectype distance , dectype zoom )
{
	m_InitData.m_Eye              = eye;
	m_InitData.m_Tag              = tag;
	m_InitData.m_ViewVec          = ( tag - eye ).Normalize();
	m_InitData.m_UpVec            = upVec;
	m_InitData.m_RightVec         = rightVec;
	m_InitData.m_Near             = nearValue;
    m_InitData.m_Far              = farValue;
    m_InitData.m_Fov              = fov;
	m_InitData.m_ViewSize.W       = width;
	m_InitData.m_ViewSize.H       = height;
	m_InitData.m_Aspect           = width / height;
	m_InitData.m_Angle            = angle;
	m_InitData.m_InclinationAngle = inclinationangle;
	m_InitData.m_Distance         = distance;
	m_InitData.m_Zoom             = zoom;
}

void Camera::Init()
{
	m_Data             = m_InitData;

	//行列も全て計算しなおし
	UpdateViewInfo( m_Data.m_Eye , m_Data.m_Tag , m_Data.m_UpVec );
	UpdateProjectionInfo( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
	UpdateOrthoInfo( m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
}

void Camera::SetDisplaySize( dectype width , dectype height )
{
	m_Data.m_ViewSize.W = width;
	m_Data.m_ViewSize.H = height;

	UpdateProjectionInfo( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
	UpdateOrthoInfo( m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
}

void Camera::UpdateViewInfo( const math::CBasicVector3 &eye , const math::CBasicVector3 &tag , const math::CBasicVector3 &upVec )
{
	m_Data.m_Eye   = eye;
	m_Data.m_Tag   = tag;
	m_Data.m_ViewVec = ( m_Data.m_Tag - m_Data.m_Eye ).Normalize();
	m_Data.m_UpVec = upVec;

#ifdef VIEW_LEFT_HAND
	m_ViewMatrix.SetupViewLH( m_Data.m_Eye , m_Data.m_Tag , m_Data.m_UpVec );
#endif

#ifdef VIEW_RIGHT_HAND
	m_ViewMatrix.SetupViewRH( m_Data.m_Eye , m_Data.m_Tag , m_Data.m_UpVec );
#endif

	//これも更新する
	m_ViewProjectionMatrix = MulMatrix4x4( m_ViewMatrix , m_ProjectionMatrix );
}

void Camera::SetDistance( dectype distance )
{
	m_Data.m_Distance = distance;
}

void Camera::SetAngle( const math::CBasicAngleVector3 &angle )
{
	m_Data.m_Angle = angle;
}

void Camera::SetEye  ( const math::CBasicVector3 &eye )
{
	UpdateViewInfo( eye , m_Data.m_Tag , m_Data.m_UpVec );
}

void Camera::SetTag  ( const math::CBasicVector3 &tag )
{
	UpdateViewInfo( m_Data.m_Eye , tag , m_Data.m_UpVec );
}

void Camera::UpdateProjectionInfo( dectype fov , dectype width , dectype height , dectype nearValue , dectype farValue )
{
	m_Data.m_Near       = nearValue;
    m_Data.m_Far        = farValue;
    m_Data.m_Fov        = fov;
	m_Data.m_ViewSize.W = width;
	m_Data.m_ViewSize.H = height;
	m_Data.m_Aspect     = width / height;

#ifdef VIEW_LEFT_HAND
	m_ProjectionMatrix.SetupProjectionLH( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
#endif

#ifdef VIEW_RIGHT_HAND
	m_ProjectionMatrix.SetupProjectionRH( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
#endif

	//これも更新する
	m_ViewProjectionMatrix = MulMatrix4x4( m_ViewMatrix , m_ProjectionMatrix );
}

void Camera::UpdateProjectionInfo_Fov( dectype fov )
{
	UpdateProjectionInfo( fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
}

void Camera::UpdateProjectionInfo_Size( dectype width , dectype height )
{
	UpdateProjectionInfo( m_Data.m_Fov , width , height , m_Data.m_Near , m_Data.m_Far );
}

void Camera::UpdateProjectionInfo_NearFar( dectype nearValue , dectype farValue )
{
	UpdateProjectionInfo( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , nearValue , farValue );
}

void Camera::UpdateOrthoInfo( dectype width , dectype height , dectype nearValue , dectype farValue , dectype zoom )
{
	m_Data.m_ViewSize.W = width;
	m_Data.m_ViewSize.H = height;
	m_Data.m_Near       = nearValue;
    m_Data.m_Far        = farValue;
    m_Data.m_Zoom       = zoom;

#ifdef VIEW_LEFT_HAND
	m_OrthoMatrix.SetupOrthoMatrixLH( m_Data.m_ViewSize.W * m_Data.m_Zoom , m_Data.m_ViewSize.H * zoom , m_Data.m_Near , m_Data.m_Far );
#endif

#ifdef VIEW_RIGHT_HAND
	m_OrthoMatrix.SetupOrthoMatrixRH( m_Data.m_ViewSize.W * m_Data.m_Zoom , m_Data.m_ViewSize.H * m_Data.m_Zoom , m_Data.m_Near , m_Data.m_Far );
#endif

	if ( IsLeftTopCenter() == skyTrue )
	{
		//左上にオフセットする（移動値の反転はSprite、Fontクラスの中）
		m_OrthoMatrix.Tx = -1.0f;
		m_OrthoMatrix.Ty = 1.0f;
	}
}

void Camera::UpdateOrthoInfo_Size( dectype width , dectype height )
{
	UpdateOrthoInfo( width , height , m_Data.m_Near , m_Data.m_Far , m_Data.m_Zoom );
}

void Camera::UpdateOrthoInfo_NearFar( dectype nearValue , dectype farValue )
{
	UpdateOrthoInfo( m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , nearValue , farValue , m_Data.m_Zoom );
}

void Camera::UpdateOrthoInfo_Zoom( dectype zoom )
{
	UpdateOrthoInfo( m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far , zoom );
}

const math::CMatrix4x3 &Camera::GetWorldMatrix()
{
	return m_WorldMatrix;
}

const math::CMatrix4x3 &Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

const math::CMatrix4x3 &Camera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

void Camera::SetOrtho( skyBool isOrthoOn )
{
	m_IsOrthoOn = isOrthoOn;
	if ( m_IsOrthoOn == skyTrue )
	{
		UpdateOrthoInfo( m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , RENDERING_2D_PRIO_FAST , RENDERING_2D_PRIO_MAX );
		SetJointUse( skyTrue );
	}
	else
	{
		UpdateViewInfo( m_Data.m_Eye , m_Data.m_Tag , m_Data.m_UpVec );
		UpdateProjectionInfo( m_Data.m_Fov , m_Data.m_ViewSize.W , m_Data.m_ViewSize.H , m_Data.m_Near , m_Data.m_Far );
		Reset();
		SetJointUse( skyFalse );
	}
}

skyBool Camera::IsOrthoOn()
{
	return m_IsOrthoOn;
}

const math::CMatrix4x3 &Camera::GetOrthoMatrix()
{
	return m_OrthoMatrix;
}

const math::CMatrix4x3 &Camera::GetCameraMatrix()
{
	if ( m_IsOrthoOn == skyTrue )
	{
		return m_OrthoMatrix;
	}
	else
	{
		return m_ViewProjectionMatrix;
	}
}

const math::CRotationMatrix &Camera::ComputeBillboardRotationMatrix()
{
	//ビュー行列の逆行列から３Ｘ３行列部分を取り出して返す
	Invert( m_ViewMatrix ).CreateRotateMatrix( m_BillBoardRotationMatrix );

	return m_BillBoardRotationMatrix;
}

void Camera::SetInitInclinationAngle( const math::CBasicAngleVector3 &angle )
{
	m_Data.m_InclinationAngle = angle;

	//上方ベクトルと右ベクトルを回転させる
	math::CMatrix4x3 rot;
	angle.ToMatrixZXY( rot );

	m_Data.m_UpVec = m_Data.m_UpVec * rot;
	m_Data.m_UpVec = m_Data.m_RightVec * rot;
}

void Camera::SetCurrentOn( skyBool isCurrentOn )
{
	m_IsCurrentOn = isCurrentOn;
}

void Camera::SetLeftTopCenter( skyBool isLeftTopCenterOn )
{
	m_IsLeftTopCenterFlg = isLeftTopCenterOn;
}

skyBool Camera::IsLeftTopCenter()
{
	return m_IsLeftTopCenterFlg;
}

void Camera::MouseEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );

	switch( m_eKind )
	{
	case eCameraKind_Satellite: // ターゲット　と　回転角度が決まっている
		{
			if ( m_IsCurrentOn == skyTrue && m_IsDefaultInputOn == skyTrue )
			{
				if ( args.Gets32( 0 ) == MOUSE_EVENT_LEFT_BTN_DOWN )
				{
					if ( m_IsMouseLBtnClick == skyFalse )
					{
						m_IsMouseLBtnClick = skyTrue;
						m_MouseClickPos = math::CBasicVector2( math::CBasicVector2( ( dectype )args.Gets32( 1 ) , ( dectype )args.Gets32( 2 ) ) );
					}
				}
				else if ( args.Gets32( 0 ) == MOUSE_EVENT_RIGHT_BTN_DOWN )
				{
					if ( m_IsMouseRBtnClick == skyFalse )
					{
						m_IsMouseRBtnClick = skyTrue;
						m_MouseClickPos = math::CBasicVector2( math::CBasicVector2( ( dectype )args.Gets32( 1 ) , ( dectype )args.Gets32( 2 ) ) );
					}
				}

				switch( m_eKind )
				{
				case eCameraKind_Satellite: // ターゲット　と　回転角度が決まっている
					{
						math::CBasicVector2 nowPos = math::CBasicVector2( ( dectype )args.Gets32( 1 ) , ( dectype )args.Gets32( 2 ) );
						math::CBasicVector2 offset = nowPos - m_MouseClickPos;
						if ( args.Gets32( 0 ) == MOUSE_EVENT_MOVE && m_IsMouseRBtnClick == skyTrue )
						{
							m_Data.m_Angle.X().AddFromDeg( offset.X );
							m_Data.m_Angle.Y().AddFromDeg( -offset.Y );

							m_MouseClickPos = nowPos;
						}
					}
					break;
				case eCameraKind_Fps:       // 回転角度　と　カメラ位置　が決まっている
					break;
				case eCameraKind_Fix:       // ターゲット　と　カメラ位置　が決まっている
					break;
				}

				if ( args.Gets32( 0 ) == MOUSE_EVENT_WHEEL_ROTATION )
				{
					m_Data.m_Distance += args.Gets32( 3 ) * 0.01f;
				}

				if ( args.Gets32( 0 ) == MOUSE_EVENT_CENTER_BTN_DOWN )
				{
					Init();
				}

				if ( args.Gets32( 0 ) == MOUSE_EVENT_LEFT_BTN_UP )
				{
					m_IsMouseLBtnClick = skyFalse;
				}

				if ( args.Gets32( 0 ) == MOUSE_EVENT_RIGHT_BTN_UP )
				{
					m_IsMouseRBtnClick = skyFalse;
				}
			}
		}
	}
}

void Camera::KeyBoradEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	SKY_UNUSED_ARG( args );

	switch( m_eKind )
	{
	case eCameraKind_Satellite: // ターゲット　と　回転角度が決まっている
		{
			if ( m_IsCurrentOn == skyTrue && m_IsDefaultInputOn == skyTrue )
			{
				if ( Ckeyboard_->IsKeyOn( KEY_UP ) )
				{
					m_Data.m_Angle.Y().AddFromDeg( 1 );
				}
				else if ( Ckeyboard_->IsKeyOn( KEY_DOWN ) )
				{
					m_Data.m_Angle.Y().AddFromDeg( -1 );
				}

				if ( Ckeyboard_->IsKeyOn( KEY_LEFT ) )
				{
					m_Data.m_Angle.X().AddFromDeg( 1 );
				}
				else if ( Ckeyboard_->IsKeyOn( KEY_RIGHT ) )
				{
					m_Data.m_Angle.X().AddFromDeg( -1 );
				}
			}
		}
		break;
	}
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_eKind , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsOrthoOn , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Eye.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Eye.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Eye.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Eye.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Eye.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Eye.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Tag.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Tag.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Tag.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Tag.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Tag.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Tag.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_UpVec.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_UpVec.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_UpVec.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_UpVec.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_UpVec.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_UpVec.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_RightVec.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_RightVec.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_RightVec.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_RightVec.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_RightVec.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_RightVec.Z , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Near , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Near , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Far , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Far , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Fov , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Fov , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Zoom , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Zoom , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Aspect , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Aspect , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Distance , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Distance , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Angle.X() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Angle.Y() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_Angle.Z() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_InclinationAngle.X() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_InclinationAngle.Y() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_InclinationAngle.Z() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Angle , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_Aspect , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_ViewSize.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Data.m_ViewSize.H , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_ViewSize.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_InitData.m_ViewSize.H , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsCurrentOn , sizeof( s32 ) , isWrite ); \

skyBool Camera::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "camera bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool Camera::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "camera bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& Camera::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CameraPtr )spContainer )->m_eKind = m_eKind;
	( ( CameraPtr )spContainer )->m_IsOrthoOn = m_IsOrthoOn;
	( ( CameraPtr )spContainer )->m_Data = m_Data;
	( ( CameraPtr )spContainer )->m_InitData = m_InitData;
	( ( CameraPtr )spContainer )->m_IsCurrentOn = m_IsCurrentOn;

	return spContainer;
}

const serialize::CSerializeInterfacePtr& Camera::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

void Camera::SetDefaultInput( skyBool isOn )
{
	m_IsDefaultInputOn = isOn;
}

math::CBasicVector3 Camera::ComputeDistancePos( dectype distance )
{
	return ( ( m_Data.m_Tag - m_Data.m_Eye ).Normalize() * distance ) + m_Data.m_Eye;
}

} } }