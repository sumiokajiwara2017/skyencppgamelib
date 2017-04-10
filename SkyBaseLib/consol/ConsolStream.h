#pragma once

namespace sky { namespace lib { namespace consol {

struct CConsolStreamMember;

static const u32 CONSOL_IO_BUF_LEN = 256;

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
///  �R���\�[���X�g���[��
/// </summary>
class CConsolStream  : public stream::AbsStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
	/// </summary>  
	skyBool     vOpen();
	skyBool     vClose();
	u32			vWriteStream( const skyString *format , ... );
	u32		    vReadStream( skyString  *pData , u32 len );

	/// <summary>
	///  �R���X�g���N�^
	/// </summary>
	CConsolStream();

	/// <summary>
	///  �f�X�g���N�^
	/// </summary>
	virtual ~CConsolStream();

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CConsolStreamMember *m_pMember;
};

} } }