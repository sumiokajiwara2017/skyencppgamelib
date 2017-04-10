#pragma once

namespace sky { namespace lib {


//-----------------------------------< �֐��Q >---------------------------------------------//

/// <summary>
/// �N�����v���
/// </summary>
enum eClampLoopType
{
	eClampLoopType_None ,		// ���[�v����
	eClampLoopType_Clamp,		// �ő�E�ŏ��l�Ń��[�v
	eClampLoopType_Excess ,		// ���ߕ��v���X�Ń��[�v
};

/// <summary>
/// �N�����v�v�Z
/// </summary>
namespace ClampUtil
{
	/// <summary>
	/// �l�̃N�����v������
	/// </summary>
	template< typename T >
	inline skyBool ValueClamp	( T *pVal , T addVal , T clampMin, T clampMax , eClampLoopType loopType=eClampLoopType_Clamp );

	/// <summary>
	/// �l���s�����藈���肷��
	/// </summary>
	template< typename T >
	inline void BtweenValueMove( T *pVal , T addVal , skyBool *pIsToUp , T clampMin, T clampMax );
}

} }

#include "ClampUtil.inl"