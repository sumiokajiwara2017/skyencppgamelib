#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace graphic {

//�e���v���[�gTypedef�S
class CPixelBufferObject;
typedef SmartPointer< CPixelBufferObject > CPixelBufferObjectPtr;		//�X�}�[�g�|�C���^�Œ�`

/// <summary>
///  PBO�̓s�N�Z���f�[�^���i�[����GPU���̃o�b�t�@
/// </summary>
class CPixelBufferObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
    virtual ~CPixelBufferObject();

	/// <summary>
	/// �t���[���o�b�t�@����PBO�ɓ]������
	/// </summary>
	void TransportFrameBufferToPBO();

	/// <summary>
	/// PBO����e�N�X�`���ɓ]��
	/// </summary>
	void TransportPBOToTexture();

	/// <summary>
	/// PBO�̓��e���O���[�X�P�[���ɂ���
	/// </summary>
	void ConvertGrayScale();

	/// <summary>
	/// ������
	/// </summary>
    void Initialize( const math::CBasicSize &size , u32 channelNum = 4 );

	/// <summary>
	/// ������
	/// </summary>
    void Dispose();

//----Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CPixelBufferObjectPtr Create();

private:

	/// <summary>
	/// ���ƍ���
	/// </summary>
	math::CBasicSize m_Size;

	/// <summary>
	/// �`�����l�����i1�s�N�Z��������̏�񐔁j
	/// </summary>
	u32 m_ChannelNum;

	/// <summary>
	/// PBO�n���h��
	/// </summary>
	GLuint m_PBOHandle;

	/// <summary>
	/// PBO�e�N�X�`���n���h��
	/// </summary>
	GLuint m_PBOTextureHandle;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CPixelBufferObject();
};

} } }

#endif
