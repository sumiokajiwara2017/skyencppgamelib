#pragma once

/// <summary>
/// キューデータ
/// </summary>
class RequestData
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RequestData( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~RequestData( void );

	/// <summary>
	/// リクエストデータ
	/// </summary>
	sky::lib::WString		m_Data;

};

typedef sky::lib::Fifo< RequestData* >			RequestDataPtrFifo;	//Fifo定義
