#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#include "SkyBaseLib/Common.h"

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// GLSLクラス
/// </summary>
class CGLSL : base::SkyObject
{

public:

	/// <summary>
	/// 頂点シェーダの読み込み
	/// </summary>
    static void CreateVertexShader( GLuint &dest , const skyString *src );
    static void CreateVertexShader( GLuint &dest , stream::IStreamPtr &src );

	/// <summary>
	/// フラグメントシェーダの読み込み
	/// </summary>
    static void CreateFragmentShader( GLuint &dest , const skyString *src );
    static void CreateFragmentShader( GLuint &dest , stream::IStreamPtr &src );

	/// <summary>
	/// 頂点シェーダとフラグメントシェーダの読み込み（両者をリンクする）
	/// </summary>
    static void CreateVertexShaderAndFragmentShader(  GLuint &dest , const skyString *srcVertex , const skyString *srcFragment );
    static void CreateVertexShaderAndFragmentShader(  GLuint &dest , stream::IStreamPtr &srcVertex , stream::IStreamPtr &srcFragment );

private:

	/// <summary>
	/// コンパイル
	/// </summary>
	static void Compile_( GLuint shader , stream::IStreamPtr &src );

	/// <summary>
	/// リンク
	/// </summary>
	static void Link_( GLuint prog );

};

} } }

#endif
