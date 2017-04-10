#include "StdAfx.h"
#include "SkyGraphicLib/OpenGL/SkyGraphicLibGL.h"

#ifdef SKYLIB_RENDER_OPENGL4

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

struct CCgMember
{
	/// <summary>
	/// �R���e�L�X�g
	/// </summary>
	CGcontext Cg_Cont;
};

CCgMember *CCg::m_pMember = skyNull;

void CCg::Initialize()
{
    SKY_ASSERT_MSG( m_pMember == skyNull , _T( "It was initialized two times.\n" ) );

	m_pMember = NEW__( CCgMember , CCgMember );

	//�R���e�L�X�g�쐬
	m_pMember->Cg_Cont = cgCreateContext();
    if( m_pMember->Cg_Cont == NULL )
    {
		SKY_PANIC_MSG( _T( "It failed in the initialization of Cg." ) ); //Cg�̏������Ɏ��s���܂����B
    }

#ifndef SW_SKYLIB_DEBUG_ON
	 cgGLSetDebugMode( CG_FALSE );
#endif
}

void CCg::Dispose()
{
	cgDestroyContext( m_pMember->Cg_Cont );
	SAFE_DELETE( m_pMember );
}

CGcontext CCg::GetContext()
{
    SKY_ASSERT_MSG( m_pMember != skyNull , _T( "Please initialize it. \n" ) );

	return m_pMember->Cg_Cont;
}

CCgProgram &CCg::CreateVertexShaderFromFile( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName )
{
    CreateVertexShaderFromFileImple( dest , pFilePath , pFuncName );

    cgGLLoadProgram( dest.Program );
	CheckForCgError( "It failed in making the vertex shader. " ); //���_�V�F�[�_�[�̍쐬�Ɏ��s���܂����B

	return dest;
}

void CCg::CreateVertexShaderFromFileImple( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName )
{

    //---------- �v���t�@�C���̐ݒ� -----------//
//  dest.Profile = cgGLGetLatestProfile( CG_GL_VERTEX );
	dest.Profile = CG_PROFILE_GLSLV; //GLSL�Ɠ����Ɏg�p���邽�߂ɂ̓v���t�@�C�������̒l�ɂ���
	CheckForCgError( "It failed in making the vertex shader profile. " ); //���_�V�F�[�_�[�v���t�@�C���̍쐬�Ɏ��s���܂����B
    cgGLSetOptimalOptions( dest.Profile );
	CheckForCgError( "It failed in making the vertex shader profile." ); //���_�V�F�[�_�[�v���t�@�C���̍쐬�Ɏ��s���܂����B

    //�t�@�C����ǂݍ���ŃR���p�C������
    dest.Program= cgCreateProgramFromFile(
                                    m_pMember->Cg_Cont ,  //�R���e�L�X�g
                                    CG_SOURCE ,           //Cg�\�[�X�R�[�h
                                    pFilePath ,           //�u.cg�t�@�C�����v
                                    dest.Profile ,        //�v���t�@�C��
                                    pFuncName ,           //�u.cg�t�@�C���v�ōŏ��ɂ����֐�
                                    NULL );               //NULL�ł悢
	CheckForCgError( "It failed in making the vertex shader." ); //���_�V�F�[�_�[�̍쐬�Ɏ��s���܂����B
}

CCgProgram &CCg::CreateFragmentShaderFromFile( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName )
{
    CreateFragmentShaderFromFileImple( dest , pFilePath , pFuncName );

    cgGLLoadProgram( dest.Program );
	CheckForCgError( "It failed in fragmentation shader making. " ); //�t���O�����g�V�F�[�_�[�̍쐬�Ɏ��s���܂����B

	return dest;
}

void CCg::CreateFragmentShaderFromFileImple( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName )
{
//  dest.Profile = cgGLGetLatestProfile( CG_GL_FRAGMENT );
    dest.Profile = CG_PROFILE_GLSLF; //GLSL�Ɠ����Ɏg�p���邽�߂ɂ̓v���t�@�C�������̒l�ɂ���
	CheckForCgError( "It failed in making the fragmentation shader profile." ); //�t���O�����g�V�F�[�_�[�v���t�@�C���̍쐬�Ɏ��s���܂����B
    cgGLSetOptimalOptions( dest.Profile );
	CheckForCgError( "It failed in making the fragmentation shader profile." );

    dest.Program = cgCreateProgramFromFile(
                    m_pMember->Cg_Cont,  //�R���e�L�X�g
                    CG_SOURCE,           //Cg�\�[�X�R�[�h
                    pFilePath,           //�u.cg�t�@�C�����v
                    dest.Profile,        //�v���t�@�C��
                    pFuncName,           //�u.cg�t�@�C���v�ōŏ��ɂ����֐�
                    NULL);               //NULL�ł悢
	CheckForCgError( "It failed in fragmentation shader making. " ); //�t���O�����g�V�F�[�_�[�̍쐬�Ɏ��s���܂����B
}

CCgCombineProgram &CCg::CreateVertexFragmentCombineShaderFromFile( CCgCombineProgram &dest , 
		                                            const skyMString *pFilePath1 , const skyMString *pFuncName1 ,
		                                            const skyMString *pFilePath2 , const skyMString *pFuncName2 )
{
	CCgProgram vertex;
	CCgProgram fragment;
    CGprogram programList[ 2 ];

	CreateVertexShaderFromFileImple( vertex , pFilePath1 , pFuncName1 );
	CreateFragmentShaderFromFileImple( fragment , pFilePath2 , pFuncName2 );

	programList[0] = vertex.Program;
	programList[1] = fragment.Program;
	dest.Program = cgCombinePrograms( 2 , programList );
    CCg::CheckForCgError( "It failed in shader uniting." ); //�V�F�[�_�[�̌����Ɏ��s���܂����B
    cgGLLoadProgram( dest.Program );
    CCg::CheckForCgError( "It failed in shader loading. " ); //�V�F�[�_�[�̃��[�h�Ɏ��s���܂����B

	dest.Profile1 = vertex.Profile;
	dest.Profile2 = fragment.Profile;

	return dest;
}


void CCg::CheckForCgError( const skyMString *situation )
{
	CGerror error;
	const skyMString *string = cgGetLastErrorString( &error );

	if ( error != CG_NO_ERROR )
	{
		SKY_PRINTF( "%s:%s \n" , situation , string );
		if ( error == CG_COMPILER_ERROR )
		{
			OutputDebugStringA( cgGetLastListing( m_pMember->Cg_Cont ) );
			SKY_PRINTF( "%s \n", cgGetLastListing( m_pMember->Cg_Cont ) );
		}
		SKY_PANIC();
	}
}

void CCg::DumpCompiledPrograms( CGprogram program )
{
	const int domains = cgGetNumProgramDomains( program );
	int i;

	SKY_PRINTF( "=================================\n" );
	for (i=0; i<domains; i++)
	{
		CGprogram subprog        = cgGetProgramDomainProgram( program , i );
//		const char *entry        = cgGetProgramString( subprog, CG_PROGRAM_ENTRY );
		const char *compiledText = cgGetProgramString( subprog, CG_COMPILED_PROGRAM );
//		const char *profile      = cgGetProgramString( subprog , CG_PROGRAM_PROFILE );

//		SKY_PRINTF( "ENTRY %s for PROFILE %s\n" , entry, profile ); �������������ė�����̂ŁA�z���g�Ɏg�������Ȃ鎞�܂ŕ���
		SKY_PRINTF( "-----------------------------------\n" );
		SKY_PRINTF( "%s\n", compiledText);
	}
}

} } }

#endif

#endif
