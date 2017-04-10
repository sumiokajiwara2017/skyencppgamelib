#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_LISP

namespace sky { namespace lib { namespace lisp {

namespace lispcontroller_const
{
	static const u32 INPUT_MAX_CHAR_NUM = 1024;
};

enum eLispUpdateMode
{
	eLispUpdateMode_InputConsole = 0 ,
	eLispUpdateMode_InputFileExec,
};

/// <summary>
/// Lisp�R���g���[���[
/// </summary>
class LispController : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LispController			();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~LispController	();

	/// <summary>
	/// Lisp�̏�����
	/// </summary>
	skyBool vIsInitialize	();

	/// <summary>
	/// Lisp�̖�����
	/// </summary>
	skyBool vIsDispose		();

	/// <summary>
	/// �t�@�C���Ǎ�
	/// </summary>

	/// <summary>
	/// �֐����s
	/// </summary>

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �V���O���g���@�\�t��
	/// <summary>
	friend class Singleton< LispController >;
	static Singleton< LispController > Singleton;

private:

	/// <summary>
	/// �������t���O
	/// </summary>
	skyBool		m_IsInit;

};

} } }

#else

#define LispControllerCreate_()	( ()0)
#define LispController_			( ()0)
#define LispControllerDelete_()	( ()0)

#endif