#pragma once

namespace sky { namespace lib { namespace process {

/// <summary>
/// ���߃L���[�T�C�Y
/// </summary>
static const u32 ORDER_FIFO_SIZE = 128;

/// <summary>
/// �v���Z�X�Ǘ��N���X
/// �v���Z�X�́A�e�@�\���Ɏ��BEvent�̂悤�ɑS�̂ň���ƁA
/// ��G�c�ɃA�v�������邪�A���G�Ȗ�肪�ł邱�Ƃ������B
/// �Ȃ̂ł��̃N���X��static�N���X�ł͂Ȃ��B
/// </summary>
class CProcessManager : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CProcessManager();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CProcessManager();

	/// <summary>
	/// �v���Z�X�̊֘A�t��
	/// </summary>
	void AttachProcessOrder( u32 prio , const IProcessPtr &process );

	/// <summary>
	/// �v���Z�X�̊֘A�t������
	/// �q�v���Z�X�����R�o�^���������
	/// �w��̃v���Z�X�̎��Ɏ��s���ꂽ�v���Z�X���؂邱�Ƃ��ł���
	/// </summary>
	void DetachProcessOrder( const IProcessPtr &process );
	void DetachProcessOrder( const hash::CHashStringPtr &name );

	/// <summary>
	/// �X�V�i�X�V����߂�ΑS�v���Z�X�͒�~���܂��j
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// �쐬
	/// </summary>
	static CProcessManagerPtr Create();

private:

	/// <summary>
	/// ���ߎ��
	/// </summary>
	enum eOrder
	{
		eOrder_Attach = 0 ,
		eOrder_Detach ,
		eOrder_Detach_Name ,
	};

	/// <summary>
	/// ���ߏ��
	/// </summary>
	struct COrderInfo : public base::SkyRefObject
	{
		eOrder					eId;					//����
		u32						Prio;					//�D�揇��
		IProcessPtr				spProcess;				//�v���Z�X
		hash::CHashStringPtr	Name;
	};
	typedef CMemoryPool< COrderInfo >   COrderInfoPool;
	typedef COrderInfo *				COrderInfoPtr;
	typedef CFifo< COrderInfoPtr >	    COrderInfoPtrFifo;

	/// <summary>
	/// �v���Z�X���X�g
	/// </summary>
	IProcessPtrList		m_ProcessList;

	/// <summary>
	/// ���߃L���[
	/// </summary>
	COrderInfoPtrFifo	m_OrderFifo;

	/// <summary>
	/// ���߃v�[��
	/// </summary>
	COrderInfoPool		m_OrderInfoPool;

	/// <summary>
	/// ���߃L���[�����s����
	/// </summary>
	void ExecOrderFifo();

	/// <summary>
	/// ���߂����s����
	/// </summary>
	void ExecOrder( COrderInfoPtr pOrderInfo );

};

} } }