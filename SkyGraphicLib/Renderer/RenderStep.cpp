#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >----------------------------------------------//

//�`��Ɋւ��邷�ׂĂ̏���
void CRenderStep::Rendering( const fps::CFpsController &fps )
{
	//�����_�����O�p�����[�^�[
	CRenderingParam param;
	param.Fps = fps;

	//Window��ς�
	param.Window = m_spWindow;

	//�X�^�b�N�ɐς�
	param.RenderTargetStack.Push( m_spRenderTarget );
	param.DepthStencilBufferStack.Push( m_spDepthStencilBuffer );

	//�����_�����O�^�[�Q�b�g��ݒ�
	CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );

	//�J���[�o�b�t�@���N���A
	CRenderCommand::ClearColorBuffer( param.RenderTargetStack.GetTop() );

	//�[�x�X�e���V���o�b�t�@�̃N���A
	CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );

	//�X�^�b�N�ɐς�
	param.RenderStateStack.Push( m_spRenderState );

	//�����_�[�X�e�[�g��ݒ肷��
	CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );

	//���̃X�e�b�v�̋@�\�Ƃ��đS�V�[����W���I�ȕ`�������
	CScenePtrList &kSceneList =  m_RenderStepManager->GetSceneList();

	//�V�[�����̕`��X�^�[�g
	FOREACH( CScenePtrList, it , kSceneList )
	{
		//�V�[�����`��I�t���A�V�[���O���t���Ȃ����A�V�[���O���t�̃��[�g���J�����O�I�t�̏ꍇ�͂Ȃɂ���������
		if (  ( * it )->m_IsOff == skyTrue || ( * it )->GetSceneGraphRoot().IsEmpty() || ( * it )->GetSceneGraphRoot()->IsAppCulling() ) continue;

		//�V�[���Ǝ��̃����_�[�X�e�[�g��ς�
		if ( ( * it )->GetRenderState( skyFalse ).IsEmpty() == skyFalse ) 
		{
			//�X�^�b�N�ɐς�
			param.RenderStateStack.Push( ( * it )->GetRenderState() );

			//�ݒ肷��
			CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );
		}

		//�V�[���Ǝ��̐[�x�X�e���V���o�b�t�@��ς�
		if ( ( * it )->GetDepthStencilBuffer( skyFalse ).IsEmpty() == skyFalse )
		{
			//�X�^�b�N�ɐς�
			param.DepthStencilBufferStack.Push( ( * it )->GetDepthStencilBuffer() );

			//�ݒ肷��
			CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
		}

		//�[�x�o�b�t�@�̃N���A�w�肪����΃N���A
		if ( ( * it )->m_IsDepthBufferClear == skyTrue )
		{
			CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//�X�e���V���o�b�t�@�̃N���A�w�肪����΃N���A����
		if ( ( * it )->m_IsStencilBufferClear == skyTrue )
		{
			CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//�V�[���Ǝ��̃V�U�[��`��ς�
		if ( ( * it )->GetScissorRect( skyFalse ).IsEmpty() == skyFalse )
		{
			( * it )->GetScissorRect()->vRendering( param );
		}

		//�r���[�|�[�g��ς�
		( * it )->GetViewPort()->vRendering( param );

		//�J�����Ǘ��̍X�V
		( *it )->GetCameraManager()->Update( param.Fps );

		//���݂̃J�������擾
		CameraPtr spCurrentCamera = ( *it )->GetCameraManager()->GetCurrentCamera();

		//�J�����W���C���g������΍s��X�^�b�N��Push����
		if ( spCurrentCamera->IsJointUse() == skyTrue )
		{
			param.MatrixStack.Push();
			math::CJointPtr camerajoint = spCurrentCamera;
			if ( ( *it )->GetSceneGraphRoot()->IsJointUse() == skyTrue )
			{//�J�����ɃW���C���g���������ꍇ�A�J�����̃W���C���g���v�Z�ɓ����B
				math::CJointPtr spJoint = ( * it )->GetSceneGraphRoot();
				param.MatrixStack.LoadAndMultLocalBeforeMatrix( spJoint->ComputeLocalMatrix() * camerajoint->ComputeLocalMatrix() );
			}
			else
			{
				param.MatrixStack.LoadAndMultLocalBeforeMatrix( camerajoint->ComputeLocalMatrix() );
			}
		}

		//�J������ς�
		spCurrentCamera->vRendering( param );

		//���C�g��ς�
		( * it )->GetLight()->vRendering( param );

		//�V�[���Ǝ��̃����_�����O�^�[�Q�b�g��ς�
		if ( ( * it )->GetRenderTarget( skyFalse ).IsEmpty() == skyFalse )
		{
			//�X�^�b�N�ɐς�
			param.RenderTargetStack.Push( ( * it )->GetRenderTarget() );

			//�ݒ肷��
			CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );

			//�����_�����O�^�[�Q�b�g�S�̂��N���A
			CRenderCommand::ClearColorBuffer( param.RenderTargetStack.GetTop() );
		}

		//�V�[���̕`��J�n
		CRenderCommand::BeginRenderingScene();

//--------------��

		//�V�[���O���t�m�[�h�c���[�̃g���o�[�X�����_�����O
		( * it )->GetSceneGraphRoot()->vRendering( param );
		( * it )->GetSceneGraphRoot()->RenderingChild( param ); //�ċA�Ŏq�����ׂă����_�����O�����B
		( * it )->GetSceneGraphRoot()->vPostRendering( param );

		//�����������_�[�f�[�^�̕`��
		param.AlphaDataRendering();

//--------------��

		//�V�[���̕`��I��
		CRenderCommand::EndRenderingScene();

		//�|�X�g�G�t�F�N�g�̕`��i�}�l�[�W���[������΃|�X�g�G�t�F�N�g������Ƃ����ӎv�Ƃ݂Ȃ��j
		const CPostEffectManagerPtr &spPostEffectManager = ( *it )->GetPostEffectManager();
		if ( spPostEffectManager.IsEmpty() == skyFalse && ( * it )->GetRenderTarget( skyFalse ).IsEmpty() == skyFalse )
		{
			spPostEffectManager->Rendering( ( * it )->GetRenderTarget() , param.DepthStencilBufferStack.GetTop() );
		}

		//���C�g�����
		( * it )->GetLight()->vPostRendering( param );

		//�J���������
		spCurrentCamera->vPostRendering( param );

		//�J�����W���C���g�W���C���g������΍s��X�^�b�N����Pop����
		if ( spCurrentCamera->IsJointUse() == skyTrue )
		{
			param.MatrixStack.Pop();
		}

		//�r���[�|�[�g�����
		( * it )->GetViewPort()->vPostRendering( param );

		//�V�[���Ǝ��̃V�U�[��`�����
		if ( ( * it )->GetScissorRect( skyFalse ).IsEmpty() == skyFalse )
		{
			( * it )->GetScissorRect( skyFalse )->vPostRendering( param );
		}

		//�V�[���Ǝ��̐[�x�X�e���V���o�b�t�@�����
		if ( ( * it )->m_IsDepthBufferClear == skyFalse )
		{
			//�X�^�b�N������
			param.DepthStencilBufferStack.Pop();

			//�ݒ肷��
			CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
		}

		//�V�[���Ǝ��̃����_�[�^�[�Q�b�g�����
		if ( ( * it )->GetRenderTarget( skyFalse ).IsEmpty() == skyFalse )
		{
			( * it )->GetRenderTarget()->vPostRendering( param );
		}

		//�V�[���Ǝ��̃����_�[�X�e�[�g�����
		if ( ( * it )->GetRenderState( skyFalse ).IsEmpty() == skyFalse )
		{
			//�X�^�b�N������
			param.RenderStateStack.Pop();

			//�ݒ肷��
			CRenderCommand::SetRenderState( param.RenderStateStack.GetTop() );
		}
	}

	//�����_�����O�^�[�Q�b�g���f�B�X�v���C�ɕ\������
	CRenderCommand::SendDisplayRenderTarget( m_spRenderTarget );

	//�X�^�b�N������
	param.RenderTargetStack.Pop();
	param.DepthStencilBufferStack.Pop();
	param.RenderStateStack.Pop();
}

} } }
