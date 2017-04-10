#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �\�P�b�g�A�v���P�[�V�����G���[ID
/// </summary>
enum eSoketAppErrorId
{
	eSoketAppErrorIdSoketNotOpen = 0 ,
	eSoketAppErrorIdSoketAlreadyOpen ,
	eSoketAppErrorIdConnectAlreadyExec ,
	eSoketAppErrorIdConnectNotExec ,
	eSoketAppErrorIdReceiveBufferOver ,
	eSoketAppErrorIdThisAddressIsInvalid ,
};

/// <summary>
/// �\�P�b�g�A�v���P�[�V������O�N���X
/// </summary>
class CSoketAppException : public exception::CExceptionBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSoketAppException( eSoketAppErrorId errorId , const skyString *pMessage );

	/// <summary>
	/// �G���[ID�擾
	/// </summary>
	eSoketAppErrorId GetErrorId();

	/// <summary>
	/// �v�����g
	/// </summary>
	void Print();

private:

	/// <summary>
	/// �G���[ID
	/// </summary>
	eSoketAppErrorId m_eErrorId;
};

} } }