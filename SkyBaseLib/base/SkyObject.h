#pragma once

namespace sky { namespace lib { namespace base {

/// <summary>
/// �S�ẴN���X�̊��N���X
/// </summary>
class SkyObject
{
SkyTypeDefRootRTTI( SkyObject );

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SkyObject();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SkyObject();

	/// <summary>
	/// �I�u�W�F�N�g�̓��e���v�����g����B�ł��邾���������邱�ƁI
	/// </summary>
	virtual void vPrintf(){}
};

typedef SkyObject*	SkyObjectPtr;

} } }
