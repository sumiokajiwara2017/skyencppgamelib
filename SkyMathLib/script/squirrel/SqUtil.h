#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace math { namespace squirrel {

/// <summary>
/// squirrel���ʏ����Q
/// </summary>
class CSqUtil
{

public:

	/// <summary>
	/// SkyMathLib�̃o�C���h�N���X��VM�ɓo�^����
	/// </summary>
	static void RegisterSkyMathLibClass( HSQUIRRELVM v);
};

} } } }

//�o�C���h�N���X��`���擾
extern sky::lib::squirrel::SqClassObject GetSqVector2();

#endif