#pragma once

namespace sky { namespace lib { namespace string {

/// <summary>
/// 文字列クラス（可変長）
/// </summary>
class CStringBuffer : public CString
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CStringBuffer();
	CStringBuffer( const skyString *pStr );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CStringBuffer();

	/// <summary>
	/// 文字列を設定する
	/// </summary>
	virtual CStringBuffer& operator =( const skyString *pStr );
	virtual CStringBuffer& operator =( const CStringBuffer &str );

	/// <summary>
	/// 文字列を連結する（バッファが溢れたらバッファを拡張する）
	/// </summary>
	virtual CStringBuffer& operator +=( const skyString *pStr );
	virtual CStringBuffer& operator +=( const CStringBuffer &str );
	virtual CStringBuffer& operator +=( skyString str );
	virtual CStringBuffer& operator +=( s32 value );
	virtual CStringBuffer& operator +=( u32 value );
	virtual CStringBuffer& operator +=( f32 value );
	virtual CStringBuffer& operator +=( d64 value );
	virtual CStringBuffer& operator +=( skyBool value );
};

} } }