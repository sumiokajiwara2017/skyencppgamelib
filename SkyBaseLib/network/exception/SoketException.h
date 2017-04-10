#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// ソケット例外クラス
/// </summary>
class CSoketException : public exception::CExceptionBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSoketException( s32 errorId , const skyString *pMessage );

	/// <summary>
	/// エラーID取得
	/// </summary>
	s32 GetErrorId();

	/// <summary>
	/// プリント
	/// </summary>
	void Print();

private:

	/// <summary>
	/// エラーID
	/// </summary>
	s32 m_ErrorId;
};

} } }