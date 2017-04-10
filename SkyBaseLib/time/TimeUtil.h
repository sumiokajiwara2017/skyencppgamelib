#pragma once

namespace sky { namespace lib { namespace time {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// 時間系便利関数群
/// </summary>
class CTimeUtil : public base::SkyObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose();

	/// <summary>
	/// 現在時間取得（ミリ秒）
	/// </summary>
	static updateTime	GetNowTime();

	/// <summary>
	/// 時間停止（ミリ秒）
	/// </summary>
	static void Wait( updateTime waitTime );

private:

#ifdef SKYLIB_PLATFORM_WIN32
	/// <summary>
	/// 時間情報
	/// </summary>
	static TIMECAPS m_Caps;
#endif

	//各機能封印
	CTimeUtil(){};												//コンストラクタ
	CTimeUtil( const CTimeUtil& ){};							//コピーコンストラクタ
	virtual ~CTimeUtil(){};										//デストラクタ
	CTimeUtil &operator=(const CTimeUtil& ){ return *this; };	//代入演算子オーバーロード

};

#define CTimeUtilInitialize_() sky::lib::time::CTimeUtil::Initialize()
#define CTimeUtil_             sky::lib::time::CTimeUtil
#define CTimeUtilDispose_()    sky::lib::time::CTimeUtil::Dispose()

} } }
