#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// レンダリングステップ優先順位
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
typedef SmartPointer< CRenderStep >					CRenderStepPtr;			//スマートポインタ型定義
typedef CList< RenderStep_Prio , CRenderStepPtr >	CRenderStepPtrList;		//スマートポインタリスト型
typedef CHash< CRenderStepPtr >						CRenderStepPtrHash;		//Hash定義

class CRenderStepManager;
typedef SmartPointer< CRenderStepManager >	        CRenderStepManagerPtr;			//スマートポインタ型定義

static const u32 RENDER_THREAD_DEFAULT_STACK_SIZE = 1024 * 2;

} } }