#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

//�m�[�h�X�^�b�N�̌^
class SqVmObject; //���X�g�錾�̂��߂̐�s�錾
typedef SmartPointer< SqVmObject >			SqVmObjectPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , SqVmObjectPtr >		SqVmObjectPtrList;	//���X�g��`
typedef CStack< SqVmObjectPtr >				SqVmObjectPtrStack;	

class AbsBindClass;

/// <summary>
/// Squirrel�o�[�`�����}�V���N���X�iComposite�p�^�[���j
/// </summary>
class SqVmObject : public base::SkyRefObject
{
public:
	/// <summary>
	/// �R���[�`���X�e�[�^�X
	/// </summary>
	typedef enum
	{
		eThreadStatus_Wait = 0 ,	//�ҋ@
		eThreadStatus_Init ,		//������
		eThreadStatus_Execute , 	//���s��
		eThreadStatus_Term ,		//������
		eThreadStatus_End ,			//���s�I��
	} eThreadStatus;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SqVmObject							( u32 stackSize );
	SqVmObject							( SqVmObjectPtr &psParent , u32 stackSize );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SqVmObject					();

	/// <summary>
	/// getter/setter
	/// </summary>
	eThreadStatus	GetASyncStatus	();

	/// <summary>
	/// �q�̂u�l�i���̓X���b�h�j���쐬���āA�e�̂u�l�̃X�^�b�N�ɐς�ŕԂ��B
	/// </summary>
	SqVmObjectPtr &CreateAndRegisterChildVm	( u32 stackSize , SqVmObjectPtr &spParent );

	/// <summary>
	/// �q�̍폜��Stack����Pop�i���������Pop���Ȃ��j
	/// </summary>
	void ChildPop					();
	void ChildAllPop				();

	/// <summary>
	/// �q���S�đ������������s���I�����Ă��邩�H�A�q�͋��Ȃ����H
	/// </summary>
	skyBool IsAllChildEnd			();

	/// <summary>
	/// �e�̎擾
	/// </summary>
	SqVmObjectPtr &Parent					();

	/// <summary>
	/// nut�t�@�C���̃X�N���v�g���u�l�ɓo�^����B�i�����炭�֐��̓N���[�W���[�Ƃ��ēo�^����A�ϐ��錾�̓O���[�o���ϐ��Ƃ��ēo�^����A�֐����s�͎��s�����j
	/// </summary>
	void RegisterScriptFile				( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// function�������s�i�Ō�܂łP��Ŏ��s������̂ŃX�^�b�N�̈ʒu�͎��s�O�Ǝ��s��œ����B�j
	/// </summary>
	eThreadStatus ExecuteFuncSync	( const hash::CHashStringPtr &spFunc, SqFuncParam &inParam , SqFuncParam &outParam );

	/// <summary>
	/// function�񓯊����s(suspend�̖߂�l��outParam�ɓ����Ă���j
	/// </summary>
	void InitExecuteFuncASync		();
	eThreadStatus ExecuteFuncASync	( const hash::CHashStringPtr &pFunc, SqFuncParam &inParam ,  SqFuncParam &outParam );
	skyBool	IsExecuteFuncASyncEnd	(){ return ( m_Status == eThreadStatus_End ); };

	/// <summary>
	/// class�̐ݒ�
	/// </summary>
	void RegisterClass					( const SqClassObject &classObj );

	/// <summary>
	/// C++�֐��̐ݒ�
	/// </summary>
	void RegisterNativeFunc( SQFUNCTION f , const skyString *fname );

	/// <summary>
	/// �X�N���v�g�̎��s�i������Ƃ��ăX�N���v�g���󂯎����s����j
	/// </summary>
	void RegisterScriptString( const hash::CHashStringPtr &pScript );

	/// <summary>
	/// SquirrelVm�̎擾
	/// </summary>
	HSQUIRRELVM &Vm();

	/// <summary>
	/// �q��VM�擾
	/// </summary>
	HSQUIRRELVM GetChildVm( u32 childNo );

	/// <summary>
	/// SkyBaseLib�̃N���X��o�^����
	/// </summary>
	void SetSkyBaseLibClass();

	/// <summary>
	/// �w���nut�X�g���[�����R���p�C�����Č��ʂ��I�u�W�F�N�g�ŕԂ�
	/// </summary>
	SqObjectInterfacePtr Compile( const stream::IStreamPtr &pSrc );

	/// <summary>
	/// �w���nut�X�g���[�����R���p�C�����Č��ʂ��X�g���[���ɏo�͂���
	/// </summary>
	void CompileOutStream( const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest );

	/// <summary>
	/// �P�s���ƂɃR�[���o�b�N��Ԃ��f�o�b�O�@�\���n�m�^�n�e�e����
	/// </summary>
	void RegisterNativeDebugHook( SQDEBUGHOOK debughook );  //�b����̊֐����P�s���ƂɃR�[���o�b�N����
	void RegisterDebugHook();							   //�X�^�b�N�̈�ԏオ�N���[�W���[�ł��邱�Ƃ�O��ɁA���̃N���[�W���[���P�s���ɃR�[���o�b�N����
	void SetDebug( skyBool isOn );				     	//�f�o�b�O���n�e�e�ɂ���

private:

	/// <summary>
	/// �e�̎Q��
	/// </summary>
	SqVmObjectPtr							m_spParent;

	/// <summary>
	/// �q�̎Q�ƃX�^�b�N
	/// </summary>
	SqVmObjectPtrStack					m_ChildStack;

	/// <summary>
	/// �o�[�`�����}�V��
	/// </summary>
	HSQUIRRELVM						m_Vm;

	/// <summary>
	/// �֐��Ăяo���O�̂u�l�X�^�b�N�s����
	/// </summary>
	u32								m_FuncStartVmStackTop;

	/// <summary>
	/// �񓯊����s���̎��s�X�e�[�^�X
	/// </summary>
	eThreadStatus					m_Status;

	/// <summary>
	/// �֐��X�^�[�g�X�^�b�N�ʒu��Save/Load�i���J�̕K�v�Ȃ��j
	/// </summary>
	void SaveFuncStartVmStackTop();
	void LoadFuncStartVmStackTop();

	/// <summary>
	/// �X�e�[�^�X�ݒ�
	/// </summary>
	void	SetASyncStatus( eThreadStatus eStatus );
};

} } }

#endif