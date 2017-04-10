/// <summary>
/// ホストクラス
/// </summary>
class Host
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Host( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Host( void );

	/// <summary>
	/// 仕事をさせる
	/// </summary>
	void DoWork( const skyWString *pStr , DataSmPtr& data );

};
