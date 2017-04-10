#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

SqVmObject::SqVmObject( u32 stackSize ) :
m_Status( eThreadStatus_Wait ) ,
m_spParent( skyNull ) ,
m_ChildStack( stackSize )
{
	//�o�[�`�����}�V���̍쐬
	m_Vm = CSqUtil_::CreateSQVM( stackSize );

	//�W�����C�u������Push
	CSqUtil_::RegisterStdLib( m_Vm );
}

SqVmObject::SqVmObject( SqVmObjectPtr &spParent , u32 stackSize ) :
m_Status( eThreadStatus_Wait ) ,
m_spParent( spParent ) ,
m_ChildStack( stackSize )
{
	m_spParent = spParent;

	//�q�X���b�h�̍쐬�i�e�̃X�^�b�N���Vm���쐬����B���Ȃ킿�APop���邱�ƂŃ������̈���J�������j
	m_Vm = CSqUtil_::CreateNewThread( spParent->Vm() , stackSize );
}

SqVmObject::~SqVmObject()
{
	//�q�����ׂĔj��
	ChildAllPop();

	//�������e�����Ȃ���Ύ����ŏ����Ă���
	if ( m_spParent.IsEmpty() )
	{
		//�o�[�`�����}�V���̔j��
		CSqUtil_::DestroySQVM( m_Vm );
	}
}

SqVmObject::eThreadStatus	SqVmObject::GetASyncStatus()
{
	return m_Status;
}

SqVmObjectPtr &SqVmObject::CreateAndRegisterChildVm( u32 stackSize , SqVmObjectPtr &spParent )
{
	m_ChildStack.Push( SqVmObjectPtr( NEW__( SqVmObject , SqVmObject( spParent , stackSize ) ) ) );
	return m_ChildStack.GetTop();
}

void SqVmObject::ChildPop()
{
	if ( m_ChildStack.GetNum() > 0 )
	{
		if ( m_ChildStack.GetTop()->GetASyncStatus() != eThreadStatus_End )
		{
            SKY_PRINTF( _T( "This child is alive.  \n" ) );
		}

		//�e����Pop(�e�̃X�^�b�N�ɋ����̂ŁAPop����邱�Ƃŗ̈���J������Ă���悤���j
		sq_pop( m_Vm , 1 );

		//�X�^�b�N�̒��̃X�}�[�g�|�C���^����̃C���X�^���X���폜�i������ƃ������j����N�����j
		m_ChildStack.GetTop().Delete();

		//�X�^�b�N�̒��������o���B
		m_ChildStack.Pop();
	}
}

void SqVmObject::ChildAllPop()
{
	//�S�q���폜
	while( m_ChildStack.GetNum() > 0 )
	{
		ChildPop();
	}
}

SqVmObjectPtr &SqVmObject::Parent()
{
	return m_spParent;
}

skyBool SqVmObject::IsAllChildEnd()
{
	//�q�����Ȃ����true
	if ( m_ChildStack.GetNum() > 0 )
	{
		//�q�̑S�Ă̎��s���m�F����
		FOREACH( SqVmObjectPtrStack , it , m_ChildStack )
		{
			if ( ( *it )->GetASyncStatus() != eThreadStatus_End )
			{
				return skyFalse;
			}
		}
	}

	return skyTrue;
}

HSQUIRRELVM &SqVmObject::Vm()
{
	return m_Vm;
}

void SqVmObject::RegisterScriptFile( const hash::CHashStringPtr &pPath )
{
	CSqUtil_::RegisterScriptFile( m_Vm , pPath );
}

SqVmObject::eThreadStatus SqVmObject::ExecuteFuncSync( const hash::CHashStringPtr &pFunc, SqFuncParam &inParam , SqFuncParam &outParam )
{
	CSqUtil_::ExecuteFunc( m_Vm , pFunc, inParam , outParam );
	return eThreadStatus_End;
}

void SqVmObject::InitExecuteFuncASync()
{
	m_Status = eThreadStatus_Init;
}

SqVmObject::eThreadStatus SqVmObject::ExecuteFuncASync( const hash::CHashStringPtr &pFunc, SqFuncParam &inParam , SqFuncParam &outParam )
{
	switch ( m_Status )
	{
		case eThreadStatus_Wait:

			SKY_PANIC_MSG( _T( "ExecuteFuncASync() had been called before InitExecuteFuncASync() was called. " ) ); //InitExecuteFuncASync()���Ă΂��O��ExecuteFuncASync���Ă΂�܂����B�K���Ă�ł���ɂ��Ă��������ˁB
			break;

		case eThreadStatus_Init:	//������------------------------��
		{
			// �Ăяo���O�̃X�^�b�N�T�C�Y��ۑ�
			SaveFuncStartVmStackTop();		

			// �֐����X�^�b�N�ɏ悹��
			sq_pushstring(m_Vm , pFunc->GetString(), -1);

			// �֐������[�g�e�[�u������擾
			if ( SQ_SUCCEEDED( sq_get(m_Vm , -2) ) )
			{					
				//������
				sq_pushroottable( m_Vm );				// "this"��push (���[�g�e�[�u����)

				//�������ȍ~�i�ϐ��j
				inParam.PushSquirrelStack( m_Vm );
			}
			else
			{
                SKY_PANIC_MSG( _T( "It failed in the registration of the function.funcName=[ %s ]" ) , pFunc );
			}

			//�ŏ��̃T�X�y���h�܂Ŏ��s
			if ( !SQ_SUCCEEDED( sq_call( m_Vm , inParam.GetParamNum() + 1 , SQTrue , SQTrue ) ) ) //���s�I�����T�X�y���h�܂ł����ɋ��邾�낤�B
			{
                SKY_PANIC_MSG( _T( "It failed in the registration of the function.funcName=[ %s ]" ) , pFunc );
			}

			//���݂�VM�̏�Ԃ��擾����
			outParam.PopSquirrelStack( m_Vm );

			//���f��ԂȂ�Ήi���X�V�ցA�������I�����Ă����疖�����֍s��
			if ( sq_getvmstate( m_Vm ) == SQ_VMSTATE_SUSPENDED )
			{
				//���s��Ԃ�
				m_Status = eThreadStatus_Execute;
			}
			else
			{
				//����������
				m_Status = eThreadStatus_Term;
			}

			break;
		}
		case eThreadStatus_Execute:		//���s��------------------------��
		{
			//���s�ĊJ
			//�������̈Ӗ����킩��񂪁A�����Pop���Ă��������Ȃ�̂�SQFalse�����Ă����܂��B�����֗��Ȃ̂��ȁH
			if ( !SQ_SUCCEEDED( sq_wakeupvm( m_Vm , SQFalse , SQTrue , SQTrue , SQFalse ) ) ) //���s�I�����T�X�y���h�܂ł����ɋ��邾�낤�B
			{
                SKY_PANIC_MSG( _T( "The execution of the function failed.\n" ) );
			}

			//�߂�l�󂯎��^�C�~���O
			outParam.PopSquirrelStack( m_Vm );

			switch ( sq_getvmstate( m_Vm ) )
			{
			case SQ_VMSTATE_IDLE:		//�ҋ@��

				//������
				m_Status = eThreadStatus_Term;

				break;
			case SQ_VMSTATE_RUNNING:	//���s��

				//���������񂾂낤�E�E�B

				break;
			case SQ_VMSTATE_SUSPENDED:	//���f��

				//�Ȃ�����Ȃ����B

				break;
			}
			break;
		}
		case eThreadStatus_Term:		//������------------------------��
		{
			// ���̃X�^�b�N�T�C�Y�ɖ߂�
			LoadFuncStartVmStackTop();

			//���s�I��
			m_Status = eThreadStatus_End;

			//no brea;
		}
		case eThreadStatus_End:			//���s�I��------------------------��
		{

			//�������Ȃ��B�I��������ƂɋC�Â��āI

			break;
		}
	}
	return m_Status;
}

HSQUIRRELVM SqVmObject::GetChildVm( u32 childNo )
{
	return m_ChildStack.GetItem( childNo )->Vm();
}

void SqVmObject::SaveFuncStartVmStackTop()
{
	// �Ăяo���O�̃X�^�b�N�T�C�Y��ۑ�
	m_FuncStartVmStackTop = sq_gettop( m_Vm );
}

void SqVmObject::LoadFuncStartVmStackTop()
{
	// ���̃X�^�b�N�T�C�Y�ɖ߂�
	sq_settop(m_Vm , m_FuncStartVmStackTop);
}

void SqVmObject::SetASyncStatus( eThreadStatus eStatus )
{
	m_Status = eStatus;
}

void SqVmObject::RegisterNativeFunc( SQFUNCTION f , const skyString *fname )
{
	CSqUtil_::RegisterNativeFunc( m_Vm , f , fname );
}

void SqVmObject::RegisterScriptString( const hash::CHashStringPtr &pScript )
{
	CSqUtil_::RegisterScriptString( m_Vm , pScript );
}

void SqVmObject::RegisterClass( const SqClassObject &classObj )
{
	CSqUtil_::RegisterClass( m_Vm , classObj );
}

void SqVmObject::SetSkyBaseLibClass()
{
	//�N���X��o�^����
	RegisterClass( GetSqInputManager() );
	RegisterClass( GetSqCDosUtil() );
}

SqObjectInterfacePtr SqVmObject::Compile( const stream::IStreamPtr &pSrc )
{
	return CSqUtil_::Compile( m_Vm , pSrc );
}

void SqVmObject::CompileOutStream( const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest )
{
	CSqUtil_::CompileOutStream( m_Vm , pSrc , pDest );
}

void SqVmObject::RegisterNativeDebugHook( SQDEBUGHOOK debughook )
{
	CSqUtil_::RegisterNativeDebugHook( m_Vm , debughook );
}
void SqVmObject::RegisterDebugHook()
{
	CSqUtil_::RegisterDebugHook( m_Vm );
}
void SqVmObject::SetDebug( skyBool isOn )
{
	CSqUtil_::SetDebug( m_Vm , isOn );
}

} } }

#endif
