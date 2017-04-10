#pragma once

namespace sky { namespace lib { namespace graphic {


//-------------------------------------< ���� >---------------------------------------------//

class CGraphicLibController : public base::SkyRefObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize			();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose				();

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
	CGraphicLibController(){};															//�R���X�g���N�^
	CGraphicLibController( const CGraphicLibController& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CGraphicLibController(){};													//�f�X�g���N�^
	CGraphicLibController &operator=(const CGraphicLibController& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

#define SkyGraphicLibControllerInitialize_()	sky::lib::graphic::CGraphicLibController::Initialize()
#define SkyGraphicLibController_				sky::lib::graphic::CGraphicLibController
#define SkyGraphicLibControllerDispose_()		sky::lib::graphic::CGraphicLibController::Dispose()

} } }