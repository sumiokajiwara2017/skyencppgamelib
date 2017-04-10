#pragma once

/// <summary>
/// スレッド間でやりとりされるデータクラス
/// </summary>
class Data
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Data( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Data( void );

	/// <summary>
	/// リクエストデータ
	/// </summary>
	sky::lib::WString		m_Data;

};

typedef sky::lib::Fifo< Data* >			DataPtrFifo;	//Fifo定義
