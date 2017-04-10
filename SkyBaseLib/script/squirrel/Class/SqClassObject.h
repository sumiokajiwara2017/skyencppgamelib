#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

namespace sqclassobject
{
	static const u32 MEMBER_HASH_VALUE = 16;
}

class SqClassObject; //リスト宣言のための先行宣言
typedef SmartPointer< SqClassObject >			SqClassObjectPtr;		//スマートポインタ版定義

/// <summary>
/// Squirrel上のクラスをC++で取得、設定するためのクラス
/// </summary>
class SqClassObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SqClassObject();
	SqClassObject( const hash::CHashStringPtr &pClassName );
	SqClassObject( const hash::CHashStringPtr &pClassName , const hash::CHashStringPtr &parentName );

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	SqClassObject		( const SqClassObject& src );

	/// <summary>
	/// クラス名
	/// </summary>
	hash::CHashStringPtr		m_kName;

	/// <summary>
	/// 親クラス
	/// </summary>
	hash::CHashStringPtr		m_ParentName;

	/// <summary>
	/// メンバー変数はSqParamObjectListで表現する
	/// </summary>
	SqParamObjectHash m_MemberHash;

	/// <summary>
	/// C++関数バインドメソッド用情報構造体
	/// </summary>
	SqCppFuncList		m_CppMethodList;

	/// <summary>
	/// 代入演算子のオーバーライド
	/// </summary>
	SqClassObject& operator =( const SqClassObject& src );

	/// <summary>
	/// メンバ変数の追加
	/// </summary>
	void AddMember(  const hash::CHashStringPtr pMemberName , const SqParamObject &param );

	/// <summary>
	/// C++関数バインドメソッドの追加
	/// </summary>
	/*
	文字	型
	o	null
	i	integer
	f	float
	n	integer, float
	s	string
	t	table
	a	array
	u	userdata
	c	closure, nativeclosure
	g	generator
	p	userpointer
	v	thread
	x	instance
	y	class
	b	bool
	.	any
	*/
	void AddMethod( const hash::CHashStringPtr pMemberFunc , SQFUNCTION f , const hash::CHashStringPtr typemask = CHS( _T("") ) );

};

} } }

#endif