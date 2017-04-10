#pragma once

namespace sky { namespace lib { namespace comif {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// COMインターフェース処理実装クラス
/// </summary>	
class ComInterfaceUtil : public base::SkyObject
{

public:

	/// <summary>
	/// COMの初期化
	/// </summary>	
	static void Initialize();

	/// <summary>
	/// COMの末期化
	/// </summary>	
	static void Dispose();

private:

	/// <summary>
	/// 初期化済みフラグ
	/// </summary>	
	static skyBool m_bIsInitialized;

	//各機能封印
	ComInterfaceUtil				(){};										//コンストラクタ
	ComInterfaceUtil				( const ComInterfaceUtil& ){};					//コピーコンストラクタ
	virtual ~ComInterfaceUtil		(){};										//デストラクタ
	ComInterfaceUtil &operator=		(const ComInterfaceUtil& ){ return *this; };	//代入演算子オーバーロード

};

#define ComInterfaceUtilInitialize_() sky::lib::comif::ComInterfaceUtil::Initialize()
#define ComInterfaceUtil_             sky::lib::comif::ComInterfaceUtil
#define ComInterfaceUtilDispose_()    sky::lib::comif::ComInterfaceUtil::Dispose()


} } }