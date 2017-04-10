#pragma once

namespace sky { namespace lib { namespace framework {


//-------------------------------------< ���� >---------------------------------------------//

class CFrameWorkLibController : public base::SkyRefObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize			();
	static void InitializePlatform	();	//�v���b�g�t�H�[���Ǝ�

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose				();
	static void DisposePlatform		();

	/// <summary>
	/// �������ςݔ���
	/// <summary>
	static void InitAssert			();

	/// <summary>
	/// �X�V
	/// <summary>
	static void Update				( updateTime time );

	/// <summary>
	/// �t���X�N���[�����H
	/// <summary>
	static skyBool	IsFullScreen	()			{ return m_bIsFullScreen; };
	static void	SetFullScreen	( skyBool isFull )	{ m_bIsFullScreen = isFull; };

private:

	/// <summary>
	/// �t���X�N���[���t���O
	/// </summary>
	static skyBool		m_bIsFullScreen;

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
	CFrameWorkLibController(){};															//�R���X�g���N�^
	CFrameWorkLibController( const CFrameWorkLibController& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CFrameWorkLibController(){};													//�f�X�g���N�^
	CFrameWorkLibController &operator=(const CFrameWorkLibController& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

#define SkyFrameWorkLibControllerInitialize_()	sky::lib::framework::CFrameWorkLibController::Initialize()
#define SkyFrameWorkLibController_				sky::lib::framework::CFrameWorkLibController
#define SkyFrameWorkLibControllerDispose_()		sky::lib::framework::CFrameWorkLibController::Dispose()

} } }