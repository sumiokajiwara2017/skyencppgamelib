#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#include "SkyBaseLib/Common.h"

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// GLSL�N���X
/// </summary>
class CGLSL : base::SkyObject
{

public:

	/// <summary>
	/// ���_�V�F�[�_�̓ǂݍ���
	/// </summary>
    static void CreateVertexShader( GLuint &dest , const skyString *src );
    static void CreateVertexShader( GLuint &dest , stream::IStreamPtr &src );

	/// <summary>
	/// �t���O�����g�V�F�[�_�̓ǂݍ���
	/// </summary>
    static void CreateFragmentShader( GLuint &dest , const skyString *src );
    static void CreateFragmentShader( GLuint &dest , stream::IStreamPtr &src );

	/// <summary>
	/// ���_�V�F�[�_�ƃt���O�����g�V�F�[�_�̓ǂݍ��݁i���҂������N����j
	/// </summary>
    static void CreateVertexShaderAndFragmentShader(  GLuint &dest , const skyString *srcVertex , const skyString *srcFragment );
    static void CreateVertexShaderAndFragmentShader(  GLuint &dest , stream::IStreamPtr &srcVertex , stream::IStreamPtr &srcFragment );

private:

	/// <summary>
	/// �R���p�C��
	/// </summary>
	static void Compile_( GLuint shader , stream::IStreamPtr &src );

	/// <summary>
	/// �����N
	/// </summary>
	static void Link_( GLuint prog );

};

} } }

#endif
