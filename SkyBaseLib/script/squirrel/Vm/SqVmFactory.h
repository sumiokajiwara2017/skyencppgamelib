#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// Squirrel�t�@�N�g���[�iFactory�p�^�[���j
/// </summary>
class SqVmFactory : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SqVmFactory();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SqVmFactory();

	/// <summary>
	/// �쐬
	/// </summary>
	SqVmObjectPtr Create( u32 stackSize );
	SqVmObjectPtr Create( SqVmObjectPtr &sqParentVmPtr , u32 stackSize );

	/// <summary>
	/// �폜
	/// </summary>
	void	Delete( SqVmObjectPtr &sqVmPtr );

	/// <summary>
	/// �V���O���g���@�\�t��
	/// <summary>
	friend class Singleton< SqVmFactory >;
    static Singleton< SqVmFactory > SingletonMember;

private:

	/// <summary>
	/// �C���X�^���X���X�g
	/// </summary>
	SqVmObjectPtrList		m_InstanceList;

};

} } }

#define SqVmFactoryCreate_()			( sky::lib::squirrel::SqVmFactory::SingletonMember.CreateInstance() )
#define SqVmFactory_					( sky::lib::squirrel::SqVmFactory::SingletonMember.GetInstance() )
#define SqVmFactoryDelete_()			( sky::lib::squirrel::SqVmFactory::SingletonMember.DeleteInstance() )

#endif
