#pragma once

namespace sky { namespace lib {

/// <summary>
///�y Singleton�N���X�̊T�v �z
/// <newpara>
/// �V���O���g���N���X�̃t�����h�N���X
/// </newpara>
/// </summary>
/// <remarks>
///�y Singleton�N���X�̉�� �z
/// <newpara>
/// �V���O���g���ŃN���X���쐬�������ۂɂ��̃N���X���t�����h�N���X�ɂ��邱�Ƃɂ��
/// �V���O���g���̋@�\����������
/// </newpara>
/// </remarks>
template< typename T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class Singleton : public ThreadingModel
{

public:

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	inline void CreateInstance();

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	inline T* GetInstance();

	/// <summary>
	/// �C���X�^���X�̍폜
	/// </summary>
	inline void DeleteInstance();

protected:

	/// <summary>
	/// �C���X�^���X�ێ��p
	/// </summary>
	T *m_pInstance;

};

template< typename T , typename PARAM , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SingletonParam : public ThreadingModel
{
public:

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	inline void CreateInstance( PARAM param );

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	inline T *GetInstance();

	/// <summary>
	/// �C���X�^���X�̍폜
	/// </summary>
	inline void DeleteInstance();

protected:

	/// <summary>
	/// �C���X�^���X�ێ��p
	/// </summary>
	T *m_pInstance;
};

template< typename T >
class SingletonMT : public Singleton< T , thread::ThreadingModels::CMultiThread >
{
};

/// <summary>
///�y Singleton�N���X�̊T�v �z
/// <newpara>
/// �V���O���g���N���X�̃t�����h�N���X
/// </newpara>
/// </summary>
/// <remarks>
///�y Singleton�N���X�̉�� �z
/// <newpara>
/// �V���O���g���ŃN���X���쐬�������ۂɂ��̃N���X���t�����h�N���X�ɂ��邱�Ƃɂ��
/// �V���O���g���̋@�\����������
/// </newpara>
/// </remarks>
template< typename T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SingletonPtr : public ThreadingModel
{

public:

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	inline void CreateInstance();

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	inline const SmartPointer< T > &GetInstance();

	/// <summary>
	/// �C���X�^���X�̍폜
	/// </summary>
	inline void DeleteInstance();

protected:

	/// <summary>
	/// �C���X�^���X�ێ��p
	/// </summary>
	SmartPointer< T > m_spInstance;

};

} }

#include "Singleton.inl"
