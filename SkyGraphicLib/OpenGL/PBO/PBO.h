#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace graphic {

//テンプレートTypedef郡
class CPixelBufferObject;
typedef SmartPointer< CPixelBufferObject > CPixelBufferObjectPtr;		//スマートポインタ版定義

/// <summary>
///  PBOはピクセルデータを格納するGPU側のバッファ
/// </summary>
class CPixelBufferObject : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
    virtual ~CPixelBufferObject();

	/// <summary>
	/// フレームバッファからPBOに転送する
	/// </summary>
	void TransportFrameBufferToPBO();

	/// <summary>
	/// PBOからテクスチャに転送
	/// </summary>
	void TransportPBOToTexture();

	/// <summary>
	/// PBOの内容をグレースケールにする
	/// </summary>
	void ConvertGrayScale();

	/// <summary>
	/// 末期化
	/// </summary>
    void Initialize( const math::CBasicSize &size , u32 channelNum = 4 );

	/// <summary>
	/// 末期化
	/// </summary>
    void Dispose();

//----Static

	/// <summary>
	/// 作成
	/// </summary>
	static CPixelBufferObjectPtr Create();

private:

	/// <summary>
	/// 幅と高さ
	/// </summary>
	math::CBasicSize m_Size;

	/// <summary>
	/// チャンネル数（1ピクセルあたりの情報数）
	/// </summary>
	u32 m_ChannelNum;

	/// <summary>
	/// PBOハンドル
	/// </summary>
	GLuint m_PBOHandle;

	/// <summary>
	/// PBOテクスチャハンドル
	/// </summary>
	GLuint m_PBOTextureHandle;

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CPixelBufferObject();
};

} } }

#endif
