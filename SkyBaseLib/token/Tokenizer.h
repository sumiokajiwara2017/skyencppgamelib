#pragma once

namespace sky { namespace lib { namespace token {
	
/// <summary>
/// トークンクラス
/// </summary>
template< class T >
class CTokenizer : virtual public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CTokenizer( u32 tokenSize , u32 separatorSize , u32 cutoffSize );

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CTokenizer();

	/// <summary>
	/// 区切り文字
	/// (呼び出し例SetSeparator( " \t\r\n,;\"" );)
	/// </summary>
	inline void SetSeparator( const T *separator );

	/// <summary>
	/// 切り出し文字
	/// (呼び出し例SetCutOff( "{}" );)
	/// </summary>
	inline void SetCutOff( const T *cutoff );

	/// <summary>
	/// バッファ設定
	/// </summary>
	inline void SetBuffer( T *buffer );

	/// <summary>
	/// 解放
	/// </summary>
	inline void Release();

	/// <summary>
	/// 次のトークンをロード（最初に一回呼ぶ）
	/// </summary>
	inline void Next();

	/// <summary>
	/// 現在ロードしたトークンと文字の比較
	/// </summary>
	inline skyBool Check( const T *pToken );
	inline skyBool CheckNext( const T *pToken );
	inline skyBool CheckNowAndNext( const T *pToken );

	/// <summary>
	/// 最後のトークンに行き着いたか
	/// </summary>
	inline skyBool IsEnd();

	/// <summary>
	/// 現在のトークンを取得
	/// </summary>
	inline T* GetChar();

	/// <summary>
	/// d64型で返す
	/// </summary>
	inline d64 GetDouble();

	/// <summary>
	/// f32型で返す
	/// </summary>
	inline f32 GetFloat();

	/// <summary>
	/// s32型で返す
	/// </summary>
	inline s32 GetInt();

	/// <summary>
	/// 次のトークンを取得（内部でNextするので現在のトークンには戻れません）
	/// </summary>
	inline T*  GetNextChar();

	/// <summary>
	/// d64型で返す（内部でNextするので現在のトークンには戻れません）
	/// </summary>
	inline d64 GetNextDouble();

	/// <summary>
	/// f32型で返す（内部でNextするので現在のトークンには戻れません）
	/// </summary>
	inline f32 GetNextFloat();

	/// <summary>
	/// s32型で返す（内部でNextするので現在のトークンには戻れません）
	/// </summary>
	inline s32 GetNextInt();

private:

	/// <summary>
	/// データバッファ
	/// </summary>
	T *m_pBuffer;

	/// <summary>
	/// トークンバッファ
	/// </summary>
	T *m_pToken;
	T *m_pTokenUseCheck;

	/// <summary>
	/// 区切り文字
	/// </summary>
	T *m_pSeparator;

	/// <summary>
	/// 切り出し文字
	/// </summary>
	T *m_pCutOff;

	/// <summary>
	/// トークンサイズ
	/// </summary>
	u32 m_TokenSize;

	/// <summary>
	/// 区切り文字
	/// </summary>
	u32 m_SeparatorSize;

	/// <summary>
	/// 切り出し文字
	/// </summary>
	u32 m_CutoffSize;
};

//シングルバイトトークンクラス
typedef CTokenizer< skyMString >     CTokenizerM;

//マルチバイトトークンクラス
typedef CTokenizer< skyWString >     CTokenizerW;

} } }

#include "Tokenizer.inl"