#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �\�P�b�g��O�N���X
/// </summary>
class CSoketException : public exception::CExceptionBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSoketException( s32 errorId , const skyString *pMessage );

	/// <summary>
	/// �G���[ID�擾
	/// </summary>
	s32 GetErrorId();

	/// <summary>
	/// �v�����g
	/// </summary>
	void Print();

private:

	/// <summary>
	/// �G���[ID
	/// </summary>
	s32 m_ErrorId;
};

} } }