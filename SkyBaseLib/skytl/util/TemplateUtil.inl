namespace sky { namespace lib {

/// <summary>
/// テンプレートを利用したユーティリティ群
/// <summary>
namespace TemplateUtil
{
	/// <summary>
	/// 任意のクラスの小さい方を返す。<演算子を独自実装して下さい。
	/// </summary>
	template <class Type>
	inline const Type &Min(const Type &a , const Type &b)
	{
		return (a < b) ? a : b;
	}

	/// <summary>
	/// 任意のクラスの大きい方を返す。>演算子を独自実装して下さい。
	/// </summary>
	template <class Type>
	inline const Type &Max(const Type &a , const Type &b)
	{
		return (a > b) ? a : b;
	}

	/// <summary>
	/// 任意のクラスを入れ替える。
	/// <summary>
	template <class Type>
	inline void Swap(Type &a , Type &b)
	{
		Type tmp(a);
		a = b;
		b = tmp;
	}

	template < class Type >
	inline Type Clamp( Type value , Type min , Type max )
	{
		SKY_ASSERT( min <= max );
		value = Min< Type >( value, min );
		value = Max< Type >( value, max );
		return value;
	}

};

} }