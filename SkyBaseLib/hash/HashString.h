#pragma once

namespace sky { namespace lib { namespace hash {

//-------------------------------------< 実装 >---------------------------------------------//

template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointerHashString : public SmartPointer< T , ThreadingModel >
{

public :

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	inline explicit SmartPointerHashString( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// コピーコンストラクタ（同型純粋コピー）
	/// </summary>
	inline SmartPointerHashString( const SmartPointerHashString< T , ThreadingModel > &src );

	/// <summary>
	/// コピーコンストラクタ（暗黙的アップキャスト付き）
	/// </summary>
	template< class T2 >
	inline SmartPointerHashString( SmartPointerHashString< T2 , ThreadingModel > &src ); //T2は親クラスを想定

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SmartPointerHashString();

	/// <summary>
	/// 比較演算子（オーバーライド）
	/// </summary>
	inline skyBool operator ==( const SmartPointer< T , ThreadingModel > &src );

	/// <summary>
	/// キャストのオーバーライド
	/// </summary>
    inline operator const skyString*() const;

};

template< class T , class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::SmartPointerHashString( T *src , s32 add ) :
SmartPointer< T , ThreadingModel >( src , add )
{
}

template< class T , class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::SmartPointerHashString( const SmartPointerHashString< T , ThreadingModel > &src ) :
SmartPointer< T , ThreadingModel >( src )
{
}

template< class T , class ThreadingModel >
template< class T2 >
inline SmartPointerHashString< T , ThreadingModel >::SmartPointerHashString( SmartPointerHashString< T2 , ThreadingModel > &src ) : //T2は親クラスを想定
SmartPointer< T , ThreadingModel >( src )
{
}

template< class T , class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::~SmartPointerHashString()
{
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointerHashString< T , ThreadingModel >::operator ==( const SmartPointer< T , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	//保持しているオブジェクトのポインタと対象とを比較
        return ( *this->m_pPtr == *src.GetPtrNoConst() );
}

template< class T ,	class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::operator const skyString*() const
{
        return this->m_pPtr->GetString();
}


/// <summary>
/// Hash文字クラス。文字データとHash計算後の値が両方入っているのが長所
/// enumでのIDをHash文字にすることのメリットはコンパイルの高速化です。
/// 共通ID記述ヘッダーの変更は全コンパイルに及びやすい。文字列ならば外部ファイルで文字列管理をすればＯＫ
/// ※マネージャーからしか生成できません。
/// </summary>
class CHashString : public base::SkyRefObject
{

friend class CHashStringManager;

public:

	/// <summary>
	/// ハッシュ値を取得
	/// </summary>
	inline u32		Get() const { return m_HashValue; } 

	/// <summary>
	/// 文字列取得
	/// </summary>
	inline const skyString*     GetString() const;

	/// <summary>
	/// 文字列取得
	/// </summary>
	inline u32                  GetSize() const;

	/// <summary>
	/// 文字列比較
	/// </summary>
	inline skyBool operator ==( const skyString *pStr ) const;
	inline skyBool operator ==( const CHashString &pStr ) const;

	/// <summary>
	/// デストラクタ(CHashStringManagerのみアクセス可能）
	/// </summary>
	virtual ~CHashString();

protected:

	/// <summary>
	/// 文字データ
	/// </summary>
	union StringType
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline StringType()
		{
			pString = skyNull;
		}
		string::CString		*pStringClass;
		const skyString		*pString;
	};
	StringType			m_String;

	/// <summary>
	/// CRC値
	/// </summary>
	u32					m_HashValue;

	/// <summary>
	/// サイズ
	/// </summary>
	u32					m_Size;

	/// <summary>
	/// 文字列をクリエイトしなかったかどうかのフラグ
	/// </summary>
	skyBool				m_IsNoCreateOn;

	/// <summary>
	/// 文字列を設定
	/// </summary>
	inline void SetString( const skyString *pKey , skyBool isNoCreateOn = skyFalse );

	/// <summary>
	/// コンストラクタ(CHashStringManagerのみアクセス可能）
	/// </summary>
	inline CHashString(){};

};

} } }

#include "HashString.inl"

namespace sky { namespace lib {

template< typename KEY , typename DATA , class ThreadingModel >class CList;
template< typename T , typename KEY , class ThreadingModel >class    CHash;
template< typename T >class                                          CVectorPrimitive;

namespace hash {

typedef SmartPointerHashString< CHashString >                                               CHashStringPtr;
typedef CList< u32 , CHashStringPtr , thread::ThreadingModels::CSingleThread >				CHashStringPtrList;						//リスト定義
typedef CHash< CHashStringPtr , u32 , thread::ThreadingModels::CSingleThread >				CHashStringPtrHash;						//Hashテーブル定義
typedef CVectorPrimitive< CHashStringPtr >                                                           CHashStringPtrVector;					//Vector定義
typedef SmartPointer< hash::CHashStringPtrList >											CHashStringPtrListPtr;					//文字リストのスマートポインタ
typedef CHash< CHashStringPtrListPtr , u32 , thread::ThreadingModels::CSingleThread >		CHashStringPtrListPtrHash;				//文字列をキーに文字のリストを取得する
typedef SmartPointer< CHashStringPtrListPtrHash >											CHashStringPtrListPtrHashPtr;			//文字列をキーに文字のリストを取得する
typedef CHash< CHashStringPtrListPtrHashPtr , u32 , thread::ThreadingModels::CSingleThread >	CHashStringPtrListPtrHashPtrHash;		//文字列をキーに文字のリストを取得する

}

} }
