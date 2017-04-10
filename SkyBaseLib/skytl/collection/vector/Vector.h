#pragma once

namespace sky { namespace lib {

static const u32 VECTOR_DEFAULT_SIZE = 32;
static const u32 VECTOR_DEFAULT_UP_SIZE = 16;

/// <summary>
/// 汎用型の動的拡張配列クラス
/// 内部が連続したアドレスなので、std::sortが使用できます。
/// Add時に容量不足が発生した場合要素が膨大だと極端に遅くなります。sizeは実際の最大使用量を設定して、動的拡張が発生しないようにしてください。
/// </summary>
template< typename T >
class CVector : public IteratorIf< T >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CVector( s32 size = VECTOR_DEFAULT_SIZE , u32 upSize = VECTOR_DEFAULT_UP_SIZE ); //設定数０でスタート

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	inline CVector( const CVector& src );

	/// <summary>
	/// 代入演算子（処理負荷:高）
	/// </summary>
	inline CVector& operator =( const CVector& src );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CVector();

	/// <summary>
	/// 追加（処理負荷:基本低、時々高）
	/// 列が足りなくなったら全Allocをし直してデータ配列全体を置き換えます。
	/// </summary>
	T &Add( const T &value, u32* idx = skyNull );
	T &Add( const T *value = skyNull , u32* idx = skyNull );

	/// <summary>
	/// 削除（処理負荷:高）
	/// 配列の任意の部分を削除し、後続を詰めます
	/// </summary>
	skyBool Delete( const T &value );

	/// <summary>
	/// 取得（処理負荷:高）
	/// </summary>
	skyBool Index( const T &value , s32 &dest );

	/// <summary>
	/// 取得（処理負荷:低）
	/// </summary>
	skyBool Get( u32 idx , T &value );

	/// <summary>
	/// 内容を破棄し、初期状態にします（★注意★初期設定数は0になります）
	/// </summary>
	void Clear( s32 size = VECTOR_DEFAULT_SIZE );

	/// <summary>
	/// リセット（★注意★初期設定数はsizeになります）
	/// </summary>
	void Reset( u32 size = VECTOR_DEFAULT_SIZE );

	/// <summary>
	/// 内容を一度破棄し、size分だけ領域を確保し、valueで埋める（★注意★初期設定数はsizeになります）
	/// </summary>
	void Assign( u32 size , T value );

	/// <summary>
	/// サイズを取得
	/// </summary>
	u32 GetSize() const;

	/// <summary>
	/// 設定数を取得
	/// </summary>
	u32 GetNum() const;

	/// <summary>
	/// 演算子のオーバーライド
	/// </summary>
	T &operator[]( s32 idx ) const;

	/// <summary>
	/// 内部バッファの先頭要素アドレスを取得する
	/// </summary>
	T* GetTopAdr() const;

	/// <summary>
	/// 内部バッファの末尾要素アドレスを取得する
	/// </summary>
	T* GetBottomAdr() const;

//---------------------イテレーター関連-----------------------▽

	/// <summary>
	/// イテレーター
	/// </summary>
	class iterator : public Iterator<CVector< T >,T>
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		iterator( CVector< T > *pVector, s32 cursor );
	};

	/// <summary>
	/// カーソルがBegin()より手前にある時True（FOREACH_REVERSのため）
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// 初期位置カーソルを指すイテレーター取得
	/// </summary>
	iterator Begin();

	/// <summary>
	/// イテレーター取得
	/// </summary>
	iterator End();

	/// <summary>
	/// イテレーターの指し示す位置を削除
	/// </summary>
	iterator Erases( iterator &it );

	/// <summary>
	/// 指定のインデックスにカーソルを当てる
	/// </summary>
	skyBool SetItCursor( u32 idx , T* &data ) const;

	/// <summary>
	/// インデックスでリスト削除
	/// </summary>
	skyBool DeleteIdx( u32 idx );

	/// <summary>
	/// コピー
	/// </summary>
	inline void Copy( CVector &dest ) const;

//---------------------イテレーター関連-----------------------△

protected:

	/// <summary>
	/// メンバー
	/// </summary>
	T*		m_pData;
	u32		m_Size;
	u32		m_nUpSize;
	u32		m_nSetNum;	//配列の登録数

private:

	/// <summary>
	/// 現在のカーソル位置アドレスを取得
	/// </summary>
	skyBool	_SetCursor( u32 idx , T* &pCur ) const;

};

/// <summary>
/// 汎用型の動的拡張配列クラス(プリミティブな型、u8 , s8 , u16 , s16 , u32 , s32 , u64 , f32 , d64 , 仮想テーブルを持たないclass struct 等はこれを使用たほうが高速です）
/// 内部が連続したアドレスなので、std::sortが使用できます。
/// 注意）もし動的拡張コンテナ(Fifo,Hash,List,Stack,Vector)の配列を作成したい場合は、コンテナのポインタを要素に持つベクターにしてください。内部でMemCopyを行っているため、クラス、構造体の配列では問題が発生します。
/// </summary>
template< typename T >
class CVectorPrimitive : public CVector< T >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CVectorPrimitive( s32 size = VECTOR_DEFAULT_SIZE , u32 upSize = VECTOR_DEFAULT_UP_SIZE ); //設定数０でスタート

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	inline CVectorPrimitive( const CVectorPrimitive& src );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CVectorPrimitive();

	/// <summary>
	/// 代入演算子
	/// </summary>
	inline CVectorPrimitive& operator =( const CVectorPrimitive& src );

	/// <summary>
	/// 追加（処理負荷:基本低、時々高）
	/// 列が足りなくなったら全Allocをし直してデータ配列全体を置き換えます。
	/// </summary>
	skyBool Add( const T &value, u32* idx = skyNull );
	skyBool Add( const T *value = skyNull , u32* idx = skyNull );

	/// <summary>
	/// コピー
	/// </summary>
	inline void Copy( CVector &dest ) const;
};

typedef CVectorPrimitive< u8 >  CVector_u8;
typedef CVectorPrimitive< s8 >  CVector_s8;
typedef CVectorPrimitive< u16 > CVector_u16;
typedef CVectorPrimitive< s16 > CVector_s16;
typedef CVectorPrimitive< u32 > CVector_u32;
typedef CVectorPrimitive< s32 > CVector_s32;
typedef CVectorPrimitive< f32 > CVector_f32;
typedef CVectorPrimitive< d64 > CVector_d64;
typedef CVectorPrimitive< u64 > CVector_u64;

typedef CVectorPrimitive< vrtxtype > CVector_vrtxtype;
typedef CVectorPrimitive< dectype > CVector_dectype;

} }

#include "Vector.inl"