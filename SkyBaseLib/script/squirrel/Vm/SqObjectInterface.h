#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

//ノードスタックの型
class SqObjectInterface; //リスト宣言のための先行宣言
typedef SmartPointer< SqObjectInterface >	SqObjectInterfacePtr;		//スマートポインタ版定義
typedef CList< u32 , SqObjectInterfacePtr >	SqObjectInterfacePtrList;	//リスト定義
typedef CStack< SqObjectInterfacePtr >		SqObjectInterfacePtrStack;	

/// <summary>
/// HSQOBJECTのインタフェース。
/// Squirrelの基本オブジェクトであるHSQOBJECTを直感的に操作するためのクラス
/// 引数として受け渡しも可能
/// SqParamObjectと機能が似ている。なりゆき上そうなっただけなので、いずれSqParamObjectを消すかもしれない。
/*  以下の型を可能な限りC++側で値を設定、取得しやすくするのがこのクラスの使命
	OT_NULL =			(_RT_NULL|SQOBJECT_CANBEFALSE),
	OT_INTEGER =		(_RT_INTEGER|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_FLOAT =			(_RT_FLOAT|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_BOOL =			(_RT_BOOL|SQOBJECT_CANBEFALSE),
	OT_STRING =			(_RT_STRING|SQOBJECT_REF_COUNTED),
	OT_TABLE =			(_RT_TABLE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_ARRAY =			(_RT_ARRAY|SQOBJECT_REF_COUNTED),
	OT_USERDATA =		(_RT_USERDATA|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_CLOSURE =		(_RT_CLOSURE|SQOBJECT_REF_COUNTED),
	OT_NATIVECLOSURE =	(_RT_NATIVECLOSURE|SQOBJECT_REF_COUNTED),
	OT_GENERATOR =		(_RT_GENERATOR|SQOBJECT_REF_COUNTED),
	OT_USERPOINTER =	_RT_USERPOINTER,
	OT_THREAD =			(_RT_THREAD|SQOBJECT_REF_COUNTED) ,
	OT_FUNCPROTO =		(_RT_FUNCPROTO|SQOBJECT_REF_COUNTED), //internal usage only
	OT_CLASS =			(_RT_CLASS|SQOBJECT_REF_COUNTED),
	OT_INSTANCE =		(_RT_INSTANCE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_WEAKREF =		(_RT_WEAKREF|SQOBJECT_REF_COUNTED),
	OT_OUTER =			(_RT_OUTER|SQOBJECT_REF_COUNTED) //internal usage only
*/
/// </summary>
class SqObjectInterface : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ。自分の産みの親であるVMの参照を持つ（色々な操作に使う）
	/// isAddRefExecがskyTrueの場合、内部で参照カウンタを上げ、デストラクタで参照カウンタを下げる
	/// </summary>
	SqObjectInterface( HSQUIRRELVM v , HSQOBJECT *sqObj , skyBool isAddRefExec );
	SqObjectInterface( HSQUIRRELVM v ); //VMがあれば内部で生成できる。

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SqObjectInterface();

	/// <summary>
	/// 各種オブジェクトを生成する
	/// </summary>
	void CreateSqObject( tagSQObjectType etype );

	/// <summary>
	/// オブジェクトの型を取得
	/// </summary>
	tagSQObjectType GetType();

	/// <summary>
	/// 各型にキャスト（型が違うとエラー）
	/// </summary>
    operator s32 () const;
    operator f32 () const;
	operator const skyString  *() const;
	operator skyBool() const;
	operator void* () const;

	/// <summary>
	/// 別のVMにコピーする（コピー結果はコピー先のVMの一番上に置かれる）
	/// </summary>
	void CopyObject( HSQUIRRELVM v );

//-----------<< 配列の場合の動作 >>-------------------------------------▽
//-----------<< テーブルの場合の動作 >>---------------------------------▽
//-----------<< クラスの場合の動作 >>-----------------------------------▽
//-----------<< クロージャーの場合の動作 >>-----------------------------▽
//-----------<< ネイティブクロージャーの場合の動作 >>--------------------▽
//-----------<< ジェネレーターの場合の動作 >>----------------------------▽

private:

	/// <summary>
	/// SquirrelのＶＭの参照
	/// </summary>
	HSQUIRRELVM m_SqVm;

	/// <summary>
	/// Squirrelの基本オブジェクトの参照。
	/// </summary>
	HSQOBJECT *m_SqObj;

	/// <summary>
	/// 参照カウンタをコンストラクタでUPしたか？
	/// </summary>
	skyBool m_IsAddRefExec;
};

} } }

#endif