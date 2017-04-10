#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace graphic {

//�e���v���[�gTypedef�S
class CFramebufferObject;
typedef SmartPointer< CFramebufferObject > CFramebufferObjectPtr;		//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �t���[���o�b�t�@�I�u�W�F�N�g�N���X
/// </summary>
class CFramebufferObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
    virtual ~CFramebufferObject();

	/// <summary>
	/// ������
	/// </summary>
	void InitializeWithGlew( const math::CBasicSize &size );
    void Initialize( const math::CBasicSize &size );

	/// <summary>
	/// �e�N�X�`��ID�̎擾
	/// </summary>
	GLuint GetTextureId(){ return m_ColorBufferTextureHandl; }

	/// <summary>
	/// ������
	/// </summary>
    void Dispose();

	/// <summary>
	/// �`��
	/// </summary>
    void PreRender();
    void Render( void ( *render_func )( void ) );
    void PostRender();

//----Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CFramebufferObjectPtr Create();

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CFramebufferObject();

	/// <summary>
	/// ���ƍ���
	/// </summary>
	math::CBasicSize m_Size;

	/// <summary>
	/// �e��n���h��
	/// </summary>
    GLuint m_FBOHandl;
    GLuint m_DepthStencilBufferHandle;
    GLuint m_ColorBufferTextureHandl;

};

} } }

#endif
