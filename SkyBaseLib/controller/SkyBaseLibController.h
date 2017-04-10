#pragma once

namespace sky { namespace lib { namespace thread { class CriticalSection; } } }


namespace sky { namespace lib { namespace controller {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���C�u�����R���g���[��
/// </summary>
class CBaseLibController : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize			( u32 playerNum = 1 );

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose				();

	/// <summary>
	/// �|��
	/// </summary>
	static void GC					();

	/// <summary>
	/// �������ςݔ���
	/// <summary>
	static void InitAssert          ();
	static skyBool IsInit           ();

private:

	/// <summary>
	/// �N���e�B�J���Z�N�V�����擾
	/// </summary>
	static thread::CriticalSection *GetCriticalSection();

	/// <summary>
	/// �N���e�B�J���Z�N�V����
	/// </summary>
	static thread::CriticalSection *m_pCs;

	/// <summary>
	/// �������t���O
	/// </summary>
	static skyBool m_bIsInitialized;

	//�e�@�\����
	CBaseLibController();															//�R���X�g���N�^
	CBaseLibController( const CBaseLibController& ){};								//�R�s�[�R���X�g���N�^
	virtual ~CBaseLibController();													//�f�X�g���N�^
	CBaseLibController &operator=(const CBaseLibController& ){ return *this; };		//������Z�q�I�[�o�[���[�h

};

} } }

//�C���X�^���X�A�N�Z�X������
#define SkyBaseLibControllerInitialize_( playerNo )		sky::lib::controller::CBaseLibController::Initialize( playerNo )
#define SkyBaseLibController_							sky::lib::controller::CBaseLibController
#define SkyBaseLibControllerDispose_()					sky::lib::controller::CBaseLibController::Dispose()
