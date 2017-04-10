#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"
#include "CgLightShaderObject.h"

#ifdef SKY_GRAPHIC_LIB_OPENGL

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

struct CCgLightShaderObjectMember
{
	CCgCombineProgram Shader;

    CGparameter World;                  // ���[���h�ϊ��s��
    CGparameter View;                   // �r���[�ϊ��s��
    CGparameter Projection;             // �����ϊ��s��
    CGparameter Diffuse;                // �f�B�t���[�Y�F
    CGparameter Light;                  // �������W(�r���[���W�n)

    CGparameter texCoord0Location;
};

static CCgCombineProgram s_Shader;
static skyBool    s_IsLoad = skyFalse;

void CCgLightShaderObject::Load( CCgLightShaderObjectMember *dest )
{
	if ( s_IsLoad == skyFalse )
	{
	    CCg::CreateVertexFragmentCombineShaderFromFile( s_Shader , 
		                                            "data/test/cg/CCgLight_vertex.cg"   , "Cg_Main" ,
		                                            "data/test/cg/CCgLight_fragment.cg" , "Cg_Fragment_Main"  );

        dest->World = cgGetNamedParameter ( s_Shader.Program, "uniforms.World" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        dest->View = cgGetNamedParameter ( s_Shader.Program, "uniforms.View" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        dest->Projection = cgGetNamedParameter ( s_Shader.Program, "uniforms.Projection" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        dest->Diffuse = cgGetNamedParameter ( s_Shader.Program, "uniforms.Diffuse" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        dest->Light = cgGetNamedParameter ( s_Shader.Program, "uniforms.Light" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

        dest->texCoord0Location = cgGetNamedParameter( s_Shader.Program , "texCoord0Location" );//���O
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        
		s_IsLoad = skyTrue;
	}

	dest->Shader = s_Shader;
}

void CCgLightShaderObject::Destroy()
{
    cgDestroyProgram( s_Shader.Program );
}

CCgLightShaderObject::CCgLightShaderObject()
{
	m_pMember = NEW CCgLightShaderObjectMember;
}

CCgLightShaderObject::~CCgLightShaderObject()
{
	DEL m_pMember;
}

//�O���[�o���p�����[�^�[
static GLfloat World     [ 16 ];
static GLfloat View      [ 16 ];
static GLfloat Projection[ 16 ];
static GLfloat Diffuse   [ 4 ];
static GLfloat Light     [ 3 ]; 

void CCgLightShaderObject::vDraw()
{
	//�V�F�[�_�[���[�h
	Load( m_pMember );
        
	//�v���W�F�N�V�����s��̐ݒ�
	m_spCamera->ComputeProjectionMatrix();
	sl_CopyMatrixf( Projection , m_spCamera->GetMember()->m_ProjectionMatrix );

	//���_���W�ϊ�
	m_spCamera->ComputeViewMatrix();
	sl_CopyMatrixf( View , m_spCamera->GetMember()->m_ViewMatrix );

	//���[���h�s��ݒ�
	sl_CopyMatrixf( World , m_WorldMatrix );

	//�s��ݒ�
	cgSetMatrixParameterfr( m_pMember->Projection , Projection );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
	cgSetMatrixParameterfr( m_pMember->View , View );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
	cgSetMatrixParameterfr( m_pMember->World , World );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

	// �_�����x�N�g���v�Z�ݒ�
	GLfloat lightPos[ 3 ] = { 3.0f, 3.0f, 3.0f };
	sl_MultVectorMatrixf( Light , lightPos , m_spCamera->GetMember()->m_ViewMatrix );
    cgSetParameter3fv( m_pMember->Light , Light );

	const CGprofile profile0 = cgGetProgramDomainProfile( m_pMember->Shader.Program , 0);
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
	const CGprofile profile1 = cgGetProgramDomainProfile( m_pMember->Shader.Program , 1);
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );

	//���_�V�F�[�_�[��ݒ�
	cgGLBindProgram( m_pMember->Shader.Program );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
    cgGLEnableProfile( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
    cgGLEnableProfile( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );

	// ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@�̐ݒ�
	m_spMesh->m_spVertexBuffer->GetMember()->m_VertexLocation = 0;
	m_spMesh->m_spVertexBuffer->GetMember()->m_NormalLocation = 1;
	m_spMesh->m_spVertexBuffer->GetMember()->m_TexCoord0Location = 2;
	m_spMesh->m_spVertexBuffer->SetGpuMemoryStream( vertex::VERTEX_FORMAT_BIT_XYZ | vertex::VERTEX_FORMAT_BIT_NORMAL | vertex::VERTEX_FORMAT_BIT_TEX0 );

	FOREACH( CTrianglesPtrList , it , m_spMesh->m_spGroupList )
	{
		//�}�e���A�������V�F�[�_�[�Ɉ����p��
		Diffuse[ 0 ] = ( GLfloat )( *it )->Material->Diffuse.X;
		Diffuse[ 1 ] = ( GLfloat )( *it )->Material->Diffuse.Y;
		Diffuse[ 2 ] = ( GLfloat )( *it )->Material->Diffuse.Z;
		Diffuse[ 3 ] = ( GLfloat )( *it )->Material->ZTransp;
        cgSetParameter4fv( m_pMember->Diffuse , Diffuse );

		//�f�B�r���[�Y�}�b�v�e�N�X�`�����g�p����
		if ( ( *it )->Material->DiffuseMap.IsEmpty() == skyFalse )
		{
            cgGLSetTextureParameter( m_pMember->texCoord0Location , ( *it )->Material->DiffuseMap->GetMember()->texId[ 0 ] ); //�e�N�X�`���Ƃ̊֘A�t��
            CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
            cgGLEnableTextureParameter( m_pMember->texCoord0Location );
            CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
		}

		//���b�V�����ɕ`��
		m_spMesh->m_spVertexBuffer->Draw();
	}

	//�V�F�[�_�[��ݒ����
    cgGLDisableProfile( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
    cgGLDisableProfile( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
	cgGLUnbindProgram( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
	cgGLUnbindProgram( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );

}

CShaderObjectPtr  CCgLightShaderObject::vCloneNode()
{
	CCgLightShaderObjectPtr spResult = Create();
	return spResult;
}

} } }

#endif

#endif