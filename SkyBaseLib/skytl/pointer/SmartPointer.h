#pragma once

namespace sky { namespace lib {

/// <summary>
/// �X�}�[�g�|�C���^�N���X�ł��B
/// �Ƃ���T�C�g���璸���Ă��܂����B
///
/// �X�}�[�g�|�C���^�Ƃ͊ȒP�Șb�A�|�C���^�Ǘ��N���X��
/// �������Ɠ����ɊǗ��I�u�W�F�N�g�������Y���ɂ�
/// �ď����̂ŁA�I�u�W�F�N�g�̍폜�����Ȃ��Ă�����
/// �Ƃ����폜�̎�Ԃ��Ȃ����|�C���^��NEW�����ł���@�\�ł���B
/// ���Ⴀ�|�C���^�Ǘ��N���X�͂Ȃ�ŏ�����́H�Ǝv���͓̂��R�ŁA
/// �|�C���^�Ǘ��N���X�͂Ȃ�炩�̕��@�ŏ����Ă���Ȃ��Ƃ����܂���Bdelete�H�Ӗ��ˁ[�I�B
/// ���₢�₱�̃X�}�[�g�|�C���^�͐F�X�Ӗ��������ł��B�����͂��܂����������ɖY�ꂻ���B
/// �Ȃ�ׂ��g���悤�ɂ��悤�B
/// </summary>

/*---------------------������----------------------------��

//�C���X�^���X����
SmartPointer<Hoge> spHoge(NEW Hoge() );

*/

/// <summary>
/// SkyRefObject���p�������N���X�̂ݎg�p�ł���X�}�[�g�|�C���^
/// </summary>
template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointer : public ThreadingModel //�C�ӂ̃X���b�h���f�����p���i�V���O���X���b�h�^�}���`�X���b�h���Ή��ɂȂ肦��j
{

public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	inline explicit SmartPointer( const T *src = skyNull , skyBool bIsNoDelete = skyFalse , s32 add = 0 );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���^�����R�s�[�j
	/// </summary>
	inline SmartPointer( const SmartPointer< T , ThreadingModel > &src );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointer( const SmartPointer< T2 , ThreadingModel > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SmartPointer();

	/// <summary>
	/// =������Z�q�i�����I�R�s�[�j
	/// </summary>
	inline SmartPointer< T , ThreadingModel >& operator =( const SmartPointer< T , ThreadingModel > &src);

	/// <summary>
	/// =������Z�q�i�����I�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointer< T , ThreadingModel >& operator =( const SmartPointer< T2 , ThreadingModel > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// ->�����o�I�����Z�q
	/// </summary>
	inline T* operator ->() const;

	/// <summary>
	/// *�����o�I�����Z�q
	/// </summary>
	T &operator*(); 

	/// <summary>
	/// ==��r���Z�q
	/// </summary>
	inline skyBool operator ==( T *val );

	template< class T2 > 
	inline skyBool operator ==( const SmartPointer< T2 , ThreadingModel > &src );

	/// <summary>
	/// !=��r���Z�q
	/// </summary>
	inline skyBool operator !=( T *val );

	template< class T2 > 
	inline skyBool operator !=( const SmartPointer< T2 , ThreadingModel > &src );

	/// <summary>
	/// �|�C���^�̖����I�ȓo�^
	/// </summary>
	inline void SetPtr( T *src = skyNull , skyBool bIsNoDelete = skyFalse ,  s32 add = 0 );

	/// <summary>
	/// �|�C���^�̏㏑��
	/// </summary>
	inline void OverWrite( T *src = skyNull );

	/// <summary>
	/// ���݂̂̑��o��
	/// </summary>
	inline const T& GetIns() const;

	/// <summary>
	/// �|�C���^�݂̑��o��
	/// </summary>
	inline const T* GetPtr() const;

	/// <summary>
	/// �폜�����t���O�̎擾
	/// </summary>
	inline skyBool IsNodelete() const;

	/// <summary>
	/// �|�C���^�݂̑��o��(�R���X�g�Ȃ��댯�j
	/// </summary>
	inline T* GetPtrNoConst() const;

	/// <summary>
	/// �|�C���^�݂̑��o��(�R���X�g�Ȃ��댯�j
	/// </summary>
	inline T& GetInsNoConst() const;

	/// <summary>
	/// �|�C���^�̃|�C���^�݂��o��
	/// </summary>
	inline const T** GetPtrPtr();

	/// <summary>
	/// �X�}�[�g�|�C���^����i�����Ǘ����Ă��Ȃ����j�ǂ���
	/// </summary>
	inline skyBool IsEmpty() const;

	/// <summary>
	/// �폜
	/// </summary>
	inline void Delete();

protected:

	/// <summary>
	/// T�^�̃I�u�W�F�N�g�̃|�C���^
	/// </summary>
	T*	m_pPtr;

	/// <summary>
	/// �폜���Ȃ��i�֐��̃C���^�[�t�F�[�X��ʂ����߂Ɉꎞ�I�ɃX�}�[�g�|�C���^�̌^�ɕϊ�����ꍇ���ɗ��Ă�B�j
	/// </summary>
	skyBool m_bIsNoDelete;

	/// <summary>
	/// �J������
	/// </summary>
	virtual void Release();

};

template< class T >
class SmartPointerMT : public SmartPointer< T , thread::ThreadingModels::CMultiThread >
{

public :

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	inline explicit SmartPointerMT( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���^�����R�s�[�j
	/// </summary>
	inline SmartPointerMT( const SmartPointerMT< T > &src );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointerMT( SmartPointerMT< T2 > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SmartPointerMT();
};

typedef	SmartPointer< base::SkyRefObject >		CommonSmartPointer;		//���ł�����V���O���X���b�h�X�}�[�g�|�C���^�^
typedef	SmartPointerMT< base::SkyRefObject >	CommonSmartPointerMT;	//���ł�����}���`�X���b�h�X�}�[�g�|�C���^�^

/// <summary>
/// �ǂ�Ȍ^�ł��i�[�\�ȃX�}�[�g�|�C���^�N���X
/// �Q�ƃJ�E���^��NEW���Ă�̂ōׂ���Malloc���������Ă��܂��̂���_
/// </summary>
template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointer2 : public ThreadingModel //�C�ӂ̃X���b�h���f�����p���i�V���O���X���b�h�^�}���`�X���b�h���Ή��ɂȂ肦��j
{

public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	inline explicit SmartPointer2( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���^�����R�s�[�j
	/// </summary>
	inline SmartPointer2( const SmartPointer2< T , ThreadingModel > &src );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointer2( const SmartPointer2< T2 , ThreadingModel > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SmartPointer2();

	/// <summary>
	/// =������Z�q�i�����I�R�s�[�j
	/// </summary>
	inline SmartPointer2< T , ThreadingModel >& operator =( const SmartPointer2< T , ThreadingModel > &src);

	/// <summary>
	/// =������Z�q�i�����I�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointer2< T , ThreadingModel >& operator =( const SmartPointer2< T2 , ThreadingModel > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// ->�����o�I�����Z�q
	/// </summary>
	inline T* operator ->() const ;

	/// <summary>
	/// *�����o�I�����Z�q
	/// </summary>
	T &operator*(); 

	/// <summary>
	/// ==��r���Z�q
	/// </summary>
	inline skyBool operator ==( T *val );

	template< class T2 > 
	inline skyBool operator ==( const SmartPointer2< T2 , ThreadingModel > &src );

	/// <summary>
	/// !=��r���Z�q
	/// </summary>
	inline skyBool operator !=( T *val );

	template< class T2 > 
	inline skyBool operator !=( const SmartPointer2< T2 , ThreadingModel > &src );

	/// <summary>
	/// �|�C���^�̖����I�ȓo�^
	/// </summary>
	inline void SetPtr( T *src = skyNull ,  s32 add = 0 );

	/// <summary>
	/// �|�C���^�̏㏑��
	/// </summary>
	inline void OverWrite( T *src = skyNull );

	/// <summary>
	/// �|�C���^�݂̑��o��
	/// </summary>
	inline const T* GetPtr() const;

	/// <summary>
	/// �|�C���^�݂̑��o��(�R���X�g�Ȃ��댯�j
	/// </summary>
	inline T* GetPtrNoConst() const;

	/// <summary>
	/// �|�C���^�̃|�C���^�݂��o��
	/// </summary>
	inline T** GetPtrPtr();

	/// <summary>
	/// �Q�ƃJ�E���^�ւ̃|�C���^���擾
	/// </summary>
	inline u32* GetRefPtr() const;
	inline u32  GetRef() const;

	/// <summary>
	/// �X�}�[�g�|�C���^����i�����Ǘ����Ă��Ȃ����j�ǂ���
	/// </summary>
	inline skyBool IsEmpty() const;

	/// <summary>
	/// �폜
	/// </summary>
	inline void Delete();

protected:

	/// <summary>
	/// �^�C�v
	/// </summary>
	u32 type;

	/// <summary>
	/// �Q�ƃJ�E���^�ւ̃|�C���^
	/// </summary>
	u32	*m_pRefCnt;

	/// <summary>
	/// T�^�̃I�u�W�F�N�g�̃|�C���^
	/// </summary>
	T*	m_pPtr;

	/// <summary>
	/// �Q�ƃJ�E���^����
	/// </summary>
	inline void AddRef();

	/// <summary>
	/// �J������
	/// </summary>
	virtual void Release();

};

typedef	SmartPointer2< void >		CommonSmartPointer2;		//���ł�����V���O���X���b�h�X�}�[�g�|�C���^�^

template< class T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class SmartPointerCom : public SmartPointer2< T , ThreadingModel >
{

public :

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	inline explicit SmartPointerCom( T *src = skyNull , s32 add = 0 );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���^�����R�s�[�j
	/// </summary>
	inline SmartPointerCom( const SmartPointerCom< T , ThreadingModel > &src );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i�ÖٓI�A�b�v�L���X�g�t���j
	/// </summary>
	template< class T2 >
	inline SmartPointerCom( SmartPointerCom< T2 , ThreadingModel > &src ); //T2�͐e�N���X��z��

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SmartPointerCom();

	/// <summary>
	/// �J������(�I�[�o�[���C�h�j
	/// </summary>
	inline void Release();

};


} }

#include "SmartPointer.inl"

//---System

//�������g���X�}�[�g�|�C���^�Ɋi�[����
//���̃}�N���͂ŃX�g���N���Ŏg�p�ł��܂���B
#define thisPtr( ptrType ) 	ptrType( this , skyFalse , this->GetRefCnt() )