#pragma once

namespace sky { namespace lib { namespace interp {

/// <summary>
/// 再生終了タイプ
/// </summary>
enum ePlayEndType
{
	ePlayEndType_End = 0    , //再生終了
	ePlayEndType_Repetition , //頭から再生開始（次があっても無視）
	ePlayEndType_Back       , //StartとEndを入れ替えて再び再生開始（次があっても無視）
};

class CInterpolateTaskPlayer;

//テンプレートTypedef郡
class CInterpolateTaskBase;
typedef CInterpolateTaskBase*                   CInterpolateTaskBasePtr;		//ポインタ定義
typedef CList< u32 , CInterpolateTaskBasePtr >	CInterpolateTaskBasePtrList;	//リスト定義

/// <summary>
/// 補間タスク基本クラス
/// ファンクションカーブを作って補間をするよりも柔軟で簡単な補間実行を提供する。
/// そのばその場の判断で自在に行き先を変えるオブジェクト等、決まった型の無い補間動作をする
/// のに最適。
/// </summary>
class CInterpolateTaskBase : public base::SkyRefObject
{

public:

	/// <summary>
	/// 更新
	/// </summary>
	virtual void *Update( updateTime time ) = 0;

	/// <summary>
	/// 再生終了判定
	/// </summary>
	virtual skyBool IsEnd() = 0;
};

/// <summary>
/// 補間タスククラス
/// </summary>
template < class T >
class CInterpolateTask : public CInterpolateTaskBase
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CInterpolateTask(){};

	/// <summary>
	/// 開始（プレイヤーに登録される）
	/// </summary>
	inline void Start( T startValue , T endValue , updateTime time , CInterpolateTaskPlayer *pPlayer , ePlayEndType endType = ePlayEndType_End , eInterpolateType eInterpType = eInterpolateType_Linear , updateTime speed = 1.0f );
	inline void Start( T endValue , updateTime time , CInterpolateTaskPlayer *pPlayer , ePlayEndType endType = ePlayEndType_End , eInterpolateType eInterpType = eInterpolateType_Linear , updateTime speed = 1.0f );//現在の値からスタートする

	/// <summary>
	/// 終了(プレイヤーから登録解除される）
	/// </summary>
	inline void End();

	/// <summary>
	/// 終了位置の追加（キューに貯められて、現在の補間が終了した値から次の値への補間が始まる）
	/// </summary>
	inline void RequestEndValue( T endValue , updateTime time , updateTime blendTime = 0.0f , eInterpolateType eInterpType = eInterpolateType_Linear , updateTime speed = 1.0f , ePlayEndType endType = ePlayEndType_End );
	inline void RequestEndValue( T endValue );
	inline void RequestEndValue( T endValue , updateTime time );

	/// <summary>
	/// リクエストの全撤回
	/// </summary>
	inline void CancelRequest();

	/// <summary>
	/// 現在再生中の終了位置の変更（補間途中の場合は現在の値から変更された値へ補間が開始される）
	/// </summary>
	inline void ChangeEndValue( T endValue );

	/// <summary>
	/// 現在再生中の補間方法の変更
	/// </summary>
	inline void ChangeInterpolateType ( eInterpolateType eInterpType );

	/// <summary>
	/// 現在再生中のスピードの変更
	/// </summary>
	inline void ChangeSpeed ( updateTime speed );

	/// <summary>
	/// 現在再生中の終了タイプを変更
	/// </summary>
	inline void ChangeEndType( ePlayEndType endType );

	/// <summary>
	/// 更新（内部時間を進める）
	/// </summary>
	virtual void *Update( updateTime time );

	/// <summary>
	/// 指定の時間の値を取得
	/// </summary>
	inline T GetValue( updateTime time );

	/// <summary>
	/// 終了判定
	/// </summary>
	virtual skyBool IsEnd();

	/// <summary>
	/// 新値算出時イベントハンドラ
	/// </summary>
	delegate::CDelegateHandler  m_GetValueEventHndl;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CInterpolateTask();

private:

	/// <summary>
	/// ステート
	/// </summary>
	enum eState_
	{
		eState_Wait_ = 0 ,
		eState_Play_ ,
	};

	/// <summary>
	/// 実行情報
	/// </summary>
	struct CExecData_
	{
		/// <summary>
		/// ステート
		/// </summary>
		 eState_          m_eState;

		/// <summary>
		/// 開始位置
		/// </summary>
		 T                m_StartValue;

		/// <summary>
		/// 終了位置
		/// </summary>
		 T                m_EndValue;

		/// <summary>
		/// 時間
		/// </summary>
		 updateTime       m_Time;

		/// <summary>
		/// 補間タイプ
		/// </summary>
		 eInterpolateType m_eInterpType;

		/// <summary>
		/// スピード
		/// </summary>
		 updateTime       m_Speed;

		/// <summary>
		/// 終了タイプ
		/// </summary>
		 ePlayEndType     m_EndType;

		/// <summary>
		/// ブレンドフラグ
		/// </summary>
		 updateTime       m_BlendTime;

		/// <summary>
		/// 現在の時間（Updateが呼ばれる度に内部で加算されていく。次の目的値がPopされたらリセットされる）
		/// </summary>
		updateTime m_NowTime;

		/// <summary>
		/// 現在の時間の値
		/// </summary>
		T m_NowValue;
	};
	typedef CFifo< CExecData_ > CEndRequestDataFifo_;	//リスト定義
	CEndRequestDataFifo_  m_EndRequestQueue;

	/// <summary>
	/// 現在再生中データ
	/// </summary>
	CExecData_ m_CurrentData;

	/// <summary>
	/// ブレンドデータ
	/// </summary>
	CExecData_ m_BlendData;

	/// <summary>
	/// 現在再生中シーケンスのウェイト値
	/// </summary>
	f32 m_CurrentWeights;

	/// <summary>
	/// 再生機の参照
	/// </summary>
	CInterpolateTaskPlayer *m_pPlayer;
};

} } }

#include "InterpolateTask.inl"