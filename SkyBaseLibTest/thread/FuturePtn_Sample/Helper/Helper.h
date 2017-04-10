/// <summary>
/// ホストクラス
/// </summary>
class Helper : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Helper( const skyWString *pStr , DataSmPtr& data );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Helper( void );

	/// <summary>
	/// 実行関数
	/// </summary>
	virtual void ThreadMain( void );

	/// <summary>
	/// ゆっくり
	/// </summary>
	void Slowly( void );

protected:

	DataSmPtr				m_Data;
	sky::lib::WString		m_Str;

};
