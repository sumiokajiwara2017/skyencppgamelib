#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// ファンクションカーブキー
/// CFKeyとCFValueは一対一。
/// 値として使用するクラスは下記の演算子のオーバーライドする必要がある。
/// この定義は補間ロジックの追加で更に増える可能性がある。コピーコンストラクタを忘れると動作するがメモリ破壊が発生する可能性がある
///
/// <summary>
/// コピーコンストラクタ
/// </summary>
///T( const T& src )
///
/// <summary>
/// =演算子オーバーライド
/// </summary>
///virtual T& operator =( const T& src )
///
/// <summary>
/// ==演算子オーバーライド
/// </summary>
///	skyBool operator ==( const T& src ) const
///
/// <summary>
/// *演算子オーバーライド
/// </summary>
///T operator *( colortype src ) const
///T operator *( const T &src ) const
///
/// <summary>
/// -演算子オーバーライド
/// </summary>
///T operator -( const T& src ) const
///
/// <summary>
/// +演算子オーバーライド
/// </summary>
///T operator +( const T& src ) const
///
/// </summary>
template < class T >
class CFValue : public base::SkyObject
{

public:

	/// <summary>
	/// 値
	/// </summary>
	T m_Value;

	/// <summary>
	/// *演算子のオーバーライド
	/// </summary>
	T& operator *(); 
};

} } }

#include "FValue.inl"