#pragma once

namespace sky { namespace lib { namespace delegate {

static const u32 DELEGATE_ARGS_SIZE = 64;

/// <summary>
/// デリゲート引数（拡張したければ継承してください）
/// </summary>
struct CDelegateArgs : public base::SkyObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CDelegateArgs( memSize args1 = 0 , memSize args2 = 0 , memSize args3 = 0 , memSize args4 = 0 , memSize args5 = 0 , memSize args6 = 0 , memSize args7 = 0 , memSize args8 = 0 )
	{
		memSize *pArgs = ( memSize * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
		pArgs[ 4 ] = args5;
		pArgs[ 5 ] = args6;
		pArgs[ 6 ] = args7;
		pArgs[ 7 ] = args8;
	}
	inline memSize GetMemSize( u32 i ) const
	{
		SKY_ASSERT( i < 4 );
		memSize *pArgs = ( memSize * )Args;
		return pArgs[ i ];
	}
	inline CDelegateArgs( s32 args1 , s32 args2 = 0 , s32 args3 = 0 , s32 args4 = 0 )
	{
		s32 *pArgs = ( s32 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
	}
	inline CDelegateArgs( d64 args1 , d64 args2 = 0 , d64 args3 = 0 , d64 args4 = 0 )
	{
		d64 *pArgs = ( d64 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
	}
	inline d64 Getd64( u32 i ) const
	{
		SKY_ASSERT( i < 4 );
		d64 *pArgs = ( d64 * )Args;
		return pArgs[ i ];
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDelegateArgs(){};

	/// <summary>
	/// u32型パラメーターの取得／設定
	/// </summary>
	inline void Setu32( u32 args1 , u32 args2 = 0 , u32 args3 = 0 , u32 args4 = 0 , u32 args5 = 0 , u32 args6 = 0 , u32 args7 = 0 , u32 args8 = 0 )
	{
		u32 *pArgs = ( u32 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
		pArgs[ 4 ] = args5;
		pArgs[ 5 ] = args6;
		pArgs[ 6 ] = args7;
		pArgs[ 7 ] = args8;
	}
	inline u32 Getu32( u32 i ) const
	{
		SKY_ASSERT( i < 8 );
		u32 *pArgs = ( u32 * )Args;
		return pArgs[ i ];
	}

	/// <summary>
	/// s32型パラメーターの取得／設定
	/// </summary>
	inline void Sets32( s32 args1 , s32 args2 = 0 , s32 args3 = 0 , s32 args4 = 0 , u32 args5 = 0 , u32 args6 = 0 , u32 args7 = 0 , u32 args8 = 0 )
	{
		s32 *pArgs = ( s32 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
		pArgs[ 4 ] = args5;
		pArgs[ 5 ] = args6;
		pArgs[ 6 ] = args7;
		pArgs[ 7 ] = args8;
	}
	inline s32 Gets32( s32 i ) const
	{
		SKY_ASSERT( i < 8 );
		s32 *pArgs = ( s32 * )Args;
		return pArgs[ i ];
	}

	/// <summary>
	/// パラメーター
	/// </summary>
	u8 Args[ DELEGATE_ARGS_SIZE ];

	/// <summary>
	/// Fps（イベント管理でこの情報が付加されてすべてのイベントに配布される）
	/// </summary>
	fps::CFpsController m_Fps;

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CDelegateArgs& operator =( const CDelegateArgs& str )
	{
		CMemoryUtil_::Copy( Args , str.Args , DELEGATE_ARGS_SIZE );

		return *this;
	}
};

/// <summary>
/// デリゲートオブジェクト
/// </summary>
struct IDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// コールバックインタフェース
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args ) = 0;
};
typedef SmartPointer< IDelegateObject >					IDelegateObjectPtr;
typedef CListMT< u32 , IDelegateObjectPtr >				IDelegateObjectPtrList;

/// <summary>
/// デリゲート生成アダプター
/// </summary>
template< typename T >
class CEventDelegateAdaptor : public IDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef void ( T::*FuncType )( const CommonSmartPointer &spSender , const CDelegateArgs &args );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CEventDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc(pFunc){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CEventDelegateAdaptor(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args )
	{
		( m_pObject->*m_pFunc )( spSender , args );
	}
	
private:

	/// <summary>
	/// クラスインスタンスの参照保持
	/// </summary>
	T			*m_pObject;

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType	m_pFunc;

};

class CEventDelegateAdaptorFunc : public IDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef void ( * FuncType )( const CommonSmartPointer &spSender , const CDelegateArgs &args );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CEventDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc(pFunc){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CEventDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args )
	{
		m_pFunc( spSender , args );
	}
	
private:

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType	m_pFunc;

};

class CEventDelegateAdaptorBool : public IDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef void ( * FuncType )( const CommonSmartPointer &spSender , const CDelegateArgs &args );

	/// <summary>
	/// コンストラクタ（自動変数のskyBoolを登録するとダメではないが危ない）
	/// </summary>
	inline CEventDelegateAdaptorBool( skyBool &flg ){ m_pFlg = &flg; };

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CEventDelegateAdaptorBool(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args )
	{
		SKY_UNUSED_ARG( spSender );
		SKY_UNUSED_ARG( args );

		//フラグを反転する
		*m_pFlg = !( *m_pFlg ); 
	}
	
private:

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	skyBool	*m_pFlg;

};


/// <summary>
/// イベントオブジェクトの保持と呼び出しを行う（このクラスをイベントを設置したいクラスのメンバにする）
/// </summary>
class CDelegateHandler : public base::SkyRefObject
{
	
public:
	
	/// <summary>
	/// デリゲートを追加する
	/// </summary>
	void operator += ( const IDelegateObjectPtr &e )
	{
		m_kList.AddTail( e );
	}

	/// <summary>
	/// デリゲートを削除する
	/// </summary>
	void operator -= ( const IDelegateObjectPtr &e )
	{
		m_kList.Delete( e );
	}
	
	/// <summary>
	/// 登録されているデリゲートをすべて呼び出す
	/// </summary>
	void operator()( const CommonSmartPointer &spSender = CommonSmartPointer() , const CDelegateArgs &args = CDelegateArgs() )
	{
		FOREACH( IDelegateObjectPtrList , it , m_kList )
		{
			IDelegateObjectPtrList::iterator nextIt = it;
			nextIt = m_kList.GetNextPos( it );

			IDelegateObject& kDelegator = *( *it ).GetPtrNoConst();
			kDelegator( spSender , args );

			if ( nextIt != m_kList.GetNextPos( it ) )
			{//デリゲートの実行でリストが変化してしまった。
                SKY_PANIC_MSG( _T( "delegate is deleted. \n" ) );
				break;
			}
		}
	}
	
	/// <summary>
	/// 登録されているデリゲートをすべて消す
	/// </summary>
	void RemoveAll()
	{
		m_kList.DeleteAll();
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDelegateHandler()
	{
		RemoveAll();
	}

private:

	/// <summary>
	/// デリゲートリスト
	/// </summary>
	IDelegateObjectPtrList m_kList;

};
typedef CHash< CDelegateHandler >	CDelegateHandlerHash;


} } }
