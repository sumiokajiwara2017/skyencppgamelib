#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// Squirrel�̑g�ݍ��݃f�o�b�O�֐��Q
/// </summary>
class SqDebugApi : public base::SkyObject
{

public:

	/// <summary>
	/// �`���������X�g���[���ɕԂ�
	/// </summary>
	static void PrintApiName( stream::IStreamPtr &spStream );

	/// <summary>
	/// �`�������u�l�ɓo�^����
	/// </summary>
	static void RegisterApi( HSQUIRRELVM v );
};

//�C���X�^���X�A�N�Z�X������
#define SqDebugApi_			sky::lib::squirrel::SqDebugApi

} } }

#endif