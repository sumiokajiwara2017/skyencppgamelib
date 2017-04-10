#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

const SqParamObject	&SqFuncParam::operator[]( u32 idx ) const
{
	SKY_ASSERT_MSG( m_ParamNum <= idx, _T( "It is an illegal index. " ) ); //�s���ȃC���f�b�N�X�ł��B

	return m_Param[ idx ];
}

SqFuncParam::SqFuncParam() :
m_ParamNum( 0 )
{
}

SqFuncParam::~SqFuncParam()
{
	for ( u32 i = 0 ; i<m_ParamNum; i++ )
	{
		if ( m_Param[ m_ParamNum ].GetType() == eParamType_Array || 
			 m_Param[ m_ParamNum ].GetType() == eParamType_Table || 
			 m_Param[ m_ParamNum ].GetType() == eParamType_CppFunction || 
			 m_Param[ m_ParamNum ].GetType() == eParamType_Class  )
		{
			if ( m_Param[ m_ParamNum ].GetVoidPointer() != NULL )
			{
				DEL m_Param[ m_ParamNum ].GetVoidPointer();
				m_Param[ m_ParamNum ].SetNull();
			}
		}
	}
}

void SqFuncParam::SetParam( s32	param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) ); //The parameter cannot be set any further. 

	m_Param[ m_ParamNum ].SetParam( ( s32 )param );

	m_ParamNum++;
}

void SqFuncParam::SetParam( f32	param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	m_Param[ m_ParamNum ].SetParam( ( f32 )param );

	m_ParamNum++;
}

void SqFuncParam::SetParam( const hash::CHashStringPtr &param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	m_Param[ m_ParamNum ].SetParam( param->GetString() );

	m_ParamNum++;
}

void SqFuncParam::SetParam( skyBool		 param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	m_Param[ m_ParamNum ].SetParam( ( skyBool )param );

	m_ParamNum++;
}

void SqFuncParam::SetParam( void *param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	m_Param[ m_ParamNum ].SetParam( ( void * )param );

	m_ParamNum++;
}

void SqFuncParam::SetParam( const SqParamObjectList *param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	//�����ł킴�Ɛ������āA�����ł̓q�[�v�̒l���g�p����ɂ悤�ɂ���B�����ăf�X�g���N�^�Ŋm���ɏ���
	SqParamObjectList *paramHeap = NEW__( SqParamObjectList , SqParamObjectList() );

	for ( u32 i = 0 ; i < param->GetNum() ; i++ )
	{
		paramHeap->AddTail( ( *param )[ i ] );
	}

	m_Param[ m_ParamNum ].SetParam( ( SqParamObjectList * )paramHeap );

	m_ParamNum++;
}

void SqFuncParam::SetParam( const SqParamObjectHash *param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	//�����ł킴�Ɛ������āA�����ł̓q�[�v�̒l���g�p����ɂ悤�ɂ���B�����ăf�X�g���N�^�Ŋm���ɏ���
	SqParamObjectHash *paramHeap = NEW__( SqParamObjectHash , SqParamObjectHash( param->GetHashSize() ) );

	//��������l���R�s�[����
	hash::CHashStringPtrList keyList;
	param->GetkeyList( keyList );
	FOREACH( hash::CHashStringPtrList , it , keyList )
	{
		paramHeap->Set( ( *it )->GetString() , ( * param )[ ( *it )->GetString() ] );
	}

	m_Param[ m_ParamNum ].SetParam( ( SqParamObjectHash * )paramHeap );

	m_ParamNum++;
}

void SqFuncParam::SetParam( const SqClassObject *param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	//�����ł킴�Ɛ������āA�����ł̓q�[�v�̒l���g�p����ɂ悤�ɂ���B�����ăf�X�g���N�^�Ŋm���ɏ���
	SqClassObject *paramClass = NEW__( SqClassObject , SqClassObject() );

	//�l���R�s�[
	*paramClass = *param;

	m_Param[ m_ParamNum ].SetParam( ( SqClassObject * )paramClass );

	m_ParamNum++;
}

void SqFuncParam::SetParam( const SqCppFunc *param )
{
	SKY_ASSERT_MSG( m_ParamNum < sq_const::FUNC_PARAM_MAX_NUM, _T( "The parameter cannot be set any further. " ) );

	//�����ł킴�Ɛ������āA�����ł̓q�[�v�̒l���g�p����ɂ悤�ɂ���B�����ăf�X�g���N�^�Ŋm���ɏ���
	SqCppFunc *paramFunc = NEW__( SqCppFunc , SqCppFunc( param->m_f , param->m_fname ) );

	m_Param[ m_ParamNum ].SetParam( ( SqCppFunc * )paramFunc );

	m_ParamNum++;
}

void SqFuncParam::PushSquirrelStack( HSQUIRRELVM v )
{
	for ( u32 i = 0 ; i<m_ParamNum; i++ )
	{
		m_Param[ i ].PushSquirrelStack( v );
	}
}

void SqFuncParam::PopSquirrelStack( HSQUIRRELVM v )
{
	//�߂�l�͍ŏ��̂P���������A���Ă��Ȃ��iSquirrel�͖߂�l�͈�����B�ł��c���[�\���Ȃ̂Ŗ߂��鐔�͖�����B
	m_Param[ 0 ].PopSquirrelStack( v );
}

SqFuncParam& SqFuncParam::operator =( const SqFuncParam *pIn )
{
	for ( u32 i = 0 ; i < sq_const::FUNC_PARAM_MAX_NUM; i++ )
	{
		m_Param[ i ] = pIn->m_Param[ i ];
	}

	return *this;
}

skyBool SqFuncParam::GetParam( u32 index , s32 &param )
{
	param = INT_MAX;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_Int )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetInt();

	//�擾����
	return skyTrue;

}
skyBool SqFuncParam::GetParam( u32 index , f32 &param )
{
	param = FLT_MAX;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_Float )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetFloat();

	//�擾����
	return skyTrue;
}
skyBool SqFuncParam::GetParam( u32 index , hash::CHashStringPtr &param )
{
	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_String )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = CHS( m_Param[ index ].GetParam().GetString() );

	//�擾����
	return skyTrue;
}
skyBool SqFuncParam::GetParam( u32 index , skyBool &param )
{
	param = skyFalse;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_Bool )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetBool();

	//�擾����
	return skyTrue;
}
skyBool SqFuncParam::GetParam( u32 index , void* &param )
{
	param = skyNull;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_Void )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetVoid();

	//�擾����
	return skyTrue;
}

skyBool SqFuncParam::GetParam( u32 index , const SqParamObjectList* &param )
{
	param = skyNull;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_Array )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetArray();

	//�擾����
	return skyTrue;
}

skyBool SqFuncParam::GetParam( u32 index , const SqParamObjectHash* &param )
{
	param = skyNull;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_Table )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetTable();

	//�擾����
	return skyTrue;
}

skyBool SqFuncParam::GetParam( u32 index , const SqCppFunc* &param )
{
	param = skyNull;

	//�p�����[�^�[�����݂��Ȃ�������false
	if ( m_ParamNum == 0 ) return skyFalse;

	//�p�����[�^�[���w��̃C���f�b�N�X�ȉ��̏ꍇ��false
	if ( m_ParamNum <= index ) return skyFalse;

	//�p�����[�^�[�̌^���������false
	if ( m_Param[ index ].GetType() != eParamType_CppFunction )
	{
		return skyFalse;
	}

	//�l�̎擾
	param = m_Param[ index ].GetParam().GetCppFunc();

	//�擾����
	return skyTrue;
}

//---------------------SqParamObject---------------------��

SqParamObject::SqParamObject()
{
}

SqParamObject::SqParamObject( s32 param )
{
	SetParam( ( s32 )param );
}

SqParamObject::SqParamObject( f32 param )
{
	SetParam( ( f32 )param );
}

SqParamObject::SqParamObject( const skyString *	param )
{
	SetParam( param );
}

SqParamObject::SqParamObject( skyBool param )
{
	SetParam( ( skyBool )param );
}

SqParamObject::SqParamObject( void	*param )
{
	SetParam( ( void* )param );
}

SqParamObject::SqParamObject( SqParamObjectList	*param )
{
	SetParam( ( SqParamObjectList* )param );
}

SqParamObject::SqParamObject( SqParamObjectHash	*param )
{
	SetParam( ( SqParamObjectHash* )param );
}

SqParamObject::SqParamObject( SqCppFunc	*param )
{
	SetParam( ( SqCppFunc* )param );
}

SqParamObject::SqParamObject( SqClassObject	*param )
{
	SetParam( ( SqClassObject* )param );
}

void SqParamObject::SetParam( s32 param )
{
	m_eParamType = eParamType_Int;
	m_Param.SetInt( param );
}

void SqParamObject::SetParam( f32 param )
{
	m_eParamType=eParamType_Float;
	m_Param.SetFloat( param );
}

void SqParamObject::SetParam( const skyString * param )
{
	m_eParamType=eParamType_String;
	m_Param.SetString( param );
}

void SqParamObject::SetParam( skyBool param )
{
	m_eParamType=eParamType_Bool;
	m_Param.SetBool( param );
}

void SqParamObject::SetParam( void *param )
{
	m_eParamType=eParamType_Void;
	m_Param.SetVoid( param );
}

void SqParamObject::SetParam( SqParamObjectList *param )
{
	m_eParamType=eParamType_Array;
	m_Param.SetArray( param );
}


void SqParamObject::SetParam( SqParamObjectHash *param )
{
	m_eParamType=eParamType_Table;
	m_Param.SetTable( param );
}

void SqParamObject::SetParam( SqClassObject *param )
{
	m_eParamType=eParamType_Class;
	m_Param.RegisterClass( param );
}

void SqParamObject::SetParam( SqCppFunc *param )
{
	m_eParamType=eParamType_CppFunction;
	m_Param.SetCppFunc( param );
}

SqParamObject::~SqParamObject()
{
}

void SqParamObject::PushSquirrelStack( HSQUIRRELVM v )
{
	CSqUtil_::PushParam( v , *this  );
}


void SqParamObject::PopSquirrelStack( HSQUIRRELVM v )
{
	//�ċA
	CSqUtil_::GetParam( v , *this  );

	//�߂�l�������B�܂����őS�������񂾂��E�E
	sq_pop( v , 1 ); 
}

SqParamObject& SqParamObject::operator =( s32 val )
{
	SetParam( val );
	return *this;
}
SqParamObject& SqParamObject::operator =( f32 val )
{
	SetParam( val );
	return *this;
}
SqParamObject& SqParamObject::operator =( const skyString * pVal )
{
	SetParam( ( hash::CHashString  * )pVal );
	return *this;
}
SqParamObject& SqParamObject::operator =( skyBool val )
{
	SetParam( val );
	return *this;
}
SqParamObject& SqParamObject::operator =( void *pVal )
{
	SetParam( pVal );
	return *this;
}
SqParamObject& SqParamObject::operator =( SqParamObjectList *pVal )
{
	SetParam( pVal );
	return *this;
}
SqParamObject& SqParamObject::operator =( SqParamObjectHash *pVal )
{
	SetParam( pVal );
	return *this;
}
SqParamObject& SqParamObject::operator =( SqCppFunc *pVal )
{
	SetParam( pVal );
	return *this;
}

SqParamObject::operator s32				() const
{
	return m_Param.GetInt();
}
SqParamObject::operator f32				() const
{
	return m_Param.GetFloat();
}
SqParamObject::operator const skyString *			() const
{
	return m_Param.GetString();
}
SqParamObject::operator skyBool			() const
{
	return m_Param.GetBool();
}
SqParamObject::operator void				*() const
{
	return m_Param.GetVoid();
}
SqParamObject::operator const SqParamObjectList	*() const
{
	return m_Param.GetArray();
}
SqParamObject::operator const SqParamObjectHash	*() const
{
	return m_Param.GetTable();
}
SqParamObject::operator const SqCppFunc	*() const
{
	return m_Param.GetCppFunc();
}


} } }

#endif