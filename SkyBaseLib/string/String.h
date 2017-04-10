#pragma once

namespace sky { namespace lib { namespace string {

/// <summary>
/// 文字列クラス（固定長）
/// </summary>
class CString : public base::SkyObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CString();
	CString( const skyWString *pStr );
	CString( const skyMString *pStr );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CString();

	/// <summary>
	/// 文字列をクリアする
	/// </summary>
	void Clear();

	/// <summary>
	/// 小文字にする
	/// </summary>
	void ToLower();

	/// <summary>
	/// 文字列を返す
	/// </summary>
	const skyString* Get() const;

	/// <summary>
	/// 文字列を設定する
	/// </summary>
	virtual CString& operator =( const skyWString *pStr );
	virtual CString& operator =( const skyMString *pStr );
	virtual CString& operator =( const CString& str );

	/// <summary>
	/// 文字列を比較する
	/// </summary>
	virtual skyBool operator ==( const skyString c ) const;
	virtual skyBool operator ==( const skyString *pStr ) const;
	virtual skyBool operator ==( const CString& str ) const;
	virtual skyBool operator !=( const skyString c ) const;
	virtual skyBool operator !=( const skyString *pStr ) const;
	virtual skyBool operator !=( const CString& str ) const;

	/// <summary>
	/// 文字列を連結する（バッファが溢れたら溢れた分は落とす）
	/// </summary>
	virtual CString& operator +=( const skyString *pStr );
	virtual CString& operator +=( const CString& str );

	/// <summary>
	/// キャストのオーバーライド
	/// </summary>
    operator const skyString*() const;

	/// <summary>
	/// 文字列の長さを返す
	/// </summary>
	u32 GetLen(){ return m_nStrBufLen; }

	/// <summary>
	/// 文字データのバイトサイズを返す
	/// </summary>
	u32 GetSize(){ return m_nStrBufSize; }

	/// <summary>
	/// _MallocStrBufで確保した領域を開放する
	/// </summary>
	void Free();

protected:

	/// <summary>
	/// メンバ変数
	/// </summary>
	skyBool		m_bIsFix;		//すでに文字列が格納されたか？
	skyString	*m_pStrBuf;		//管理している文字列
	u32			m_nStrBufLen;	//管理している文字列の長さ
	u32			m_nStrBufSize;	//管理している文字列のメモリサイズ

	/// <summary>
	/// 指定の文字で内部バッファを作成
	/// </summary>
	void _Create( const skyString *pStr );

	/// <summary>
	/// 指定の文字列長さのメモリを確保して先頭アドレスをm_StrBufLenに格納する
	/// </summary>
	void _MallocStrBuf( u32 len );

	/// <summary>
	/// _MallocStrBufで確保した領域に文字列をコピーする
	/// </summary>
	void _Copy( const skyString *pStr );

	/// <summary>
	/// _MallocStrBufで確保した領域に文字列を連結する
	/// </summary>
	void _Cat( const skyString *pStr );
};

} } }


