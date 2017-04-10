#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// ソケットアプリケーションエラーID
/// </summary>
enum eSoketAppErrorId
{
	eSoketAppErrorIdSoketNotOpen = 0 ,
	eSoketAppErrorIdSoketAlreadyOpen ,
	eSoketAppErrorIdConnectAlreadyExec ,
	eSoketAppErrorIdConnectNotExec ,
	eSoketAppErrorIdReceiveBufferOver ,
	eSoketAppErrorIdThisAddressIsInvalid ,
};

/// <summary>
/// ソケットアプリケーション例外クラス
/// </summary>
class CSoketAppException : public exception::CExceptionBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSoketAppException( eSoketAppErrorId errorId , const skyString *pMessage );

	/// <summary>
	/// エラーID取得
	/// </summary>
	eSoketAppErrorId GetErrorId();

	/// <summary>
	/// プリント
	/// </summary>
	void Print();

private:

	/// <summary>
	/// エラーID
	/// </summary>
	eSoketAppErrorId m_eErrorId;
};

} } }