#pragma once

namespace sky { namespace lib { namespace time {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// 時間計測用クラス
/// コンストラクタで計測を開始してデストラクタで計測結果をコンソール出力する
/// スコープの開始にTIMER_OBJCETを宣言して、終わりにデストラクタで時間を出す
/// 通常は関数の最初に宣言するが、関数内の局所的な計測も下記のように書けばできる。
/// {
///		TIMER_OBJCET( "hoge" );//計測開始
///		
///		//＊＊＊＊処理負荷が気になる処理＊＊＊＊
///
///	} //ここで計測終了（スコープから抜け、デストラクタが呼ばれる）
/// </summary>
class CTimerObject : public base::SkyObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CTimerObject( const skyString *pName );

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline ~CTimerObject();

	/// <summary>
	/// 名前
	/// </summary>
	const skyString		*m_Name;

#ifdef SKYLIB_PLATFORM_WIN32
	/// <summary>
	/// 時間計測開始時情報
	/// </summary>
	LARGE_INTEGER m_liBegin;

	/// <summary>
	/// 時間計測終了時情報
	/// </summary>
	LARGE_INTEGER m_liEnd;

	/// <summary>
	/// 時間計測時環境情報
	/// </summary>
	LARGE_INTEGER m_liFreq;
#endif

	/// <summary>
	/// 時間計測開始時間（ミリ秒）
	/// </summary>
	updateTime m_msBegin;

};

#ifdef SKYLIB_PLATFORM_WIN32

inline CTimerObject::CTimerObject( const skyString *pName ) : m_Name( pName )
{
	QueryPerformanceCounter( &m_liBegin );

	m_msBegin = CTimeUtil_::GetNowTime();
}

inline CTimerObject::~CTimerObject()
{
	if ( m_Name == NULL ) return;

	QueryPerformanceCounter( &m_liEnd );
	QueryPerformanceFrequency( &m_liFreq );
	_tprintf( _T( "%s = %f ms( %f ) \n" ), m_Name , CTimeUtil_::GetNowTime() - m_msBegin , ( d64 )( m_liEnd.QuadPart - m_liBegin.QuadPart) / (d64)m_liFreq.QuadPart );

	m_Name = NULL;
}

#endif

#ifdef SW_SKYLIB_TIMER_ON

#define TIMER_OBJCET( name ) sky::lib::time::CTimerObject tm( name )

#else

#define TIMER_OBJCET( name ) 

#endif

} } }