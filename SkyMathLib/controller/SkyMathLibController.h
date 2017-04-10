#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���w���C�u��������N���X
/// </summary>
class CMathLibController : public base::SkyRefObject
{

public:

	/// <summary>
	/// ������
	/// <summary>
	static void Initialize();

	/// <summary>
	/// ������
	/// <summary>
	static void Dispose();

	/// <summary>
	/// �������ςݔ���
	/// <summary>
	static void InitAssert();

private:

	//�e�@�\����
	CMathLibController(){};													//�R���X�g���N�^
	CMathLibController( const CMathLibController& ){};								//�R�s�[�R���X�g���N�^
	virtual ~CMathLibController(){};											//�f�X�g���N�^
	CMathLibController &operator=(const CMathLibController& ){ return *this; };		//������Z�q�I�[�o�[���[�h

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
};

} } }

//�V���O���g���C���X�^���X�A�N�Z�X������
#define SkyMathLibControllerInitialize_()	sky::lib::math::CMathLibController::Initialize()
#define SkyMathLibController_				sky::lib::math::CMathLibController
#define SkyMathLibControllerDispose_()		sky::lib::math::CMathLibController::Dispose()
