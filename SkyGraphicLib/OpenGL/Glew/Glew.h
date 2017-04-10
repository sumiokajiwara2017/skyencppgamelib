#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// Glewクラス
/// </summary>
class CGlew : base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize	();
	static void Initialize( GLEWContext *pContext );

	/// <summary>
	/// コンテキストの生成
	/// </summary>
	static GLEWContext *CreateContext();

	/// <summary>
	/// コンテキストの削除
	/// </summary>
	static void DeleteContext( GLEWContext * );

	/// <summary>
	/// 現在のコンテキストを設定する
	/// </summary>
	static void SetCurrentContext( GLEWContext *current );

};

} } }