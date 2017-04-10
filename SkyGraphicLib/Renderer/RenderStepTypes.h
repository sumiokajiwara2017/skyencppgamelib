#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �����_�����O�X�e�b�v�D�揇��
/// </summary>
enum RenderStep_Prio
{
	RenderStep_Prio_1 = 0 ,
	RenderStep_Prio_2 ,
	RenderStep_Prio_3 ,
	RenderStep_Prio_4 ,
	RenderStep_Prio_5 ,
	RenderStep_Prio_6 ,
	RenderStep_Prio_7 ,
	RenderStep_Prio_8 ,
	RenderStep_Prio_9 ,
	RenderStep_Prio_10 ,
	RenderStep_Prio_11 ,
};

class CRenderStep;
typedef SmartPointer< CRenderStep >					CRenderStepPtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< RenderStep_Prio , CRenderStepPtr >	CRenderStepPtrList;		//�X�}�[�g�|�C���^���X�g�^
typedef CHash< CRenderStepPtr >						CRenderStepPtrHash;		//Hash��`

class CRenderStepManager;
typedef SmartPointer< CRenderStepManager >	        CRenderStepManagerPtr;			//�X�}�[�g�|�C���^�^��`

static const u32 RENDER_THREAD_DEFAULT_STACK_SIZE = 1024 * 2;

} } }