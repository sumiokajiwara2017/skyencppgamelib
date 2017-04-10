#include "StdAfx.h"
#include "SkyGraphicLib/Camera.h"

namespace sky { namespace lib { namespace graphic {

SkyImplementRTTI( sky::lib::graphic::CameraManager , sky::lib::graphic::CSceneGraphNode );

void CameraManager::ResisterCamera( const CameraPtr &spCamera )
{
	AttachLeaf( spCamera );
}

void CameraManager::UnResisterCamera( const CameraPtr &spCamera )
{
	DetachLeaf( spCamera );
}

void CameraManager::NextCameraRequest( updateTime time , eInterpolateType eInterpType )
{
	if ( m_LeafList.GetNum() > 0 )
	{//次のカメラを捜す
		CameraPtr spCamera = m_LeafList[ 0 ];

		skyBool isFound = skyFalse;
		FOREACH( CSceneGraphNodePtrList , it , LeafList() )
		{
			if ( isFound == skyTrue )
			{
				spCamera = *it;
				break;
			}
			else if( ( *it ) == GetBranchRoot() )
			{
				isFound = skyTrue;
			}
		}

		SetCurrentCameraRequest( spCamera , time , eInterpType );
	}
}

skyBool CameraManager::IsInterpolate()
{
	return ( m_FunctionState.GetState() == eState_Interpolate );
}

void CameraManager::SetCurrentCameraRequest( const CameraPtr &spCamera , updateTime time , eInterpolateType eInterpType )
{
	if ( GetBranchRoot().IsEmpty() == skyTrue )
	{//カメラが全くなかったら補間無しで設定
		m_Branch = spCamera;
	} 
	else if ( spCamera.GetPtr() != m_Branch.GetPtr() )
	{//カメラが変化したら補間する

		//現在のカメラを取得
		CameraPtr spCurrentCamera = ( CameraPtr )GetBranchRoot();

		m_InterpQueue.Push();
		CInterpRequestData &interpReqData = m_InterpQueue.GetBack();
		m_FunctionState.PushState( eState_Interpolate );
		m_FunctionState.PushState( eState_Wait );

		//補間のための情報を取得
		interpReqData.m_Time        = time;
		interpReqData.m_eInterpType = eInterpType;

		//新しいカメラの位置を古いカメラの位置にしてしまう（２Ｄと３Ｄで違う）
		if( spCurrentCamera->IsOrthoOn() == skyTrue )
		{//２Ｄはジョイントを操作することで移動（移動しているのはカメラではなくカメラが写している対象ということになる）

			interpReqData.m_ToEye       = spCamera->GetPosition_XYZ();
			interpReqData.m_FromEye     = spCurrentCamera->GetPosition_XYZ();

			spCamera->SetPosition_XYZ( spCurrentCamera->GetPosition_XYZ() );
		}
		else
		{//３Ｄは視点、注視点を移動することで移動（このフレームワークではカメラの持つジョイントは使っていない）

			interpReqData.m_ToEye         = spCamera->GetData().m_Eye;
			interpReqData.m_ToTag         = spCamera->GetData().m_Tag;

			interpReqData.m_FromEye          = spCurrentCamera->GetData().m_Eye;
			interpReqData.m_FromTag          = spCurrentCamera->GetData().m_Tag;

			spCamera->SetEye( spCurrentCamera->GetData().m_Eye );
			spCamera->SetTag( spCurrentCamera->GetData().m_Tag );
		}

		//過去のカメラのカレント指定を解除
		( ( CameraPtr )GetBranchRoot() )->SetCurrentOn( skyFalse );

		//現在のカメラを切り替える
		m_Branch = spCamera;
	}

	//カレント指定
	( ( CameraPtr )GetBranchRoot() )->SetCurrentOn( skyTrue );
}

void CameraManager::SetCurrentCameraRequest( const hash::CHashStringPtr &name , updateTime time , eInterpolateType eInterpType )
{
	FOREACH( CSceneGraphNodePtrList , it , LeafList() )
	{
		if ( ( *it )->Name == name )
		{
			SetCurrentCameraRequest( ( const CameraPtr & )( *it ) , time , eInterpType );
			break;
		}
	}
}

CameraPtr CameraManager::GetCurrentCamera()
{
	return GetBranchRoot();
}

void CameraManager::KeyBordEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	SKY_UNUSED_ARG( args );

	if ( m_IsDefaultInputOn == skyTrue )
	{
		if ( Ckeyboard_->IsKeyOn( KEY_C ) && Ckeyboard_->IsKeyPress( KEY_N ) )
		{
			NextCameraRequest( CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType_Linear );
		}
	}
}

void CameraManager::Update( const fps::CFpsController &fps )
{
	m_FunctionState.Update( fps );
}

CameraManager::CameraManager() :
m_FunctionState( this ) ,
m_IsDefaultInputOn( skyFalse ) ,
m_InterpQueue( CAMERAMANAGER_QUEUE_SIZE )
{
	//キーボードのイベント登録
	m_spKeyBordEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CameraManager > , delegate::CEventDelegateAdaptor< CameraManager >( this , &CameraManager::KeyBordEventDelegate ) ) );
	Ckeyboard_->AttachInputListener( m_spKeyBordEventDelegate );

	m_FunctionState.RegisterState( eState_Start       , &CameraManager::State_Start );
	m_FunctionState.RegisterState( eState_Wait        , &CameraManager::State_Wait );
	m_FunctionState.RegisterState( eState_Interpolate , &CameraManager::State_Interpolate );
	m_FunctionState.RegisterState( eState_End         , &CameraManager::State_End );

	//ステートをキューに投入
	m_FunctionState.PushState( eState_Start );
	m_FunctionState.PushState( eState_Wait );
}

//-----------State Function-----------------▽

skyBool CameraManager::State_Start( const fps::CFpsController & )
{
	return skyTrue;
}

skyBool CameraManager::State_Wait( const fps::CFpsController & )
{
	return ( m_FunctionState.GetQueueNum() > 0 );
}

skyBool CameraManager::State_Interpolate( const fps::CFpsController &fps )
{
	skyBool result = skyFalse;
	CInterpRequestData &interpReqData = m_InterpQueue.GetFront();

	if( m_FunctionState.GetFrameCount() == 0 )
	{
		interpReqData.m_ElapsedTime = 0.0f;
	}
	else
	{
		math::CBasicVector3 nowEye;
		math::CBasicVector3 nowTag;

		//現在のカメラを取得
		CameraPtr spCurrentCamera = ( CameraPtr )GetBranchRoot();

		//補間値を設定（２Ｄと３Ｄで違う）
		if( spCurrentCamera->IsOrthoOn() == skyTrue )
		{//２Ｄはジョイントを操作することで移動（移動しているのはカメラではなくカメラが写している対象ということになる）

			//補間計算をする
			interp::InterpolateCommon< math::CBasicVector3 >( nowEye , interpReqData.m_FromEye , interpReqData.m_ToEye , ( f32 )( interpReqData.m_ElapsedTime / interpReqData.m_Time ) , interpReqData.m_eInterpType );

			spCurrentCamera->SetPosition_XYZ( nowEye );
		}
		else 
		{//３Ｄは視点、注視点を移動することで移動（このフレームワークではカメラの持つジョイントは使っていない）

			//補間計算をする
			interp::InterpolateCommon< math::CBasicVector3 >( nowEye         , interpReqData.m_FromEye         , interpReqData.m_ToEye         , ( f32 )( interpReqData.m_ElapsedTime / interpReqData.m_Time ) , interpReqData.m_eInterpType );
			interp::InterpolateCommon< math::CBasicVector3 >( nowTag         , interpReqData.m_FromTag         , interpReqData.m_ToTag         , ( f32 )( interpReqData.m_ElapsedTime / interpReqData.m_Time ) , interpReqData.m_eInterpType );

			spCurrentCamera->SetEye( nowEye );
			spCurrentCamera->SetTag( nowTag );
		}

		//時間を進める
		interpReqData.m_ElapsedTime += fps.GetPassageTime_Second();

		//指定の時間まで進んだら終了
		if ( interpReqData.m_ElapsedTime >= interpReqData.m_Time )
		{
			//最後はぴったりくるように位置を合わせる
			if( spCurrentCamera->IsOrthoOn() )
			{
				spCurrentCamera->SetPosition_XYZ( interpReqData.m_ToEye );
			}
			else
			{
				spCurrentCamera->SetEye( interpReqData.m_ToEye );
				spCurrentCamera->SetTag( interpReqData.m_ToTag );
			}

			//キューから要求を削除
			m_InterpQueue.Pop();

			//ステート終了
			result = skyTrue;
		}
	}

	//経過時間が補間時間に到達すれば終了
	return result;
}

skyBool CameraManager::State_End( const fps::CFpsController & )
{
	return skyFalse;
}


const serialize::CSerializeInterfacePtr &CameraManager::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CameraManager::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CameraManager::Clone()
{
	return Copy( CameraManagerPtr( NEW__( CameraManager , CameraManager() ) ) );
}

serialize::CSerializeInterfacePtr CameraManager::CloneSharing()
{
	return Clone();
}


} } }
