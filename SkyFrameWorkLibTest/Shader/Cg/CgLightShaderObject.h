#pragma once

#ifdef SKY_GRAPHIC_LIB_OPENGL

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct CCgLightShaderObjectMember;

class   CCgLightShaderObject;
typedef SmartPointer< CCgLightShaderObject >		CCgLightShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// ��{�V�F�[�_�[
/// ���C�u�������T�|�[�g���Ă��郉�C�g�iShaderLightManager�Q�Ɓj�����ׂĎg����
/// </summary>
class CCgLightShaderObject : public CShaderObject
{

public:

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	static void Load( CCgLightShaderObjectMember *dest );

	/// <summary>
	/// �j��
	/// </summary>
	static void Destroy();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CCgLightShaderObject();

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void              vDraw();
	CShaderObjectPtr  vCloneNode();

	/// <summary>
	/// �쐬
	/// </summary>
	static CCgLightShaderObjectPtr Create()
    {
		return CCgLightShaderObjectPtr( NEW CCgLightShaderObject() );
    }

private:

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CCgLightShaderObjectMember*    m_pMember;

	/// <summary>
	/// �R���X�g���N�^�i���O�K�{�j
	/// </summary>
	CCgLightShaderObject();

};

} } }

#endif

#endif