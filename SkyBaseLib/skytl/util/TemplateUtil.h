#pragma once

namespace sky { namespace lib {

/// <summary>
/// テンプレートを利用したユーティリティ群
/// <summary>
namespace TemplateUtil
{
	/// <summary>
	/// 任意のクラスの小さい方を返す。<演算子を独自実装して下さい。
	/// </summary>
	template < class Type >
	inline const Type &Min( const Type &a , const Type &b );

	/// <summary>
	/// 任意のクラスの大きい方を返す。>演算子を独自実装して下さい。
	/// </summary>
	template < class Type >
	inline const Type &Max( const Type &a , const Type &b );

	/// <summary>
	/// 任意のクラスを入れ替える。
	/// <summary>
	template < class Type >
	inline void Swap( Type &a , Type &b );
};

} }

#include "TemplateUtil.inl"