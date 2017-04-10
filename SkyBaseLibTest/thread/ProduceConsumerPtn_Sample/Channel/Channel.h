#pragma once

/// <summary>
/// データの受け渡しを担当するクラス
/// </summary>
/*
ProducerThreadからはPutData(データを置く）される。
置かれたデータが、queue構造で管理しているかどうかは実装による。
ConsumerThreadからはGetData(データを取り出す）される。
もし、ConsumerThreadからGetDataされた時にデータが存在しなければ、
ProducerThreadからPutDataされるのを待つ
*/

/// <summary>
/// 定数
/// <summary>
namespace channel_const
{
	static const u32 BUFFER_LENGTH = 3;
}
using namespace channel_const;

class Channel
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Channel( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Channel( void );

	/// <summary>
	/// データ取得
	/// </summary>
	Data *GetData( void );

	/// <summary>
	/// データ格納
	/// </summary>
	void PutData( Data* pData );

private:

	/// <summary>
	/// リクエストデータFifo
	/// </summary>
	DataPtrFifo	m_DataFifo;

	/// <summary>
	/// ロックオブジェクト
	/// </summary>
	sky::lib::thread::ILockObject *m_pLock;
};