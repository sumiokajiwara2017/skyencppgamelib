#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// ファンクションカーブ再生機
/// ファンクションカーブの再生要求をキューで保存し逐次再生していく。
/// 再生中のファンクションカーブと次に再生するファンクションカーブでブレンドをすることができる。
/// </summary>
template < class T >
class CFCurvePlayer : public base::SkyObject
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CFCurvePlayer(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFCurvePlayer(){};

	/// <summary>
	/// シーケンス再生要求をする(再生キューにPushする）
	/// startIntvl    : 開始待ち時間
	/// speed         : 再生スピード(1.0fで通常スピード）早送り等ができる。
	/// startSeqTime  : 再生開始シーケンス時間。(0.0fで最初から再生）シーケンスの途中から再生等ができる。
	/// </summary>
	inline void PlayFCurveRequest( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End );
	inline void PlayReverseFCurveRequest( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End );

	/// <summary>
	/// シーケンスを再生する（現在再生中のキューをクリアし、要求のシーケンスを再生する）
	/// </summary>
	inline void PlayFCurve( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End );
	inline void PlayReverseFCurve( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End );

	/// <summary>
	/// グローバルシーケンスの再生（グローバルシーケンス再生リストに登録され、キュー再生されているシーケンスと並行で再生される）
	/// </summary>
	inline CKfmSequencePlayer< T >* PlayGlobalFCurve( const CFCurve< T >* pFCurve , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End ); 
	inline CKfmSequencePlayer< T >* PlayReverseGlobalFCurve( const CFCurve< T >* pFCurve , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End ); 

	/// <summary>
	/// シーケンス一時停止
	/// </summary>
	inline void StopCurrentFCurve();
	inline void StopGlobalFCurve();
	inline void StopFCurveAll();

	/// <summary>
	/// キューに溜まっている次のシーケンスに移行命令（繰り返し、反復などの再生を終了に向かわせる）
	/// </summary>
	inline void NextCurrentFCurve();
	inline void NextGlobalFCurve();
	inline void NextAllFCurve();

	/// <summary>
	/// シーケンスキャンセル
	/// </summary>
	inline void CancelCurrentFCurve();
	inline void CancelGlobalFCurve();
	inline void CancelFCurveAll();

	/// <summary>
	/// シーケンス一時停止再開
	/// </summary>
	inline void ReStartCurrentFCurve(); 
	inline void ReStartGlobalFCurve(); 
	inline void ReStartFCurveAll(); 

	/// <summary>
	/// 再生(引数は進める時間）
	/// </summary>
	inline const CKfmSequence< T >* Play( updateTime timeOffset );

///---再生パラメーター途中変更

	/// <summary>
	/// 再生スピード設定
	/// </summary>
	inline void SetSpeed( updateTime speed );

	/// <summary>
	/// シーケンス時間設定
	/// </summary>
	inline void SetNowTime( updateTime time );

	/// <summary>
	/// 再生状態設定
	/// </summary>
	inline void SetPlayState( ePlayState state );

	/// <summary>
	/// 再生最小時間設定
	/// </summary>
	inline void SetMiniTime( updateTime time );

	/// <summary>
	/// 再生最大時間設定
	/// </summary>
	inline void SetMaxTime( updateTime time );

	/// <summary>
	/// 再生終了種別設定
	/// </summary>
	inline void SetEndType( ePlayEndType eEndType );

	/// <summary>
	/// ウェイトを設定(wrrning!! 注意）自動で行われる次のシーケンスとのブレンド処理に影響を与えます。）
	/// </summary>
	inline void SetWeights( f32 weights );

	/// <summary>
	/// 新値算出時イベント取得
	/// </summary>
	inline delegate::CDelegateHandler &GetColValueEventHndl();

private:

	/// <summary>
	/// 新値算出時イベントハンドラ
	/// </summary>
	delegate::CDelegateHandler  m_ColValueEventHndl;

	/// <summary>
	/// 中身はシーケンスプレイヤー
	/// </summary>
	CKfmSequencePlayer< T > m_SeqPlayer;
};

typedef CFCurvePlayer< dectype > DecTypeFCurvePlayer;

} } }


#include "FCurvePlayer.inl"