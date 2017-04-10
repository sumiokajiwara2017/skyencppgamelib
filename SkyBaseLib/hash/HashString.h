#pragma once

namespace sky { namespace lib { namespace hash {

//-------------------------------------< ���� >---------------------------------------------//

template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointerHashString : public SmartPointer< T , ThreadingModel >
{

public :

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	inline explicit SmartPointerHashString( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���^�����R�s�[�j
	/// </summary>
	inline SmartPointerHashString( const SmartPointerHashString< T , ThreadingModel > &src );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointerHashString( SmartPointerHashString< T2 , ThreadingModel > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SmartPointerHashString();

	/// <summary>
	/// ��r���Z�q�i�I�[�o�[���C�h�j
	/// </summary>
	inline skyBool operator ==( const SmartPointer< T , ThreadingModel > &src );

	/// <summary>
	/// �L���X�g�̃I�[�o�[���C�h
	/// </summary>
    inline operator const skyString*() const;

};

template< class T , class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::SmartPointerHashString( T *src , s32 add ) :
SmartPointer< T , ThreadingModel >( src , add )
{
}

template< class T , class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::SmartPointerHashString( const SmartPointerHashString< T , ThreadingModel > &src ) :
SmartPointer< T , ThreadingModel >( src )
{
}

template< class T , class ThreadingModel >
template< class T2 >
inline SmartPointerHashString< T , ThreadingModel >::SmartPointerHashString( SmartPointerHashString< T2 , ThreadingModel > &src ) : //T2�͐e�N���X��z��
SmartPointer< T , ThreadingModel >( src )
{
}

template< class T , class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::~SmartPointerHashString()
{
}

template< class T ,	class ThreadingModel >
inline skyBool SmartPointerHashString< T , ThreadingModel >::operator ==( const SmartPointer< T , ThreadingModel > &src )
{
	thread::CriticalLock Lock( this->m_pCs );

	//�ێ����Ă���I�u�W�F�N�g�̃|�C���^�ƑΏۂƂ��r
        return ( *this->m_pPtr == *src.GetPtrNoConst() );
}

template< class T ,	class ThreadingModel >
inline SmartPointerHashString< T , ThreadingModel >::operator const skyString*() const
{
        return this->m_pPtr->GetString();
}


/// <summary>
/// Hash�����N���X�B�����f�[�^��Hash�v�Z��̒l�����������Ă���̂�����
/// enum�ł�ID��Hash�����ɂ��邱�Ƃ̃����b�g�̓R���p�C���̍������ł��B
/// ����ID�L�q�w�b�_�[�̕ύX�͑S�R���p�C���ɋy�т₷���B������Ȃ�ΊO���t�@�C���ŕ�����Ǘ�������΂n�j
/// ���}�l�[�W���[���炵�������ł��܂���B
/// </summary>
class CHashString : public base::SkyRefObject
{

friend class CHashStringManager;

public:

	/// <summary>
	/// �n�b�V���l���擾
	/// </summary>
	inline u32		Get() const { return m_HashValue; } 

	/// <summary>
	/// ������擾
	/// </summary>
	inline const skyString*     GetString() const;

	/// <summary>
	/// ������擾
	/// </summary>
	inline u32                  GetSize() const;

	/// <summary>
	/// �������r
	/// </summary>
	inline skyBool operator ==( const skyString *pStr ) const;
	inline skyBool operator ==( const CHashString &pStr ) const;

	/// <summary>
	/// �f�X�g���N�^(CHashStringManager�̂݃A�N�Z�X�\�j
	/// </summary>
	virtual ~CHashString();

protected:

	/// <summary>
	/// �����f�[�^
	/// </summary>
	union StringType
	{
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		inline StringType()
		{
			pString = skyNull;
		}
		string::CString		*pStringClass;
		const skyString		*pString;
	};
	StringType			m_String;

	/// <summary>
	/// CRC�l
	/// </summary>
	u32					m_HashValue;

	/// <summary>
	/// �T�C�Y
	/// </summary>
	u32					m_Size;

	/// <summary>
	/// ��������N���G�C�g���Ȃ��������ǂ����̃t���O
	/// </summary>
	skyBool				m_IsNoCreateOn;

	/// <summary>
	/// �������ݒ�
	/// </summary>
	inline void SetString( const skyString *pKey , skyBool isNoCreateOn = skyFalse );

	/// <summary>
	/// �R���X�g���N�^(CHashStringManager�̂݃A�N�Z�X�\�j
	/// </summary>
	inline CHashString(){};

};

} } }

#include "HashString.inl"

namespace sky { namespace lib {

template< typename KEY , typename DATA , class ThreadingModel >class CList;
template< typename T , typename KEY , class ThreadingModel >class    CHash;
template< typename T >class                                          CVectorPrimitive;

namespace hash {

typedef SmartPointerHashString< CHashString >                                               CHashStringPtr;
typedef CList< u32 , CHashStringPtr , thread::ThreadingModels::CSingleThread >				CHashStringPtrList;						//���X�g��`
typedef CHash< CHashStringPtr , u32 , thread::ThreadingModels::CSingleThread >				CHashStringPtrHash;						//Hash�e�[�u����`
typedef CVectorPrimitive< CHashStringPtr >                                                           CHashStringPtrVector;					//Vector��`
typedef SmartPointer< hash::CHashStringPtrList >											CHashStringPtrListPtr;					//�������X�g�̃X�}�[�g�|�C���^
typedef CHash< CHashStringPtrListPtr , u32 , thread::ThreadingModels::CSingleThread >		CHashStringPtrListPtrHash;				//��������L�[�ɕ����̃��X�g���擾����
typedef SmartPointer< CHashStringPtrListPtrHash >											CHashStringPtrListPtrHashPtr;			//��������L�[�ɕ����̃��X�g���擾����
typedef CHash< CHashStringPtrListPtrHashPtr , u32 , thread::ThreadingModels::CSingleThread >	CHashStringPtrListPtrHashPtrHash;		//��������L�[�ɕ����̃��X�g���擾����

}

} }
