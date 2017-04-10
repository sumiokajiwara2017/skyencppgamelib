#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

Singleton< SqVmFactory >	SqVmFactory::SingletonMember;

SqVmFactory::SqVmFactory()
{
}

SqVmFactory::~SqVmFactory()
{
	m_InstanceList.DeleteAll();
}

SqVmObjectPtr SqVmFactory::Create( u32 stackSize )
{
	//VM���쐬
	SqVmObjectPtr sqVmPtr( NEW__( SqVmObject , SqVmObject( stackSize ) ) );

	//VM���C���X�^���X���X�g�ɂȂ�
	m_InstanceList.AddTail( sqVmPtr );

	//�Ԃ�
	return sqVmPtr;
}
SqVmObjectPtr SqVmFactory::Create( SqVmObjectPtr &sqParentVmPtr , u32 stackSize )
{
	//�N���[�����g���Ď��̂��쐬
	SqVmObjectPtr sqVmPtr( NEW__( SqVmObject , SqVmObject( sqParentVmPtr , stackSize )  ) );

	//VM���C���X�^���X���X�g�ɂȂ�
	m_InstanceList.AddTail( sqVmPtr );

	//�Ԃ�
	return sqVmPtr;
}


void SqVmFactory::Delete( SqVmObjectPtr &sqVmPtr )
{
	m_InstanceList.Delete( sqVmPtr );
}

} } }

#endif
