#pragma once

/// <summary>
/// リクエストキュー
/// </summary>
/*
ClientThreadからはPutRequest(キューに積む）される。
ServerThreadからはGetRequest(キューから取得する）される。
もし、CServerThreadからGetRequestされたときにキューにデータが
存在しなければ、ClientThreadからPutRequestされるのを待つ。
*/
class RequestQueue
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RequestQueue( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~RequestQueue( void );

	/// <summary>
	/// リクエストデータ設定（ヒープで確保したRequestDataのポインタを渡してください。）
	/// </summary>
	void PutRequest( RequestData *pRequest );

	/// <summary>
	/// リクエストデータ取得
	/// </summary>
	RequestData *GetRequest( void );

private:

	/// <summary>
	/// リクエストデータFifo
	/// </summary>
	RequestDataPtrFifo	m_spRequestFifo;

	/// <summary>
	/// ロックオブジェクト
	/// </summary>
	sky::lib::thread::ILockObject *m_pLock;

};


typedef sky::lib::SmartPointerMT< RequestQueue >			RequestQueuePtr;	//SmartPointer型
