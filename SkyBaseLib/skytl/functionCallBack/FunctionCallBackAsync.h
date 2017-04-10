#pragma once

namespace sky { namespace lib {

class FunctionCallBackAsync : public base::SkyRefObject
{
/**

�g����
		
using namespace sky::lib;

//�e�X�g�p�֐�
static void HogeFunc1( u32 param1 ){ param1; }
static u32  HogeFunc2( u32 param1 ){ return param1; }

//�e�X�g�p�N���X
class Hoge1
{
public:

	u32 m_Member1;

	skyBool Func1( u32 param1 )
	{
		m_Member1 = param1;
		return skyTrue;
	}

	void Func2()
	{
		m_Member1 = 555;
	}

};

TEST( skytl , 1 ) 
{
	SkyBaseLibControllerCreate_();	//���C�u�����R���g���[���[�̍쐬
	SkyBaseLibController_->Init();	//���C�u�����̏�����

	//static�֐��̒x�����s
	SmartPointer< FunctionCallBackAsync > f1( FunctionCallBackAsync_v::Create( &HogeFunc1 , 333 ) );
	f1->run();

	//static�֐��i�߂�l����j�̒x�����s
	SmartPointer< FunctionCallBackAsync_r< u32 > > f2( FunctionCallBackAsync_r< u32 >::Create( HogeFunc2 , 777 ) );
	f2->run();
	u32 result = f2->GetResult();
	result;

	//�N���X�֐��̒x�����s
	Hoge1 hoge1;
	SmartPointer< FunctionCallBackAsync > f3( FunctionCallBackAsync_v::Create( &Hoge1::Func2 , &hoge1 ) );
	f3->run();

	//�N���X�֐��i�߂�l����j�̒x�����s
	SmartPointer< FunctionCallBackAsync_r< skyBool > > f4( FunctionCallBackAsync_r< skyBool >::Create( &Hoge1::Func1 , &hoge1 , 999 ) );
	f4->run();
	skyBool resultB = f4->GetResult();
	resultB;

	SkyBaseLibController_->Term();	//���C�u�����̖�����
	SkyBaseLibControllerDelete_();	//���C�u�����R���g���[���̍폜
}
*/
public:

	/// <summary>
	/// �֐����s
	/// </summary>
	virtual	void run() = 0;

	/// <summary>
	/// ���s�I������
	/// </summary>
	skyBool	IsEnd() const { return m_IsEnd; }

	/// <summary>
	/// ���Z�b�gEnd
	/// </summary>
	void ResetEnd(){ m_IsEnd = skyFalse; }

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	FunctionCallBackAsync() : m_IsEnd( skyFalse ){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~FunctionCallBackAsync(){}

	/// <summary>
	/// �R�[���o�b�N����this�|�C���^�������ƂŕύX����
	/// </summary>
	template < class T >
	void Bind( T * t )
	{ 
		inner_bind( ( void * ) t );
	}

protected:

	/// <summary>
	/// ���s�I���t���O
	/// </summary>
	skyBool m_IsEnd;

	virtual void inner_bind( void * )
	{
		SKY_PANIC_MSG( _T( "not have no static function." ) );
	}
};

template < class A1 , class Arg1 >
class FunctionCallBackAsync_vg1;
template < class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vg2;
template < class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vg3;
template < class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_vg4;
template < class A1 , class A2 , class A3 , class A4 , class A5,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vg5;
template < class ObjectType>
class FunctionCallBackAsync_vm0;
template < class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_vm1;
template < class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vm2;
template < class ObjectType , class A1 , class A2 , class A3 ,
    class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vm3;
template < class ObjectType , class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_vm4;
template < class ObjectType , class A1 , class A2 , class A3 , class A4 , class A5,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vm5;

//�e���v���[�gTypedef�S
typedef SmartPointer< FunctionCallBackAsync >			FunctionCallBackAsyncPtr;		//�X�}�[�g�|�C���^��`
typedef CFifo< FunctionCallBackAsyncPtr >				FunctionCallBackAsyncPtrFifo;	//Fifo��`
typedef CList< s32 , FunctionCallBackAsyncPtr >			FunctionCallBackAsyncPtrList;	//List��`
typedef CListMT< s32 , FunctionCallBackAsyncPtr >		FunctionCallBackAsyncPtrListMT;	//List��`

/// <summary>
/// �߂�l�Ȃ��̃R�[���o�b�N�֐����쐬���邽�߂̃e���v���[�g�N���X
/// </summary>
class FunctionCallBackAsync_v : public FunctionCallBackAsync
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~FunctionCallBackAsync_v(){}

	/// <summary>
	/// �߂�l�Ȃ��A�O���[�o���֐��A�����Ȃ��֐��p
	/// </summary>
	static FunctionCallBackAsync_v *Create( void ( * f )() );

	template < class Arg1 , class A1 >
	static FunctionCallBackAsync_v *Create( void ( * f )( A1 ) , const Arg1 & a1 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�O���[�o���֐��A�����P�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg1< A1 , Arg1 >( f , a1 );
	}

	template < class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 ) , const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�O���[�o���֐��A�����Q�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg2< A1 , A2 , Arg1 , Arg2 >( f , a1 , a2 );
	}

	template < class Arg1 , class Arg2 , class Arg3 , class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 , A3 ) ,
		 const Arg1 & a1 , const Arg2 & a2 , const Arg3 & a3 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�O���[�o���֐��A�����R�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg3< A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , a1 , a2 , a3 );
	}
	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 , class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 , A3 , A4 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�O���[�o���֐��A�����S�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg4< A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , a1 , a2 , a3 , a4 );
	}
	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 , class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 , A3 , A4 , A5 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�O���[�o���֐��A�����T�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg5< A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , a1 , a2 , a3 , a4 , a5 );	
	}

	template < class obj >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )() , obj* o )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�����o�֐��A�����Ȃ�
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm0< obj >( f , o );
	}

	template < class obj , class Arg1 , class A1 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 ) , obj* o , const Arg1& a1 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�����o�֐��A�����P�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm1< obj , A1 , Arg1 >( f , o , a1 );
	}

	template < class obj , class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�����o�֐��A�����Q�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm2< obj , A1 , A2 , Arg1 , Arg2 >( f , o , a1 , a2 );
	}

	template < class obj , class Arg1 , class Arg2 , class Arg3 , class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 , A3 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�����o�֐��A�����R�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm3< obj , A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , o , a1 , a2 , a3 );
	}

	template < class obj , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 , A3 , A4 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�����o�֐��A�����S�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm4< obj , A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , o , a1 , a2 , a3 , a4 );
	}

	template < class obj , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 ,
	class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 , A3 , A4 , A5 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// �߂�l�Ȃ��A�����o�֐��A�����T�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm5< obj , A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , o , a1 , a2 , a3 , a4 , a5 );
	}
};

template < class Result , class A1 , class Arg1 >
class FunctionCallBackAsync_rg1;
template < class Result , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rg2;
template < class Result , class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rg3;
template < class Result , class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_rg4;
template < class Result , class A1 , class A2 , class A3 , class A4 , class A5 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rg5;
template < class Result , class ObjectType>
class FunctionCallBackAsync_rm0;
template < class Result , class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_rm1;
template < class Result , class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rm2;
template < class Result , class ObjectType , class A1 , class A2 , class A3 ,
    class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rm3;
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_rm4;
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
    class A5 , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rm5;

/// <summary>
/// �߂肠��̃R�[���o�b�N�֐����쐬���邽�߂̃e���v���[�g�N���X
/// </summary>
template < class Result >
class FunctionCallBackAsync_r : public FunctionCallBackAsync
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~FunctionCallBackAsync_r(){}


	Result GetResult() const { return _r; }
	Result GetResultWait() const 
	{ 
		while( this->IsEnd() == false ){}
		return _r;
	}

	/// <summary>
	/// �߂�l����A�O���[�o���֐��A�����Ȃ�
	/// </summary>
	static FunctionCallBackAsync_r* Create( Result ( * f )() );

	template < class Arg1 , class A1 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 ) , const Arg1& a1 )
	{
		/// <summary>
		/// �߂�l����A�O���[�o���֐��A�����P�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg1< Result , A1 , Arg1 >( f , a1 );
	}

	template < class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 ) ,
		 const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// �߂�l����A�O���[�o���֐��A�����Q�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg2< Result, A1 , A2 , Arg1 , Arg2 >( f , a1 , a2 );
	}

	template < class Arg1 , class Arg2 , class Arg3 , class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 , A3 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 )
	{
		/// <summary>
		/// �߂�l����A�O���[�o���֐��A�����R�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg3< Result, A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , a1 , a2 );
	}

	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 ,
		class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 , A3 , A4 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// �߂�l����A�O���[�o���֐��A�����S�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg4< Result, A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , a1 , a2 , a3 , a4 );	}

	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5,
		class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 , A3 , A4 , A5 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// �߂�l����A�O���[�o���֐��A�����T�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg5< Result, A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , a1 , a2 , a3 , a4 , a5 );
	}

	template < class obj >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )() , obj* o )
	{
		/// <summary>
		/// �߂�l����A�����o�֐��A�����O�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm0< Result, obj >( f , o );
	}

	template < class obj, class Arg1 , class A1 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 ) , obj* o , const Arg1& a1 )
	{
		/// <summary>
		/// �߂�l����A�����o�֐��A�����P�̊֐��p
		/// </summary>
		return : NEW_() FunctionCallBackAsync_rm1< Result, obj, A1 , Arg1 >( f , o , a1 );
	}

	template < class obj, class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// �߂�l����A�����o�֐��A�����Q�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm2< Result, obj, A1 , A2 , Arg1 , Arg2 >( f , o , a1 , a2 );
	}

	template < class obj, class Arg1 , class Arg2 , class Arg3 ,
		class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 , A3 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 )
	{
		/// <summary>
		/// �߂�l����A�����o�֐��A�����R�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm3< Result, obj, A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , o , a1 , a2 , a3 );
	}

	template < class obj, class Arg1 , class Arg2 , class Arg3 , class Arg4 ,
		class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 , A3 , A4 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// �߂�l����A�����o�֐��A�����S�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm4< Result, obj, A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , o , a1 , a2 , a3 , a4 );
	}

	template < class obj, class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5,
		class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 , A3 , A4 , A5 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// �߂�l����A�����o�֐��A�����T�̊֐��p
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm5< Result, obj, A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , o , a1 , a2 , a3 , a4 , a5 );
	}

protected:

	Result _r;
};

/// <summary>
/// �߂�l�Ȃ��A�O���[�o���֐��A�����Ȃ��֐��p
/// </summary>
class FunctionCallBackAsync_vg0 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vg0(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( * function_type )();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vg0( function_type f ) : _f( f ){ }

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ _f(); }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;
};

/// <summary>
/// �߂�l�Ȃ��A�O���[�o���֐��A�����P�̊֐��p
/// </summary>
template < class A1 , class Arg1 >
class FunctionCallBackAsync_vg1 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vg1(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( * function_type ) ( A1 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vg1( function_type f , Arg1 a1 ) : _f( f ) , _a1( a1 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ _f( _a1 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
};

/// <summary>
/// �߂�l�Ȃ��A�O���[�o���֐��A�����Q�̊֐��p
/// </summary>
template < class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vg2 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vg2(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vg2( function_type f , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// �߂�l�Ȃ��A�O���[�o���֐��A�����R�̊֐��p
/// </summary>
template < class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vg3 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vg3(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vg3( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 , _a3 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// �߂�l�Ȃ��A�O���[�o���֐��A�����S�̊֐��p
/// </summary>
template < class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_vg4 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vg4(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vg4( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 , _a3 , _a4 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// �߂�l�Ȃ��A�O���[�o���֐��A�����T�̊֐��p
/// </summary>
template < class A1 , class A2 , class A3 , class A4 , class A5,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vg5 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vg5(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vg5( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3
			, Arg4 a4 , Arg5 a5 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 , _a3 , _a4 , _a5 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

/// <summary>
/// �߂�l����A�O���[�o���֐��A�����Ȃ��֐��p
/// </summary>
template < class Result>
class FunctionCallBackAsync_rg0 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rg0(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( * function_type ) ();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rg0( function_type f) : _f( f ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r = _f(); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;
};

/// <summary>
/// �߂�l����A�O���[�o���֐��A�����P�֐��p
/// </summary>
template < class Result , class A1 , class Arg1 >
class FunctionCallBackAsync_rg1 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rg1(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( * function_type ) ( A1 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rg1( function_type f , Arg1 a1 ) : _f( f ) , _a1( a1 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r = _f( _a1 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;
		Arg1 _a1;
};

/// <summary>
/// �߂�l����A�O���[�o���֐��A�����Q�֐��p
/// </summary>
template < class Result , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rg2 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rg2(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rg2( function_type f , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r = _f( _a1 , _a2 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// �߂�l����A�O���[�o���֐��A�����R�֐��p
/// </summary>
template < class Result , class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rg3 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rg3(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rg3( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r=_f( _a1 , _a2 , _a3 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// �߂�l����A�O���[�o���֐��A�����S�֐��p
/// </summary>
template < class Result , class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_rg4 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rg4(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rg4( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r=_f( _a1 , _a2 , _a3 , _a4 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// �߂�l����A�O���[�o���֐��A�����T�֐��p
/// </summary>
template < class Result , class A1 , class A2 , class A3 , class A4 , class A5 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rg5 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rg5(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rg5( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3
			, Arg4 a4 , Arg5 a5 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r=_f( _a1 , _a2 , _a3 , _a4 , _a5 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

/// <summary>
/// �߂�l�Ȃ��A�����o�֐��A�����Ȃ��֐��p
/// </summary>
template < class ObjectType>
class FunctionCallBackAsync_vm0 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vm0(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( ObjectType::* function_type ) ();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vm0( function_type f ,ObjectType*o) : _f( f ) , _o(o) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ ( _o->*_f)(); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }
		ObjectType* _o;	//	�������擪�łȂ��Ə��������ł��Ȃ�
		function_type _f;
};

/// <summary>
/// �߂�l�Ȃ��A�����o�֐��A�����P�֐��p
/// </summary>
template < class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_vm1 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vm1(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vm1( function_type f ,ObjectType*o , Arg1 a1 )
			: _f( f ) , _o(o) , _a1( a1 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
};

/// <summary>
/// �߂�l�Ȃ��A�����o�֐��A�����Q�֐��p
/// </summary>
template < class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vm2 : public FunctionCallBackAsync_v
{

	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vm2(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vm2( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// �߂�l�Ȃ��A�����o�֐��A�����R�֐��p
/// </summary>
template < class ObjectType , class A1 , class A2 , class A3 ,
	class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vm3 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vm3(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vm3( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 , _a3 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// �߂�l�Ȃ��A�����o�֐��A�����S�֐��p
/// </summary>
template < class ObjectType , class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_vm4 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vm4(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vm4( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2
			, Arg3 a3 , Arg4 a4 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 , _a3 , _a4 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// �߂�l�Ȃ��A�����o�֐��A�����T�֐��p
/// </summary>
template < class ObjectType , class A1 , class A2 , class A3 , class A4 , class A5,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vm5 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_vm5(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_vm5( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2
			, Arg3 a3 , Arg4 a4 , Arg5 a5 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 , _a3 , _a4 , _a5 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

/// <summary>
/// �߂�l����A�����o�֐��A�����Ȃ��֐��p
/// </summary>
template < class Result , class ObjectType>
class FunctionCallBackAsync_rm0 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rm0(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rm0( function_type f ,ObjectType*o) : _f( f ) , _o(o) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r = ( _o->*_f)(); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;
};

/// <summary>
/// �߂�l����A�����o�֐��A�����P�֐��p
/// </summary>
template < class Result , class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_rm1 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rm1(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rm1( function_type f ,ObjectType*o , Arg1 a1 )
			: _f( f ) , _o(o) , _a1( a1 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r = ( _o->*_f)( _a1 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
};

/// <summary>
/// �߂�l����A�����o�֐��A�����Q�֐��p
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rm2 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rm2(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rm2( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r = ( _o->*_f)( _a1 , _a2 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// �߂�l����A�����o�֐��A�����R�֐��p
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class A3 ,
	class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rm3 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rm3(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rm3( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r=( _o->*_f)( _a1 , _a2 , _a3 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// �߂�l����A�����o�֐��A�����S�֐��p
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_rm4 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rm4(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rm4( function_type f ,ObjectType*o ,
			Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r=( _o->*_f)( _a1 , _a2 , _a3 , _a4 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// �߂�l����A�����o�֐��A�����T�֐��p
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
	class A5 , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rm5 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~FunctionCallBackAsync_rm5(){}

		/// <summary>
		/// �R�[���o�b�N������֐��̌^
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FunctionCallBackAsync_rm5( function_type f ,ObjectType*o ,
			Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 , Arg5 a5 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// �R�[���o�b�N���s
		/// </summary>
        virtual void run(){ this->_r=( _o->*_f)( _a1 , _a2 , _a3 , _a4 , _a5 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// �R�[���o�b�N������֐�����������I�u�W�F�N�g�̎Q��
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// �R�[���o�b�N������֐�
		/// </summary>
		function_type _f;

		/// <summary>
		/// ����
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

} }
