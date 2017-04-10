#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// ファンクションカーブシーケンス再生機
/// シーケンスの再生要求をキューで保存し逐次再生していく。
/// 再生中のシーケンスと次に再生するシーケンスでブレンドをすることができる。
/// </summary>
template < class T >
class CKfmSequencePlayer : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CKfmSequencePlayer(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CKfmSequencePlayer();

	/// <summary>
	/// シーケンス再生要求をする(再生キューにPushする）
	/// startIntvl    : 開始待ち時間
	/// speed         : 再生スピード(1.0fで通常スピード）早送り等ができる。
	/// startSeqTime  : 再生開始シーケンス時間。(0.0fで最初から再生）シーケンスの途中から再生等ができる。
	/// </summary>
	inline void PlaySequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );
	inline void PlayReverseSequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );

	/// <summary>
	/// シーケンスを再生する（現在再生中のキューをクリアし、要求のシーケンスを再生する）
	/// </summary>
	inline void PlaySequence( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );
	inline void PlayReverseSequence( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );

	/// <summary>
	/// グローバルシーケンスの再生（グローバルシーケンス再生リストに登録され、キュー再生されているシーケンスと並行で再生される）
	/// </summary>
	inline CKfmSequencePlayer< T >* PlayGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse ); 
	inline CKfmSequencePlayer< T >* PlayReverseGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse ); 

	/// <summary>
	/// シーケンス一時停止
	/// </summary>
	inline void StopCurrentSequence();
	inline void StopGlobalSequence();
	inline void StopSequenceAll();

	/// <summary>
	/// キューに溜まっている次のシーケンスに移行命令（繰り返し、反復などの再生を終了に向かわせる）
	/// </summary>
	inline void NextCurrentSequence();
	inline void NextGlobalSequence();
	inline void NextAllSequence();

	/// <summary>
	/// シーケンスキャンセル
	/// </summary>
	inline void CancelCurrentSequence();
	inline void CancelGlobalSequence();
	inline void CancelSequenceAll();

	/// <summary>
	/// シーケンス一時停止再開
	/// </summary>
	inline void ReStartCurrentSequence(); 
	inline void ReStartGlobalSequence(); 
	inline void ReStartSequenceAll(); 

	/// <summary>
	/// 再生
	/// 戻り値に再生したシーケンスを返す。再生していない場合はskyNullが帰る
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
	/// シーケンスの設定
	/// </summary>
	inline void SetSequence( const CKfmSequence< T >* spSequence );

	/// <summary>
	/// ウェイトを設定(wrrning!! 注意）自動で行われる次のシーケンスとのブレンド処理に影響を与えます。）
	/// </summary>
	inline void SetWeights( f32 weights );

	/// <summary>
	/// 現在再生中のシーケンスを取得する
	/// </summary>
	const CKfmSequence< T >* GetCurrentSequence() const;

	/// <summary>
	/// 現在再生中のシーケンスの時間を取得する
	/// </summary>
	updateTime GetCurrentSequenceTime() const;

	/// <summary>
	/// シーケンス再生情報
	/// </summary>
	struct CSequencePlayInfo
	{
		virtual ~CSequencePlayInfo()
		{
			if ( m_IsSequenceDelete == skyTrue )
			{
				SAFE_DELETE( m_spSequencePtr );
			}
		}

		//内部管理情報
		ePlayState                   m_eState;     //基本ステート
		updateTime                   m_NowTime;

		//再生調整情報
		updateTime                   m_StartIntvl;
		updateTime                   m_MaxTime;
		updateTime                   m_MinTime;
		updateTime                   m_Speed;
		ePlayEndType                 m_EndType;
		updateTime                   m_BlendTime;
		eInterpolateType             m_BlendInterpType;

		//シーケンス
		CKfmSequence< T >*           m_spSequencePtr;
		skyBool                      m_IsSequenceDelete;

		/// <summary>
		/// 代入演算子オーバーライド
		/// </summary>
		virtual CSequencePlayInfo& operator =( const CSequencePlayInfo& src )
		{
			m_eState           = src.m_eState;
			m_NowTime          = src.m_NowTime;
			m_MaxTime          = src.m_MaxTime;
			m_MinTime          = src.m_MinTime;
			m_StartIntvl       = src.m_StartIntvl;
			m_Speed            = src.m_Speed;
			m_EndType          = src.m_EndType;
			m_BlendTime        = src.m_BlendTime;
			m_BlendInterpType  = src.m_BlendInterpType;
			m_IsSequenceDelete = skyFalse; //コピー先では削除しない。
			m_spSequencePtr    = src.m_spSequencePtr;

			return *this;
		}

	};
	typedef CFifo< CSequencePlayInfo >       CSequencePlayInfoFifo;
	typedef CList< u32 , CSequencePlayInfo > CSequencePlayInfoList;

	/// <summary>
	/// 再生済みのシーケンスをキューに貯めておくか？（これを使用して同じシーケンスを再生することや、逆時間再生ができる）
	/// </summary>
	inline void IsSavePlayedSequence( skyBool isSaveExec );

	/// <summary>
	/// 再生済みシーケンスキューの取得
    /// キューの最初のシーケンスは保存を開始した時から再生が終了した一番最初のシーケンスが入っている
	/// つまりキューを順番に順再生すると同じ動きの再現ができる。キューを末尾から順番に逆再生すると再生した動きの
	/// 逆再生になる
	/// </summary>
	inline CSequencePlayInfoFifo &GetPlayedSequenceFifo();

private:

	/// <summary>
	/// シーケンス再生要求キュー（登録された順に再生されていく）
	/// </summary>
	CSequencePlayInfoFifo              m_SequencePlayInfoFifo;

	/// <summary>
	/// 再生済みシーケンス保存フラグ
	/// </summary>
	skyBool                            m_IsSaveExec;

	/// <summary>
	/// 再生済みシーケンス情報キュー
	/// </summary>
	CSequencePlayInfoFifo              m_SequencePlayedInfoFifo;

	/// <summary>
	/// 現在再生中シーケンス情報
	/// </summary>
	CSequencePlayInfo                  m_CurrentPlaySequenceInfo;

	/// <summary>
	/// 現在再生中シーケンスのブレンド中シーケンス情報
	/// </summary>
	CSequencePlayInfo                  m_CurrentPlayBlendSequenceInfo;

	/// <summary>
	/// グローバルシーケンス
	/// </summary>
	typedef CList< u32 , CKfmSequencePlayer< T >* >  CKfmSequencePlayerPtrList;
	CKfmSequencePlayerPtrList          m_GlobalSequencePlayList;

	/// <summary>
	/// 現在再生中シーケンスのウェイト値( 1.0f - m_CurrentPlaySequenceBlendValue でブレンドシーケンスの )
	/// </summary>
	f32                                m_CurrentPlaySequenceWeights;

	/// <summary>
	/// 再生情報の設定
	/// </summary>
	inline void SetSequencePlayInfo_( CSequencePlayInfo &dest , const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete ); 

	/// <summary>
	/// 逆再生情報の設定
	/// </summary>
	inline void SetSequencePlayReverseInfo_( CSequencePlayInfo &dest , const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete ); 
};

} } }

#include "KfmSequencePlayer.inl"