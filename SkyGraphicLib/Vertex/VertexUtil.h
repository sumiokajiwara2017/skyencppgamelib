#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// 頂点情報ユーティリティ
/// </summary>
class CVertexUtil : public base::SkyObject
{

public:

	/// <summary>
	/// 点の作成
	/// </summary>
	static void CreatePoint( const math::CBasicVector3 &point , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// ２Ｄ線分群の作成
	/// </summary>
	static void CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector2Vector &vector , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// ２Ｄ線の作成
	/// </summary>
	static void CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &start = math::CBasicVector2( 0.0f , 0.0f ) , const math::CBasicVector2 &end = math::CBasicVector2( 0.0f , 5.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// ２Ｄ線のの矩形の作成
	/// </summary>
	static void CreateSquareLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , dectype width = 1.0f , dectype height = 1.0f , const CColor &color = CColor_Red );

	/// <summary>
	/// ３Ｄ線分群の作成
	/// </summary>
	static void CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CMeshPtr &spMesh , const CColor &color = CColor_Red );
	static void CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicLine3List &lineList , const CMeshPtr &spMesh , const CColor &color = CColor_Red );
	static void CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &start = math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , const math::CBasicVector3 &end = math::CBasicVector3( 0.0f , 5.0f , 0.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// ３Ｄエンティティを設定して頂点座標変換する(この処理はCPUメモリ側の頂点を変更します。もしGPU側への反映が必要な場合はspVertexBuffer->vWirteGpuMemory();という処理をして反映してください。）
	/// </summary>
	static void ComputeEntity3D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity3DPtr &entity3d );

	/// <summary>
	/// ２Ｄエンティティを設定して頂点座標変換する(この処理はCPUメモリ側の頂点を変更します。もしGPU側への反映が必要な場合はspVertexBuffer->vWirteGpuMemory();という処理をして反映してください。）
	/// </summary>
	static void ComputeEntity2D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity2DPtr &entity2d );


//---------------便利な描画を見つけたら追加していこう（でも基本はモデリングツールのデータを元にする）-----------------▽

	/// <summary>
	/// 半球の作成
	/// </summary>
	static void CreateSphere( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , dectype sc = 1.0f , vrtxtype divNum = 12, const CColor &color = CColor_Red );

	/// <summary>
	/// 球の作成（ＵＶ値が設定されていて、地球のテクスチャを貼ると地球になる）
	/// radius : 半径
	/// xnum   : Ｘ軸方向の細度（この数値が高いとより綺麗な球体になるが、頂点数は増える）
	/// ynum   : Ｙ軸方向の細度（この数値が高いとより綺麗な球体になるが、頂点数は増える）
	/// </summary>
	static void CreateSphereTexture( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype radius = 1.0f , vrtxtype xnum = 8 , vrtxtype ynum = 8 , const CColor &color = CColor_Red );

	/// <summary>
	/// 床の作成（４頂点固定。ＵＶが設定されていてテクスチャとテクスチャリピート設定で地面を出せる）
	/// size     : サイズ
	/// uvrelnum : ＵＶ繰り返し回数
	/// </summary>
	static void CreateFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype size = 10.0f , dectype uvrelnum = 10.0f , const CColor &color = CColor_Red );

	/// <summary>
	/// 円柱の作成（法線が内側に伸びている。ＵＶ値が設定されていて、空のテクスチャを貼ると空になる。）
	/// ypolnum      : 縦方向のポリゴン数
	/// circlepolnum : 円一周で何個のポリゴンを使うか
	/// </summary>
	static void CreateCylinderRevers( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMaterial , dectype radius = 1.0f , dectype height = 1.0f , vrtxtype ypolnum = 1 , vrtxtype circlepolnum = 32 , const CColor &color = CColor_Red );

	/// <summary>
	/// 立方体の頂点情報の作成(頂点バッファとインデックスバッファは生成しておいてください）
	/// </summary>
	static void	CreateCube( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &scale = math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// 矩形の作成
	/// </summary>
	static void CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &color = CColor_Red );
	static void CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype width , dectype height , const math::CBasicVector2 &uvLt , const math::CBasicVector2 &uvRt , const math::CBasicVector2 &uvLb , const math::CBasicVector2 &uvRb );

	/// <summary>
	/// XZ平面にグリッド線を書く
	/// </summary>
	static void CreateGrid_XZ( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// YX平面にグリッド線を書く
	/// </summary>
	static void CreateGrid_YX( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color = CColor_Red );

	/// <summary>
	/// 格子状の地面を作成
	/// </summary>
	static void CreateGridFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale = math::CBasicVector2( 1.0f , 1.0f ) , const CColor &color = CColor_Red );

	/// <summary>
	/// 頂点数指定でのインデックス付き頂点バッファの作成（座標、インデックスは全部０です。書きこんで使用してください）
	/// </summary>
	static CVertexBufferPtr CreateVertexBuffer(  vrtxtype vertexNum = 4 , vrtxtype indexNum = 6 , vertex::ePrimitiveType ePrimType = vertex::ePrimitiveType_Trianglelist , const CIndexBufferPtr &spIndexBuffer = CIndexBufferPtr() ); 

//---------------便利な描画を見つけたら追加していこう（でも基本はモデリングツールのデータを元にする）-----------------△

	/// <summary>
	/// 頂点ストリームリストをレンダラーに設定する
	/// </summary>
	static void SetRenderBufferStream( const CVertexBufferStreamPtrList &list , const CIndexBufferPtr &spIndexBuffer , vertex::ePrimitiveType ePrimType = vertex::ePrimitiveType_None );

//---------------線で様々な形を描く-----------------▽

	/// <summary>
	/// ８頂点を指定して立方体の線図を書く。実際は頂点の位置によって形は決定するので、立方体になるかどうかは頂点の座標によってきまる。
	/// </summary>
	static void CreateCubeLine( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMaterial , const CColor &color = CColor_Red );

	/// <summary>
	/// ５頂点を指定してピラミッドの線図を書く。実際は頂点の位置によって形は決定するので、立方体になるかどうかは頂点の座標によってきまる。
	/// </summary>
	static void CreatePyramid( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMaterial , const CColor &color = CColor_Red );

//---------------座標もインデックスも外部指定-----------------▽

	/// <summary>
	/// 作成
	/// </summary>
	static void Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh );
};

} } }

