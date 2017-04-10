#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// ファンクションカーブシーケンスベース
/// </summary>
class CKfmSequenceBase : public base::SkyObject 
{

public:

	/// <summary>
	/// 識別ID（イベント時に引数として渡す。必要がないなら設定しなくてもよい）
	/// </summary>
	s32 ID;

	/// <summary>
	/// 再生イベント( 0.0f時に開始イベント、m_TimeRange時に終了イベントを発生）
	/// </summary>
	delegate::CDelegateHandler m_PlayEventHndl;

	/// <summary>
	/// 時間幅を取得する
	/// </summary>
	updateTime GetTimeRange() const;

protected:

	/// <summary>
	/// シーケンスが完了するまでの時間
	/// </summary>
	updateTime m_TimeRange;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CKfmSequenceBase();
};

/// <summary>
/// ファンクションカーブシーケンス
/// </summary>
template < class T >
class CKfmSequence : public CKfmSequenceBase
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CKfmSequence(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CKfmSequence();

	/// <summary>
	/// ファンクションカーブを登録する
	/// </summary>
	inline u32 AddFCurve( updateTime startTime , const CFCurve< T > *pFCurve );

	/// <summary>
	/// ファンクションカーブを登録解除する
	/// </summary>
	inline void DeleteAll();

	/// <summary>
	/// インデックスで取得
	/// </summary>
	inline const CFCurve< T > *GetFCurve( u32 index ) const;

	/// <summary>
	/// ファンクションカーブ数の取得
	/// </summary>
	inline u32 GetFCurveNum() const;

	/// <summary>
	/// 再生する（純粋仮想の実装）
	/// </summary>
	inline void Play( updateTime time , f32 weight = 1.0f , skyBool isCallBackValue = skyTrue , const CKfmSequence< T > *pBlendSequence = skyNull );

private:

	/// <summary>
	/// ファンクションカーブ情報
	/// </summary>
	struct CFcurveInfo
	{
		updateTime    m_StartTime;
		CFCurve< T >* m_pFCurvePtr;
	};
	typedef CList< u32 , CFcurveInfo > CFcurveInfoPtrList;

	/// <summary>
	/// ファンクションカーブ情報リスト
	/// </summary>
	CFcurveInfoPtrList m_FCurveList;
};

} } }

#include "KfmSequence.inl"