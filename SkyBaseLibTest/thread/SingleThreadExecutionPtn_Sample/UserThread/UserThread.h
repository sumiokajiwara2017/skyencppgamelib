#pragma once

/// <summary>
/// ���[�U�[�X���b�h�N���X
/// </summary>
class UserThread : public sky::lib::thread::ThreadWin32
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UserThread( const sky::lib::SmartPointerMT< ThreadCommonRes >& res , s32 threadNo );

	/// <summary>
	/// �������\�b�h
	/// </summary>
	void ThreadMain( void );

protected:

	/// <summary>
	/// �����o
	/// </summary>
	sky::lib::SmartPointerMT< ThreadCommonRes >	m_Res;
	s32											m_ThreadNo;
};
