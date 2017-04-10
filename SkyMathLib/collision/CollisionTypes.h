#pragma once

namespace sky { namespace lib { namespace math {

class CollisionData;
typedef CollisionData*	                CollisionDataPtr;
typedef CList< u32 , CollisionDataPtr >	CollisionDataPtrList;

//�Փˉ������
struct CollisionResultInfo
{
	CBasicVector3 m_CrossPos;    //�������ʒu
	CBasicVector3 m_CrossDir;    //����������
	CBasicVector3 m_CrossNorm;   //�������@��
	CBasicVector3 m_CrossCenter; //�������ꏊ(off�̋���ׂ��ʒu)
	dectype       m_Distance;    //����

	//�R���X�g���N�^
	CollisionResultInfo() : m_CrossPos( FLT_MAX , FLT_MAX , FLT_MAX ) ,
		                    m_CrossDir( FLT_MAX , FLT_MAX , FLT_MAX ) ,
							m_CrossNorm( FLT_MAX , FLT_MAX , FLT_MAX ) ,
							m_CrossCenter( FLT_MAX , FLT_MAX , FLT_MAX ) ,
							m_Distance( FLT_MAX )
	{}
};

//---------------------------------------------------------------------------------------

/// <summary>
/// �W�I���g���擾�f���Q�[�g
/// </summary>
struct IGeometoryGetDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R�[���o�b�N�C���^�t�F�[�X
	/// </summary>
	virtual const CGeometryData* operator()() = 0;
};
typedef SmartPointer< IGeometoryGetDelegateObject >	IGeometoryGetDelegateObjectPtr;

/// <summary>
/// �W�I���g���擾�f���Q�[�g�����A�_�v�^
/// </summary>
template< typename T >
class CGeometoryGetDelegateAdaptor : public IGeometoryGetDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef const CGeometryData* ( T::*FuncType )();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGeometoryGetDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc( pFunc ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CGeometoryGetDelegateAdaptor(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual const CGeometryData* operator()()
	{
		return ( m_pObject->*m_pFunc )();
	}
	
private:

	/// <summary>
	/// �N���X�C���X�^���X�̎Q�ƕێ�
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType m_pFunc;
};
class CGeometoryGetDelegateAdaptorFunc : public IGeometoryGetDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef const CGeometryData* ( * FuncType )();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGeometoryGetDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc( pFunc ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CGeometoryGetDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual const CGeometryData* operator()()
	{
		return ( m_pFunc )();
	}
	
private:

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType m_pFunc;
};


//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// �Փ˔�����s����f���Q�[�g
/// </summary>
struct IExecDetectionDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R�[���o�b�N�C���^�t�F�[�X
	/// </summary>
	virtual const skyBool operator()( const CollisionDataPtr &pData ) = 0;
};
typedef SmartPointer< IExecDetectionDelegateObject > IExecDetectionDelegateObjectPtr;

/// <summary>
/// �Փ˔�����s����f���Q�[�g�����A�_�v�^
/// </summary>
template< typename T >
class CExecDetectionDelegateAdaptor : public IExecDetectionDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef const skyBool ( T::*FuncType )( const CollisionDataPtr &pData );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CExecDetectionDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc( pFunc ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CExecDetectionDelegateAdaptor(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual const skyBool operator()( const CollisionDataPtr &pData )
	{
		return ( m_pObject->*m_pFunc )( pData );
	}
	
private:

	/// <summary>
	/// �N���X�C���X�^���X�̎Q�ƕێ�
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType m_pFunc;
};
class CExecDetectionDelegateAdaptorFunc : public IExecDetectionDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef const skyBool ( * FuncType )( const CollisionDataPtr &pData );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CExecDetectionDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc( pFunc ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CExecDetectionDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual const skyBool operator()( const CollisionDataPtr &pData )
	{
		return ( m_pFunc )( pData );
	}
	
private:

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType m_pFunc;
};

//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// �Փ˔����R�[���o�b�N�f���Q�[�g
/// </summary>
struct ICollisionCBDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R�[���o�b�N�C���^�t�F�[�X
	/// </summary>
	virtual void operator()( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo = skyNull ) = 0;
};
typedef SmartPointer< ICollisionCBDelegateObject >	ICollisionCBDelegateObjectPtr;

/// <summary>
/// �Փ˔����R�[���o�b�N�f���Q�[�g�����A�_�v�^
/// </summary>
template< typename T >
class CollisionCBDelegateAdaptor : public ICollisionCBDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef void ( T::*FuncType )( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CollisionCBDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc( pFunc ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CollisionCBDelegateAdaptor(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual void operator()( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo = skyNull )
	{
		( m_pObject->*m_pFunc )( pGeometry , pData , pResultInfo );
	}
	
private:

	/// <summary>
	/// �N���X�C���X�^���X�̎Q�ƕێ�
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType m_pFunc;
};
class CollisionCBDelegateAdaptorFunc : public ICollisionCBDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef void ( * FuncType )( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CollisionCBDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc( pFunc ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CollisionCBDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual void operator()( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo = skyNull )
	{
		( m_pFunc )( pGeometry , pData , pResultInfo );
	}
	
private:

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType m_pFunc;
};

} } }

