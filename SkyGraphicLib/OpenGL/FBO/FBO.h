#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace graphic {

//テンプレートTypedef郡
class CFramebufferObject;
typedef SmartPointer< CFramebufferObject > CFramebufferObjectPtr;		//スマートポインタ版定義

/// <summary>
/// フレームバッファオブジェクトクラス
/// </summary>
class CFramebufferObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
    virtual ~CFramebufferObject();

	/// <summary>
	/// 初期化
	/// </summary>
	void InitializeWithGlew( const math::CBasicSize &size );
    void Initialize( const math::CBasicSize &size );

	/// <summary>
	/// テクスチャIDの取得
	/// </summary>
	GLuint GetTextureId(){ return m_ColorBufferTextureHandl; }

	/// <summary>
	/// 末期化
	/// </summary>
    void Dispose();

	/// <summary>
	/// 描画
	/// </summary>
    void PreRender();
    void Render( void ( *render_func )( void ) );
    void PostRender();

//----Static

	/// <summary>
	/// 作成
	/// </summary>
	static CFramebufferObjectPtr Create();

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CFramebufferObject();

	/// <summary>
	/// 幅と高さ
	/// </summary>
	math::CBasicSize m_Size;

	/// <summary>
	/// 各種ハンドル
	/// </summary>
    GLuint m_FBOHandl;
    GLuint m_DepthStencilBufferHandle;
    GLuint m_ColorBufferTextureHandl;

};

} } }

#endif
