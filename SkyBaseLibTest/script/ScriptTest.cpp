#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SCRIPT

#include "SkyBaseLib/Script.h"
#include "SkyBaseLib/ReLoad.h"

static const skyString *TEST1_NUTFILE_PATH = _T( "data/test/script/FunctionCall.nut" );

//�X�N���v�g����̊֐����b�{�{����Ă�
TEST( Script , 1 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::time::CTimerObject loadTime( _T( "�X�N���v�g�Ǎ�" ) );

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 24 );
	spSqVm->RegisterScriptFile( CHS( TEST1_NUTFILE_PATH ) );

	loadTime.~CTimerObject();

	{ sky::lib::time::CTimerObject callTime( _T( "script call." ) );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l
	inParam.SetParam( ( s32 )1 );
	inParam.SetParam( ( f32 )99.5f );
	inParam.SetParam( CHS( _T("go!" ) ) );

	sky::lib::squirrel::SqParamObjectList arrayParam;
	arrayParam.AddTail( _T( "aaa" ) );
	arrayParam.AddTail( _T( "bbb" ) );
	arrayParam.AddTail( _T( "ccc" ) );
	inParam.SetParam( &arrayParam );

	sky::lib::squirrel::SqParamObjectHash tableParam( 3 );
	tableParam.Set( _T( "key1" ) , _T("val1" ) );
	tableParam.Set( _T( "key2" ) , _T("val2" ) );
	tableParam.Set( _T( "key3" ) , _T("val3" ) );
	inParam.SetParam( &tableParam );

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	}

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST2_NUTFILE_PATH = _T( "data/test/script/CoroutineCall.nut" );

//C++��ŃX�N���v�g�̃R���[�`���𐶐����Ď��s����
TEST( Script , 2 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST2_NUTFILE_PATH ) );

	//�������쐬
	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l
	inParam.SetParam( (s32)5 );

	//�q�X���b�h�̍쐬
	sky::lib::squirrel::SqVmObjectPtr child[5];
	for ( u32 i = 0 ; i<5; i++ )
	{
		child[ i ] = spSqVm->CreateAndRegisterChildVm( 1024 , spSqVm );
	}

	//���s����
	for ( u32 i = 0 ; i<5; i++ )
	{
		child[ i ]->InitExecuteFuncASync();
	}

	//���s(�q�X���b�h��S�����s���A�S�����I��������I���j
	BOOL isExec = TRUE;
	while( isExec ) 
	{
		isExec = FALSE;
		for ( u32 i = 0 ; i<5; i++ )
		{
			if ( child[ i ]->ExecuteFuncASync( CHS( _T( "foo_thread" ) ), inParam , outParam ) != sky::lib::squirrel::SqVmObject::eThreadStatus_End )
			{
				isExec = TRUE; //��ł����s���I����Ă��Ȃ�������I���Ȃ�
			}
		}
	}

	//�q�X���b�h�̍폜�i�������j��I�j
	spSqVm->ChildAllPop();

	//�폜
	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

static const skyString *TEST3_NUTFILE_PATH = _T( "data/test/script/ClassBind.nut" );

//�X�N���v�g��œ��͎�t������i������X�N���v�g�ɂ�点��j
TEST( Script , 3 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST3_NUTFILE_PATH ) );
	spSqVm->InitExecuteFuncASync();

	//�N���X��o�^����
	spSqVm->RegisterClass( GetSqInputManager() );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "please push key 'A' \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//�I������
		{ sky::lib::time::CTimerObject callTime( _T( "script call" ) );

		if ( spSqVm->ExecuteFuncASync( CHS( _T( "foo_classbind" ) ), inParam , outParam ) == sky::lib::squirrel::SqVmObject::eThreadStatus_End )
		{
			GAME_LOOP_EXIT();
		}

		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

#endif

	SKY_BASE_LIB_TERM();
}

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//���s����Sq�X�N���v�g�i�t�@�C���ł͂Ȃ����ߍ��ݕ����ł���̂��|�C���g�j
static const skyString TEST4_Script[] =
_T( " function foo( i , f ) " ) //f�͈ꎞ�e�[�u���ł��邻���ɓo�^���ꂽ�l�C�e�B�u�N���[�W���[���ĂԁB�ق�Ƃ���f���W�F�l���[�^�ł������炢���̂�������Ȃ����B�B
_T( " { " )
_T( " print(\"i=\" + i + \" \\n\" );" )
_T( " cppfoo();" )
_T( " f.cppfoo2();" )
_T( " return \" ���� \" " )
_T( " } " )
_T( " print( \" RegisterScriptString���s���Ɏ��s�����̂��|�C���g \\n \" ); " );

//���s�֐�
SQInteger cppfoo(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "cppfoo���� \n" ) );
	return 0;
};

//���s�֐�
SQInteger cppfoo2(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "cppfoo2����2 \n" ) );
	return 0;
};

#endif

//�����񂩂�X�N���v�g�����[�h���Ď��s����
TEST( Script , 4 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	spSqVm->RegisterNativeFunc( cppfoo , _T( "cppfoo" ) );
	spSqVm->RegisterScriptString( CHS( TEST4_Script ) );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l
	inParam.SetParam( (s32)1 );
	sky::lib::squirrel::SqCppFunc func( cppfoo2 , CHS( _T( "cppfoo2" ) ) );
	inParam.SetParam( &func );

	{ sky::lib::time::CTimerObject callTime( _T( "script call." ) );

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	}

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();

}

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

SQInteger menberfunc0(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "memberfunc0���� \n" ) );
	return 0;
}

SQInteger menberfunc1(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "memberfunc1���� \n" ) );
	return 0;
}

#endif

static const skyString *TEST5_NUTFILE_PATH = _T( "data/test/script/ClassParam.nut" );

//�N���X���쐬���ēo�^���A���̂܂܎��s����B
TEST( Script , 5 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST5_NUTFILE_PATH ) );

	//�e�N���X�����
	sky::lib::squirrel::SqClassObject classobjBase( CHS( _T( "HogeBase" ) ) );					//HogeBase�Ƃ����e�N���X�����
	classobjBase.AddMethod( CHS( _T( "memberfunc0" ) ) , menberfunc0 );							//memberfunc0�Ƃ������\�b�h������

	//�e�N���X��o�^����
	spSqVm->RegisterClass( classobjBase );

	//�N���X�����
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "Hoge" ) ) , CHS( _T( "HogeBase" ) ) );	//Hoge�Ƃ����N���X�����iHogeBase���p������j
	classobj.AddMethod( CHS( _T( "memberfunc1" ) ) , menberfunc1 );									//memberfunc1�Ƃ������\�b�h������
	classobj.AddMember( CHS( _T( "member1" ) ) , sky::lib::squirrel::SqParamObject( (s32)1 ) );		//member1�Ƃ����ϐ�������

	//�N���X��o�^����
	spSqVm->RegisterClass( classobj );

	//�֐����Ăт���
	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l
	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST6_NUTFILE_PATH = _T( "data/test/script/ClassParam2.nut" );

//�֐��̃p�����[�^�[�ɃN���X�̃C���X�^���X���g�p����
TEST( Script , 6 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST6_NUTFILE_PATH ) );

	//�N���X�����
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "Hoge" ) ) , CHS( _T( "HogeBase" ) ) );	//Hoge�Ƃ����N���X�����iHogeBase���p������j
	classobj.AddMethod( CHS( _T( "memberfunc1" ) ) , menberfunc1 );									//memberfunc1�Ƃ������\�b�h������
	classobj.AddMember( CHS( _T( "member1" ) ) , sky::lib::squirrel::SqParamObject( (s32)1 ) );		//member1�Ƃ����ϐ�������

	//�֐����Ăт���
	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	//�N���X�������Ƃ��ēn��
	inParam.SetParam( &classobj );

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ) , inParam , outParam ); //�֐����s

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST7_NUTFILE_PATH_1 = _T( "data/test/script/CallFoo1.nut" );
static const skyString *TEST7_NUTFILE_PATH_2 = _T( "data/test/script/CallFoo2.nut" );


//foo�֐��̏����ꂽ�t�@�C�������[�h���A���s���A�Ⴄfoo�֐��������ꂽ�t�@�C�������[�h�����s����i���I�����[�h�̎����j
TEST( Script , 7 )
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	//�t�@�C���ǂ�
	spSqVm->RegisterScriptFile( CHS( TEST7_NUTFILE_PATH_1 ) );

	//�֐����Ăт���
	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l
	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	//�t�@�C���ǂ�
	spSqVm->RegisterScriptFile( CHS( TEST7_NUTFILE_PATH_2 ) );

	//�֐����Ăт���
	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s
	
	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

//nut�t�@�C�������[�h�N���X
class NutReLoad : public sky::lib::reload::AbsReLoadObject
{

public:

	sky::lib::squirrel::SqVmObjectPtr m_spVm;
	sky::lib::hash::CHashStringPtr	  m_filePath;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	NutReLoad( sky::lib::squirrel::SqVmObjectPtr &spVm , const sky::lib::hash::CHashStringPtr &filePath )
	{
		m_spVm = spVm;
		m_filePath = filePath;
		AddFilePath( filePath );
	}

	/// <summary>
	/// ���[�h�֐�
	/// </summary>
	virtual void Load()
	{
		m_spVm->RegisterScriptFile( CHS( m_filePath->GetString() ) );
	}

};

static const skyString *TEST8_NUTFILE_PATH = _T( "data/test/script/ReLoad.nut" );

//�X�N���v�g�����s���ɏ���������B
TEST( Script , 8 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST8_NUTFILE_PATH ) );

	//�����[�h�I�u�W�F�N�g�̍쐬
	sky::lib::reload::IReLoadObjectPtr nutReLoad( NEW NutReLoad( spSqVm , CHS( TEST8_NUTFILE_PATH ) ) );

	CReLoadObjectManager_::Add( nutReLoad );

	//�N���X��o�^����
	spSqVm->RegisterClass( GetSqInputManager() );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "please push key 'A' \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//�����[�h�I�u�W�F�N�g�X�V
		CReLoadObjectManager_::Update( fpsCtrl.GetStartTime() );

		//�I������
		{ sky::lib::time::CTimerObject callTime( _T( "script call" ) );

		//���s
		spSqVm->ExecuteFuncSync( CHS( _T( "foo_ReLoad" ) ), inParam , outParam );

		if ( ( skyBool )outParam[ 0 ] != skyFalse )
		{
			GAME_LOOP_EXIT();
		}

		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST9_NUTFILE_PATH_1 = _T( "data/test/script/ReLoadMain.nut" );
static const skyString *TEST9_NUTFILE_PATH_2 = _T( "data/test/script/ReLoadFunc.nut" );


//�Q��nut�t�@�C����ǂށB��͐��䂪������Ă��āA���t���[�����f������A�����ЂƂ͏�����������Ă��āA�t�@�C���������Ȃ����ƃ����[�h����
TEST( Script , 9 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST9_NUTFILE_PATH_1 ) );
	spSqVm->RegisterScriptFile( CHS( TEST9_NUTFILE_PATH_2 ) );
	spSqVm->InitExecuteFuncASync();

	//�����[�h�I�u�W�F�N�g�̍쐬
	sky::lib::reload::IReLoadObjectPtr nutReLoad( NEW NutReLoad( spSqVm , CHS( TEST9_NUTFILE_PATH_2 ) ) );

	CReLoadObjectManager_::Add( nutReLoad );

	//�N���X��o�^����
	spSqVm->RegisterClass( GetSqInputManager() );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "please push key 'A' \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//�����[�h�I�u�W�F�N�g�X�V
		CReLoadObjectManager_::Update( fpsCtrl.GetStartTime() );

		//�I������
		{ sky::lib::time::CTimerObject callTime( _T( "script call." ) );

		if ( spSqVm->ExecuteFuncASync( CHS( _T( "foo_ReLoadMain" ) ), inParam , outParam ) == sky::lib::squirrel::SqVmObject::eThreadStatus_End )
		{
			GAME_LOOP_EXIT();
		}

		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}


static const skyString *TEST10_COMPILE_FILE_SRC_PATH = _T( "data/test/script/CompileSrc.nut" );
static const skyString *TEST10_COMPILE_FILE_DEST_PATH = _T( "data/test/script/CompileDest.nut" );

//�X�N���v�g���R���p�C�����ăt�@�C���ɏo�͂���B�����Ă��̃t�@�C�������s����
TEST( Script , 10 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	//�t�@�C����ǂ�
	sky::lib::stream::IStreamPtr spSrcStream( NEW sky::lib::file::CDiscFileStream( TEST10_COMPILE_FILE_SRC_PATH , sky::lib::file::eOpenMode_Binary_R_NoCreate ) );
	sky::lib::stream::IStreamPtr spDestStream( NEW sky::lib::file::CDiscFileStream( TEST10_COMPILE_FILE_DEST_PATH , sky::lib::file::eOpenMode_Binary_RW_Create ) );

	//�X�g���[�����J��
	spSrcStream->vOpen();
	spDestStream->vOpen();

	//�R���p�C�����Ă݂�B
	spSqVm->CompileOutStream( spSrcStream , spDestStream );

	//�X�g���[�������
	spSrcStream->vClose();
	spDestStream->vClose();

	//�t�@�C�������[�h����B
	spSqVm->RegisterScriptFile( CHS( TEST10_COMPILE_FILE_SRC_PATH ) );
//	spSqVm->RegisterScriptFile( CHS( TEST10_COMPILE_FILE_DEST_PATH ) );

	//�R���p�C����̃t�@�C�������s
	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST11_CALLBACK_TEST_SQ_PATH = _T( "data/test/script/OneLineCallBackTest.nut" );

void CallBackFunc( HSQUIRRELVM v , SQInteger type , const SQChar * sourcename , SQInteger line , const SQChar * funcname )
{
	v;
	SKY_PRINTF( _T( "type=%d , sourcename=%s , line=%d , funcname=%s \n" ) , type , sourcename , line , funcname );
}

//���s�P�s���ƂɃR�[���o�b�N���󂯂�
TEST( Script , 11 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	spSqVm->RegisterNativeDebugHook( CallBackFunc );
	spSqVm->SetDebug( skyTrue ); //��������Ȃ��ƍs���Ƃ̃R�[���o�b�N�����Ȃ��i���m�ɂ͎����ŕt�����ꂽmain�֐��̂��̂�������j

	//�t�@�C�������[�h����B
	spSqVm->RegisterScriptFile( CHS( TEST11_CALLBACK_TEST_SQ_PATH ) );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST12_DEBUG_API_TEST_SCRIPT_FILE_PATH = _T( "data/test/script/DebugApiTest.nut" );

//�f�o�b�O�`�o�h�����s����
TEST( Script , 12 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	SqDebugApi_::RegisterApi(spSqVm->Vm());

	//�t�@�C�������[�h����B
	spSqVm->RegisterScriptFile( CHS( TEST12_DEBUG_API_TEST_SCRIPT_FILE_PATH ) );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST13_INCLUDE_TEST = _T( "data/test/script/IncludeTest.nut" );

//�X�N���v�g��include����������
TEST( Script , 13 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	//SkyLibBase��Api��o�^
	CSqUtil_::RegisterSkyBaseLibApi( spSqVm->Vm() );

	//�t�@�C�������[�h����B
	spSqVm->RegisterScriptFile( CHS( TEST13_INCLUDE_TEST ) );

	sky::lib::squirrel::SqFuncParam inParam;		//����
	sky::lib::squirrel::SqFuncParam outParam;		//�߂�l

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //�֐����s

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

//�X�N���v�g����̖߂�l���X�N���v�g�̊֐��̈����Ɏg�p����
TEST( Script , 14 ) 
{
}


#endif