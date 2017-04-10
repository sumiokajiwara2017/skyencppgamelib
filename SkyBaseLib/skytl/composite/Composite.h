#pragma once

namespace sky { namespace lib {

/// <summary>
/// �֌W�����
/// </summary>
enum eRelationKind
{
	eRelationKind_Branch = 0 ,
	eRelationKind_Leaf ,
	eRelationKind_Child ,
	eRelationKind_Root ,
};

/// <summary>
/// Composite�p�^�[���̃e���v���[�g
/// ���̃N���X���p������ꍇ�ARTTI���������Ă��������B���L�L�q��cpp���ɋL�q���Ă��������B
/// SkyImplementRTTI( sky::lib::CComposite< hoge > , sky::lib::base::SkyRefObject );
/// </summary>
template< class T , typename LIST_KEY_TYPE = u32 >
class CComposite : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CComposite();
	inline CComposite( T pParent );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CComposite();

	/// <summary>
	/// �q�̊֘A�t��
	/// </summary>
	inline T AttachChild( LIST_KEY_TYPE key, const T &child );

	/// <summary>
	/// �q�̊֘A�t��
	/// </summary>
	inline T AttachChild( const T &child );
	inline T AttachChildFront( const T &child );

	/// <summary>
	/// �q�̊֘A�t������
	/// </summary>
	inline void DetachChild( const T &child );

	/// <summary>
	/// �S�Ă̎q�̊֘A�t������
	/// </summary>
	inline void DetachAllChild();

	/// <summary>
	/// �t�̊֘A�t��
	/// </summary>
	inline T AttachLeaf( LIST_KEY_TYPE key, const T &leaf );

	/// <summary>
	/// �t�̊֘A�t��
	/// </summary>
	inline T AttachLeaf( const T &leaf );
	inline T AttachLeafFront( const T &leaf );

	/// <summary>
	/// �t�̊֘A�t������
	/// </summary>
	inline void DetachLeaf( const T &leaf );

	/// <summary>
	/// �S�Ă̗t�̊֘A�t������
	/// </summary>
	inline void DetachAllLeaf();

	/// <summary>
	/// �}�̊֘A�t��
	/// </summary>
	inline T AttachBranch( const T &spLeaf );

	/// <summary>
	/// �}�̊֘A�t������
	/// </summary>
	inline void DetachBranch();

	/// <summary>
	/// �}�̎擾
	/// </summary>
	inline const T& GetBranch();

	/// <summary>
	/// �q�̎擾
	/// </summary>
	inline T Child( LIST_KEY_TYPE no );

	/// <summary>
	/// �e�̎擾
	/// </summary>
	inline T Parent();

	/// <summary>
	/// �e�̊֘A�t��
	/// </summary>
	inline void AttachParent( T pParent );

	/// <summary>
	/// �e�̊֘A�t������
	/// </summary>
	inline void DetachParent();

	/// <summary>
	/// �ŏ�ʂ��ǂ������肷��
	/// </summary>
	inline skyBool IsRoot();

	/// <summary>
	/// �q�����݂��邩�m�F����
	/// </summary>
	inline skyBool IsChildOn();

	/// <summary>
	/// �q�̃��X�g��Ԃ�
	/// </summary>
	CList< LIST_KEY_TYPE , T > &ChildList();

	/// <summary>
	/// �t�̃��X�g��Ԃ�
	/// </summary>
	CList< LIST_KEY_TYPE , T > &LeafList();

protected:

	/// <summary>
	/// �e�̎Q��
	/// </summary>
	T							m_pParent;
	skyBool						m_IsParentOn;

	/// <summary>
	/// �q�̎Q�ƃ��X�g
	/// </summary>
	typedef CList< LIST_KEY_TYPE , T > CCompositeList;
	CCompositeList	m_ChildList;

	/// <summary>
	/// �t�̎Q�ƃ��X�g
	/// </summary>
	CCompositeList	m_LeafList;

	/// <summary>
	/// �}
	/// </summary>
	T 	           m_Branch;
};

} }

#include "Composite.inl"