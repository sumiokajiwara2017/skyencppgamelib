#pragma once

namespace sky { namespace lib { namespace coroutine {

//コルーチンの終了値
static const s32 COROUTINE_END_VALUE = 0;

/// <summary>
/// コルーチンベースクラス
/// インターネットで見つけた簡易なコルーチン実装。
/// いくつかルールを守ればコルーチン的な動きを簡単に実装できる。
/// 詳しくはテストコード参照。本格的な実装は実装が難しく問題が起きやすいのでこのライブラリではここまでとする。
/// </summary>
class CCoroutineBase : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CCoroutineBase(){ Initialize(); }

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() { m_State = 0; }

	/// <summary>
	/// コルーチン処理の実装
	/// </summary>
	virtual s32 vExecute( void *pParam ) = 0;

protected:

	/// <summary>
	/// 現在の位置
	/// </summary>
	s32 m_State;
};

#define COROUTINE_BEGIN_ switch( m_State ) { case 0:
#define COROUTINE_END_   default: break; }
#define YIELD_( ret ) { m_State = __LINE__; return ret; case __LINE__:; }

} } }