#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

//�m�[�h�X�^�b�N�̌^
class SqObjectInterface; //���X�g�錾�̂��߂̐�s�錾
typedef SmartPointer< SqObjectInterface >	SqObjectInterfacePtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , SqObjectInterfacePtr >	SqObjectInterfacePtrList;	//���X�g��`
typedef CStack< SqObjectInterfacePtr >		SqObjectInterfacePtrStack;	

/// <summary>
/// HSQOBJECT�̃C���^�t�F�[�X�B
/// Squirrel�̊�{�I�u�W�F�N�g�ł���HSQOBJECT�𒼊��I�ɑ��삷�邽�߂̃N���X
/// �����Ƃ��Ď󂯓n�����\
/// SqParamObject�Ƌ@�\�����Ă���B�Ȃ�䂫�セ���Ȃ��������Ȃ̂ŁA������SqParamObject��������������Ȃ��B
/*  �ȉ��̌^���\�Ȍ���C++���Œl��ݒ�A�擾���₷������̂����̃N���X�̎g��
	OT_NULL =			(_RT_NULL|SQOBJECT_CANBEFALSE),
	OT_INTEGER =		(_RT_INTEGER|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_FLOAT =			(_RT_FLOAT|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_BOOL =			(_RT_BOOL|SQOBJECT_CANBEFALSE),
	OT_STRING =			(_RT_STRING|SQOBJECT_REF_COUNTED),
	OT_TABLE =			(_RT_TABLE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_ARRAY =			(_RT_ARRAY|SQOBJECT_REF_COUNTED),
	OT_USERDATA =		(_RT_USERDATA|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_CLOSURE =		(_RT_CLOSURE|SQOBJECT_REF_COUNTED),
	OT_NATIVECLOSURE =	(_RT_NATIVECLOSURE|SQOBJECT_REF_COUNTED),
	OT_GENERATOR =		(_RT_GENERATOR|SQOBJECT_REF_COUNTED),
	OT_USERPOINTER =	_RT_USERPOINTER,
	OT_THREAD =			(_RT_THREAD|SQOBJECT_REF_COUNTED) ,
	OT_FUNCPROTO =		(_RT_FUNCPROTO|SQOBJECT_REF_COUNTED), //internal usage only
	OT_CLASS =			(_RT_CLASS|SQOBJECT_REF_COUNTED),
	OT_INSTANCE =		(_RT_INSTANCE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_WEAKREF =		(_RT_WEAKREF|SQOBJECT_REF_COUNTED),
	OT_OUTER =			(_RT_OUTER|SQOBJECT_REF_COUNTED) //internal usage only
*/
/// </summary>
class SqObjectInterface : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^�B�����̎Y�݂̐e�ł���VM�̎Q�Ƃ����i�F�X�ȑ���Ɏg���j
	/// isAddRefExec��skyTrue�̏ꍇ�A�����ŎQ�ƃJ�E���^���グ�A�f�X�g���N�^�ŎQ�ƃJ�E���^��������
	/// </summary>
	SqObjectInterface( HSQUIRRELVM v , HSQOBJECT *sqObj , skyBool isAddRefExec );
	SqObjectInterface( HSQUIRRELVM v ); //VM������Γ����Ő����ł���B

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SqObjectInterface();

	/// <summary>
	/// �e��I�u�W�F�N�g�𐶐�����
	/// </summary>
	void CreateSqObject( tagSQObjectType etype );

	/// <summary>
	/// �I�u�W�F�N�g�̌^���擾
	/// </summary>
	tagSQObjectType GetType();

	/// <summary>
	/// �e�^�ɃL���X�g�i�^���Ⴄ�ƃG���[�j
	/// </summary>
    operator s32 () const;
    operator f32 () const;
	operator const skyString  *() const;
	operator skyBool() const;
	operator void* () const;

	/// <summary>
	/// �ʂ�VM�ɃR�s�[����i�R�s�[���ʂ̓R�s�[���VM�̈�ԏ�ɒu�����j
	/// </summary>
	void CopyObject( HSQUIRRELVM v );

//-----------<< �z��̏ꍇ�̓��� >>-------------------------------------��
//-----------<< �e�[�u���̏ꍇ�̓��� >>---------------------------------��
//-----------<< �N���X�̏ꍇ�̓��� >>-----------------------------------��
//-----------<< �N���[�W���[�̏ꍇ�̓��� >>-----------------------------��
//-----------<< �l�C�e�B�u�N���[�W���[�̏ꍇ�̓��� >>--------------------��
//-----------<< �W�F�l���[�^�[�̏ꍇ�̓��� >>----------------------------��

private:

	/// <summary>
	/// Squirrel�̂u�l�̎Q��
	/// </summary>
	HSQUIRRELVM m_SqVm;

	/// <summary>
	/// Squirrel�̊�{�I�u�W�F�N�g�̎Q�ƁB
	/// </summary>
	HSQOBJECT *m_SqObj;

	/// <summary>
	/// �Q�ƃJ�E���^���R���X�g���N�^��UP�������H
	/// </summary>
	skyBool m_IsAddRefExec;
};

} } }

#endif