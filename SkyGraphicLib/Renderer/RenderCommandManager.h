#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �����_�����O�R�}���h�Ǘ�
/// </summary>
class CRenderCommandManager : public base::SkyObject
{

friend class CRenderThread;

public:

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	enum eState
	{
		eState_Stop = 0 ,
		eState_Rendering ,
		eState_RenderingStopRequest ,
		eState_RendringStop ,
	};

	/// <summary>
	/// ������
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// �L���[�̍쐬�i�Ȃ��������ō쐬���Ȃ����́A�ǂꂾ���̒����̃L���[�����邩���e�Q�[���Ō��肵�Ă��炢��������j
	/// </summary>
	static void    CreateQueue( u32 queueSize );

	/// <summary>
	/// �R�}���h�o�b�t�@�̍쐬
	/// </summary>
	static void    CreateBuffer();

	/// <summary>
	/// �R�}���h�o�b�t�@�ɃR�}���h��ǉ�����
	/// </summary>
	static void    InvokeCommand( const CRenderingCommandPtr &command );

	/// <summary>
	/// �R�}���h�o�b�t�@�����s���Ă��܂��B
	/// </summary>
	static void    FlashBuffer();

	/// <summary>
	/// �R�}���h�o�b�t�@���L���[�ɒǉ�����
	/// </summary>
	static void    PushBuffer();

	/// <summary>
	/// �R�}���h�o�b�t�@���L���[����擾����
	/// </summary>
	static CRenderingCommandPtrList*  PopBuffer();

	/// <summary>
	/// �R�}���h���s
	/// </summary>
	static void ExecCommand( CRenderingCommandPtrList *pBuffer );

	/// <summary>
	/// �`���~�v��
	/// </summary>
	static void RequestRenderingStop();

	/// <summary>
	/// �`��ĊJ�v��
	/// </summary>
	static void RequestRenderingReStart();

	/// <summary>
	/// �`���~�҂�
	/// </summary>
	static void WaiteRenderingStop();

	/// <summary>
	/// �`�撆����
	/// </summary>
	static skyBool IsRenderingExec();

	/// <summary>
	/// �X���b�h���s
	/// </summary>
	static void StartThread();

	/// <summary>
	/// �X���b�h��~
	/// </summary>
	static void StopThread();

	/// <summary>
	/// �����_�����O���O�R�[���f���Q�[�g
	/// </summary>
	static delegate::CDelegateHandler& PreRendaringEvent();

	/// <summary>
	/// �����_�����O����R�[���f���Q�[�g
	/// </summary>
	static delegate::CDelegateHandler& PostRendaringEvent();

	/// <summary>
	/// �����_�����O���O�^�X�N���s�^�X�N�v��
	/// �^�X�N�͉��L�̂悤�ɍ쐬���A���̊֐��̈����ɓn��
	/// �� �jFunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderTarget , spRenderTarget , skyFalse ) )
	/// </summary>
	static void AddPreRenderingExecEvent( s32 prio , const CRenderingCommandPtr &spFunc );

	/// <summary>
	/// �����_�����O����^�X�N���s�^�X�N�v��
	/// �^�X�N�͉��L�̂悤�ɍ쐬���A���̊֐��̈����ɓn��
	/// �� �jFunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderTarget , spRenderTarget , skyFalse ) )
	/// </summary>
	static void AddPostRenderingExecEvent( s32 prio , const CRenderingCommandPtr &spFunc );

private:

	static eState m_eState;

	/// <summary>
	/// �R�}���h�o�b�t�@
	/// </summary>
	static CRenderingCommandPtrList*          m_pCmmandBuffer;

	/// <summary>
	/// �R�}���h�o�b�t�@�L���[
	/// </summary>
	static CRenderingCommandPtrListPtrFifo*   m_pCommandBufferQueue;

	/// <summary>
	/// �����_�����O���O�����C�x���g
	/// </summary>
	static delegate::CDelegateHandler*        m_pPreRenderingEventHndl;

	/// <summary>
	/// �����_�����O���㔭���C�x���g
	/// </summary>
	static delegate::CDelegateHandler*        m_pPostRenderingEventHndl;

	/// <summary>
	/// �����_�����O���O���s�R�}���h���X�g
	/// </summary>
	static CRenderingCommandPtrListMT*          m_pPreRenderingCommand;

	/// <summary>
	/// �����_�����O������s�R�}���h���X�g
	/// </summary>
	static CRenderingCommandPtrListMT*          m_pPostRenderingCommand;

};

} } }

//�C���X�^���X�A�N�Z�X������
#define CRenderCommandManagerInitialize_()	sky::lib::graphic::CRenderCommandManager::Initialize()
#define CRenderCommandManager_				sky::lib::graphic::CRenderCommandManager
#define CRenderCommandManagerDispose_()		sky::lib::graphic::CRenderCommandManager::Dispose()
