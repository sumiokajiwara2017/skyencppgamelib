#include "StdAfx.h"
#include "SkyMathLib/Joint.h"

namespace sky { namespace lib { namespace math {

#define JOINT_STACK_INIT_NUM ( 256 )

void CMotionController::AttachSkeleton( const CSkeletonPtr &spSkeleton )
{
	m_spSkeleton = spSkeleton;

	m_WorldTransforms.Reset( spSkeleton->m_spJointPallet->GetJointNum() );

#ifdef SW_SKYLIB_DEBUG_ON

	m_DebugJointTransforms.Reset( spSkeleton->m_spJointPallet->GetJointNum() );

#endif	

}

void CMotionController::SetAnimationContena( const CMotionDataContenaPtr &spAnimationContena )
{
	m_spAnimationContena = spAnimationContena;
}

void CMotionController::InitJointTransformsMatrix()
{
	for( u32 i = 0 ; i < m_spSkeleton->m_spJointPallet->GetJointNum() ; i++ )
	{
		m_spSkeleton->m_spJointPallet->GetJoint( i )->SetJointTransForm( m_spSkeleton->m_spJointPallet->GetJoint( i )->GetInitialTransform() );
#ifdef SW_SKYLIB_DEBUG_ON
		m_DebugJointTransforms[ i ] = m_spSkeleton->m_spJointPallet->GetJoint( i )->GetInitialTransform();
#endif
	}
}

void CMotionController::Play( u32 motionNo , updateTime blendTime )
{
	m_CurrentAnimationNo = motionNo;
	m_BlendTime = blendTime;
	m_CurrentAnimationData = m_spAnimationContena->GetAnimation( motionNo );

    m_CurrentTime = 0.0f;

	InitJointTransformsMatrix();
}

CMotionDataPtr CMotionController::GetCurrentAnimationData()
{
	return m_CurrentAnimationData;
}

void CMotionController::Update( updateTime time , skyBool relativeToCurTime )
{
	SKY_ASSERT( m_spSkeleton.IsEmpty() == skyFalse );
	SKY_ASSERT( m_spAnimationContena.IsEmpty() == skyFalse );

	//�W���C���g�𓮂���
	UpdateJointTransform( time , relativeToCurTime );

	//���[���h���W��̈ʒu�𓮂���
	UpdateWorldTransform();

	//�X�L�j���O�s����X�V
	UpdateSkinTransform();
}

void CMotionController::UpdateJointTransform( updateTime time , skyBool relativeToCurTime )
{
	SKY_ASSERT( m_spSkeleton.IsEmpty() == skyFalse );
	SKY_ASSERT( m_spSkeleton->m_spJointPallet.IsEmpty() == skyFalse );
	SKY_ASSERT( m_spSkeleton->m_spJointRoot.IsEmpty() == skyFalse );
	SKY_ASSERT( m_spAnimationContena.IsEmpty() == skyFalse );
	SKY_ASSERT( m_CurrentAnimationNo != UINT_MAX );

    if ( relativeToCurTime )
    {
        //�@���Ԃ�i�߂�
        time += m_CurrentTime;

        //�@�p�����Ԃ����傫���Ȃ�����C���߂ɖ߂�
        while( time >= m_CurrentAnimationData->GetMaxTime() )
        {
            time -= m_CurrentAnimationData->GetMaxTime();
        }
    }

	SKY_ASSERT( time >= 0.0f );
	SKY_ASSERT( time < m_CurrentAnimationData->GetMaxTime() );

    //�@���ݎ��Ԃ����������Ȃ�C������
    if ( time < m_CurrentTime )
    {
		InitJointTransformsMatrix();
    }

    //�@���ݎ��Ԃ��X�V
    m_CurrentTime = time;

    u32 num_frames = m_CurrentAnimationData->GetAnimationNum();
    for( u32 i = 0 ; i < num_frames ; ++i )
    {
        //�@���[�V�����f�[�^�擾
        const CAnimationData &animation = m_CurrentAnimationData->GetAnimationData( i );
		CJointPtr spJoint = m_spSkeleton->m_spJointPallet->GetJoint( animation.m_JointName );

		u32 iKey;
		u32 iKey2 = 0;
		u32 iKey3 = 0;
		u32 fTime1;
		u32 fTime2;
		f32 fLerpValue;
		math::CMatrix4x3 mat;

		mat.Identity();

		if( animation.m_KeyFrameArrayMatrix.GetNum() > 0 )
		{//�s��

			for ( iKey = 0 ; iKey < animation.m_KeyFrameArrayMatrix.GetNum() ; iKey++ )
			{
				if ( ( updateTime )animation.m_KeyFrameArrayMatrix[ iKey ].m_Time > m_CurrentTime )
				{
					iKey3 = iKey;
					iKey2= ( 0 < iKey )?( iKey - 1 ) : iKey;
					break;
				}
			}
			math::CMatrix4x3 mat1 = animation.m_KeyFrameArrayMatrix[ iKey2 ].m_Matrix;
			math::CMatrix4x3 mat2 = animation.m_KeyFrameArrayMatrix[ iKey3 ].m_Matrix;

			//�@�W���C���g�̕ϊ��s������߂�
			spJoint->SetJointTransForm( mat1 + mat2 );

#ifdef SW_SKYLIB_DEBUG_ON
			m_DebugJointTransforms[ spJoint->Index ] = mat2;
#endif
		}
		else
		{
			//�X�P�[��
			if( animation.m_KeyFrameArrayScale.GetNum() > 0 )
			{
				for ( iKey = 0 ; iKey < animation.m_KeyFrameArrayScale.GetNum() ; iKey++ )
				{
					if ( ( updateTime )animation.m_KeyFrameArrayScale[ iKey ].m_Time > m_CurrentTime )
					{
						iKey3 = iKey;
						iKey2= ( 0 < iKey )?( iKey - 1 ) : iKey;
						break;
					}
				}
				fTime1 = animation.m_KeyFrameArrayScale[ iKey2 ].m_Time;
				fTime2 = animation.m_KeyFrameArrayScale[ iKey3 ].m_Time;

				// ���Ԃ̑O��̃L�[�t���[���ŋ߂�����I������
				fLerpValue = ( ( fTime2 - fTime1 ) != 0 ) ? ( ( f32 )( m_CurrentTime - fTime1 )  / ( f32 )( fTime2 - fTime1 ) ) : 0;

				//���`���
				math::CBasicVector3 sc;
				interp::Linear< math::CBasicVector3 >( sc , animation.m_KeyFrameArrayScale[ iKey2 ].m_Scale , animation.m_KeyFrameArrayScale[ iKey3 ].m_Scale , fLerpValue );

				math::CMatrix4x3 scMat;
				scMat.SetupScale( sc );

				mat *= scMat;
			}

			//��]
			if( animation.m_KeyFrameArrayRot.GetNum() > 0 )
			{
				for ( iKey = 0 ; iKey < animation.m_KeyFrameArrayRot.GetNum() ; iKey++ )
				{
					if ( ( updateTime )animation.m_KeyFrameArrayRot[ iKey ].m_Time > m_CurrentTime )
					{
						iKey3 = iKey;
						iKey2= ( 0 < iKey )?( iKey - 1 ) : iKey;
						break;
					}
				}
				fTime1 = animation.m_KeyFrameArrayRot[ iKey2 ].m_Time;
				fTime2 = animation.m_KeyFrameArrayRot[ iKey3 ].m_Time;

				// ���Ԃ̑O��̃L�[�t���[���ŋ߂�����I������
				fLerpValue = ( ( fTime2 - fTime1 ) != 0 ) ? ( ( f32 )( m_CurrentTime - fTime1 )  / ( f32 )( fTime2 - fTime1 ) ) : 0;

				//���ʐ��`���
				math::CQuaternion rot = Slerp( animation.m_KeyFrameArrayRot[ iKey2 ].m_Rot , animation.m_KeyFrameArrayRot[ iKey3 ].m_Rot , ( dectype )fLerpValue );

				math::CMatrix4x3 rotMat;
				rotMat.SetupRotate( rot );

				mat *= rotMat;
			}
			
			//���s�ړ�
			if( animation.m_KeyFrameArrayTrans.GetNum() > 0 )
			{
				for ( iKey = 0 ; iKey < animation.m_KeyFrameArrayTrans.GetNum() ; iKey++ )
				{
					if ( ( updateTime )animation.m_KeyFrameArrayTrans[ iKey ].m_Time > m_CurrentTime )
					{
						iKey3 = iKey;
						iKey2= ( 0 < iKey )?( iKey - 1 ) : iKey;
						break;
					}
				}
				fTime1 = animation.m_KeyFrameArrayTrans[ iKey2 ].m_Time;
				fTime2 = animation.m_KeyFrameArrayTrans[ iKey3 ].m_Time;

				// ���Ԃ̑O��̃L�[�t���[���ŋ߂�����I������
				fLerpValue = ( ( fTime2 - fTime1 ) != 0 ) ? ( ( f32 )( m_CurrentTime - fTime1 )  / ( f32 )( fTime2 - fTime1 ) ) : 0;

				//���`���
				math::CBasicVector3 trans;
				interp::Linear< math::CBasicVector3 >( trans , animation.m_KeyFrameArrayTrans[ iKey2 ].m_Trans , animation.m_KeyFrameArrayTrans[ iKey3 ].m_Trans , fLerpValue );

				math::CMatrix4x3 transMat;
				transMat.SetupTranslation( trans );

				mat *= transMat;
			}
			else
			{
				// ���s�ړ��̏ꍇ�̓A�j����������΃f�B�t�H���g�̈ʒu���w�肷��
				math::CMatrix4x3 transMat;
				transMat.SetTranslation( math::CBasicVector3( spJoint->GetInitialTransform().Tx , spJoint->GetInitialTransform().Ty , spJoint->GetInitialTransform().Tz ) );

				mat *= transMat;
			}

			//�@�W���C���g�̕ϊ��s������߂�
			spJoint->SetJointTransForm( mat );

#ifdef SW_SKYLIB_DEBUG_ON
			m_DebugJointTransforms[ spJoint->Index ] = mat;
#endif
		}
    }
}

void CMotionController::UpdateWorldTransform()
{

	SKY_ASSERT( m_JointStack.GetNum() == 0 );

	m_JointStack.Push( m_spSkeleton->m_spJointRoot );

	while( m_JointStack.GetNum() > 0 )
	{
		CJointPtr spCurrentJoint;
		m_JointStack.Pop( spCurrentJoint );

		//�W���C���g�ϊ��s��~�W���C���g�̋�ԃ}�g���b�N�X
		if( m_JointStack.GetNum() == 0 )
		{//root
			m_WorldTransforms[ spCurrentJoint->Index ] = MulMatrix4x4Full( spCurrentJoint->GetJointTransForm() , spCurrentJoint->ComputeLocalMatrix() );
		}
		else
		{//child
			m_WorldTransforms[ spCurrentJoint->Index ] = MulMatrix4x4Full( spCurrentJoint->GetJointTransForm() , MulMatrix4x4Full( spCurrentJoint->ComputeLocalMatrix() , m_WorldTransforms[ spCurrentJoint->Parent()->Index ] ) );

#ifdef SW_SKYLIB_DEBUG_ON
			SKY_PRINTF( _T( "=====< WorldTransforms >===== \n" ) );
			m_WorldTransforms[ spCurrentJoint->Index ].DebugPrint();
			SKY_PRINTF( _T( "=====< JointTransForm >===== \n" ) );
			spCurrentJoint->GetJointTransForm().DebugPrint();
			SKY_PRINTF( _T( "=====< Parent JointTransForm >===== \n" ) );
			m_WorldTransforms[ spCurrentJoint->Parent()->Index ].DebugPrint();
#endif
		}

		if ( spCurrentJoint->ChildList().GetNum() > 0 )
		{
			FOREACH( CJointPtrList , it , spCurrentJoint->ChildList() )
			{
				m_JointStack.Push( *it );
			}
			continue;
		}
	}

	SKY_ASSERT( m_JointStack.GetNum() == 0 );
}

void CMotionController::UpdateSkinTransform()
{
	u32 boneNum = m_spSkeleton->m_spJointPallet->GetJointNum();
	for( u32 i=0; i <boneNum ; i++ )
	{
		// �X�L�j���O�s������߂�
		m_spSkeleton->m_spJointPallet->m_SkinTransforms[ i ] = MulMatrix4x4Full( m_spSkeleton->m_spJointPallet->GetJoint( i )->GetOffsetMatrix() , m_WorldTransforms[ i ] );
/*
#ifdef SW_SKYLIB_DEBUG_ON
		SKY_PRINTF( _T( "=====< OffsetMatrix[ %s ][ %d ] >===== \n" ) , m_spSkeleton->m_spJointPallet->GetJoint( i )->Name.GetString() , m_spSkeleton->m_spJointPallet->GetJoint( i )->Index );
		m_spSkeleton->m_spJointPallet->GetJoint( i )->GetOffsetMatrix().DebugPrint();
		SKY_PRINTF( _T( "=====< WorldTransforms >===== \n" ) );
		m_WorldTransforms[ i ].DebugPrint();
		SKY_PRINTF( _T( "=====< m_SkinTransforms >===== \n" ) );
		m_spSkeleton->m_spJointPallet->m_SkinTransforms[ i ].DebugPrint();
#endif
*/
	}
}

CMotionController::CMotionController() : m_CurrentTime( 0 ) , m_CurrentAnimationNo( UINT_MAX ) , m_BlendTime( 0 ) , m_JointStack( JOINT_STACK_INIT_NUM )
{
}

} } }