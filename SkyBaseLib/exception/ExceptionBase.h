#pragma once

namespace sky { namespace lib { namespace exception {

#define THROW_EXCEPTION( EXCEPTION_TYPE, message ) \
		throw EXCEPTION_TYPE( message , T__FILE__ , T__FUNCTION__ , __LINE__) 

#define ASSERT_THROW_EXCEPTION( isSuccess , constractor ) \
	if ( isSuccess == skyFalse ) \
	{ \
		throw constractor; \
	} \

#define PANIC_THROW_EXCEPTION( constractor ) \
		throw constractor; \

/// <summary>
/// スタックトレース出力最大行数
/// </summary>
static const u32 STACK_TRACE_MAX = 256;

/// <summary>
/// 例外ベースクラス
/// </summary>
class CExceptionBase : public std::exception , public base::SkyObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CExceptionBase( const skyString *pMessage );
	CExceptionBase( const skyString *pMessage , 
				   const skyString *pFileName ,
				   const skyString *pFunctionName , u32 line );

	/// <summary>
	/// デストラクタ
	/// </summary>
    virtual ~CExceptionBase() throw();

	/// <summary>
	/// メッセージ
	/// </summary>
	const skyString *GetMessage();

	/// <summary>
	/// ファイル名
	/// </summary>
	const skyString *GetFileName();

	/// <summary>
	/// 関数名
	/// </summary>
	const skyString *GetFunction();

	/// <summary>
	/// 行数
	/// </summary>
	u32 GetLine();

protected:

	/// <summary>
	/// メッセージ
	/// </summary>
	string::CString m_Message;

	/// <summary>
	/// ファイル名
	/// </summary>
	string::CString m_FileName;

	/// <summary>
	/// 関数名
	/// </summary>
	string::CString m_FunctionName;

	/// <summary>
	/// 行数
	/// </summary>
	u32		m_Line;
};

} } }
