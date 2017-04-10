#pragma once

namespace sky { namespace lib { namespace delegate {

static const u32 DELEGATE_ARGS_SIZE = 64;

/// <summary>
/// �f���Q�[�g�����i�g����������Όp�����Ă��������j
/// </summary>
struct CDelegateArgs : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CDelegateArgs( memSize args1 = 0 , memSize args2 = 0 , memSize args3 = 0 , memSize args4 = 0 , memSize args5 = 0 , memSize args6 = 0 , memSize args7 = 0 , memSize args8 = 0 )
	{
		memSize *pArgs = ( memSize * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
		pArgs[ 4 ] = args5;
		pArgs[ 5 ] = args6;
		pArgs[ 6 ] = args7;
		pArgs[ 7 ] = args8;
	}
	inline memSize GetMemSize( u32 i ) const
	{
		SKY_ASSERT( i < 4 );
		memSize *pArgs = ( memSize * )Args;
		return pArgs[ i ];
	}
	inline CDelegateArgs( s32 args1 , s32 args2 = 0 , s32 args3 = 0 , s32 args4 = 0 )
	{
		s32 *pArgs = ( s32 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
	}
	inline CDelegateArgs( d64 args1 , d64 args2 = 0 , d64 args3 = 0 , d64 args4 = 0 )
	{
		d64 *pArgs = ( d64 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
	}
	inline d64 Getd64( u32 i ) const
	{
		SKY_ASSERT( i < 4 );
		d64 *pArgs = ( d64 * )Args;
		return pArgs[ i ];
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDelegateArgs(){};

	/// <summary>
	/// u32�^�p�����[�^�[�̎擾�^�ݒ�
	/// </summary>
	inline void Setu32( u32 args1 , u32 args2 = 0 , u32 args3 = 0 , u32 args4 = 0 , u32 args5 = 0 , u32 args6 = 0 , u32 args7 = 0 , u32 args8 = 0 )
	{
		u32 *pArgs = ( u32 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
		pArgs[ 4 ] = args5;
		pArgs[ 5 ] = args6;
		pArgs[ 6 ] = args7;
		pArgs[ 7 ] = args8;
	}
	inline u32 Getu32( u32 i ) const
	{
		SKY_ASSERT( i < 8 );
		u32 *pArgs = ( u32 * )Args;
		return pArgs[ i ];
	}

	/// <summary>
	/// s32�^�p�����[�^�[�̎擾�^�ݒ�
	/// </summary>
	inline void Sets32( s32 args1 , s32 args2 = 0 , s32 args3 = 0 , s32 args4 = 0 , u32 args5 = 0 , u32 args6 = 0 , u32 args7 = 0 , u32 args8 = 0 )
	{
		s32 *pArgs = ( s32 * )Args;

		pArgs[ 0 ] = args1;
		pArgs[ 1 ] = args2;
		pArgs[ 2 ] = args3;
		pArgs[ 3 ] = args4;
		pArgs[ 4 ] = args5;
		pArgs[ 5 ] = args6;
		pArgs[ 6 ] = args7;
		pArgs[ 7 ] = args8;
	}
	inline s32 Gets32( s32 i ) const
	{
		SKY_ASSERT( i < 8 );
		s32 *pArgs = ( s32 * )Args;
		return pArgs[ i ];
	}

	/// <summary>
	/// �p�����[�^�[
	/// </summary>
	u8 Args[ DELEGATE_ARGS_SIZE ];

	/// <summary>
	/// Fps�i�C�x���g�Ǘ��ł��̏�񂪕t������Ă��ׂẴC�x���g�ɔz�z�����j
	/// </summary>
	fps::CFpsController m_Fps;

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CDelegateArgs& operator =( const CDelegateArgs& str )
	{
		CMemoryUtil_::Copy( Args , str.Args , DELEGATE_ARGS_SIZE );

		return *this;
	}
};

/// <summary>
/// �f���Q�[�g�I�u�W�F�N�g
/// </summary>
struct IDelegateObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R�[���o�b�N�C���^�t�F�[�X
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args ) = 0;
};
typedef SmartPointer< IDelegateObject >					IDelegateObjectPtr;
typedef CListMT< u32 , IDelegateObjectPtr >				IDelegateObjectPtrList;

/// <summary>
/// �f���Q�[�g�����A�_�v�^�[
/// </summary>
template< typename T >
class CEventDelegateAdaptor : public IDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef void ( T::*FuncType )( const CommonSmartPointer &spSender , const CDelegateArgs &args );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CEventDelegateAdaptor( T *pObject, FuncType pFunc ) :
		m_pObject( pObject ) ,
		m_pFunc(pFunc){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CEventDelegateAdaptor(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args )
	{
		( m_pObject->*m_pFunc )( spSender , args );
	}
	
private:

	/// <summary>
	/// �N���X�C���X�^���X�̎Q�ƕێ�
	/// </summary>
	T			*m_pObject;

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType	m_pFunc;

};

class CEventDelegateAdaptorFunc : public IDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef void ( * FuncType )( const CommonSmartPointer &spSender , const CDelegateArgs &args );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CEventDelegateAdaptorFunc( FuncType pFunc ) :
		m_pFunc(pFunc){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CEventDelegateAdaptorFunc(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args )
	{
		m_pFunc( spSender , args );
	}
	
private:

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	FuncType	m_pFunc;

};

class CEventDelegateAdaptorBool : public IDelegateObject
{

public:

	/// <summary>
	/// �i�[�֐��̌^
	/// </summary>
	typedef void ( * FuncType )( const CommonSmartPointer &spSender , const CDelegateArgs &args );

	/// <summary>
	/// �R���X�g���N�^�i�����ϐ���skyBool��o�^����ƃ_���ł͂Ȃ�����Ȃ��j
	/// </summary>
	inline CEventDelegateAdaptorBool( skyBool &flg ){ m_pFlg = &flg; };

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CEventDelegateAdaptorBool(){};
	
	/// <summary>
	/// ()���Z�q�̃I�[�o�[���[�h
	/// </summary>
	virtual void operator()( const CommonSmartPointer &spSender , const CDelegateArgs &args )
	{
		SKY_UNUSED_ARG( spSender );
		SKY_UNUSED_ARG( args );

		//�t���O�𔽓]����
		*m_pFlg = !( *m_pFlg ); 
	}
	
private:

	/// <summary>
	/// �֐��̎Q�ƕێ�
	/// </summary>
	skyBool	*m_pFlg;

};


/// <summary>
/// �C�x���g�I�u�W�F�N�g�̕ێ��ƌĂяo�����s���i���̃N���X���C�x���g��ݒu�������N���X�̃����o�ɂ���j
/// </summary>
class CDelegateHandler : public base::SkyRefObject
{
	
public:
	
	/// <summary>
	/// �f���Q�[�g��ǉ�����
	/// </summary>
	void operator += ( const IDelegateObjectPtr &e )
	{
		m_kList.AddTail( e );
	}

	/// <summary>
	/// �f���Q�[�g���폜����
	/// </summary>
	void operator -= ( const IDelegateObjectPtr &e )
	{
		m_kList.Delete( e );
	}
	
	/// <summary>
	/// �o�^����Ă���f���Q�[�g�����ׂČĂяo��
	/// </summary>
	void operator()( const CommonSmartPointer &spSender = CommonSmartPointer() , const CDelegateArgs &args = CDelegateArgs() )
	{
		FOREACH( IDelegateObjectPtrList , it , m_kList )
		{
			IDelegateObjectPtrList::iterator nextIt = it;
			nextIt = m_kList.GetNextPos( it );

			IDelegateObject& kDelegator = *( *it ).GetPtrNoConst();
			kDelegator( spSender , args );

			if ( nextIt != m_kList.GetNextPos( it ) )
			{//�f���Q�[�g�̎��s�Ń��X�g���ω����Ă��܂����B
                SKY_PANIC_MSG( _T( "delegate is deleted. \n" ) );
				break;
			}
		}
	}
	
	/// <summary>
	/// �o�^����Ă���f���Q�[�g�����ׂď���
	/// </summary>
	void RemoveAll()
	{
		m_kList.DeleteAll();
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDelegateHandler()
	{
		RemoveAll();
	}

private:

	/// <summary>
	/// �f���Q�[�g���X�g
	/// </summary>
	IDelegateObjectPtrList m_kList;

};
typedef CHash< CDelegateHandler >	CDelegateHandlerHash;


} } }
