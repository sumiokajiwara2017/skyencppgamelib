#pragma once

namespace sky { namespace lib { namespace comif {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// COM�C���^�[�t�F�[�X���������N���X
/// </summary>	
class ComInterfaceUtil : public base::SkyObject
{

public:

	/// <summary>
	/// COM�̏�����
	/// </summary>	
	static void Initialize();

	/// <summary>
	/// COM�̖�����
	/// </summary>	
	static void Dispose();

private:

	/// <summary>
	/// �������ς݃t���O
	/// </summary>	
	static skyBool m_bIsInitialized;

	//�e�@�\����
	ComInterfaceUtil				(){};										//�R���X�g���N�^
	ComInterfaceUtil				( const ComInterfaceUtil& ){};					//�R�s�[�R���X�g���N�^
	virtual ~ComInterfaceUtil		(){};										//�f�X�g���N�^
	ComInterfaceUtil &operator=		(const ComInterfaceUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

#define ComInterfaceUtilInitialize_() sky::lib::comif::ComInterfaceUtil::Initialize()
#define ComInterfaceUtil_             sky::lib::comif::ComInterfaceUtil
#define ComInterfaceUtilDispose_()    sky::lib::comif::ComInterfaceUtil::Dispose()


} } }