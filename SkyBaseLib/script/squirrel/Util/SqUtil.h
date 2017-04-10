#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// �萔
/// </summary>
namespace squtil_const
{
	static const u32 SQ_PRINTF_MSG_LENGHT_WCHAR = 256;
	static const u32 SQ_PRINTF_MSG_LENGHT_SCHAR = SQ_PRINTF_MSG_LENGHT_WCHAR * sizeof( skyString );
	static const u32 STACK_SIZE			= 1024;
	static const u32 RET_STRING_LENGTH	= 512;
}

//-------------------------------------< ���� >---------------------------------------------//

class CSqUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �f�t�H���g�r�p�o��
	/// </summary>
	static void DefaultPrint( HSQUIRRELVM v , const skyString *s , ... );

	/// <summary>
	/// VM���쐬����
	/// </summary>
	static HSQUIRRELVM CreateSQVM( u32 stackSize );

	/// <summary>
	/// VM��j������
	/// </summary>
	static void DestroySQVM( HSQUIRRELVM v );

	/// <summary>
	/// �t�@�C����ǂݍ��ݍŌ�܂Ŏ��s����i�����j
	/// </summary>
	static void RegisterScriptFileAndExecuteFunc( HSQUIRRELVM v , const hash::CHashStringPtr &pFilePath , const hash::CHashStringPtr &pFuncName , SqFuncParam &param , SqFuncParam &outParam ); //�֐����w��ł���

	/// <summary>
	/// �R���[�`�����쐬����i�R���[�`���͐e�̃X�^�b�N�Ƀv�b�V������Ă���j
	/// </summary>
	static HSQUIRRELVM CreateNewThread( HSQUIRRELVM v , u32 stackSize );

	/// <summary>
	/// �t�@�C����ǂݍ���
	/// </summary>
	static void RegisterScriptFile( HSQUIRRELVM &v , const hash::CHashStringPtr &pFilePath );

	/// <summary>
	/// �t�@�C���Ǎ��ς݂�VM���󂯎��֐������s����
	/// </summary>
	static void ExecuteFunc( HSQUIRRELVM &v , const hash::CHashStringPtr &pFuncName , SqFuncParam &inParam , SqFuncParam &outParam );

	/// <summary>
	/// Squirrel�W���֐���Push�isq_pushroottable���X�^�b�N��Top�ɐݒ肵������i�������͓Ǝ��̃e�[�u�����j��Call���ĉ������j
	/// </summary>
	static void RegisterStdLib( HSQUIRRELVM v);

	/// <summary>
	/// SkyBaseLib��SquirrelApi��Vm�ɓo�^
	/// </summary>
	static void RegisterSkyBaseLibApi( HSQUIRRELVM v);

	/// <summary>
	/// SkyBaseLib��Squirrel�o�C���h�N���X��Vm�ɓo�^
	/// </summary>
	static void RegisterSkyBaseLibClass( HSQUIRRELVM v);

	/// <summary>
	/// �֐��̓o�^
	/// </summary>
	static void RegisterNativeFunc( HSQUIRRELVM v , SQFUNCTION f , const skyString *fname );

	/// <summary>
	/// �X�N���v�g�̎��s
	/// </summary>
	static void RegisterScriptString( HSQUIRRELVM v , const hash::CHashStringPtr &pScript );

	/// <summary>
	/// �N���X�̓o�^
	/// </summary>
	static void RegisterClass( HSQUIRRELVM v , const SqClassObject &classObj );

	/// <summary>
	/// �p�����[�^�[�̐ݒ�
	/// </summary>
	static void PushParam( HSQUIRRELVM v , const SqParamObject &param  );

	/// <summary>
	/// �p�����[�^�[�̎擾
	/// </summary>
	static void GetParam( HSQUIRRELVM v , SqParamObject &param );

	/// <summary>
	/// �w���nut�X�g���[�����R���p�C�����Č��ʂ��I�u�W�F�N�g�ŕԂ�
	/// </summary>
	static SqObjectInterfacePtr Compile( HSQUIRRELVM v , const stream::IStreamPtr &pSrc );

	/// <summary>
	/// �w���nut�X�g���[�����R���p�C�����Č��ʂ��X�g���[���ɏo�͂���
	/// </summary>
	static void CompileOutStream( HSQUIRRELVM v , const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest );

	/// <summary>
	/// �P�s���ƂɃR�[���o�b�N��Ԃ��f�o�b�O�@�\���n�m�^�n�e�e����
	/// </summary>
	static void RegisterNativeDebugHook( HSQUIRRELVM v, SQDEBUGHOOK debughook );
	static void RegisterDebugHook( HSQUIRRELVM v );
	static void SetDebug( HSQUIRRELVM v , skyBool isOn );

private:

	//�e�@�\����
	CSqUtil				();										//�R���X�g���N�^
	CSqUtil				( const CSqUtil& ){};					//�R�s�[�R���X�g���N�^
	virtual ~CSqUtil	();										//�f�X�g���N�^
	CSqUtil &operator=	(const CSqUtil& ){ return *this; };		//������Z�q�I�[�o�[���[�h
};

} } }

//�C���X�^���X�A�N�Z�X������
#define CSqUtil_			sky::lib::squirrel::CSqUtil

#endif

//�o�C���h�N���X��`���擾
extern sky::lib::squirrel::SqClassObject GetSqInputManager();
extern sky::lib::squirrel::SqClassObject GetSqCDosUtil();
