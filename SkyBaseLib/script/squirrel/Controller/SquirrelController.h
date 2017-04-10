#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// ���C�u�����R���g���[���[
/// </summary>
class SquirrelController : public base::SkyObject
{

public:

	/// <summary>
	/// SkySquirrelLib�̏�����
	/// </summary>
	static void Initialize							();

	/// <summary>
	/// SkySquirrelLib�̖�����
	/// </summary>
	static void Dispose							();

	/// <summary>
	/// SkySquirrelLib�̏������m�F
	/// </summary>
	static skyBool IsInit							();

private:

	/// <summary>
	/// �������t���O
	/// </summary>
	static skyBool		m_IsInit;

};

} } }

#define SquirrelControllerInitialize_() sky::lib::squirrel::SquirrelController::Initialize()
#define SquirrelController_             sky::lib::squirrel::SquirrelController
#define SquirrelControllerDispose_()    sky::lib::squirrel::SquirrelController::Dispose()
#endif