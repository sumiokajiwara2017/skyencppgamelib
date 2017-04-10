namespace sky { namespace lib {

/// <summary>
/// �e���v���[�g�𗘗p�������[�e�B���e�B�Q
/// <summary>
namespace TemplateUtil
{
	/// <summary>
	/// �C�ӂ̃N���X�̏���������Ԃ��B<���Z�q��Ǝ��������ĉ������B
	/// </summary>
	template <class Type>
	inline const Type &Min(const Type &a , const Type &b)
	{
		return (a < b) ? a : b;
	}

	/// <summary>
	/// �C�ӂ̃N���X�̑傫������Ԃ��B>���Z�q��Ǝ��������ĉ������B
	/// </summary>
	template <class Type>
	inline const Type &Max(const Type &a , const Type &b)
	{
		return (a > b) ? a : b;
	}

	/// <summary>
	/// �C�ӂ̃N���X�����ւ���B
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