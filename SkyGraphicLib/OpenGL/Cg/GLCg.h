#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct CCgMember;

/// <summary>
/// Cg�v���O�������
/// </summary>
struct CCgProgram
{
    CGprogram Program;
    CGprofile Profile;
};

/// <summary>
/// Cg�����v���O�������
/// </summary>
struct CCgCombineProgram
{
    CGprogram Program;
    CGprofile Profile1;
    CGprofile Profile2;
};

/// <summary>
/// GLSL�N���X
/// </summary>
class CCg : base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize	();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose		();

	/// <summary>
	/// �R���e�L�X�g�̎擾
	/// </summary>
	static CGcontext GetContext	();

	/// <summary>
	/// ���_�V�F�[�_�[�̓ǂݍ���
	/// </summary>
	static CCgProgram &CreateVertexShaderFromFile( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );
	static void        CreateVertexShaderFromFileImple( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );

	/// <summary>
	/// �t���O�����g�V�F�[�_�[�̓ǂݍ���
	/// </summary>
	static CCgProgram &CreateFragmentShaderFromFile( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );
	static void        CreateFragmentShaderFromFileImple( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );

	/// <summary>
	/// �����v���O�������쐬����
	//�@������
	//�@���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�\�����������ꍇ�A���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�\�œ����O���[�o���ϐ����`�����
	//�@�s�N�Z���V�F�[�_�[���̃O���[�o���ϐ��ɒl������܂���B�܂�A���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�[�̂��ꂼ��̃t�@�C�����œ�����
	//�@�O���[�o���ϐ����`���Ă͂����܂���B
	/// </summary>
	static CCgCombineProgram &CreateVertexFragmentCombineShaderFromFile( CCgCombineProgram &dest , 
		                                            const skyMString *pFilePath1 , const skyMString *pFuncName1 ,
		                                            const skyMString *pFilePath2 , const skyMString *pFuncName2 );

	/// <summary>
	/// �G���[�`�F�b�N
	/// </summary>
    static void CheckForCgError( const skyMString *situation );

	/// <summary>
	/// �V�F�[�_�[�̓��e���_���v
	/// </summary>
    static void DumpCompiledPrograms( CGprogram program );

private:

	/// <summary>
	/// �����o�ϐ��Q
	/// </summary>
	static CCgMember *m_pMember;
};

} } } 

#define CCgInitialize_()	sky::lib::graphic::CGraphicLibController::Initialize()
#define CCg_				sky::lib::graphic::CGraphicLibController
#define CCgDispose_()		sky::lib::graphic::CGraphicLibController::Dispose()

#endif

#endif
