#pragma once

namespace sky { namespace lib { namespace fps {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// ＦＰＳ管理
/// </summary>
class CFpsController : public base::SkyObject
{
public:

	/// <summary>
	/// 更新フレームレート設定／取得
	/// </summary>
	inline void		    SetFrameRate( u16 frameRate = DEFAULT_FPS );
	inline u16		    GetFrameRate();

	/// <summary>
	/// 1フレームあたりの時間（ミリ秒）
	/// </summary>
	inline updateTime	GetOneFrameTime();

	/// <summary>
	/// 開始時間(ミリ秒）設定／取得
	/// </summary>
	inline void			SetStartTime( u32 startTime );
	inline updateTime	GetStartTime();

	/// <summary>
	/// 経過フレーム取得/カウントＵＰ
	/// </summary>
	inline void		    IncElapsedFrame();

	/// <summary>
	/// フレーム開始
	/// </summary>
	inline void		    StartFrame();

	/// <summary>
	/// まだフレーム終了予定時間を超えていないか？（処理落ちしていないか？）skyTrue:越えていない（時間が余っている）skyFalse:越えている（次のフレームにいかなくてはいけない）
	/// </summary>
	inline skyBool      IsEndTimeStillRate();

	/// <summary>
	/// フレーム終了（更新が早すぎる場合ここで待つ）
	/// </summary>
	inline void	        EndFrame();

	/// <summary>
	/// 前回のフレーム開始時間から今回のフレーム開始時間との経過時間
	/// </summary>
	inline updateTime   GetPassageTime_Msec() const;      //ミリセック(1/1000秒)
	inline updateTime   GetPassageTime_Second() const;    //秒

	/// <summary>
	/// アプリケーション開始から現在までの経過時間
	/// </summary>
	inline updateTime   GetPassageAllTime_Msec() const;   //ミリセック(1/1000秒)
	inline updateTime   GetPassageAllTime_Second() const; //秒

	/// <summary>
	/// アプリケーション開始から現在までの経過フレーム数
	/// </summary>
	inline u32		    GetElapsedFrame() const;

	/// <summary>
	/// 指定フレーム数の１フレームあたりの平均時間を返す
	/// </summary>
	inline skyBool      GetAverageOneFrameTime( u32 colFrameNum , updateTime &destOneFrameTime );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline              CFpsController();

private:

	/// <summary>
	/// アプリケーション開始時間
	/// </summary>
	updateTime m_StartAppTime;

	/// <summary>
	/// 現在のフレームレート
	/// </summary>
	u16 m_FrameRate;

	/// <summary>
	/// 経過フレーム数
	/// </summary>
	u32 m_ElapsedFrame;

	/// <summary>
	/// 前回開始時間（ミリ秒）
	/// </summary>
	updateTime m_BeforStartTime;

	/// <summary>
	/// 開始時間（ミリ秒）
	/// </summary>
	updateTime m_StartTime;

	/// <summary>
	/// 終了時間（ミリ秒）
	/// </summary>
	updateTime m_EndTime;

	/// <summary>
	/// １フレームあたりの時間
	/// </summary>
	updateTime m_OneFrameTime;

	/// <summary>
	/// １フレームあたりの平均時間を出すための計測フレーム数
	/// </summary>
	u32 m_ColAverageFrameNum;

	/// <summary>
	/// １フレームあたりの平均時間を出すための合計値格納計算バッファ
	/// </summary>
	updateTime m_ColAverageSumTimeBuffer;

	/// <summary>
	/// １フレームあたりの平均時間を出すための加算フレームカウンター
	/// </summary>
	u32 m_ColAverageFrameCounter;
};

} } }

#include "FpsController.inl"