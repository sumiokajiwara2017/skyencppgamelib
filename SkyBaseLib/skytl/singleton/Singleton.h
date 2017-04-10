#pragma once

namespace sky { namespace lib {

/// <summary>
///【 Singletonクラスの概要 】
/// <newpara>
/// シングルトンクラスのフレンドクラス
/// </newpara>
/// </summary>
/// <remarks>
///【 Singletonクラスの解説 】
/// <newpara>
/// シングルトンでクラスを作成したい際にこのクラスをフレンドクラスにすることにより
/// シングルトンの機能を実装する
/// </newpara>
/// </remarks>
template< typename T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class Singleton : public ThreadingModel
{

public:

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	inline void CreateInstance();

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	inline T* GetInstance();

	/// <summary>
	/// インスタンスの削除
	/// </summary>
	inline void DeleteInstance();

protected:

	/// <summary>
	/// インスタンス保持用
	/// </summary>
	T *m_pInstance;

};

template< typename T , typename PARAM , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SingletonParam : public ThreadingModel
{
public:

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	inline void CreateInstance( PARAM param );

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	inline T *GetInstance();

	/// <summary>
	/// インスタンスの削除
	/// </summary>
	inline void DeleteInstance();

protected:

	/// <summary>
	/// インスタンス保持用
	/// </summary>
	T *m_pInstance;
};

template< typename T >
class SingletonMT : public Singleton< T , thread::ThreadingModels::CMultiThread >
{
};

/// <summary>
///【 Singletonクラスの概要 】
/// <newpara>
/// シングルトンクラスのフレンドクラス
/// </newpara>
/// </summary>
/// <remarks>
///【 Singletonクラスの解説 】
/// <newpara>
/// シングルトンでクラスを作成したい際にこのクラスをフレンドクラスにすることにより
/// シングルトンの機能を実装する
/// </newpara>
/// </remarks>
template< typename T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SingletonPtr : public ThreadingModel
{

public:

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	inline void CreateInstance();

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	inline const SmartPointer< T > &GetInstance();

	/// <summary>
	/// インスタンスの削除
	/// </summary>
	inline void DeleteInstance();

protected:

	/// <summary>
	/// インスタンス保持用
	/// </summary>
	SmartPointer< T > m_spInstance;

};

} }

#include "Singleton.inl"
