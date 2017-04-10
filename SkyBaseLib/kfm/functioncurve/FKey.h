#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// ファンクションカーブキーベース
/// </summary>
class CFKeyBase : public base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CFKeyBase(){};

	/// <summary>
	/// 時間の取得／設定
	/// </summary>
	updateTime  GetTime() const { return m_Time; }
	void SetTime( updateTime time ){ m_Time = time; }

	/// <summary>
	/// 補間タイプの取得／設定
	/// </summary>
	eInterpolateType GetInterpType() const { return m_eInterpType; }
	void SetInterpType( eInterpolateType eInterpType ){ m_eInterpType = eInterpType; }

protected:

	/// <summary>
	/// 時間
	/// </summary>
	updateTime m_Time;

	/// <summary>
	/// 補間タイプ
	/// </summary>
	eInterpolateType m_eInterpType;

};

/// <summary>
/// ファンクションカーブキー
/// CFKeyとFValueは一対一。
/// </summary>
template < class T >
class CFKey : public CFKeyBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFKey( T value );

	/// <summary>
	/// 値の取得
	/// </summary>
	inline CFValue< T > &GetValue();

	/// <summary>
	/// *演算子のオーバーライド
	/// </summary>
	T& operator *(); 

	/// <summary>
	/// *演算子のオーバーライド
	/// </summary>
	T* operator &(); 

private:

	/// <summary>
	/// 値
	/// </summary>
	CFValue< T > m_Value;

};

} } }

#include "FKey.inl"