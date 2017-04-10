#include "StdAfx.h"
#include "SkyGraphicLib/Camera.h"

namespace sky { namespace lib { namespace graphic {

namespace CameraUtil
{

math::CBasicVector3 CameraUtil::ConvertScreenPosTo3DPos( const math::CBasicVector2 &screenPos , const CWindowPtr &spWindow , const CameraPtr &spCamera , dectype distance )
{
	math::CBasicVector3 result( screenPos.X , screenPos.Y , 0.0f );
	math::CMatrix4x3    viewPortMatrix;
	math::CMatrix4x3    projMatrix;
	math::CMatrix4x3    viewMatrix;

	viewPortMatrix.Identity();
	projMatrix.Identity();
	viewMatrix.Identity();

	viewPortMatrix.M11 = spWindow->GetRect().W() / 2;
	viewPortMatrix.M22 = -spWindow->GetRect().H() / 2;
	viewPortMatrix.Tx  = spWindow->GetRect().W() / 2;
	viewPortMatrix.Ty  = spWindow->GetRect().H() / 2;

	viewMatrix = spCamera->GetViewMatrix();
	projMatrix = spCamera->GetProjectionMatrix();

	//�t�s��ɂ���
	math::CMatrix4x3 invViewPortMatrix = Invert( viewPortMatrix );
	math::CMatrix4x3 invProjMatrix = Invert( projMatrix );
	math::CMatrix4x3 invViewMatrix = Invert( viewMatrix );

	math::CMatrix4x3 screenPosMat;
	screenPosMat.Tx = screenPos.X;
	screenPosMat.Ty = screenPos.Y;
	screenPosMat.Tz = 0.0f;
	screenPosMat.Tw = 1.0f;

	result = result * invViewPortMatrix * invProjMatrix * invViewMatrix;

	return result;
}

void SetDefaultCameraPos_Left( const CameraPtr &dest , dectype distance )
{
	//���������Ă��܂��B
	dest->Init();

	//�f�t�H���g�͉q���J�����ŁA�q���J�����͋����Ɗp�x�ňʒu�����肷�邽�߁A�����Ɗp�x��ݒ肷��
	math::CBasicAngleVector3 angle( 0 , 0 , 0 , 0 );
	angle.X().FromDeg( 90 );
	dest->SetAngle( angle );
	dest->SetDistance( distance );

	//�X�V
	dest->Update( fps::CFpsController() );
}

void SetDefaultCameraPos_Right( const CameraPtr &dest , dectype distance )
{
	//���������Ă��܂��B
	dest->Init();

	//�f�t�H���g�͉q���J�����ŁA�q���J�����͋����Ɗp�x�ňʒu�����肷�邽�߁A�����Ɗp�x��ݒ肷��
	math::CBasicAngleVector3 angle( 0 , 0 , 0 , 0 );
	angle.X().FromDeg( -90 );
	dest->SetAngle( angle );
	dest->SetDistance( distance );

	//�X�V
	dest->Update( fps::CFpsController() );
}

void SetDefaultCameraPos_Top( const CameraPtr &dest , dectype distance )
{
	//���������Ă��܂��B
	dest->Init();

	//�f�t�H���g�͉q���J�����ŁA�q���J�����͋����Ɗp�x�ňʒu�����肷�邽�߁A�����Ɗp�x��ݒ肷��
	math::CBasicAngleVector3 angle( 0 , 0 , 0 , 0 );
	angle.Y().FromDeg( 90 );
	dest->SetAngle( angle );
	dest->SetDistance( distance );

	//�X�V
	dest->Update( fps::CFpsController() );
}

}

} } }