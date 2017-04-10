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
/// �X�^�b�N�g���[�X�o�͍ő�s��
/// </summary>
static const u32 STACK_TRACE_MAX = 256;

/// <summary>
/// ��O�x�[�X�N���X
/// </summary>
class CExceptionBase : public std::exception , public base::SkyObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CExceptionBase( const skyString *pMessage );
	CExceptionBase( const skyString *pMessage , 
				   const skyString *pFileName ,
				   const skyString *pFunctionName , u32 line );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
    virtual ~CExceptionBase() throw();

	/// <summary>
	/// ���b�Z�[�W
	/// </summary>
	const skyString *GetMessage();

	/// <summary>
	/// �t�@�C����
	/// </summary>
	const skyString *GetFileName();

	/// <summary>
	/// �֐���
	/// </summary>
	const skyString *GetFunction();

	/// <summary>
	/// �s��
	/// </summary>
	u32 GetLine();

protected:

	/// <summary>
	/// ���b�Z�[�W
	/// </summary>
	string::CString m_Message;

	/// <summary>
	/// �t�@�C����
	/// </summary>
	string::CString m_FileName;

	/// <summary>
	/// �֐���
	/// </summary>
	string::CString m_FunctionName;

	/// <summary>
	/// �s��
	/// </summary>
	u32		m_Line;
};

} } }
