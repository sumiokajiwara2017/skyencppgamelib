#include "StdAfx.h"
#include "SkyBaseLib/Script.h"
#include "SkyBaseLib/Directory.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

using namespace squtil_const;

//-------------------------------------< ���� >---------------------------------------------//

static void SqDefaultPrintf( HSQUIRRELVM v , const SQChar* s , ... )
{
	v;

	//�o�b�t�@�m��
	skyString s_Buf[ SQ_PRINTF_MSG_LENGHT_WCHAR ];

	//��������o�b�t�@�Ɋi�[
	VA_LIST( s_Buf , SQ_PRINTF_MSG_LENGHT_WCHAR , s );

	SKY_PRINTF( s_Buf );
}

void CSqUtil::DefaultPrint( HSQUIRRELVM v , const skyString *s , ... )
{
	//�o�b�t�@�m��
	skyString s_Buf[ SQ_PRINTF_MSG_LENGHT_WCHAR ];

	//��������o�b�t�@�Ɋi�[
	VA_LIST( s_Buf , SQ_PRINTF_MSG_LENGHT_WCHAR , s );

	//�v�����g
	SqDefaultPrintf( v , s_Buf );
}

HSQUIRRELVM CSqUtil::CreateSQVM( u32 stackSize )
{
	//VM�쐬
	HSQUIRRELVM v; 
	v = sq_open( stackSize );

	//�ŏ��� sqstd_seterrorhandlers() �֐��͕W���̃G���[�n���h�������z�}�V���ɐݒ肷�閽�߂ł��B
	//�W���̂��̂��g�������Ȃ��ꍇ�͎������g�ŗp�ӂ��邱�Ƃ��\�ł��B
#ifdef SW_USE_SQ_STD_LIB
	sqstd_seterrorhandlers( v );
#endif

	//2�s�ڂ� sq_setprintfunc() �͉��z�}�V���Ɏ���̏o�͊֐����Z�b�g���閽�߂ł��B
	//��������W���� print ���߂����݂��Ă���̂ł�������g���Ă�OK�ł����A����ȊO�̏o�͂��������ꍇ�͂��̊֐��Őݒ肵�܂��B
	sq_setprintfunc( v , SqDefaultPrintf , SqDefaultPrintf );

	//���[�g�e�[�u���̐ݒ�
	sq_pushroottable( v );

	return v;
}

void CSqUtil::DestroySQVM( HSQUIRRELVM v )
{
	//VM�j��
	sq_close( v );
}

void CSqUtil::RegisterScriptFileAndExecuteFunc( HSQUIRRELVM v , const hash::CHashStringPtr &pFilePath , const hash::CHashStringPtr &pFuncName , SqFuncParam &param , SqFuncParam &outParam )
{
	//nut�t�@�C���̎��s
	RegisterScriptFile( v, pFilePath );

	//�֐��̎��s
	ExecuteFunc( v, pFuncName , param , outParam );
}

void CSqUtil::RegisterScriptFile( HSQUIRRELVM &v , const hash::CHashStringPtr &pFilePath )
{
	//�t�@�C�����݃`�F�b�N
	if ( !CPathUtil_::IsFileOn( pFilePath->GetString() ) )
	{
        SKY_PANIC_MSG( _T( "The file is not found. filePath=[%s]" ), pFilePath->GetString() );
	}

#ifdef SW_USE_SQ_STD_LIB
	//nut�t�@�C���̎��s
	s32 top = sq_gettop(v);						// �Ăяo���O�̃X�^�b�N�T�C�Y��ۑ�
	sq_pushroottable(v);
	if( !SQ_SUCCEEDED( sqstd_dofile( v , pFilePath->GetString() , 0 , 1 ) ) ) //�t�@�C���̎��s
	{
        SKY_PANIC_MSG( _T( "It failed in the execution of the file.fileName=[%s]" ), pFilePath->GetString() );
	}
	sq_settop( v , top ); // ���̃X�^�b�N�T�C�Y�ɖ߂�
#endif
}

void CSqUtil::ExecuteFunc( HSQUIRRELVM &v , const hash::CHashStringPtr &pFuncName , SqFuncParam &inParam , SqFuncParam &outParam )
{
	s32 top = sq_gettop(v);						// �Ăяo���O�̃X�^�b�N�T�C�Y��ۑ�
	sq_pushroottable(v);						// ���[�g�e�[�u����push
	sq_pushstring(v, pFuncName->GetString() , -1);			// �֐����X�^�b�N�ɏ悹��

	if (SQ_SUCCEEDED(sq_get( v , -2 ) ) ) {			// �֐������[�g�e�[�u������擾

		sq_pushroottable( v );					// "this"��push (���̗�ł̓��[�g�e�[�u����)

		CSqUtil_::RegisterStdLib( v );

		inParam.PushSquirrelStack( v );

		if ( !SQ_SUCCEEDED( sq_call( v , inParam.GetParamNum() + 1 , SQTrue , SQTrue) ) )	// �֐����Ă�(this��+1����j
		{
            SKY_PANIC_MSG( _T( "It failed in the execution of the file.It failed in the execution of the function.funcName=[%s]" ), pFuncName->GetString() );
		}
	}

	outParam.PopSquirrelStack( v );
	sq_settop( v , top ); // ���̃X�^�b�N�T�C�Y�ɖ߂�
}

HSQUIRRELVM CSqUtil::CreateNewThread( HSQUIRRELVM v , u32 stackSize )
{
	//�V�����X���b�h�̍쐬
	HSQUIRRELVM  new_v = sq_newthread( v , stackSize );				//��1�����Ƃ��ēn���ꂽVM�̃t�����hVM��V���ɍ쐬���A������X���b�h�I�u�W�F�N�g�Ƃ��ăX�^�b�N��push����B

	//���[�g�e�[�u���̐ݒ�
	sq_pushroottable( new_v );

	return new_v;
}

void CSqUtil::RegisterStdLib( HSQUIRRELVM v )
{

#ifdef SW_USE_SQ_STD_LIB
	if ( !SQ_SUCCEEDED( sqstd_register_iolib( v ) ) ) //I/O���C�u����
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_bloblib( v ) ) ) //blob���C�u����
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_stringlib( v ) ) ) //�����񃉃C�u����
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_systemlib( v ) ) ) //�V�X�e�����C�u����
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_mathlib( v ) ) ) //���w���C�u����
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
#endif
}

void CSqUtil::RegisterSkyBaseLibApi( HSQUIRRELVM v)
{
	SqDebugApi_::RegisterApi( v );
	SqExtensionApi_::RegisterApi( v );
}

void CSqUtil::RegisterSkyBaseLibClass( HSQUIRRELVM v)
{
	RegisterClass( v , GetSqInputManager() );
	RegisterClass( v , GetSqCDosUtil() );
}

void CSqUtil::RegisterNativeFunc( HSQUIRRELVM v , SQFUNCTION f ,  const skyString *fname )
{
	sq_pushroottable( v );
	sq_pushstring( v , fname , -1 );
	sq_newclosure( v , f , 0 );    // �V�����֐����쐬
	sq_createslot( v , -3 );
	sq_pop(v, 1);
}

void CSqUtil::RegisterScriptString( HSQUIRRELVM v , const hash::CHashStringPtr &pScript )
{
	sq_compilebuffer( v , pScript->GetString() , ( s32 )CStringUtil_::Len( pScript->GetString() ) * sizeof( SQChar ) , _T( "compile" ) , 1 );	//�X�N���v�g���R���p�C��
	sq_pushroottable( v );
	sq_call( v , 1 , 1 , 1 ); //���s
}

void CSqUtil::PushParam( HSQUIRRELVM v , const SqParamObject &param  )
{
	switch ( param.GetType() )
	{
		case eParamType_Int:
		{
			sq_pushinteger( v , param.m_Param.GetInt() );
			break;
		}
		case eParamType_Float:
		{
			sq_pushfloat( v , param.m_Param.GetFloat() );
			break;
		}
		case eParamType_String:
		{
			sq_pushstring( v , param.m_Param.GetString() , -1);
			break;
		}
		case eParamType_Bool:
		{
			sq_pushbool( v , param.m_Param.GetBool() );
			break;
		}
		case eParamType_Void:
		{
			if ( param.m_Param.GetVoid() != skyNull )
			{
				sq_pushuserpointer( v , param.m_Param.GetVoid() );
			}
			else
			{
				sq_pushnull( v );
			}
			break;
		}
		case eParamType_Array:
		{
			//�z����쐬
			sq_newarray( v , 0 );
			SqParamObjectList *pList = param.m_Param.GetArray();
			FOREACH( SqParamObjectList , it , *pList )
			{
				PushParam( v , ( *it ) ); //�ċA
				sq_arrayappend( v , -2 );
			}
			break;
		}
		case eParamType_Table:
		{
			SqParamObjectHash *pHash = param.m_Param.GetTable();

			//�e�[�u�����쐬
			sq_newtable( v );

			hash::CHashStringPtrList keyList;
			pHash->GetkeyList( keyList );
			FOREACH( hash::CHashStringPtrList , it , keyList )
			{
				sq_pushstring( v , ( *it )->GetString() , -1 );					//�L�[��ݒ�
				PushParam( v , ( * pHash )[ ( *it )->GetString() ] );	//�ċA
				sq_createslot( v , -3 );										//�L�[�ƒl�̃Z�b�g���e�[�u���ɐݒ�
			}
			break;
		}
		case eParamType_CppFunction:
		{
			SqCppFunc *pFunc = param.m_Param.GetCppFunc();

			sq_newtable( v );

			sq_pushstring( v , pFunc->m_fname->GetString() , -1 );
			sq_newclosure( v , pFunc->m_f , 0 );    // �V�����֐����쐬
			sq_setparamscheck( v , SQ_MATCHTYPEMASKSTRING , pFunc->m_typemask->GetString() );
			sq_setnativeclosurename( v , -1 , pFunc->m_fname->GetString() );
			sq_createslot( v , -3 );

			break;
		}
		case eParamType_Class:
		{
			SqClassObject *pClass = param.m_Param.GetClass();
			RegisterClass( v ,*pClass );

			//���[�g�e�[�u�����玩����T��
			sq_pushroottable( v );										//���[�g�e�[�u����ݒ�
			sq_pushstring( v , pClass->m_kName->GetString() , -1 );		//�N���X�̖��O�������L�[�ɂ���
			sq_get( v , -2 );											//�N���X�����i�������ʂ̓X�^�b�N�̈�ԏ�ɂ���
			HSQOBJECT cs;
			sq_getstackobj( v , -1 , &cs );								//�N���X�����o��
			sq_pop( v , 2 );											//�X�^�b�N����N���X�ƃ��[�g�e�[�u���𔲂�
			sq_pushobject( v , cs );									//�N���X��Push����
			sq_createinstance( v, -1 );									//�N���X�̃C���X�^���X���쐬����
			HSQOBJECT ins;
			sq_getstackobj( v , -1 , &ins );							//�C���X�^���X�����o��
			sq_addref( v , &ins );										//�C���X�^���X�̎Q�ƃJ�E���^���グ��i�������Ȃ��Ɖ���sq_pop�ŏ�����Ă��܂��j
			sq_pop( v , 2 );											//�X�^�b�N����C���X�^���X�ƃ��[�g�e�[�u���𔲂�
			sq_pushobject( v , ins );									//�C���X�^���X��Push����

			break;
		}

	}
}

void CSqUtil::GetParam( HSQUIRRELVM v , SqParamObject &param )
{
	tagSQObjectType rs = sq_gettype( v , -1 );
	switch( rs )
	{
		case OT_NULL:
			param = ( void * )skyNull;
#ifdef SW_DISPLAY_SQ_LOG
            CSqUtil_::DefaultPrint( v , _T( "skyNull returned.\n" ) );
#endif
			break;
		case OT_INTEGER:
			{
				SQInteger  ret;
				sq_getinteger( v , -1 , &ret );
				param = ( s32 )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQInteger returnd. ret=[ %d ]\n" ) , ret );
#endif
				break;
			}
		case OT_FLOAT:
			{
				SQFloat  ret;
				sq_getfloat( v , -1 , &ret );
				param = ( f32 )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQFloat returned. ret=[ %f ]\n" ) , ret );
#endif
				break;
			}
		case OT_STRING:
			{
				const SQChar *ret;
				sq_getstring( v , -1 , &ret );
				param = ( skyString * )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQChar returned. ret=[ %s ]\n" ) , ret );
#endif
				break;
			}
		case OT_BOOL:
			{
				SQBool  ret;
				sq_getbool( v , -1 , &ret );
				param = ( skyBool )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQBool returned. ret=[ %d ]\n" ) , ret );
#endif
				break;
			}
		case OT_USERPOINTER:
			{
				SQUserPointer  ret;
				sq_getuserpointer( v , -1 , &ret );
				param = ( void * )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQUserPointer returned. ret=[ %d ]\n" ) , ret );
#endif
				break;
			}
		case OT_ARRAY:
			{
				SqParamObjectList *ret = NEW__( SqParamObjectList , SqParamObjectList() );

				sq_pushnull(v);							// null�����q
				while( SQ_SUCCEEDED( sq_next( v , -2 ) ) )
				{
					//�ċA
					SqParamObject arrayp;
					GetParam( v , arrayp );

					ret->AddTail( arrayp );
    
					sq_pop(v, 1); // ���̔����̑O�ɃL�[�ƒl��pop����
				}
				sq_pop(v, 1); // null�����q��pop

				//�߂�l�Ƃ��ĕԂ�
				param = ( SqParamObjectList * )ret;

				break;
			}
		case OT_TABLE:
			{
				SqParamObjectHash *ret = NEW__( SqParamObjectHash , SqParamObjectHash( 31 ) );

				sq_pushnull(v);							// null�����q
				while( SQ_SUCCEEDED( sq_next( v , -2 ) ) )
				{
					//�ċA
					SqParamObject hashp;
					GetParam( v , hashp ); //�ċA

					//�l���X�^�b�N����POP����
					sq_pop( v , 1 ); //�l���|�b�v

					//�L�[�𕶎��Ƃ��Ď擾
					const SQChar *keySt;
					sq_getstring( v , -1 , &keySt );
					sq_pop( v , 1 ); //�L�[���|�b�v

					//Hash�Ɋi�[
					ret->Set( keySt , hashp );
#ifdef SW_DISPLAY_SQ_LOG
                    CSqUtil_::DefaultPrint( v , _T( "Type SQTable returnd. ret=[ %s ]\n" ) , keySt );
#endif
				}
				sq_pop(v, 1); // null�����q��pop

				param = ( SqParamObjectHash * )ret;
				break;
			}
		case OT_CLASS:
			{
				break;
			}
		case OT_NATIVECLOSURE:
			{
				break;
			}
		case OT_USERDATA:
		case OT_CLOSURE:
		case OT_GENERATOR:
		case OT_INSTANCE:
		case OT_WEAKREF:
		case OT_OUTER:
		case OT_THREAD:
		case OT_FUNCPROTO:
		default:
#ifdef SW_DISPLAY_SQ_LOG
            CSqUtil_::DefaultPrint( v, _T( "It is a value not supported.\n" ) );
#endif
//			SKY_PANIC_MSG( _T( "�T�|�[�g�O�̌^�̖߂�l���Ԃ�܂����B" ) );
			return; //Pop���Ȃ�
	}
}

void CSqUtil::RegisterClass( HSQUIRRELVM v , const SqClassObject &classObj )
{
	//�N���X����ݒ�
	sq_pushstring( v , classObj.m_kName->GetString() , -1 );

//---�����Őe���X�^�b�N�ɐςށ�

	if ( classObj.m_ParentName.IsEmpty() == skyFalse )
	{//���[�g�e�[�u������e��T��
		sq_pushroottable( v );													//���[�g�e�[�u����ݒ�
		sq_pushstring( v , classObj.m_ParentName->GetString() , -1 );		//�e�N���X�̖��O�������L�[�ɂ���
		if ( SQ_SUCCEEDED( sq_get( v , -2 ) ) )									//����
		{//������
			HSQOBJECT obj;
			sq_getstackobj( v , -1 , &obj );									//�e�N���X�����o��
			sq_pop( v , 2 );													//�X�^�b�N����e�N���X�ƃ��[�g�e�[�u���𔲂�
			sq_pushobject( v , obj );											//�e�N���X��Push����i�p���̏������ł����j
		}
		else
		{
			SKY_PANIC_MSG( _T( "Please set parent class (%s) to VM previously. " ) , classObj.m_ParentName->GetString() ); //�e�N���X%s���p������ɂ́A�e�N���X����VM�ɐݒ肵�Ă��������B
		}
	}

//---�����Őe���X�^�b�N�ɐςށ�

	//�N���X���쐬
	sq_newclass(v,  classObj.m_ParentName.IsEmpty() == skyFalse );

	//�����o�֐���ݒ肷��
	for ( u32 i = 0 ; i < classObj.m_CppMethodList.GetNum() ; i++ )
	{
		sq_pushstring( v , classObj.m_CppMethodList[ i ].m_fname->GetString() , -1 );
		sq_newclosure( v , classObj.m_CppMethodList[ i ].m_f , 0 );
		sq_setparamscheck( v , SQ_MATCHTYPEMASKSTRING , classObj.m_CppMethodList[ i ].m_typemask->GetString() );
		sq_setnativeclosurename( v , -1 , classObj.m_CppMethodList[ i ].m_fname->GetString() );
		sq_createslot( v , -3 );
	}

	//�����o�ϐ���ݒ肷��
	hash::CHashStringPtrList keyList;
	classObj.m_MemberHash.GetkeyList( keyList );
	FOREACH( hash::CHashStringPtrList , it , keyList )
	{
		sq_pushstring( v , ( *it )->GetString() , -1 );								//�L�[��ݒ�
		PushParam( v , ( classObj.m_MemberHash )[ ( *it )->GetString() ] );		//�ċA
		sq_createslot( v , -3 );													//�L�[�ƒl�̃Z�b�g���e�[�u���ɐݒ�
	}

	sq_createslot( v, -3 );

}
////////////////////////////////////////////////
//Sq�p���L�V�J���A�i���C�U�[�i�X�g���[���Łj //
///////////////////////////////////////////////
static u32 s_CompiledFileSize = 0;
SQInteger SqLex_Stream( SQUserPointer destStream )
{//�X�g���[���ŕ���������Ă���B

	stream::IStreamPtr *pspStream = ( stream::IStreamPtr * )destStream;

	SKY_ASSERT_MSG( ( *pspStream )->vIsOpen() , _T( "This stream has not been opened. " ) );

	//���̓X�g�[���[���̃T�C�Y���O�������ꍇ�G���[
	SKY_ASSERT_MSG( ( *pspStream )->vGetSize() > 0 , _T( "It tried to compile an empty file. " ) ); //��̃t�@�C�����R���p�C�����悤�Ƃ��܂����B
	SKY_ASSERT_MSG( ( *pspStream )->vGetSize() > 4 , _T( "The size of the file is too small. " ) ); //�ɒ[�ɒZ���t�@�C�����R���p�C�����悤�Ƃ��܂����B

	SQChar ret = 0;

	( *pspStream )->vReadStream( ( u8 * )&ret , sizeof( SQChar )  );

	//BOM�̏ꍇ�͓ǂݍ���ŕԂ��Ȃ�
	if ( ret == BOM )
	{
		( *pspStream )->vReadStream( ( u8 * )&ret , sizeof( SQChar )  );
	}

	return ret;
}

//////////////////////////////////////////////
//Sq�p�������݊֐��i�X�g���[���Łj          //
//////////////////////////////////////////////
SQInteger SqWrite_Stream( SQUserPointer destStream ,SQUserPointer pSrcData , SQInteger srcDataSize )
{
	stream::IStreamPtr *pspStream = ( stream::IStreamPtr * )destStream;

	SKY_ASSERT_MSG( ( *pspStream )->vIsOpen() , _T( "This stream has not been opened." ) );

	//�O�������炱�̏����͂O��Ԃ��B�O��Ԃ��΂����炭�I������H
	if ( srcDataSize == 0 ) return 0;

	return ( *pspStream )->vWriteStream( ( u8 * )pSrcData , ( u32 )srcDataSize );
}

////////////////////////////////////////////
//Sq�p�Ǎ��֐��i�X�g���[���Łj              //
////////////////////////////////////////////
SQInteger SqRead_Stream( SQUserPointer destStream ,SQUserPointer pSrcData , SQInteger srcDataSize )
{
	stream::IStreamPtr *pspStream = ( stream::IStreamPtr * )destStream;

	SKY_ASSERT_MSG( ( *pspStream )->vIsOpen() , _T( "This stream has not been opened." ) );

	return ( *pspStream )->vReadStream( ( u8 * )pSrcData , ( u32 )srcDataSize );
}


SqObjectInterfacePtr CSqUtil::Compile( HSQUIRRELVM v , const stream::IStreamPtr &pSrc )
{
	s32 top = sq_gettop(v);						// �Ăяo���O�̃X�^�b�N�T�C�Y��ۑ�

	if( SQ_FAILED( sq_compile( v, SqLex_Stream, ( SQUserPointer * )&pSrc, pSrc->vGetName(), SQFalse ) ) )
	{
		SKY_PANIC_MSG( _T( "It failed in the Sq compilation. " ) ); //It failed in the Sq compilation. 
	}

	HSQOBJECT po;
	sq_getstackobj( v , -1 , &po );

	sq_settop( v , top ); // ���̃X�^�b�N�T�C�Y�ɖ߂�

	return SqObjectInterfacePtr( NEW__( SqObjectInterface , SqObjectInterface( v , &po , skyTrue ) ) );
}

void CSqUtil::CompileOutStream( HSQUIRRELVM v , const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest )
{
	s32 top = sq_gettop(v);						// �Ăяo���O�̃X�^�b�N�T�C�Y��ۑ�

	if( SQ_FAILED( sq_compile( v, SqLex_Stream, ( SQUserPointer * )&pSrc, pSrc->vGetName(), SQFalse ) ) )
	{
		SKY_PANIC_MSG( _T( "It failed in the Sq compilation. " ) );
	}

	if( SQ_FAILED( sq_writeclosure ( v , SqWrite_Stream , ( SQUserPointer * )&pDest ) ) )
	{
		SKY_PANIC_MSG( _T( "It failed in the output of the file that had compiled Sq. " ) ); //Sq�R���p�C���ς݃t�@�C���̏o�͂Ɏ��s���܂����B
	}

	sq_settop( v , top ); // ���̃X�^�b�N�T�C�Y�ɖ߂�
}

void CSqUtil::RegisterNativeDebugHook( HSQUIRRELVM v, SQDEBUGHOOK debughook )
{
	sq_setnativedebughook( v, debughook );
}
void CSqUtil::RegisterDebugHook( HSQUIRRELVM v )
{
	sq_setdebughook( v );
}
void CSqUtil::SetDebug( HSQUIRRELVM v , skyBool isOn )
{
	sq_enabledebuginfo( v , isOn );
}

} } }

void *sq_vm_malloc( SQUnsignedInteger size )
{	
	return SkyMalloc__( size );
}

void *sq_vm_realloc( void *p, SQUnsignedInteger oldsize, SQUnsignedInteger size )
{ 
	SKY_UNUSED_ARG( oldsize );
	return  SkyRealloc__( p, size );
}

void sq_vm_free( void *p, SQUnsignedInteger size )
{ 
	SKY_UNUSED_ARG( size );
	SkyFree( p );
}

#endif


