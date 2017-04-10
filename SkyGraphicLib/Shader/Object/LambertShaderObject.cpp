#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CLambertShaderObject       , sky::lib::graphic::CShaderObject );

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

skyBool CLambertShaderObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

skyBool CLambertShaderObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

const serialize::CSerializeInterfacePtr& CLambertShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CLambertShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CLambertShaderObject::Clone()
{
	return Copy( CLambertShaderObjectPtr( NEW__( CLambertShaderObject , CLambertShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CLambertShaderObject::CloneSharing()
{
	return Clone();
}

CLambertShaderObject::CLambertShaderObject()
{
	m_PassNum = 1;
}

void CLambertShaderObject::vDraw()
{
	//�V�F�[�_�[ID�̍\�z�iLambertShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
	u32 shaderId = 0;

	//�V�F�[�_�[ID���ω����Ă�����V�F�[�_�[���擾
	if ( m_spShaderInfo.IsEmpty() || m_ShaderId != shaderId )
	{
		//Hash��������΂���
		if ( s_pShaderHash == skyNull )
		{
			s_pShaderHash = NEW__( CShaderInfoPtrHash , CShaderInfoPtrHash( SHADER_HASH_SIZE ) );
		}

		m_spShaderInfo = GetShaderInfo( shaderId );
		m_ShaderId = shaderId; //���݂̃V�F�[�_�[��ύX
	}

	//�p�X���̕`��
	if ( m_RenderingPassNo == 0 )
	{
		Pass1Draw();
	}
}

} } }

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

// �萔�o�b�t�@�̃f�[�^��`
struct LambertShaderConst
{
	FLOAT      Alpha;	    // �A���t�@�l
};

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CLambertShaderObject::Pass1Draw()
{
}

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		// �V�F�[�_�̓ǂݍ���
		GLuint hm_ProgramObject0;
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			hm_ProgramObject0 ,
			_T( "data/lib/shader/glsl/LambertShader.vert" ), 
			_T( "data/lib/shader/glsl/LambertShader.frag" ) );

		spShaderInfo->m_Pass1.m_ProgramObject = hm_ProgramObject0;
 	
		//���_�������
		spShaderInfo->m_Pass1.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inPos");
		spShaderInfo->m_Pass1.m_NormalLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inNorm");

		//�O���[�o�����
		spShaderInfo->m_Pass1.m_WorldLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "World");
		spShaderInfo->m_Pass1.m_ViewLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "View");
		spShaderInfo->m_Pass1.m_ProjectionLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Projection");

		spShaderInfo->m_Pass1.m_DiffuseLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Diffuse");
		spShaderInfo->m_Pass1.m_AlphaLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Alpha");
		spShaderInfo->m_Pass1.m_LightDirLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "LightDir");
		spShaderInfo->m_Pass1.m_ViewDirLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "ViewDir");
		spShaderInfo->m_Pass1.m_LightAmbientLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "LightAmbient");
		spShaderInfo->m_Pass1.m_LightDiffuseLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "LightDiffuse");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#endif

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CLambertShaderObject::Pass1Draw()
{
	//�O���[�o���p�����[�^�[
	GLfloat World[ 16 ];
	GLfloat View[ 16 ];
	GLfloat Projection[ 16 ];
	GLfloat LightDir[ 3 ];
	GLfloat ViewDir[ 3 ];
	GLfloat LightAmbient[ 3 ];
	GLfloat LightDiffuse[ 3 ];
	GLfloat Diffuse[ 3 ];

	//�V�F�[�_�[�̐ݒ�
	glUseProgram( m_spShaderInfo->m_Pass1.m_ProgramObject );

	//�s����̐ݒ�
	m_spCamera->GetProjectionMatrix().CopyArray< f32 >( Projection );
	m_spCamera->GetViewMatrix().CopyArray< f32 >( View );
	m_WorldMatrix.CopyArray< f32 >( World );

    // ���W�ϊ��s��
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WorldLocation , 1 , GL_FALSE , World );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_ViewLocation , 1 , GL_FALSE , View );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_ProjectionLocation , 1 , GL_FALSE , Projection ); 

	//�x�N�g��
	m_spLight->GetDirectionalLightData().m_Vec.Copyf32Array( LightDir );
	m_spCamera->GetData().m_ViewVec.Copyf32Array( ViewDir );
	glUniform3fv( m_spShaderInfo->m_Pass1.m_LightDirLocation , 1 , LightDir );
	glUniform3fv( m_spShaderInfo->m_Pass1.m_ViewDirLocation , 1 , ViewDir );

	//���C�g
	m_spLight->GetDirectionalLightData().m_AmbientColor.Copyf32Array3RGB( LightAmbient );
	m_spLight->GetDirectionalLightData().m_DiffuseColor.Copyf32Array3RGB( LightDiffuse );
	glUniform3fv( m_spShaderInfo->m_Pass1.m_LightAmbientLocation , 1 , LightAmbient );
	glUniform3fv( m_spShaderInfo->m_Pass1.m_LightDiffuseLocation , 1 , LightDiffuse );

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�}�e���A�������V�F�[�_�[�Ɉ����p��
		spMatDrawData->GetMaterial()->Diffuse.Copyf32Array3RGB( Diffuse );
		glUniform1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , 1 , &spMatDrawData->GetMaterial()->Alpha );
		glUniform3fv( m_spShaderInfo->m_Pass1.m_DiffuseLocation , 1 , Diffuse );

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_VertexLocation;
		spVertexBuffer->GetNormalStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_NormalLocation;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetNormalStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex , pMesh->GetIndexBuffer() );
	}
}

#endif //SW_SKYGRAPHICLIB_USE_GLSL_ON

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CLambertShaderObject::Pass1Draw()
{
}

#endif // SW_SKYGRAPHICLIB_USE_CG_ON

} } }

#endif // SKYLIB_RENDER_OPENGL4
