#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"
#include "SkyMathLib/Script.h"

//---------------------------Vector2------------------------------▽

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

/// <summary>
/// デストラクタ（コンストラクタより上に書くのは意味がある。
/// </summary>
static SQInteger	destructor( SQUserPointer p, SQInteger size )
{
	size;
	DEL ( sky::lib::math::CBasicVector2 * )p;
	return 1;
}

/// <summary>
/// コンストラクタ(constructorという名前には意味がある）
/// </summary>
static SQInteger	constructor( HSQUIRRELVM v )
{
	//インスタンスを生成
	sky::lib::math::CBasicVector2 *pObj = NEW__( sky::lib::math::CBasicVector2 , sky::lib::math::CBasicVector2() ) ;

	//インスタンスを戻り値に
	sq_setinstanceup( v , 1 , (SQUserPointer * )pObj );

	//デストラクタの登録
	sq_setreleasehook( v , 1 , destructor );

	return 1; //戻り値1
}

/// <summary>
/// 値のSetter（_setという名前には意味がある）
/// </summary>
static SQInteger	_set( HSQUIRRELVM v )
{
	//引数xの受け取り
	sky::lib::math::CBasicVector2 *pObj; sq_getinstanceup( v , 1 , ( SQUserPointer * )&pObj , 0);

	//引数sの受け取り
	const SQChar* key; sq_getstring( v , 2 , &key );

	//引数に応じた処理を行う
	if ( CStringUtil_::Comp( key , _T( "X" ) ) )
	{
		SQFloat val;
		sq_getfloat( v , 3 , &val );			//引数.の受け取り
		pObj->X = val;							//引数.の代入
	}
	else if ( CStringUtil_::Comp( key , _T( "Y" ) ) )
	{
		SQFloat val; 
		sq_getfloat( v , 3 , &val );			//引数.の受け取り
		pObj->Y = val;							//引数.の代入
	}
	else
	{
		return sq_throwerror( v , _T( "There is no member variable of this name in this class. " ) ); //このクラスにはそんなメンバ変数は居ません。
	}
	return 0; //戻り値0
}

/// <summary>
/// 値のGetter（_getという名前には意味がある）
/// </summary>
static SQInteger	_get( HSQUIRRELVM v )
{
	//引数xを受け取る
	sky::lib::math::CBasicVector2 *pObj; sq_getinstanceup( v , 1 , ( SQUserPointer * )&pObj , 0 );

	//引数sを受け取る
	const SQChar* key; sq_getstring( v , 2 , &key );

	//引数に応じて処理を行う
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
		return sq_throwerror( v , _T( "There is no member variable of this name in this class." ) ); //このクラスにはそんなメンバ変数は居ません。
	}
	return 1;  //戻り値1
}

sky::lib::squirrel::SqClassObject GetSqVector2() //コピーコンストラクタが呼ばれるので毎フレームは呼ばないでください。おそらく一回の実行で数回しか呼ばないはずです。
{
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "CVector2" ) ) );						
	classobj.AddMethod( CHS( _T( "constructor" ) ) , constructor , CHS( _T( "" ) ) );			
	classobj.AddMethod( CHS( _T( "_set" ) ) , _set , CHS( _T( "xs." ) ) );				
	classobj.AddMethod( CHS( _T( "_get" ) ) , _get , CHS( _T( "xs" ) ) );

	return classobj;
}

#endif