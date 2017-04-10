#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

class SqClassObject;

class SqParamObject;
typedef CList< u32 , SqParamObject >		SqParamObjectList;	//���X�g��`
typedef CHash< SqParamObject >				SqParamObjectHash;	//���X�g��`

/// <summary>
/// �p�����[�^�[�̌^
/// </summary>
enum eParamType
{
	eParamType_Int = 0 ,
	eParamType_Float ,
	eParamType_String ,
	eParamType_Bool ,
	eParamType_Void ,
	eParamType_Array ,
	eParamType_Table ,
	eParamType_Class ,
	eParamType_CppFunction ,
};

/// <summary>
/// �b�{�{�֐����
/// </summary>
class SqCppFunc : public base::SkyObject
{
public:
	SQFUNCTION m_f;
	hash::CHashStringPtr m_fname;
	hash::CHashStringPtr m_typemask;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SqCppFunc(){}
	SqCppFunc( SQFUNCTION f , const hash::CHashStringPtr &fname , const hash::CHashStringPtr &typemask = CHS( _T("") ) )
	{
		Set( f , fname , typemask );
	}

	/// <summary>
	/// �ݒ�
	/// </summary>
	void Set( SQFUNCTION f , const hash::CHashStringPtr  &fname , const hash::CHashStringPtr &typemask = CHS( _T("") ) )
	{
		m_f = f;
		m_fname = fname;
		m_typemask = typemask;
	}

	/// <summary>
	/// ������Z�q�̃I�[�o�[���C�h
	/// </summary>
	SqCppFunc& operator =( const SqCppFunc& src )
	{
		this->Set( src.m_f , src.m_fname , src.m_typemask );
		return *this;
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SqCppFunc()
	{
	}
};
typedef CList< u32 , SqCppFunc >		SqCppFuncList;	//���X�g��`
typedef CHash< SqCppFunc >				SqCppFuncHash;	//���X�g��`

/// <summary>
/// Squirrel�֐��p�����[�^�[���
/// </summary>
class SqParamObject : public base::SkyObject
{
public :

	/// <summary>
	/// �p�����[�^�[���p��
	/// </summary>
	union Param {

	public:

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		inline Param()
		{
			m_Int = 0;
		}

		inline void               SetInt         ( s32 param )               { m_Int = param;	}
		inline s32	              GetInt         ()	const                    { return m_Int;	}

		inline void	              SetFloat       ( f32 param )               { m_Float = param;	}
		inline f32	              GetFloat       ()	const                    { return m_Float;	}

		inline void	              SetString      ( const skyString *param )  { m_String = param; }
		inline const skyString*   GetString	     ()	const                    { return m_String;	}

		inline void               SetBool        ( skyBool param )           { m_Bool = param;	}
		inline skyBool            GetBool        ()	const                    { return m_Bool;	}

		inline void	              SetVoid        ( void *param )             { m_Void = param;	}
		inline void*              GetVoid        ()	const                    { return m_Void;	}

		inline void               SetArray       ( SqParamObjectList *param ){ m_Array = param;	}
		inline SqParamObjectList* GetArray       ()	const				     { return m_Array;	}

		inline void	              SetTable       ( SqParamObjectHash *param ){ m_Table = param;	}
		inline SqParamObjectHash* GetTable       ()	const                    { return m_Table;	}

		inline void               RegisterClass	 ( SqClassObject *param )    { m_Class = param;	}
		inline SqClassObject*     GetClass       ()	const                    { return m_Class;	}

		inline void               SetCppFunc	 ( SqCppFunc *param )        { m_CppFunc = param; }
		inline SqCppFunc*         GetCppFunc	 ()	const                    { return m_CppFunc;  }

		union {
			s32                m_Int;
			f32                m_Float;
			const skyString*   m_String;
			skyBool            m_Bool;
			void*              m_Void;
			SqParamObjectList* m_Array;
			SqParamObjectHash* m_Table;
			SqClassObject*     m_Class;
			SqCppFunc*         m_CppFunc;
		};
	};

	/// <summary>
	/// �p�����[�^�[
	/// </summary>
	Param		m_Param;

	/// <summary>
	/// �p�����[�^�[�̎��
	/// </summary>
	eParamType	m_eParamType;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SqParamObject();
	SqParamObject( s32			param );			//�g������( s32 )�ŃL���X�g���邱��
	SqParamObject( f32			param );			//�g������( f32 )�ŃL���X�g���邱��
	SqParamObject(const skyString *param );
	SqParamObject( skyBool		param );			//�g������( skyBool )�ŃL���X�g���邱��
	SqParamObject( void			*param );			//�g������( void * )�ŃL���X�g���邱��
	SqParamObject( SqParamObjectList	*param );	//�g������( SqParamObjectList * )�ŃL���X�g���邱��
	SqParamObject( SqParamObjectHash	*param );	//�g������( SqParamObjectHash * )�ŃL���X�g���邱��
	SqParamObject( SqClassObject	*param );		//�g������( SqClassObject * )�ŃL���X�g���邱��
	SqParamObject( SqCppFunc	*param );			//�g������( SqCppFunc * )�ŃL���X�g���邱��

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SqParamObject();

	/// <summary>
	/// �lSetter
	/// </summary>
	void SetParam( s32			param );			//�g������( s32 )�ŃL���X�g���邱��
	void SetParam( f32			param );			//�g������( f32 )�ŃL���X�g���邱��
	void SetParam( const skyString *param );
	void SetParam( skyBool		param );			//�g������( skyBool )�ŃL���X�g���邱��
	void SetParam( void			*param );			//�g������( void * )�ŃL���X�g���邱��
	void SetParam( SqParamObjectList	*param );	//�g������( SqParamObjectList * )�ŃL���X�g���邱��
	void SetParam( SqParamObjectHash	*param );	//�g������( SqParamObjectHash * )�ŃL���X�g���邱��
	void SetParam( SqClassObject	*param );		//�g������( SqClassObject * )�ŃL���X�g���邱��
	void SetParam( SqCppFunc	*param );			//�g������( SqCppFunc * )�ŃL���X�g���邱��

	/// <summary>
	/// ������Z�q�̃I�[�o�[���C�h
	/// </summary>
	SqParamObject& operator =( s32 val );
	SqParamObject& operator =( f32 val );
	SqParamObject& operator =( const skyString *pVal );
	SqParamObject& operator =( skyBool val );
	SqParamObject& operator =( void *pVal );
	SqParamObject& operator =( SqParamObjectList *pVal );
	SqParamObject& operator =( SqParamObjectHash *pVal );
	SqParamObject& operator =( SqClassObject *pVal );
	SqParamObject& operator =( SqCppFunc *pVal );

	/// <summary>
	/// �L���X�g�̃I�[�o�[���C�h
	/// </summary>
    operator s32						() const;
    operator f32						() const;
    operator const skyString *			() const;
    operator skyBool					() const;
    operator void						*() const;
    operator const SqParamObjectList	*() const;
    operator const SqParamObjectHash	*() const;
    operator const SqClassObject		*() const;
    operator const SqCppFunc			*() const;

	/// <summary>
	/// �l��Void�|�C���^�Ƃ��Ď擾����
	/// </summary>
	void *GetVoidPointer(){ return m_Param.m_Void; }

	/// <summary>
	/// �l��skyNull��ݒ肷��
	/// </summary>
	void SetNull(){ m_Param.m_Void = skyNull; }

	/// <summary>
	/// �l�̌^���擾
	/// </summary>
	eParamType GetType() const { return m_eParamType; }

	/// <summary>
	/// �l���擾
	/// </summary>
	Param &GetParam(){ return m_Param; }

	/// <summary>
	/// �l���X�^�b�N�Ƀv�b�V������
	/// </summary>
	void PushSquirrelStack( HSQUIRRELVM v );

	/// <summary>
	/// �l���X�^�b�N����|�b�v����
	/// </summary>
	void PopSquirrelStack( HSQUIRRELVM v );
};

/// <summary>
/// Squirrel�֐��p�����[�^�[
/// </summary>
class SqFuncParam : public base::SkyObject
{

public:

	/// <summary>
	/// []���Z�q�I�[�o�[���C�h
	/// </summary>
	const SqParamObject	&operator[]( u32 idx ) const;

	/// <summary>
	/// �p�����[�^�[�z��
	/// </summary>
	SqParamObject	m_Param[ sq_const::FUNC_PARAM_MAX_NUM ];

	/// <summary>
	/// �p�����[�^�[�Z�b�g��
	/// </summary>
	u32			m_ParamNum;

	/// <summary>
	/// ������Z�q
	/// </summary>
	SqFuncParam& operator =( const SqFuncParam *pIn );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SqFuncParam();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SqFuncParam();

	/// <summary>
	/// �lSetter(�l���ݒ肳����SqParamObject�\���̂ɒl���n�����j
	/// </summary>
	void SetParam( s32			 param );				//�g������( s32 )�ŃL���X�g���邱��
	void SetParam( f32			 param );				//�g������( f32 )�ŃL���X�g���邱��
	void SetParam( const hash::CHashStringPtr &param );
	void SetParam( skyBool		 param );				//�g������( skyBool )�ŃL���X�g���邱��
	void SetParam( void			*param );				//�g������( void * )�ŃL���X�g���邱��
	void SetParam( const SqParamObjectList *param );	//�g������( SqParamObjectList * )�ŃL���X�g���邱��
	void SetParam( const SqParamObjectHash *param );	//�g������( SqParamObjectHash * )�ŃL���X�g���邱��
	void SetParam( const SqClassObject *param );		//�g������( SqClassObject * )�ŃL���X�g���邱��
	void SetParam( const SqCppFunc *param );			//�g������( SqCppFunc * )�ŃL���X�g���邱�ƁB�ꎞ�I�ȃe�[�u���Ƀl�C�e�B�u�N���[�W���[���o�^�����̂ŁA�����Ƃ��Ď󂯎�����l��.�������̌�֐����ŌĂяo��(�� a.FuncName() )

	/// <summary>
	/// �l���X�^�b�N�Ƀv�b�V������
	/// </summary>
	void PushSquirrelStack( HSQUIRRELVM v );

	/// <summary>
	/// �l���X�^�b�N����|�b�v����
	/// </summary>
	void PopSquirrelStack( HSQUIRRELVM v );

	/// <summary>
	/// �p�����[�^�[�����擾
	/// </summary>
	inline u32 GetParamNum(){ return m_ParamNum; };

	/// <summary>
	/// �p�����[�^�[���擾
	/// </summary>
	skyBool GetParam( u32 index , s32 &param );
	skyBool GetParam( u32 index , f32 &param );
	skyBool GetParam( u32 index , hash::CHashStringPtr &param );
	skyBool GetParam( u32 index , skyBool &param );
	skyBool GetParam( u32 index , void* &param );
	skyBool GetParam( u32 index , const SqParamObjectList* &param );
	skyBool GetParam( u32 index , const SqParamObjectHash* &param );
	skyBool GetParam( u32 index , const SqClassObject* &param );
	skyBool GetParam( u32 index , const SqCppFunc* &param );

};

} } }

#endif