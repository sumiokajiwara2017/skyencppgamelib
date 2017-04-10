#pragma once

namespace sky { namespace lib { namespace kfm {

static const u32 CFCURVE_DEFAULT_KEY_NUM = 16;

/// <summary>
/// ファンクションカーブ基本クラス
/// </summary>
class CFCurveBase : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CFCurveBase();

	/// <summary>
	/// 時間幅（登録されているキーのTimeの合計）
	/// </summary>
	inline updateTime GetTimeRange() const;

	/// <summary>
	/// 新値算出時イベント取得
	/// </summary>
	inline delegate::CDelegateHandler &GetColValueEventHndl();

	/// <summary>
	/// キーのクリア
	/// </summary>
	virtual void ClearKey() = 0;

protected:

	/// <summary>
	/// ファンクションカーブが完了するまでの合計時間
	/// </summary>
	updateTime m_TimeRange;

	/// <summary>
	/// 新値算出時イベントハンドラ
	/// </summary>
	delegate::CDelegateHandler  m_ColValueEventHndl;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFCurveBase();
};

/// <summary>
/// ファンクションカーブ
/// </summary>
template < class T >
class CFCurve : public CFCurveBase
{
	/// <summary>
	/// ソートロジック定義
	/// </summary>
	class SortPred
	{
	public:
		skyBool operator()( const CFKey< T > *key1 , const CFKey< T > *key2 )
		{
			return key1->GetTime() < key2->GetTime();
		}
	};

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CFCurve();

	/// <summary>
	/// 型定義
	/// </summary>
	typedef CVectorPrimitive< CFKey< T > * > CKeyVector;

	/// <summary>
	/// 初期値の設定
	/// </summary>
	void SetInitValue( T value );
	T&   GetInitValue();

	/// <summary>
	/// キーの追加
	/// </summary>
	void AddKey( updateTime time , T keyValue , eInterpolateType eType );

	/// <summary>
	/// キーの取得
	/// </summary>
	skyBool GetKey( u32 index , CFKey< T >* &pDest );

	/// <summary>
	/// キーの削除
	/// </summary>
	void        DeleteKey( CFKey< T >* &pKey );
	CFKey< T > *DeleteKey( u32 index );

	/// <summary>
	/// ベースの純粋仮想関数の実装
	/// </summary>
	skyBool     ColTimeValue( updateTime time , f32 weight = 1.0f , skyBool isCallBackValue = skyTrue , const CFCurve< T > *pBlendFCurve = skyNull );
	const T&    GetTimeValue() const;
	void        ClearKey();

	/// <summary>
	/// キーリストの最適化（キーとキーの値の間が近い場合キーを減らす）
	/// </summary>
	void Optimization();

	/// <summary>
	/// コンストラクタ
	/// キーが一つだけ設定されている場合、最初の補間はこのコンストラクタに設定された初期値からキーへの補間になります。
	/// </summary>
	CFCurve( T initValue , u32 keyNum = CFCURVE_DEFAULT_KEY_NUM ) : m_InitValue( initValue ) , m_KeyArray( ( s32 )keyNum ){}

//---system

	/// <summary>
	/// キー管理配列の参照の取得（デバッグプリント等に使用）
	/// このリストを使用して内容を変更しないでください。
	/// </summary>
	CKeyVector &GetKeyArray();

private:

	/// <summary>
	/// 管理キー格納配列
	/// </summary>
	CKeyVector m_KeyArray;

	/// <summary>
	/// 初期値
	/// </summary>
	T m_InitValue;

	/// <summary>
	/// GetTimeValue()で最後に取得された値
	/// </summary>
	T m_TimeValue;
};

//よく使用する型のファンクションカーブ定義
typedef CFCurve< dectype >       DecTypeFCurve;

} } }

#include "FCurve.inl"