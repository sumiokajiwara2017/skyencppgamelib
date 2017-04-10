#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

namespace sqclassobject
{
	static const u32 MEMBER_HASH_VALUE = 16;
}

class SqClassObject; //���X�g�錾�̂��߂̐�s�錾
typedef SmartPointer< SqClassObject >			SqClassObjectPtr;		//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// Squirrel��̃N���X��C++�Ŏ擾�A�ݒ肷�邽�߂̃N���X
/// </summary>
class SqClassObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SqClassObject();
	SqClassObject( const hash::CHashStringPtr &pClassName );
	SqClassObject( const hash::CHashStringPtr &pClassName , const hash::CHashStringPtr &parentName );

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	SqClassObject		( const SqClassObject& src );

	/// <summary>
	/// �N���X��
	/// </summary>
	hash::CHashStringPtr		m_kName;

	/// <summary>
	/// �e�N���X
	/// </summary>
	hash::CHashStringPtr		m_ParentName;

	/// <summary>
	/// �����o�[�ϐ���SqParamObjectList�ŕ\������
	/// </summary>
	SqParamObjectHash m_MemberHash;

	/// <summary>
	/// C++�֐��o�C���h���\�b�h�p���\����
	/// </summary>
	SqCppFuncList		m_CppMethodList;

	/// <summary>
	/// ������Z�q�̃I�[�o�[���C�h
	/// </summary>
	SqClassObject& operator =( const SqClassObject& src );

	/// <summary>
	/// �����o�ϐ��̒ǉ�
	/// </summary>
	void AddMember(  const hash::CHashStringPtr pMemberName , const SqParamObject &param );

	/// <summary>
	/// C++�֐��o�C���h���\�b�h�̒ǉ�
	/// </summary>
	/*
	����	�^
	o	null
	i	integer
	f	float
	n	integer, float
	s	string
	t	table
	a	array
	u	userdata
	c	closure, nativeclosure
	g	generator
	p	userpointer
	v	thread
	x	instance
	y	class
	b	bool
	.	any
	*/
	void AddMethod( const hash::CHashStringPtr pMemberFunc , SQFUNCTION f , const hash::CHashStringPtr typemask = CHS( _T("") ) );

};

} } }

#endif