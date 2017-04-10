#pragma once

namespace sky { namespace lib { namespace base {

/// <summary>
/// �������Ǘ��p�N���X
/// </summary>
class SkyRefObject : public SkyObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline SkyRefObject();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SkyRefObject();

	/// <summary>
	/// �Q�ƃJ�E���^�擾�^�ݒ�
	/// </summary>
	inline u32  GetRefCnt() const;
	inline void SetRefCnt( u32 cnt );

	/// <summary>
	/// �Q�ƃJ�E���^�t�o
	/// </summary>
	inline u32 RefCntUp();

	/// <summary>
	/// �Q�ƃJ�E���^�c�n�v�m
	/// </summary>
	inline skyBool RefCntDown();

private:

	/// <summary>
	/// �Q�ƃJ�E���^
	/// </summary>
	u32 m_nRefCnt;

};

} } }

#include "SkyRefObject.inl"