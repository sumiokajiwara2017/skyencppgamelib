#pragma once

namespace sky { namespace lib { namespace math {

class CollisionData;
typedef CollisionData*	                CollisionDataPtr;
typedef CList< u32 , CollisionDataPtr >	CollisionDataPtrList;

//衝突応答情報
struct CollisionResultInfo
{
	CBasicVector3 m_CrossPos;    //当った位置
	CBasicVector3 m_CrossDir;    //当った方向
	CBasicVector3 m_CrossNorm;   //当った法線
	CBasicVector3 m_CrossCenter; //当った場所(offの居るべき位置)
	dectype       m_Distance;    //距離

	//コンストラクタ
	CollisionResultInfo() : m_CrossPos( FLT_MAX , FLT_MAX , FLT_MAX ) ,
		                    m_CrossDir( FLT_MAX , FLT_MAX , FLT_MAX ) ,
							m_CrossNorm( FLT_MAX , FLT_MAX , FLT_MAX ) ,
							m_CrossCenter( FLT_MAX , FLT_MAX , FLT_MAX ) ,
							m_Distance( FLT_MAX )
	{}
};

//---------------------------------------------------------------------------------------

/// <summary>
/// ジオメトリ取得デリゲート
/// </summary>
struct IGeometoryGetDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// コールバックインタフェース
	/// </summary>
	virtual const CGeometryData* operator()() = 0;
};
typedef SmartPointer< IGeometoryGetDelegateObject >	IGeometoryGetDelegateObjectPtr;

/// <summary>
/// ジオメトリ取得デリゲート生成アダプタ
/// </summary>
template< typename T >
class CGeometoryGetDelegateAdaptor : public IGeometoryGetDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef const CGeometryData* ( T::*FuncType )();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGeometoryGetDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc( pFunc ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CGeometoryGetDelegateAdaptor(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual const CGeometryData* operator()()
	{
		return ( m_pObject->*m_pFunc )();
	}
	
private:

	/// <summary>
	/// クラスインスタンスの参照保持
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType m_pFunc;
};
class CGeometoryGetDelegateAdaptorFunc : public IGeometoryGetDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef const CGeometryData* ( * FuncType )();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGeometoryGetDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc( pFunc ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CGeometoryGetDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual const CGeometryData* operator()()
	{
		return ( m_pFunc )();
	}
	
private:

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType m_pFunc;
};


//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// 衝突判定実行判定デリゲート
/// </summary>
struct IExecDetectionDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// コールバックインタフェース
	/// </summary>
	virtual const skyBool operator()( const CollisionDataPtr &pData ) = 0;
};
typedef SmartPointer< IExecDetectionDelegateObject > IExecDetectionDelegateObjectPtr;

/// <summary>
/// 衝突判定実行判定デリゲート生成アダプタ
/// </summary>
template< typename T >
class CExecDetectionDelegateAdaptor : public IExecDetectionDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef const skyBool ( T::*FuncType )( const CollisionDataPtr &pData );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CExecDetectionDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc( pFunc ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CExecDetectionDelegateAdaptor(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual const skyBool operator()( const CollisionDataPtr &pData )
	{
		return ( m_pObject->*m_pFunc )( pData );
	}
	
private:

	/// <summary>
	/// クラスインスタンスの参照保持
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType m_pFunc;
};
class CExecDetectionDelegateAdaptorFunc : public IExecDetectionDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef const skyBool ( * FuncType )( const CollisionDataPtr &pData );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CExecDetectionDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc( pFunc ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CExecDetectionDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual const skyBool operator()( const CollisionDataPtr &pData )
	{
		return ( m_pFunc )( pData );
	}
	
private:

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType m_pFunc;
};

//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// 衝突発生コールバックデリゲート
/// </summary>
struct ICollisionCBDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// コールバックインタフェース
	/// </summary>
	virtual void operator()( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo = skyNull ) = 0;
};
typedef SmartPointer< ICollisionCBDelegateObject >	ICollisionCBDelegateObjectPtr;

/// <summary>
/// 衝突発生コールバックデリゲート生成アダプタ
/// </summary>
template< typename T >
class CollisionCBDelegateAdaptor : public ICollisionCBDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef void ( T::*FuncType )( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CollisionCBDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc( pFunc ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CollisionCBDelegateAdaptor(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual void operator()( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo = skyNull )
	{
		( m_pObject->*m_pFunc )( pGeometry , pData , pResultInfo );
	}
	
private:

	/// <summary>
	/// クラスインスタンスの参照保持
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType m_pFunc;
};
class CollisionCBDelegateAdaptorFunc : public ICollisionCBDelegateObject
{

public:

	/// <summary>
	/// 格納関数の型
	/// </summary>
	typedef void ( * FuncType )( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CollisionCBDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc( pFunc ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CollisionCBDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()演算子のオーバーロード
	/// </summary>
	virtual void operator()( const CGeometryData *pGeometry , const CollisionData *pData , CollisionResultInfo *pResultInfo = skyNull )
	{
		( m_pFunc )( pGeometry , pData , pResultInfo );
	}
	
private:

	/// <summary>
	/// 関数の参照保持
	/// </summary>
	FuncType m_pFunc;
};

} } }

