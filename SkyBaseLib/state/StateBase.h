#pragma once

namespace sky { namespace lib { namespace state {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

/// <summary>
/// �X�e�[�g�N���X
/// </summary>
class IState : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �J�n
	/// </summary>
	virtual void vEnter   () = 0;

	/// <summary>
	/// �I��
	/// </summary>
	virtual void vExit    () = 0;

	/// <summary>
	/// ���f(���������State���悹��ꂽ���ɌĂ΂��B�����͏悹��ꂽ�X�e�[�g�̖��O�j
	/// </summary>
	virtual void vSuspend ( const IStatePtr overState ) = 0;

	/// <summary>
	/// �ĊJ(���������State�����ꂽ���Ă΂��B�����͏悹���Ă����X�e�[�g�̖��O�j
	/// </summary>
	virtual void vResume  ( const IStatePtr &overState ) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void vUpdate    ( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// ���O�擾
	/// </summary>
	virtual const hash::CHashStringPtr &GetName() = 0;

	/// <summary>
	/// ���������s�����ǂ������肷��
	/// </summary>
	virtual skyBool vIsActive() = 0;

	/// <summary>
	/// �Ǘ��̊֘A�t���^�֘A�t�������^�擾
	/// </summary>
	virtual void vAttatchManager( const CStateManagerPtr &spManager ) = 0;
	virtual void vDetachManager() = 0;
	virtual const CStateManagerPtr &vGetManager() = 0;

	/// <summary>
	/// �Ǘ����g�p�i���[�U�[�͎g�p���Ȃ��ł��������j
	/// </summary>
	virtual void vSetActive( skyBool isActive ) = 0;
};

//-----------------------------------< ���ێ��� >-------------------------------------------//

class AbsState : public IState
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^�i���O�K�{�j
	/// </summary>
	AbsState( const hash::CHashStringPtr &name )
	{
		m_Name = name;
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~AbsState(){};

	/// <summary>
	/// ���O�̎擾
	/// </summary>
	const hash::CHashStringPtr &GetName(){ return m_Name; }

	/// <summary>
	/// �Ǘ��̐ݒ�擾
	/// </summary>
	void vAttatchManager( const CStateManagerPtr &spManager ) { m_spManager = spManager; }
	void vDetachManager()                                     { m_spManager.Delete(); }
	const CStateManagerPtr &vGetManager()                     { return m_spManager; }

	/// <summary>
	/// ���z�֐��̎����v��(�������������邽�߂ɂ����Ŏ����������j
	/// </summary>
	virtual void vEnter   (){}
	virtual void vExit    (){}
	virtual void vSuspend ( const lib::state::IStatePtr overState ){ SKY_UNUSED_ARG( overState ); }
	virtual void vResume  ( const lib::state::IStatePtr &overState ){ SKY_UNUSED_ARG( overState ); }
	virtual void vUpdate  ( const fps::CFpsController &fps ){ SKY_UNUSED_ARG( fps ); }
	virtual skyBool vIsActive(){ return m_IsActive; }
	virtual void vSetActive( skyBool isActive ){ m_IsActive = isActive; }

protected:

	/// <summary>
	/// �����̊Ǘ���
	/// </summary>
	CStateManagerPtr        m_spManager;

private:

	/// <summary>
	/// ���O
	/// </summary>
	hash::CHashStringPtr	m_Name;

	/// <summary>
	/// ���s���t���O
	/// </summary>
	skyBool                 m_IsActive;

};

} } }
