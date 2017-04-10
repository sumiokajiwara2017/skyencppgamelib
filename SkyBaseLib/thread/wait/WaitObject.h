#pragma once

namespace sky { namespace lib { namespace thread {

class CWaitObject;
typedef SmartPointer< CWaitObject >	CWaitObjectPtr;

/// <summary>
/// WaitObjectクラス
/// スレッドがとある位置で実行を停止したい場合、WaitObjectをそこに起き、その場所で
/// シグナルが発生するまで待ち続ける。仕事をしていないスレッドが無駄なアイドリング
////をしないための処置として必須な概念。
/// Windows上ではEvent機能として実装されるが、Linux上ではEventという概念が無いため、
////EventクラスではなくWaitObjectクラスという頑張って抽象化した名前のクラスで実装。
/// </summary>
class CWaitObject : public base::SkyRefObject
{

public:

	/// <summary>
	///	デストラクタ
	/// </summary>
	virtual ~CWaitObject();

	/// <summary>
	///	シグナル状態にする(この瞬間待っていたスレッドが起きる）
	/// </summary>
	void SetSignal();

	/// <summary>
	///	ノンシグナル状態にする
	/// </summary>
	void SetUnSignal();

	/// <summary>
	///	オブジェクトがシグナル状態（利用できる状態）になるまで待つ
	/// time = -1の場合、シグナルがくるまで無限に待つ。
	/// </summary>
	eWaitResultKind	Wait( s32 time= -1 );

	/// <summary>
	///	シグナル状態かどうか判定する
	/// </summary>
	skyBool	IsSignal() const;

	/// <summary>
	///	作成
	/// </summary>
	static CWaitObjectPtr Create();

private:

	/// <summary>
	/// クリティカルセクション
	/// </summary>
	CriticalSection *m_pCs;

	/// <summary>
	///　シグナル状態フラグ
	/// </summary>
	volatile skyBool	m_IsSignal;

#ifdef SKYLIB_PLATFORM_WIN32

	/// <summary>
	///	イベント識別ハンドル
	/// </summary>
	HANDLE	m_Handle;

#endif

	/// <summary>
	///	コンストラクタ
	/// </summary>
	CWaitObject();
};

} } }