#pragma once

namespace sky { namespace lib { namespace state {

class IState;
typedef SmartPointer< IState >		IStatePtr;
typedef CStack< IStatePtr >			IStatePtrStack;
typedef CHash< IStatePtr >			IStatePtrHash;

//�萔
static const u32 REGISTER_STATE_HASH_SIZE	= 256;		//�X�e�[�g�o�^�p�n�b�V���T�C�Y
static const u32 ACTIVE_STATE_STACK_SIZE	= 256;		//���s���̃X�e�[�g�i�[�X�^�b�N�T�C�Y
static const u32 ORDER_FIFO_SIZE			= 256;		//���ߊi�[�L���[�T�C�Y

class CStateManager;
typedef SmartPointer< CStateManager >	CStateManagerPtr;
typedef CList< u32 , CStateManagerPtr >	CStateManagerPtrList;

} } }