#pragma once

namespace sky { namespace lib {

/// <summary>
/// スマートポインタクラスです。
/// とあるサイトから頂いてきました。
///
/// スマートポインタとは簡単な話、ポインタ管理クラスが
/// 消されると同時に管理オブジェクトも巻き添えにし
/// て消すので、オブジェクトの削除をしなくてもいい
/// という削除の手間を省いたポインタをNEW生成できる機能である。
/// じゃあポインタ管理クラスはなんで消えるの？と思うのは当然で、
/// ポインタ管理クラスはなんらかの方法で消えてくれないといけません。delete？意味ねー！。
/// いやいやこのスマートポインタは色々意味があるんです。理解はしましたがすぐに忘れそう。
/// なるべく使うようにしよう。
/// </summary>

/*---------------------実装例----------------------------▽

//インスタンス生成
SmartPointer<Hoge> spHoge(NEW Hoge() );

*/

/// <summary>
/// SkyRefObjectを継承したクラスのみ使用できるスマートポインタ
/// </summary>
template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointer : public ThreadingModel //任意のスレッドモデルを継承（シングルスレッド／マルチスレッド両対応になりえる）
{

public:

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	inline explicit SmartPointer( const T *src = skyNull , skyBool bIsNoDelete = skyFalse , s32 add = 0 );

	/// <summary>
	/// コピーコンストラクタ（同型純粋コピー）
	/// </summary>
	inline SmartPointer( const SmartPointer< T , ThreadingModel > &src );

	/// <summary>
	/// コピーコンストラクタ（暗黙的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointer( const SmartPointer< T2 , ThreadingModel > &src ); //T2は親クラスを想定

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SmartPointer();

	/// <summary>
	/// =代入演算子（明示的コピー）
	/// </summary>
	inline SmartPointer< T , ThreadingModel >& operator =( const SmartPointer< T , ThreadingModel > &src);

	/// <summary>
	/// =代入演算子（明示的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointer< T , ThreadingModel >& operator =( const SmartPointer< T2 , ThreadingModel > &src ); //T2は親クラスを想定

	/// <summary>
	/// ->メンバ選択演算子
	/// </summary>
	inline T* operator ->() const;

	/// <summary>
	/// *メンバ選択演算子
	/// </summary>
	T &operator*(); 

	/// <summary>
	/// ==比較演算子
	/// </summary>
	inline skyBool operator ==( T *val );

	template< class T2 > 
	inline skyBool operator ==( const SmartPointer< T2 , ThreadingModel > &src );

	/// <summary>
	/// !=比較演算子
	/// </summary>
	inline skyBool operator !=( T *val );

	template< class T2 > 
	inline skyBool operator !=( const SmartPointer< T2 , ThreadingModel > &src );

	/// <summary>
	/// ポインタの明示的な登録
	/// </summary>
	inline void SetPtr( T *src = skyNull , skyBool bIsNoDelete = skyFalse ,  s32 add = 0 );

	/// <summary>
	/// ポインタの上書き
	/// </summary>
	inline void OverWrite( T *src = skyNull );

	/// <summary>
	/// 実体の貸し出し
	/// </summary>
	inline const T& GetIns() const;

	/// <summary>
	/// ポインタの貸し出し
	/// </summary>
	inline const T* GetPtr() const;

	/// <summary>
	/// 削除無効フラグの取得
	/// </summary>
	inline skyBool IsNodelete() const;

	/// <summary>
	/// ポインタの貸し出し(コンストなし危険）
	/// </summary>
	inline T* GetPtrNoConst() const;

	/// <summary>
	/// ポインタの貸し出し(コンストなし危険）
	/// </summary>
	inline T& GetInsNoConst() const;

	/// <summary>
	/// ポインタのポインタ貸し出し
	/// </summary>
	inline const T** GetPtrPtr();

	/// <summary>
	/// スマートポインタが空（何も管理していないか）どうか
	/// </summary>
	inline skyBool IsEmpty() const;

	/// <summary>
	/// 削除
	/// </summary>
	inline void Delete();

protected:

	/// <summary>
	/// T型のオブジェクトのポインタ
	/// </summary>
	T*	m_pPtr;

	/// <summary>
	/// 削除しない（関数のインターフェースを通すために一時的にスマートポインタの型に変換する場合等に立てる。）
	/// </summary>
	skyBool m_bIsNoDelete;

	/// <summary>
	/// 開放する
	/// </summary>
	virtual void Release();

};

template< class T >
class SmartPointerMT : public SmartPointer< T , thread::ThreadingModels::CMultiThread >
{

public :

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	inline explicit SmartPointerMT( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// コピーコンストラクタ（同型純粋コピー）
	/// </summary>
	inline SmartPointerMT( const SmartPointerMT< T > &src );

	/// <summary>
	/// コピーコンストラクタ（暗黙的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointerMT( SmartPointerMT< T2 > &src ); //T2は親クラスを想定

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SmartPointerMT();
};

typedef	SmartPointer< base::SkyRefObject >		CommonSmartPointer;		//何でも入るシングルスレッドスマートポインタ型
typedef	SmartPointerMT< base::SkyRefObject >	CommonSmartPointerMT;	//何でも入るマルチスレッドスマートポインタ型

/// <summary>
/// どんな型でも格納可能なスマートポインタクラス
/// 参照カウンタをNEWしてるので細かいMallocが発生してしまうのが弱点
/// </summary>
template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointer2 : public ThreadingModel //任意のスレッドモデルを継承（シングルスレッド／マルチスレッド両対応になりえる）
{

public:

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	inline explicit SmartPointer2( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// コピーコンストラクタ（同型純粋コピー）
	/// </summary>
	inline SmartPointer2( const SmartPointer2< T , ThreadingModel > &src );

	/// <summary>
	/// コピーコンストラクタ（暗黙的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointer2( const SmartPointer2< T2 , ThreadingModel > &src ); //T2は親クラスを想定

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SmartPointer2();

	/// <summary>
	/// =代入演算子（明示的コピー）
	/// </summary>
	inline SmartPointer2< T , ThreadingModel >& operator =( const SmartPointer2< T , ThreadingModel > &src);

	/// <summary>
	/// =代入演算子（明示的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointer2< T , ThreadingModel >& operator =( const SmartPointer2< T2 , ThreadingModel > &src ); //T2は親クラスを想定

	/// <summary>
	/// ->メンバ選択演算子
	/// </summary>
	inline T* operator ->() const ;

	/// <summary>
	/// *メンバ選択演算子
	/// </summary>
	T &operator*(); 

	/// <summary>
	/// ==比較演算子
	/// </summary>
	inline skyBool operator ==( T *val );

	template< class T2 > 
	inline skyBool operator ==( const SmartPointer2< T2 , ThreadingModel > &src );

	/// <summary>
	/// !=比較演算子
	/// </summary>
	inline skyBool operator !=( T *val );

	template< class T2 > 
	inline skyBool operator !=( const SmartPointer2< T2 , ThreadingModel > &src );

	/// <summary>
	/// ポインタの明示的な登録
	/// </summary>
	inline void SetPtr( T *src = skyNull ,  s32 add = 0 );

	/// <summary>
	/// ポインタの上書き
	/// </summary>
	inline void OverWrite( T *src = skyNull );

	/// <summary>
	/// ポインタの貸し出し
	/// </summary>
	inline const T* GetPtr() const;

	/// <summary>
	/// ポインタの貸し出し(コンストなし危険）
	/// </summary>
	inline T* GetPtrNoConst() const;

	/// <summary>
	/// ポインタのポインタ貸し出し
	/// </summary>
	inline T** GetPtrPtr();

	/// <summary>
	/// 参照カウンタへのポインタを取得
	/// </summary>
	inline u32* GetRefPtr() const;
	inline u32  GetRef() const;

	/// <summary>
	/// スマートポインタが空（何も管理していないか）どうか
	/// </summary>
	inline skyBool IsEmpty() const;

	/// <summary>
	/// 削除
	/// </summary>
	inline void Delete();

protected:

	/// <summary>
	/// タイプ
	/// </summary>
	u32 type;

	/// <summary>
	/// 参照カウンタへのポインタ
	/// </summary>
	u32	*m_pRefCnt;

	/// <summary>
	/// T型のオブジェクトのポインタ
	/// </summary>
	T*	m_pPtr;

	/// <summary>
	/// 参照カウンタ増加
	/// </summary>
	inline void AddRef();

	/// <summary>
	/// 開放する
	/// </summary>
	virtual void Release();

};

typedef	SmartPointer2< void >		CommonSmartPointer2;		//何でも入るシングルスレッドスマートポインタ型

template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointerCom : public SmartPointer2< T , ThreadingModel >
{

public :

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	inline explicit SmartPointerCom( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// コピーコンストラクタ（同型純粋コピー）
	/// </summary>
	inline SmartPointerCom( const SmartPointerCom< T , ThreadingModel > &src );

	/// <summary>
	/// コピーコンストラクタ（暗黙的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointerCom( SmartPointerCom< T2 , ThreadingModel > &src ); //T2は親クラスを想定

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SmartPointerCom();

	/// <summary>
	/// 開放する(オーバーライド）
	/// </summary>
	inline void Release();

};


} }

#include "SmartPointer.inl"

//---System

//自分自身をスマートポインタに格納する
//このマクロはでストラク内で使用できません。
#define thisPtr( ptrType ) 	ptrType( this , skyFalse , this->GetRefCnt() )