#pragma once

namespace sky { namespace lib { namespace serialize {

class CSerializeInterface;
typedef SmartPointer< CSerializeInterface >	CSerializeInterfacePtr;			//�X�}�[�g�|�C���^�^��`
typedef CHash< CSerializeInterfacePtr >	    CSerializeInterfacePtrHash;		//�X�}�[�g�|�C���^���X�g�^

/// <summary>
/// �N���X�̃V���A���C�Y�C���^�[�t�F�[�X
/// </summary>
class CSerializeInterface : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSerializeInterface() : 
	  m_spIsActivated( NEW__( skyBool , skyBool ) )
	  {
		  *m_spIsActivated = skyFalse;
	  }

	/// <summary>
	/// �������ς݃t���O�i���L����ꍇ���l���ăX�}�[�g�|�C���^�Ŏ����Ă���j
	/// </summary>
	skyBoolPtr m_spIsActivated;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CSerializeInterface(){}

	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream ) = 0;
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream ) = 0;

	/// <summary>
	/// �R�s�[������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	virtual const CSerializeInterfacePtr &Copy( const CSerializeInterfacePtr &spContainer ) = 0;

	/// <summary>
	/// ���ׂĂ��R�s�[������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// ���L������͓̂����ŋ��L����
	/// </summary>
	virtual const CSerializeInterfacePtr &CopySharing( const CSerializeInterfacePtr &spContainer ) = 0;

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	virtual CSerializeInterfacePtr 	Clone() = 0;

	/// <summary>
	/// ���ׂĂ𕡐�������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// ���L������͓̂����ŋ��L����
	/// </summary>
	virtual CSerializeInterfacePtr 	CloneSharing() = 0;

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	virtual void Activate(){ *m_spIsActivated = skyTrue; }

#define ACTIVATE_COMMON() \
	if ( *m_spIsActivated == skyTrue ) return; \
	this->serialize::CSerializeInterface::Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	virtual void Deactivate(){ *m_spIsActivated = skyFalse; }

#define DEACTIVATE_COMMON() \
	if ( *m_spIsActivated == skyFalse ) return; \
	this->serialize::CSerializeInterface::Deactivate();

};

} } }