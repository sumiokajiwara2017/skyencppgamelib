#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// スプライト便利関数群
/// </summary>
class CSpriteUtil
{

public:

	/// <summary>
	/// ２Ｄの線の作成
	/// </summary>
	static CSpritePtr CreateLine2D( const math::CBasicVector2Vector &vector , const CColor &diffuseColor = CColor_Red , dectype prio = RENDERING_2D_PRIO_FAST , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// ２Ｄの線の作成
	/// </summary>
	static CSpritePtr CreateSquareLine2D( dectype width = 1.0f , dectype height = 1.0f , const CColor &diffuseColor = CColor_Red , dectype prio = RENDERING_2D_PRIO_FAST , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// 矩形の作成
	/// </summary>
	static CSpritePtr CreateSquare( const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &diffuseColor = CColor_Red ,  dectype prio = RENDERING_2D_PRIO_FAST , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// テクスチャをそのまま出力する矩形の作成
	/// </summary>
	static CSpritePtr CreateSquare( const CTexturePtr &texPtr , dectype prio = RENDERING_2D_PRIO_FAST , const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &diffuseColor = CColor_White , const math::CBasicVector2 &offset = math::CBasicVector2( 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CTextureMappingShaderObject::Create() );
    static CSpritePtr CreateSquare( const CTexturePtr &texPtr , const math::CBasicVector2 &size , dectype prio = RENDERING_2D_PRIO_FAST , const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &diffuseColor = CColor_White , const math::CBasicVector2 &offset = math::CBasicVector2( 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CTextureMappingShaderObject::Create() );

	/// <summary>
	/// WavefrontOBJファイルから作成
	/// </summary>
    static CSpritePtr CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , dectype prio = RENDERING_2D_PRIO_FAST , CColor color = CColor_White , math::CBasicVector2 pos = math::CBasicVector2( 0.0f , 0.0f )  , dectype rot = 0.0f , math::CBasicVector2 sc = math::CBasicVector2( 1.0f , 1.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CTextureMappingShaderObject::Create() );

	/// <summary>
	/// 円を作成する（等倍で半径が１）
	/// </summary>
    static CSpritePtr CreateTestCircle( CColor color = CColor_White , math::CBasicVector2 pos = math::CBasicVector2( 0.0f , 0.0f )  , dectype rot = 0.0f , math::CBasicVector2 sc = math::CBasicVector2( 1.0f , 1.0f ) , dectype prio = RENDERING_2D_PRIO_FAST , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = graphic::CMaterialDiffuseColorShaderObject::Create() );

	/// <summary>
	/// ２軸の基準線
	/// </summary>
	static CSpritePtr CreateStandardLine2(dectype length = 500 , dectype prio = RENDERING_2D_PRIO_FAST );

	/// <summary>
	/// 任意の点集合でスプライトを作成する
	/// </summary>
	static CSpritePtr Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , dectype prio = RENDERING_2D_PRIO_FAST , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CTextureMappingShaderObject::Create() );

//----Geometory線描画

	/// <summary>
	/// AABB2
	/// </summary>

	/// <summary>
	/// OBB2
	/// </summary>

	/// <summary>
	/// Circle
	/// </summary>

	/// <summary>
	/// Ellipse
	/// </summary>

	/// <summary>
	/// Line2
	/// </summary>

	/// <summary>
	/// Ray2
	/// </summary>

	/// <summary>
	/// Rect
	/// </summary>

	/// <summary>
	/// Triangle2
	/// </summary>

	/// <summary>
	/// Vector2
	/// </summary>
};

} } }