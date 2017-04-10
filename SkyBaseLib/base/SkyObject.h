#pragma once

namespace sky { namespace lib { namespace base {

/// <summary>
/// 全てのクラスの基底クラス
/// </summary>
class SkyObject
{
SkyTypeDefRootRTTI( SkyObject );

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkyObject();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SkyObject();

	/// <summary>
	/// オブジェクトの内容をプリントする。できるだけ実装すること！
	/// </summary>
	virtual void vPrintf(){}
};

typedef SkyObject*	SkyObjectPtr;

} } }
