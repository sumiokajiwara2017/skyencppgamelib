#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

void CRenderCommand::InitializeDevice( CDevicePtr pDevice , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::InitializeDevice , pDevice , skyFalse ) ) );
	}
	else
	{
		//�f�o�C�X�̏�����
		pDevice->Initialize();
	}
}

void CRenderCommand::SetRadyWindow( const CWindowPtr &spWindow , const math::CBasicRect &rect , skyBool isCommand )
{
    SKY_ASSERT_MSG( spWindow.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyWindow , spWindow , rect , skyFalse ) ) );
	}
	else
	{
		//Window�̏���
		spWindow->SetUp( rect );
	}
}

void CRenderCommand::SetRadyFrameBufferRenderTarget( const CRenderTargetPtr &spRenderTarget , const CWindowPtr &spWindow  , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyFrameBufferRenderTarget , spRenderTarget , spWindow , skyFalse ) ) );
	}
	else
	{
		//�t���[���o�b�t�@�̏���
		spRenderTarget->SetUpFrameBuffer( spWindow );
	}
}

inline void CRenderCommand::SetRadyTextureRenderTarget( const CRenderTargetPtr &spRenderTarget , const math::CBasicSize &size , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyTextureRenderTarget , spRenderTarget , size , skyFalse ) ) );
	}
	else
	{
		//�e�N�X�`���t���[���o�b�t�@�̏���
		spRenderTarget->SetUpTexture( size );
	}
}

void CRenderCommand::SetRadyDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , const CRenderTargetPtr &spRenderTarget , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRadyDepthStencilBuffer , spBuffer , spRenderTarget , skyFalse ) ) );
	}
	else
	{
		//�[�x�X�e���V���o�b�t�@�̐ݒ�
		spBuffer->SetUp( spRenderTarget->GetSize() );
	}
}

void CRenderCommand::SetViewPort( const CViewPortPtr &spViewPort , skyBool isCommand )
{
    SKY_ASSERT_MSG( spViewPort.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetViewPort , spViewPort , skyFalse ) ) );
	}
	else
	{
		//�r���[�|�[�g��ݒ�
		spViewPort->SetRenderer();
	}
}

void CRenderCommand::SetScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand )
{
    SKY_ASSERT_MSG( spScissor.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetScissorRect , spScissor , skyFalse ) ) );
	}
	else
	{
		//�V�U�����O��`��ݒ�
		spScissor->SetRenderer();
	}
}

void CRenderCommand::ClearScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand )
{
    SKY_ASSERT_MSG( spScissor.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearScissorRect , spScissor , skyFalse ) ) );
	}
	else
	{
		//�V�U�����O��`��ݒ�
		spScissor->ClearRenderer();
	}
}

void CRenderCommand::SetRenderTarget( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderTarget , spRenderTarget , spBuffer , skyFalse ) ) );
	}
	else
	{
		//�����_�����O�^�[�Q�b�g�ɐݒ�
		spRenderTarget->SetRenderer( spBuffer );
	}
}

void CRenderCommand::SetRenderState( const CRenderStatePtr &state , skyBool isCommand )
{
    SKY_ASSERT_MSG( state.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderState , state , skyFalse ) ) );
	}
	else
	{
		//�����_�[�X�e�[�g�̐ݒ�
		state->SetRenderer();
	}
}

void CRenderCommand::ClearColorBuffer( const CRenderTargetPtr &spRenderTarget , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearColorBuffer , spRenderTarget , skyFalse ) ) );
	}
	else
	{
		//�J���[�o�b�t�@�̃N���A
		spRenderTarget->ClearColorBuffer();
	}
}

void CRenderCommand::ReverseRenderState( const CRenderStatePtr &state , skyBool isCommand )
{
    SKY_ASSERT_MSG( state.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ReverseRenderState , state , skyFalse ) ) );
	}
	else
	{
		state->ReverseRenderer();
	}
}

void CRenderCommand::ClearDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearDepthStencilBuffer , spBuffer , skyFalse ) ) );
	}
	else
	{
		//�����_�����O�^�[�Q�b�g�ɐݒ�
        spBuffer->ClearDepthStencilBuffer();
	}
}

void CRenderCommand::ClearDepthBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearDepthBuffer , spBuffer , skyFalse ) ) );
	}
	else
	{
		//�����_�����O�^�[�Q�b�g�ɐݒ�
		spBuffer->ClearDepthBuffer();
	}
}

void CRenderCommand::ClearStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand )
{
    SKY_ASSERT_MSG( spBuffer.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::ClearStencilBuffer , spBuffer , skyFalse ) ) );
	}
	else
	{
		//�����_�����O�^�[�Q�b�g�ɐݒ�
		spBuffer->ClearStencilBuffer();
	}
}


void CRenderCommand::SendDisplayRenderTarget( const CRenderTargetPtr &spRenderTarget , skyBool isCommand )
{
    SKY_ASSERT_MSG( spRenderTarget.IsEmpty() == skyFalse , _T( "A necessary parameter is not set. \n" ) );

	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SendDisplayRenderTarget , spRenderTarget , skyFalse ) ) );
	}
	else
	{
		//�o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
		spRenderTarget->Present();
	}
}

void CRenderCommand::BeginRenderingScene( skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::BeginRenderingScene , skyFalse ) ) );
	}
	else
	{
		//Begin�Ɉ�����n�����ƂŁA�f�o�t����l���擾������A�f�o�t�̏������ׂ��v���ł���B
		CDevice_->BeginRendering();
	}
}
void CRenderCommand::EndRenderingScene( skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::EndRenderingScene , skyFalse ) ) );
	}
	else
	{
		//Begin�Ɉ�����n�����ƂŁA�f�o�t����l���擾������A�f�o�t�̏������ׂ��v���ł���B
		CDevice_->EndRendering();
	}
}

void CRenderCommand::RenderingMesh( const RenderingMeshParam &param , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , param , skyFalse ) ) );
	}
	else
	{
        //�`��̈�T�C�Y��ݒ�
		param.m_spCamera->SetDisplaySize( param.m_spViewPort->GetRect().W() , param.m_spViewPort->GetRect().H() );

		//�V�F�[�_�[�ɃJ������ʒm����i����ŕ��̂̌�����������ł���B�������Ƃ͕`�挋�ʂ��̂��́j
		param.m_spShader->SetCamera( param.m_spCamera );

		//�V�F�[�_�[�ɒ��_���W�ϊ��s���ݒ肷��i����ŕ��̂��ǂ��ɕ`�悳��邩����ł���j
		param.m_spShader->SetWorldMatrix( param.m_w );

		//�V�F�[�_�[�Ƀ��b�V���̏���n��
		param.m_spShader->SetMesh( param.m_spMesh );

		//�V�F�[�_�[�ɃG�L�X�g���f�[�^�̏���n��
		param.m_spShader->SetExtraData( param.m_ExtraData );

		//�p�X�ԍ��ݒ�
		param.m_spShader->SetRenderingPass( param.m_PassNo );

		//����ݒ肷��
		param.m_spShader->SetLight( param.m_spLight );

		//�V�F�[�_�[�̕`��
		param.m_spShader->vDraw();
	}
}

void CRenderCommand::RenderingPostEffectMesh( const RenderingPostEffectMeshParam &param , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingPostEffectMesh , param , skyFalse ) ) );
	}
	else
	{
		//�`��
		param.m_spShader->vRendering( param.m_PassNo , param.m_spRenderTargetTexture , param.m_SamplerData , param.m_spMesh , param.m_spCamera->GetCameraMatrix() );
	}
}


void CRenderCommand::RenderingEffect( const RenderingEffectParam &param , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingEffect , param , skyFalse ) ) );
	}
	else
	{
        //�`��̈�T�C�Y��ݒ�
		param.m_spCamera->SetDisplaySize( param.m_spViewPort->GetRect().W() , param.m_spViewPort->GetRect().H() );

		//�V�F�[�_�[�ɃJ������ʒm����i����ŕ��̂̌�����������ł���B�������Ƃ͕`�挋�ʂ��̂��́j
		param.m_spEffect->SetCamera( param.m_spCamera );

		//�V�F�[�_�[�ɒ��_���W�ϊ��s���ݒ肷��i����ŕ��̂��ǂ��ɕ`�悳��邩����ł���j
		param.m_spEffect->SetWorldMatrix( param.m_w );

		//�V�F�[�_�[�ɃG�L�X�g���f�[�^�̏���n��
		param.m_spEffect->SetExtraData( param.m_ExtraData );

		//�p�X�ԍ��ݒ�
		param.m_spEffect->SetRenderingPass( param.m_PassNo );

		//����ݒ肷��
		param.m_spEffect->SetLight( param.m_spLight );

		//�V�F�[�_�[�̕`��
		param.m_spEffect->vDraw();
	}
}

void CRenderCommand::LoadTexture( const CTexturePtr &spTex , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::LoadTexture , spTex , skyFalse ) ) );
	}
	else
	{
		spTex->Load();
	}
}

void CRenderCommand::CreateMeshGpuMemory( const CMeshContenaPtr &spMesh , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::CreateMeshGpuMemory , spMesh , skyFalse ) ) );
	}
	else
	{
		spMesh->CreateGpuMemory();
	}
}

void CRenderCommand::WirteMeshGpuMemory( const CMeshContenaPtr &spMesh , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::WirteMeshGpuMemory , spMesh , skyFalse ) ) );
	}
	else
	{
		spMesh->WirteGpuMemory();
	}
}

void CRenderCommand::DeleteMeshCpuMemory( const CMeshContenaPtr &spMesh , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::DeleteMeshCpuMemory , spMesh , skyFalse ) ) );
	}
	else
	{
		spMesh->DeleteCpuMemory();
	}
}

/*
inline void CRenderCommand::Invoke( const CRenderingCommandPtr &command , skyBool isCommand )
{
	if ( isCommand )
	{
		//�R�}���h��o�^
		CRenderCommandManager::InvokeCommand( command );
	}
	else
	{
		command->run();
	}
}
*/
} } }
