#pragma once

namespace sky { namespace lib { namespace hash {

/// <summary>
/// HashString�֗̕��֐�
/// </summary>
class CHashStringUtil : public base::SkyObject
{
	/// <summary>
	/// �w��̕�������w��̃L�[���[�h�ŕ������ă��X�g�ŕԂ�
	/// </summary>
	static void GeTokenList( CHashStringPtrList &dest , const skyString *pSrc , const skyString key = _T( ' ' ) );

};

} } }
