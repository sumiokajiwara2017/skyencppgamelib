/// <summary>
/// ホストクラス
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
	/// データの設定
	/// </summary>
	void SetData( const skyWString *pStr );

	/// <summary>
	/// データの取得
	/// </summary>
	const skyWString *GetData( void );

protected:

	/// <summary>
	/// データ準備完了フラグ
	/// </summary>
	skyBool							m_IsDataSet;

	/// <summary>
	/// データ
	/// </summary>
	sky::lib::WString				m_Data;

	/// <summary>
	/// ロック
	/// </summary>
	sky::lib::thread::ILockObject	*m_pLock;

};

typedef sky::lib::SmartPointerMT< Data >			DataSmPtr;	//SmartPointer型
