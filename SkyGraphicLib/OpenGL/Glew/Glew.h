#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// Glew�N���X
/// </summary>
class CGlew : base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize	();
	static void Initialize( GLEWContext *pContext );

	/// <summary>
	/// �R���e�L�X�g�̐���
	/// </summary>
	static GLEWContext *CreateContext();

	/// <summary>
	/// �R���e�L�X�g�̍폜
	/// </summary>
	static void DeleteContext( GLEWContext * );

	/// <summary>
	/// ���݂̃R���e�L�X�g��ݒ肷��
	/// </summary>
	static void SetCurrentContext( GLEWContext *current );

};

} } }