#pragma once

namespace sky { namespace lib {

/// <summary>
/// �e���v���[�g�𗘗p�������[�e�B���e�B�Q
/// <summary>
namespace TemplateUtil
{
	/// <summary>
	/// �C�ӂ̃N���X�̏���������Ԃ��B<���Z�q��Ǝ��������ĉ������B
	/// </summary>
	template < class Type >
	inline const Type &Min( const Type &a , const Type &b );

	/// <summary>
	/// �C�ӂ̃N���X�̑傫������Ԃ��B>���Z�q��Ǝ��������ĉ������B
	/// </summary>
	template < class Type >
	inline const Type &Max( const Type &a , const Type &b );

	/// <summary>
	/// �C�ӂ̃N���X�����ւ���B
	/// <summary>
	template < class Type >
	inline void Swap( Type &a , Type &b );
};

} }

#include "TemplateUtil.inl"