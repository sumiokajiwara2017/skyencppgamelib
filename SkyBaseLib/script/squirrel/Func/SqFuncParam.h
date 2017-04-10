#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

class SqClassObject;

class SqParamObject;
typedef CList< u32 , SqParamObject >		SqParamObjectList;	//リスト定義
typedef CHash< SqParamObject >				SqParamObjectHash;	//リスト定義

/// <summary>
/// パラメーターの型
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
/// Ｃ＋＋関数情報
/// </summary>
class SqCppFunc : public base::SkyObject
{
public:
	SQFUNCTION m_f;
	hash::CHashStringPtr m_fname;
	hash::CHashStringPtr m_typemask;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SqCppFunc(){}
	SqCppFunc( SQFUNCTION f , const hash::CHashStringPtr &fname , const hash::CHashStringPtr &typemask = CHS( _T("") ) )
	{
		Set( f , fname , typemask );
	}

	/// <summary>
	/// 設定
	/// </summary>
	void Set( SQFUNCTION f , const hash::CHashStringPtr  &fname , const hash::CHashStringPtr &typemask = CHS( _T("") ) )
	{
		m_f = f;
		m_fname = fname;
		m_typemask = typemask;
	}

	/// <summary>
	/// 代入演算子のオーバーライド
	/// </summary>
	SqCppFunc& operator =( const SqCppFunc& src )
	{
		this->Set( src.m_f , src.m_fname , src.m_typemask );
		return *this;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SqCppFunc()
	{
	}
};
typedef CList< u32 , SqCppFunc >		SqCppFuncList;	//リスト定義
typedef CHash< SqCppFunc >				SqCppFuncHash;	//リスト定義

/// <summary>
/// Squirrel関数パラメーター情報
/// </summary>
class SqParamObject : public base::SkyObject
{
public :

	/// <summary>
	/// パラメーター共用体
	/// </summary>
	union Param {

	public:

		/// <summary>
		/// コンストラクタ
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
	/// パラメーター
	/// </summary>
	Param		m_Param;

	/// <summary>
	/// パラメーターの種類
	/// </summary>
	eParamType	m_eParamType;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SqParamObject();
	SqParamObject( s32			param );			//使う時は( s32 )でキャストすること
	SqParamObject( f32			param );			//使う時は( f32 )でキャストすること
	SqParamObject(const skyString *param );
	SqParamObject( skyBool		param );			//使う時は( skyBool )でキャストすること
	SqParamObject( void			*param );			//使う時は( void * )でキャストすること
	SqParamObject( SqParamObjectList	*param );	//使う時は( SqParamObjectList * )でキャストすること
	SqParamObject( SqParamObjectHash	*param );	//使う時は( SqParamObjectHash * )でキャストすること
	SqParamObject( SqClassObject	*param );		//使う時は( SqClassObject * )でキャストすること
	SqParamObject( SqCppFunc	*param );			//使う時は( SqCppFunc * )でキャストすること

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SqParamObject();

	/// <summary>
	/// 値Setter
	/// </summary>
	void SetParam( s32			param );			//使う時は( s32 )でキャストすること
	void SetParam( f32			param );			//使う時は( f32 )でキャストすること
	void SetParam( const skyString *param );
	void SetParam( skyBool		param );			//使う時は( skyBool )でキャストすること
	void SetParam( void			*param );			//使う時は( void * )でキャストすること
	void SetParam( SqParamObjectList	*param );	//使う時は( SqParamObjectList * )でキャストすること
	void SetParam( SqParamObjectHash	*param );	//使う時は( SqParamObjectHash * )でキャストすること
	void SetParam( SqClassObject	*param );		//使う時は( SqClassObject * )でキャストすること
	void SetParam( SqCppFunc	*param );			//使う時は( SqCppFunc * )でキャストすること

	/// <summary>
	/// 代入演算子のオーバーライド
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
	/// キャストのオーバーライド
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
	/// 値をVoidポインタとして取得する
	/// </summary>
	void *GetVoidPointer(){ return m_Param.m_Void; }

	/// <summary>
	/// 値にskyNullを設定する
	/// </summary>
	void SetNull(){ m_Param.m_Void = skyNull; }

	/// <summary>
	/// 値の型を取得
	/// </summary>
	eParamType GetType() const { return m_eParamType; }

	/// <summary>
	/// 値を取得
	/// </summary>
	Param &GetParam(){ return m_Param; }

	/// <summary>
	/// 値をスタックにプッシュする
	/// </summary>
	void PushSquirrelStack( HSQUIRRELVM v );

	/// <summary>
	/// 値をスタックからポップする
	/// </summary>
	void PopSquirrelStack( HSQUIRRELVM v );
};

/// <summary>
/// Squirrel関数パラメーター
/// </summary>
class SqFuncParam : public base::SkyObject
{

public:

	/// <summary>
	/// []演算子オーバーライド
	/// </summary>
	const SqParamObject	&operator[]( u32 idx ) const;

	/// <summary>
	/// パラメーター配列
	/// </summary>
	SqParamObject	m_Param[ sq_const::FUNC_PARAM_MAX_NUM ];

	/// <summary>
	/// パラメーターセット数
	/// </summary>
	u32			m_ParamNum;

	/// <summary>
	/// 代入演算子
	/// </summary>
	SqFuncParam& operator =( const SqFuncParam *pIn );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SqFuncParam();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SqFuncParam();

	/// <summary>
	/// 値Setter(値が設定されるとSqParamObject構造体に値が渡される）
	/// </summary>
	void SetParam( s32			 param );				//使う時は( s32 )でキャストすること
	void SetParam( f32			 param );				//使う時は( f32 )でキャストすること
	void SetParam( const hash::CHashStringPtr &param );
	void SetParam( skyBool		 param );				//使う時は( skyBool )でキャストすること
	void SetParam( void			*param );				//使う時は( void * )でキャストすること
	void SetParam( const SqParamObjectList *param );	//使う時は( SqParamObjectList * )でキャストすること
	void SetParam( const SqParamObjectHash *param );	//使う時は( SqParamObjectHash * )でキャストすること
	void SetParam( const SqClassObject *param );		//使う時は( SqClassObject * )でキャストすること
	void SetParam( const SqCppFunc *param );			//使う時は( SqCppFunc * )でキャストすること。一時的なテーブルにネイティブクロージャーが登録されるので、引数として受け取った値に.をつけその後関数名で呼び出す(例 a.FuncName() )

	/// <summary>
	/// 値をスタックにプッシュする
	/// </summary>
	void PushSquirrelStack( HSQUIRRELVM v );

	/// <summary>
	/// 値をスタックからポップする
	/// </summary>
	void PopSquirrelStack( HSQUIRRELVM v );

	/// <summary>
	/// パラメーター数を取得
	/// </summary>
	inline u32 GetParamNum(){ return m_ParamNum; };

	/// <summary>
	/// パラメーターを取得
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