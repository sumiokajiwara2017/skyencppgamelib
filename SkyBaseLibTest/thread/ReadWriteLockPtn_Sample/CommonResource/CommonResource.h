#pragma once

/// <summary>
/// 
/// </summary>
class CommonResource
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommonResource( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CommonResource( void );

	/// <summary>
	/// データ読み込み
	/// </summary>
	const skyWString *Read( void );

	/// <summary>
	/// データ書き込み
	/// </summary>
	void Write( skyWString *pData );

private:

	/// <summary>
	/// データ
	/// </summary>
	sky::lib::WString	m_Data;

};
