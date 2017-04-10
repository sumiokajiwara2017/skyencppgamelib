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
	{//���̃J������{��
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
	{//�J�������S���Ȃ��������Ԗ����Őݒ�
		m_Branch = spCamera;
	} 
	else if ( spCamera.GetPtr() != m_Branch.GetPtr() )
	{//�J�������ω��������Ԃ���

		//���݂̃J�������擾
		CameraPtr spCurrentCamera = ( CameraPtr )GetBranchRoot();

		m_InterpQueue.Push();
		CInterpRequestData &interpReqData = m_InterpQueue.GetBack();
		m_FunctionState.PushState( eState_Interpolate );
		m_FunctionState.PushState( eState_Wait );

		//��Ԃ̂��߂̏����擾
		interpReqData.m_Time        = time;
		interpReqData.m_eInterpType = eInterpType;

		//�V�����J�����̈ʒu���Â��J�����̈ʒu�ɂ��Ă��܂��i�Q�c�ƂR�c�ňႤ�j
		if( spCurrentCamera->IsOrthoOn() == skyTrue )
		{//�Q�c�̓W���C���g�𑀍삷�邱�Ƃňړ��i�ړ����Ă���̂̓J�����ł͂Ȃ��J�������ʂ��Ă���ΏۂƂ������ƂɂȂ�j

			interpReqData.m_ToEye       = spCamera->GetPosition_XYZ();
			interpReqData.m_FromEye     = spCurrentCamera->GetPosition_XYZ();

			spCamera->SetPosition_XYZ( spCurrentCamera->GetPosition_XYZ() );
		}
		else
		{//�R�c�͎��_�A�����_���ړ����邱�Ƃňړ��i���̃t���[�����[�N�ł̓J�����̎��W���C���g�͎g���Ă��Ȃ��j

			interpReqData.m_ToEye         = spCamera->GetData().m_Eye;
			interpReqData.m_ToTag         = spCamera->GetData().m_Tag;

			interpReqData.m_FromEye          = spCurrentCamera->GetData().m_Eye;
			interpReqData.m_FromTag          = spCurrentCamera->GetData().m_Tag;

			spCamera->SetEye( spCurrentCamera->GetData().m_Eye );
			spCamera->SetTag( spCurrentCamera->GetData().m_Tag );
		}

		//�ߋ��̃J�����̃J�����g�w�������
		( ( CameraPtr )GetBranchRoot() )->SetCurrentOn( skyFalse );

		//���݂̃J������؂�ւ���
		m_Branch = spCamera;
	}

	//�J�����g�w��
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
	//�L�[�{�[�h�̃C�x���g�o�^
	m_spKeyBordEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CameraManager > , delegate::CEventDelegateAdaptor< CameraManager >( this , &CameraManager::KeyBordEventDelegate ) ) );
	Ckeyboard_->AttachInputListener( m_spKeyBordEventDelegate );

	m_FunctionState.RegisterState( eState_Start       , &CameraManager::State_Start );
	m_FunctionState.RegisterState( eState_Wait        , &CameraManager::State_Wait );
	m_FunctionState.RegisterState( eState_Interpolate , &CameraManager::State_Interpolate );
	m_FunctionState.RegisterState( eState_End         , &CameraManager::State_End );

	//�X�e�[�g���L���[�ɓ���
	m_FunctionState.PushState( eState_Start );
	m_FunctionState.PushState( eState_Wait );
}

//-----------State Function-----------------��

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

		//���݂̃J�������擾
		CameraPtr spCurrentCamera = ( CameraPtr )GetBranchRoot();

		//��Ԓl��ݒ�i�Q�c�ƂR�c�ňႤ�j
		if( spCurrentCamera->IsOrthoOn() == skyTrue )
		{//�Q�c�̓W���C���g�𑀍삷�邱�Ƃňړ��i�ړ����Ă���̂̓J�����ł͂Ȃ��J�������ʂ��Ă���ΏۂƂ������ƂɂȂ�j

			//��Ԍv�Z������
			interp::InterpolateCommon< math::CBasicVector3 >( nowEye , interpReqData.m_FromEye , interpReqData.m_ToEye , ( f32 )( interpReqData.m_ElapsedTime / interpReqData.m_Time ) , interpReqData.m_eInterpType );

			spCurrentCamera->SetPosition_XYZ( nowEye );
		}
		else 
		{//�R�c�͎��_�A�����_���ړ����邱�Ƃňړ��i���̃t���[�����[�N�ł̓J�����̎��W���C���g�͎g���Ă��Ȃ��j

			//��Ԍv�Z������
			interp::InterpolateCommon< math::CBasicVector3 >( nowEye         , interpReqData.m_FromEye         , interpReqData.m_ToEye         , ( f32 )( interpReqData.m_ElapsedTime / interpReqData.m_Time ) , interpReqData.m_eInterpType );
			interp::InterpolateCommon< math::CBasicVector3 >( nowTag         , interpReqData.m_FromTag         , interpReqData.m_ToTag         , ( f32 )( interpReqData.m_ElapsedTime / interpReqData.m_Time ) , interpReqData.m_eInterpType );

			spCurrentCamera->SetEye( nowEye );
			spCurrentCamera->SetTag( nowTag );
		}

		//���Ԃ�i�߂�
		interpReqData.m_ElapsedTime += fps.GetPassageTime_Second();

		//�w��̎��Ԃ܂Ői�񂾂�I��
		if ( interpReqData.m_ElapsedTime >= interpReqData.m_Time )
		{
			//�Ō�͂҂����肭��悤�Ɉʒu�����킹��
			if( spCurrentCamera->IsOrthoOn() )
			{
				spCurrentCamera->SetPosition_XYZ( interpReqData.m_ToEye );
			}
			else
			{
				spCurrentCamera->SetEye( interpReqData.m_ToEye );
				spCurrentCamera->SetTag( interpReqData.m_ToTag );
			}

			//�L���[����v�����폜
			m_InterpQueue.Pop();

			//�X�e�[�g�I��
			result = skyTrue;
		}
	}

	//�o�ߎ��Ԃ���Ԏ��Ԃɓ��B����ΏI��
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
