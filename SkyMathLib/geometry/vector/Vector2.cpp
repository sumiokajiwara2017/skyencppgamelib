#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"
#include "SkyMathLib/Script.h"

//---------------------------Vector2------------------------------��

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

/// <summary>
/// �f�X�g���N�^�i�R���X�g���N�^����ɏ����͈̂Ӗ�������B
/// </summary>
static SQInteger	destructor( SQUserPointer p, SQInteger size )
{
	size;
	DEL ( sky::lib::math::CBasicVector2 * )p;
	return 1;
}

/// <summary>
/// �R���X�g���N�^(constructor�Ƃ������O�ɂ͈Ӗ�������j
/// </summary>
static SQInteger	constructor( HSQUIRRELVM v )
{
	//�C���X�^���X�𐶐�
	sky::lib::math::CBasicVector2 *pObj = NEW__( sky::lib::math::CBasicVector2 , sky::lib::math::CBasicVector2() ) ;

	//�C���X�^���X��߂�l��
	sq_setinstanceup( v , 1 , (SQUserPointer * )pObj );

	//�f�X�g���N�^�̓o�^
	sq_setreleasehook( v , 1 , destructor );

	return 1; //�߂�l1
}

/// <summary>
/// �l��Setter�i_set�Ƃ������O�ɂ͈Ӗ�������j
/// </summary>
static SQInteger	_set( HSQUIRRELVM v )
{
	//����x�̎󂯎��
	sky::lib::math::CBasicVector2 *pObj; sq_getinstanceup( v , 1 , ( SQUserPointer * )&pObj , 0);

	//����s�̎󂯎��
	const SQChar* key; sq_getstring( v , 2 , &key );

	//�����ɉ������������s��
	if ( CStringUtil_::Comp( key , _T( "X" ) ) )
	{
		SQFloat val;
		sq_getfloat( v , 3 , &val );			//����.�̎󂯎��
		pObj->X = val;							//����.�̑��
	}
	else if ( CStringUtil_::Comp( key , _T( "Y" ) ) )
	{
		SQFloat val; 
		sq_getfloat( v , 3 , &val );			//����.�̎󂯎��
		pObj->Y = val;							//����.�̑��
	}
	else
	{
		return sq_throwerror( v , _T( "There is no member variable of this name in this class. " ) ); //���̃N���X�ɂ͂���ȃ����o�ϐ��͋��܂���B
	}
	return 0; //�߂�l0
}

/// <summary>
/// �l��Getter�i_get�Ƃ������O�ɂ͈Ӗ�������j
/// </summary>
static SQInteger	_get( HSQUIRRELVM v )
{
	//����x���󂯎��
	sky::lib::math::CBasicVector2 *pObj; sq_getinstanceup( v , 1 , ( SQUserPointer * )&pObj , 0 );

	//����s���󂯎��
	const SQChar* key; sq_getstring( v , 2 , &key );

	//�����ɉ����ď������s��
	if ( CStringUtil_::Comp( key , _T( "X" ) ) )
	{
		sq_pushfloat( v , ( f32 )pObj->X );
	}
	else if ( CStringUtil_::Comp( key , _T( "Y" ) ) )
	{
		sq_pushfloat( v , ( f32 )pObj->Y );
	}
	else
	{
		return sq_throwerror( v , _T( "There is no member variable of this name in this class." ) ); //���̃N���X�ɂ͂���ȃ����o�ϐ��͋��܂���B
	}
	return 1;  //�߂�l1
}

sky::lib::squirrel::SqClassObject GetSqVector2() //�R�s�[�R���X�g���N�^���Ă΂��̂Ŗ��t���[���͌Ă΂Ȃ��ł��������B�����炭���̎��s�Ő��񂵂��Ă΂Ȃ��͂��ł��B
{
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "CVector2" ) ) );						
	classobj.AddMethod( CHS( _T( "constructor" ) ) , constructor , CHS( _T( "" ) ) );			
	classobj.AddMethod( CHS( _T( "_set" ) ) , _set , CHS( _T( "xs." ) ) );				
	classobj.AddMethod( CHS( _T( "_get" ) ) , _get , CHS( _T( "xs" ) ) );

	return classobj;
}

#endif