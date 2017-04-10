#pragma once

/// <summary>
/// 更新スレッド
/// </summary>
class DrawCommand
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DrawCommand( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DrawCommand( void );

	/// <summary>
	/// リクエストデータ
	/// </summary>
	sky::lib::WString		m_Data;

};

typedef sky::lib::List< s32 , DrawCommand * >			DrawCommandPtrList;	//List定義
