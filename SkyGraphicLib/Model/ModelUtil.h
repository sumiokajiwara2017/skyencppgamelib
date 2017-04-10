#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// モデル便利関数群
/// </summary>
class CModelUtil
{

public:

	/// <summary>
	/// モデルを生成します。Meshは他で生成済みとします。シェーダーはPointLightShaderを設定。
	/// </summary>
	static CModelPtr CreateModelFromMesh( const CMeshContenaPtr &spMesh , CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// WavefrontOBJファイルからモデルを生成します。シェーダーはPointLightShaderを設定。
	/// </summary>
    static CModelPtr CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test用立方体
	/// </summary>
    static CModelPtr CreateTestCube( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test用三角すい
	/// </summary>
    static CModelPtr CreateTestCone( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test床（スケール無しの大きさで10x10のXZ平面）
	/// </summary>
    static CModelPtr CreateTestPlane( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// Test球
	/// </summary>
    static CModelPtr CreateTestSphere( CColor color = CColor_White , math::CBasicVector3 sc = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicAngleVector3 rot = math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3 pos = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CDirectionalLightShaderObjectCreate_() );

	/// <summary>
	/// ３Ｄ点を作成
	/// </summary>
    static CModelPtr CreatePoint( const math::CBasicVector3 &point , CColor color = CColor_Gold , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// ３Ｄの線の作成
	/// </summary>
	static CModelPtr CreateLine3D( const math::CBasicVector3Vector &vector , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );
	static CModelPtr CreateLine3D( const math::CBasicLine3List &lineList , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// 任意の点集合でモデルを作成する
	/// </summary>
	static CModelPtr Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBufferCreate_() , const CShaderObjectPtr &effectPtr = CTextureMappingShaderObject::Create() );

	/// <summary>
	/// ３軸の基準線
	/// </summary>
	static CModelPtr CreateStandardLine3( dectype length = 20 );

	/// <summary>
	/// ＸＺ平面格子
	/// </summary>
	static CModelPtr CreateGrid_XZ( dectype large = 1 , const CColor &color = CColor_Silver , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// ＸＺ平面格子
	/// </summary>
	static CModelPtr CreateGrid_YX( dectype large = 1 , const CColor &color = CColor_Silver , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// ＸＺ平面円を表示する
	/// </summary>
	static CModelPtr CreateCircle_XZ( const math::CBasicCircle& circle , const CColor &color = CColor_Silver , u32 granularity = 32 );

	/// <summary>
	/// ＸＹ平面矩形を作成する
	/// </summary>
	static CModelPtr CreateSquare( const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );

	/// <summary>
	/// ３Ｄの線立方体の作成
	/// </summary>
	static CModelPtr CreateLine3DCube( const math::CBasicVector3Vector &vector , const CColor &color = CColor_Red , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );
	static CModelPtr CreateLine3DCube( const math::CMatrix4x3 &trans = math::CMatrix4x3_Identity , const CColor &color = CColor_Red );

	/// <summary>
	/// 視野台立方体のモデルの作成
	/// </summary>
	static CModelPtr CreateFrustum( const math::CBasicVector3 &pos , const CameraPtr &spCamera , const CColor &color = CColor_Red );

	/// <summary>
	/// ピラミッド型の線のモデルを作成
	/// </summary>
	static CModelPtr CreateLinePyramid(  const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex = CVertexBuffer::Create() , const CIndexBufferPtr &spIndex = CIndexBuffer::Create() , const CShaderObjectPtr &effectPtr = CVertexDiffuseColorShaderObject::Create() );
	static CModelPtr CreateLinePyramid( const math::CMatrix4x3 &trans = math::CMatrix4x3_Identity , const CColor &color = CColor_Red );

	/// <summary>
	/// ３Ｄの線球体の作成（原点が中心で指定の半径の球を擬似的に描画する
	/// </summary>
	static CModelPtr CreateLine3DSphere( const math::CBasicSphere &sphere , const CColor &color = CColor_Red , u32 granularity = 32 );

	/// <summary>
	/// ＡＡＢＢを線で表示するモデルを作成
	/// </summary>
	static CModelPtr CreateAABBDisp( const math::CAABB3 &aabb , const CColor &color );

	/// <summary>
	/// ＯＢＢを線で表示するモデルを作成
	/// </summary>
	static CModelPtr CreateOBBDisp( const math::COBB3 &obb , const CColor &color );
};

} } }