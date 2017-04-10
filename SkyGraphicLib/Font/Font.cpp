#include "StdAfx.h"
#include "SkyGraphicLib/Font.h"
#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CFont       , sky::lib::graphic::CSceneGraphNode );
SkyImplementRTTI( sky::lib::graphic::CFontSimple , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

static vrtxtype s_FontIndexBufferTbl[] = { 0 , 1 , 2 , 2 , 1 , 3 };
static vrtxtype s_FontIndexBufferTblNum = array_sizeof( s_FontIndexBufferTbl );

CFont::CFont( font::eFontStyleType eStyle , dectype prio  ) : m_printFontNum( 0 )
{ 
	m_SentenceData.Color         = CColor_White;
	m_SentenceData.Sc            = 1.0f;
	m_SentenceData.Sentence[ 0 ] = NULL;
	m_SentenceData.Pos           = math::CBasicVector2( 0.0f , 0.0f );
	m_SentenceData.OutIntvl      = 0;
    m_Prio = CONVERT_PRIO_TO_Z( prio );

	//文字スタイル
	m_Style = eStyle;

	//幅と高さ
    m_SentenceSize.W = 0;
    m_SentenceSize.H = 0;
	m_FontSize.W = 0;
	m_FontSize.H = 0;

	m_IsReWrite = skyTrue;
}

void CFont::SetUp()
{
	if ( m_IsSetUp == skyFalse )
	{
		//シェーダー生成
		AttachBranch( CTextureMappingShaderObject::Create( CHS( _T( "font_default_shader" ) ) ) );

		m_IsSetUp = skyTrue;
	}
}

CTextureMappingShaderObjectPtr CFont::GetShader( skyBool isRequired )
{
	CTextureMappingShaderObjectPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CTextureMappingShaderObject , CTextureMappingShaderObjectPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "TextureMappingShaderObject is not found." ) );
	}
#endif
	return spResult;
}

void CFont::PrintfSentence( dectype posX , dectype posY , const CColor &color , dectype sc , u32 outIntvl , const skyString *format , ... )
{
	SKY_ASSERT_MSG( m_IsSetUp == skyTrue ,  _T( "It has not setup yet." ) );

	m_SentenceData.Pos.X = posX;
	m_SentenceData.Pos.Y = posY;
	m_SentenceData.Color = color;
	m_SentenceData.Sc    = sc;
	m_SentenceData.OutIntvl = outIntvl;

	VA_LIST( m_SentenceData.Sentence , font::ONE_LINE_FONT_NUM_MAX , format );

	//文字数をカウント（負荷が高い）
	m_printFontNum = ( vrtxtype )CStringUtil_::Len( m_SentenceData.Sentence );

	//幅と高さを計算（負荷が高い）
	dectype wX = 0 , wY = 0 , minWidth = 0 , maxWidth = 0 , minHeight = 0 , maxHeight = 0;

	//計算用ワーク
	math::CBasicVector2 vec2( 0 , 0 );

	vrtxtype i = 0;
	dectype maxHeightNum = 0; //出力文字の最大値
	dectype space = 0;
	u32 ecNum = 1; //改行数
	while( m_SentenceData.Sentence[ i ] != EOT )
	{
		//改行コードの場合座標だけ変化させて次へ
		if ( m_SentenceData.Sentence[ i ] == EC )
		{
			//座標は中心座標にして格納
			vec2.Y = vec2.Y + maxHeightNum * -1;
			vec2.X = m_SentenceData.Pos.X;
			ecNum++;
		}
		else if ( m_SentenceData.Sentence[ i ] == _T( ' ' ) )
		{
			vec2.X += space;
		}
		else 
		{
			//文字コードに対応した描画情報を取得する
			COneFontData &data = CFontManager_::GetFont( m_SentenceData.Sentence[ i ] , m_Style );

			//頂点情報の取得
			for ( vrtxtype j = 0 ; j < 4 ; j++ )
			{
				//座標を変える（ ( もともとの矩形座標 * 拡大縮小 ) ＋ ワールド座標 ）
				wX = ( f32 )( vec2.X + ( data.pos[ j ].X * m_SentenceData.Sc ) );
				wY = ( f32 )( vec2.Y + ( data.pos[ j ].Y * m_SentenceData.Sc ) );

				if( minWidth > wX )
				{
					minWidth = wX;
				} 
				else if ( maxWidth < wX )
				{
					maxWidth = wX;
				}
				else if ( minHeight > wY )
				{
					minHeight = wY;
				}
				else if ( maxHeight < wY )
				{
					maxHeight = wY;
				}
			}

			//描画位置をずらす。
			space = ( dectype )( m_SentenceData.Sc * data.size.W ); //スペースに使う
			vec2.X += space;
			dectype height = ( dectype )( m_SentenceData.Sc * data.size.H );
			if ( maxHeightNum < height ) maxHeightNum = height;
		}
		i++;
	}

	//文字列の長さ
    m_SentenceSize.W = maxWidth - minWidth;
    m_SentenceSize.H = maxHeight - minHeight;

	//1文字の縦横幅
	m_FontSize.W = m_SentenceSize.W / i - 1;
	m_FontSize.H = m_SentenceSize.H / ecNum;

	m_IsReWrite = skyTrue;
}

void CFont::vRendering( CRenderingParam &param )
{
	if ( m_SentenceData.Sentence[ 0 ] == NULL ) return;

	if ( m_IsReWrite == skyTrue )
	{//文字の内容が変化したらメッシュをつくり直す。毎フレームつくり直すのは現実的ではない。

		//計算用ワーク
		math::CBasicVector2 vec2;

		//最大文字出力数×４の頂点数で頂点バッファを初期化する。
		//１文字４頂点。
		//１文字６インデックス
		//★非同描画のため毎フレーム確保している。頂点バッファの内容が毎フレーム変化するものは頂点バッファの確保を毎フレーム行う必要がある）
		//非同期
		m_MeshContena = CMeshContenaCreate_();
		m_MeshContena->AttachMesh( CMeshCreate_() );
		m_MeshContena->GetMesh()->AttachMaterialContena( CMaterialContenaCreate_() );
		m_spMaterial = CMaterialCreate_();
		m_spMaterial->AttachDiffuseMap( CFontManager_::GetFontStyleTex( m_Style ) );

		CIndexBufferPtr spIndexBuffer = CIndexBufferCreate_();
		CVertexBufferPtr spVertexBuffer = CVertexUtil::CreateVertexBuffer( m_printFontNum * 4 , m_printFontNum * 6 , vertex::ePrimitiveType_Trianglelist , spIndexBuffer );
		vrtxtype spriteIndexNum = 6;
		m_MeshContena->GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , spriteIndexNum );
		spVertexBuffer->CreateColorStream( spVertexBuffer->GetVertexNum() );
		spVertexBuffer->CreateUvStream( spVertexBuffer->GetVertexNum() );

		vrtxtype vertexCoursor = 0;
		vrtxtype indexCoursor = 0;

		vec2.X = 0;
		vec2.Y = 0;

		//もし左上原点だった場合は、渡された座標が左上原点座標だと仮定してY移動方向を反転する
		CameraPtr spCamera = param.CameraStack.GetTop();
		if ( spCamera->IsLeftTopCenter() )
		{
			vec2.Y *= -1;
		}

		u32 i = 0;
		dectype maxHeight = 0; //出力文字の最大値
		dectype space = 0;

		while( m_SentenceData.Sentence[ i ] != EOT )
		{
			//改行コードの場合座標だけ変化させて次へ
			if ( m_SentenceData.Sentence[ i ] == EC )
			{
				//座標は中心座標にして格納
				vec2.Y = vec2.Y + maxHeight * -1;
				vec2.X = m_SentenceData.Pos.X;
			}
			else if ( m_SentenceData.Sentence[ i ] == _T( ' ' ) )
			{
				vec2.X += space;
			}
			else 
			{
				//文字コードに対応した描画情報を取得する
				COneFontData &data = CFontManager_::GetFont( m_SentenceData.Sentence[ i ] , m_Style );

				//頂点情報の取得
				for ( vrtxtype j = 0 ; j < 4 ; j++ )
				{
					//UVを設定する
					spVertexBuffer->GetUvStream()->GetUv( j + vertexCoursor )->tu = ( dectype )data.uv[ j ].X;
					spVertexBuffer->GetUvStream()->GetUv( j + vertexCoursor )->tv = ( dectype )data.uv[ j ].Y;

					//色を変える
					*spVertexBuffer->GetColorStream()->GetColor( j + vertexCoursor ) = m_SentenceData.Color;

					//座標を変える（ ( もともとの矩形座標 * 拡大縮小 ) ＋ ワールド座標 ）
					spVertexBuffer->GetPositionStream()->GetPosition( j + vertexCoursor )->x = ( f32 )( vec2.X + ( data.pos[ j ].X * m_SentenceData.Sc ) );
					spVertexBuffer->GetPositionStream()->GetPosition( j + vertexCoursor )->y = ( f32 )( vec2.Y + ( data.pos[ j ].Y * m_SentenceData.Sc ) );
					spVertexBuffer->GetPositionStream()->GetPosition( j + vertexCoursor )->z = ( f32 )( 0 ); //前後関係は平行移動で調整する
				}

				//インデックス情報の取得
				for ( vrtxtype j = 0 ; j < s_FontIndexBufferTblNum; j++ )
				{
					spIndexBuffer->m_pCpuMemoryIndexBuffer[ j + indexCoursor ] = s_FontIndexBufferTbl[ j ] + vertexCoursor;
				}

				//カーソルを動かす
				vertexCoursor += 4;
				indexCoursor += s_FontIndexBufferTblNum;

				//描画位置をずらす。
				space = ( dectype )( m_SentenceData.Sc * data.size.W ); //スペースに使う
				vec2.X += space;
				dectype height = ( dectype )( m_SentenceData.Sc * data.size.H );
				if ( maxHeight < height ) maxHeight = height;
			}
			i++;
		}
		//使用頂点数、インデックス数の更新（これはかなり特殊です普通こんなことはやりません）
		spVertexBuffer->UpdateUseVertexNum( vertexCoursor );
		spIndexBuffer->UpdateUseIndexNum( indexCoursor );

		//グループのインデックス数を設定する
		m_MeshContena->GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , indexCoursor );

		//メッシュに頂点バッファを設定
		m_MeshContena->GetMesh()->AttachVertexBuffer( spVertexBuffer );
		m_MeshContena->GetMesh()->AttachIndexBuffer( spIndexBuffer );

		//GPUメモリに書き込む
		CRenderCommand::CreateMeshGpuMemory( m_MeshContena );
		CRenderCommand::WirteMeshGpuMemory( m_MeshContena );

		//CPU側のメモリは消す
		CRenderCommand::DeleteMeshCpuMemory( m_MeshContena );

		m_IsReWrite = skyFalse;
	}

	//座標の設定(親の座標系に移動する）
	//親の座標系の２Ｄ移動情報のみを反映する。
	math::CMatrix4x3 transMat;
	transMat.Identity();
	transMat.SetTranslation( math::CBasicVector3( m_SentenceData.Pos.X + param.MatrixStack.GetTop().Tx , -m_SentenceData.Pos.Y + param.MatrixStack.GetTop().Ty , CONVERT_PRIO_TO_Z( m_Prio ) ) );

	//絶対半透明なのであとからいっきにレンダリングする
	CShaderObjectPtr shader = GetShader();
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	for ( u32 i = 0 ; i < m_MeshContena->GetMeshNum() ; i++ )
	{
		param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , 
			CRenderCommand::RenderingMeshParam( 0 , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , shader , m_MeshContena->GetMesh( i ) , transMat , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
	}
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsSetUp , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Color.a , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Sc , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&sentencesize , sizeof( u32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )sentence , sentencesize , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.OutIntvl , sizeof( u32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Pos.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceData.Pos.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Style , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_printFontNum , sizeof( u16 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceSize.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_SentenceSize.H , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_FontSize.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_FontSize.H , sizeof( d64 ) , isWrite ); \

skyBool CFont::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "font bin save. \n" ) );

	string::CString  sentencestr  =  ( m_SentenceData.Sentence == skyNull )? _T( "" ) : m_SentenceData.Sentence;
	u32              sentencesize = sentencestr.GetSize();
	const skyString* sentence     = sentencestr.Get();

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CFont::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "font bin load. \n" ) );

	u32 sentencesize;
	skyString sentence[ PATH_LEN_MAX ] = _T( "none" );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	//名前取得
	CStringUtil_::Copy( m_SentenceData.Sentence , font::ONE_LINE_FONT_NUM_MAX , sentence );

	return result;
}
const serialize::CSerializeInterfacePtr& CFont::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CFontPtr )spContainer )->m_IsSetUp = m_Style;
	( ( CFontPtr )spContainer )->m_SentenceData = m_SentenceData;
	( ( CFontPtr )spContainer )->m_Style = m_Style;
	( ( CFontPtr )spContainer )->m_printFontNum = m_printFontNum;
	( ( CFontPtr )spContainer )->m_SentenceSize = m_SentenceSize;
	( ( CFontPtr )spContainer )->m_FontSize = m_FontSize;
	return spContainer;
}
const serialize::CSerializeInterfacePtr& CFont::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CFont::Clone()
{
	return Copy( Create( hash::CHashStringPtr() , font::eFontStyleType_Gosic13 , RENDERING_2D_PRIO_FAST ) );
}
serialize::CSerializeInterfacePtr CFont::CloneSharing()
{
	return Clone();
}

void CFont::Activate()
{
	ACTIVATE_COMMON();

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();
}

#ifdef SW_SKYLIB_DEBUG_ON

void CFont::NodeTreePrint( u32 indent , eRelationKind eKind )
{
	SKY_UNUSED_ARG( eKind );

	SKY_PRINT_INDENT( indent * 2 );
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
	SKY_PRINTF( _T( "%s pos[ %4.3f , %4.3f ] prio[ %4.3f ] offsetPrio[ %4.3f ] \n" ) , m_SentenceData.Sentence , m_SentenceData.Pos.X , m_SentenceData.Pos.Y , m_Prio );
}

#endif

//----------------------------------------------------------------------------

CFontSimple::CFontSimple( font::eFontStyleType eStyle , u32 lineNum , dectype prio ) : 
m_SentencePool( lineNum ) ,
m_IsStanderdPositionDraw( skyFalse ) ,
m_IsStanderdPosition( 0 , 0 )
{
	//文字スタイル
	m_Style = eStyle;

	//優先順位の設定
	m_Prio = CONVERT_PRIO_TO_Z( prio );
}

void CFontSimple::SetUp()
{
	if ( m_IsSetUp == skyFalse )
	{
		//シェーダー生成
		AttachBranch( CTextureMappingShaderObject::Create( CHS( _T( "font_default_shader" ) ) ) );

		//頂点バッファの作成
		vrtxtype printFontNum = ( vrtxtype )( font::ONE_LINE_NUM_MAX * font::ONE_LINE_FONT_NUM_MAX );
		CIndexBufferPtr  spIndexBuffer = CIndexBufferCreate_();
		CVertexBufferPtr spVertexBuffer = CVertexUtil::CreateVertexBuffer( printFontNum * 4 , printFontNum * 6 , vertex::ePrimitiveType_Trianglelist , spIndexBuffer );
		spVertexBuffer->CreateColorStream( spVertexBuffer->GetVertexNum() );
		spVertexBuffer->CreateUvStream( spVertexBuffer->GetVertexNum() ); 
		AttachBranch( spVertexBuffer );

		//マテリアルの作成
		CMeshPtr spMesh = CMeshCreateName_( CHS( _T( "font_default_material" ) ) );
		spMesh->AttachMaterialContena( CMaterialContenaCreate_() );
		CMaterialPtr  spMaterial = CMaterialCreateName_( CHS( _T( "font_default_material" ) ) );
		spMaterial->AttachDiffuseMap( CFontManager_::GetFontStyleTex( m_Style ) );
		spMesh->GetMaterialContena()->AddMaterial( spMaterial , 0 , 0 );
		spMesh->AttachVertexBuffer( spVertexBuffer );
		spMesh->AttachIndexBuffer( spIndexBuffer );
		AttachBranch( spMesh );

		//GPUメモリを作成する
		spMesh->CreateGpuMemory();

		m_IsSetUp = skyTrue;
	}
}

void CFontSimple::PrintfSentence( dectype posX , dectype posY , const CColor &color , dectype sc , const skyString *format , ... )
{
	SKY_ASSERT_MSG( m_IsSetUp == skyTrue ,  _T( "It has not setup yet." ) );

	CSentenceData_* pItem = m_SentencePool.Create();
	VA_LIST( pItem->Sentence , font::ONE_LINE_FONT_NUM_MAX , format );
	pItem->Color = color;
	pItem->Sc = sc;
	pItem->Pos.X = posX;
	pItem->Pos.Y = posY;

	m_SentenceList.AddTail( pItem );
}

void CFontSimple::vRendering( CRenderingParam &param )
{
	if ( m_SentenceList.GetNum() == 0 ) return;
	if ( m_SentenceList.GetNum() > font::ONE_LINE_NUM_MAX )
	{
		//リストを削除
		m_SentenceList.DeleteAll();
		return; //限界ライン数を超えたら終わり
	}
	if ( IS_COMMAND == skyTrue ) return; //非同期実行には対応してません。

	//計算用ワーク
	math::CBasicVector2 vec2;

	//最大文字出力数×４の頂点数で頂点バッファを初期化する。
	//１文字４頂点。
	//１文字６インデックス
	//★非同描画のため毎フレーム確保している。頂点バッファの内容が毎フレーム変化するものは頂点バッファの確保を毎フレーム行う必要がある）
	//非同期
	CVertexBufferPtr spVertexBuffer = GetVertexBuffer();
	CIndexBufferPtr spIndexBuffer = GetMesh()->GetIndexBuffer();
	CMeshContenaPtr meshContena = CMeshContenaCreate_();
	vrtxtype spriteIndexNum = 6;
	meshContena->AttachMesh( GetMesh() );
	GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , spriteIndexNum );

	vrtxtype vertexCoursor = 0;
	vrtxtype indexCoursor = 0;
	dectype maxHeight = 0; //出力文字の最大値
	math::CBasicVector2 offsetPos( m_IsStanderdPosition.X , m_IsStanderdPosition.Y );
	FOREACH( CSentenceDataPtrList_ , it , m_SentenceList )
	{
		//１文字ずつレンダリングしていく
		CSentenceData_ *snt = *it;

		if( m_IsStanderdPositionDraw == skyTrue )
		{
			offsetPos.Y +=  maxHeight;
		}

		//座標の設定(親の座標系に移動する）
		vec2.X = snt->Pos.X + offsetPos.X;
		vec2.Y = snt->Pos.Y + offsetPos.Y;

		//もし左上原点だった場合は、渡された座標が左上原点座標だと仮定してY移動方向を反転する
		CameraPtr spCamera = param.CameraStack.GetTop();
		if ( spCamera->IsLeftTopCenter() )
		{
			vec2.Y *= -1;
		}
		vertex::CVertexFmt_UV *pToUv = spVertexBuffer->GetUvStream()->GetUv( 0 );
		vertex::CVertexFmt_Color *pToColor = spVertexBuffer->GetColorStream()->GetColor( 0 );
		vertex::CVertexFmt_Pos *pToPos = spVertexBuffer->GetPositionStream()->GetPosition( 0 );

		u32 i = 0;
		maxHeight = 0; //出力文字の最大値
		dectype space = 0;
		while( snt->Sentence[ i ] != EOT )
		{
			//改行コードの場合座標だけ変化させて次へ
			if ( snt->Sentence[ i ] == EC )
			{
				//座標は中心座標にして格納
				vec2.Y = vec2.Y + maxHeight * -1;
				vec2.X = snt->Pos.X;
			}
			else if ( snt->Sentence[ i ] == _T( ' ' ) )
			{
				vec2.X += space;
			}
			else 
			{
				//文字コードに対応した描画情報を取得する
				COneFontData &data = CFontManager_::GetFont( snt->Sentence[ i ] , m_Style );

				//頂点情報の取得
				for ( vrtxtype j = 0 ; j < 4 ; j++ )
				{
					//UVを設定する
					pToUv[ j + vertexCoursor ].tu = ( dectype )data.uv[ j ].X;
					pToUv[ j + vertexCoursor ].tv = ( dectype )data.uv[ j ].Y;

#ifdef SKYLIB_RENDER_OPENGL4 //OpenGLのときは少しUVを補正する
					pToUv[ j + vertexCoursor ].tv += 0.002f;
#endif
					//色を変える
					pToColor[ j + vertexCoursor ] = snt->Color;

					//座標を変える（ ( もともとの矩形座標 * 拡大縮小 ) ＋ ワールド座標 ）
					pToPos[ j + vertexCoursor ].x = ( f32 )( vec2.X + ( data.pos[ j ].X * snt->Sc ) );
					pToPos[ j + vertexCoursor ].y = ( f32 )( vec2.Y + ( data.pos[ j ].Y * snt->Sc ) );
					pToPos[ j + vertexCoursor ].z = ( f32 )( CONVERT_PRIO_TO_Z( m_Prio ) );
				}

				//インデックス情報の取得
				for ( vrtxtype j = 0 ; j < s_FontIndexBufferTblNum; j++ )
				{
					spIndexBuffer->m_pCpuMemoryIndexBuffer[ j + indexCoursor ] = s_FontIndexBufferTbl[ j ] + vertexCoursor;
				}

				//カーソルを動かす
				vertexCoursor += 4;
				indexCoursor += s_FontIndexBufferTblNum;

				//描画位置をずらす。
				space = ( dectype )( snt->Sc * data.size.W ); //スペースに使う
				vec2.X += space;
				dectype height = ( dectype )( snt->Sc * data.size.H );
				if ( maxHeight < height ) maxHeight = height;
			}
			i++;
		}

		//描画データに集めたら消してしまう。
		m_SentencePool.Delete( *it );
	}

	//使用頂点数、インデックス数の更新（これはかなり特殊です普通こんなことはやりません）
	spVertexBuffer->UpdateUseVertexNum( vertexCoursor );
	spIndexBuffer->UpdateUseIndexNum( indexCoursor );

	//グループのインデックス数を設定する
	GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , indexCoursor );

	//メッシュに頂点バッファを設定
	meshContena->GetMesh()->AttachVertexBuffer( spVertexBuffer );

	//GPUメモリに書き込む
	CRenderCommand::WirteMeshGpuMemory( meshContena );

	//絶対半透明なのであとからいっきにレンダリングする
	CShaderObjectPtr shader = GetShader();
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	//親行列から2D平行移動情報だけ引き継いだ行列を作成する。
	math::CMatrix4x3 mat;
	mat.Identity();
	math::CMatrix4x3 &parentmat = param.MatrixStack.GetTop();
	mat.Tx = parentmat.Tx;
	mat.Ty = parentmat.Ty;
	mat.Tz = CONVERT_PRIO_TO_Z( m_Prio );

	for ( u32 i = 0 ; i < meshContena->GetMeshNum() ; i++ )
	{
		//半透明として描画データキューに格納する
		param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , 
			CRenderCommand::RenderingMeshParam( 0 , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , shader , meshContena->GetMesh( i ) , mat , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
	}

	//リストを削除
	m_SentenceList.DeleteAll();

	//描画終了
	m_RenderingEnd();
}

#define GETSET_BIN_2( isGet ) \
	stream->vWriteReadStream( ( u8 * )&m_IsSetUp , sizeof( s32 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_Style , sizeof( s32 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStanderdPositionDraw , sizeof( s32 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStanderdPosition.X , sizeof( d64 ) , isGet ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStanderdPosition.Y , sizeof( d64 ) , isGet ); \
	
skyBool CFontSimple::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "fontsimple bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN_2( skyTrue );

	return result;
}

skyBool CFontSimple::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "fontsimple bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN_2( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CFontSimple::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CFontSimplePtr )spContainer )->m_IsSetUp = m_IsSetUp;
	( ( CFontSimplePtr )spContainer )->m_Style = m_Style;
	( ( CFontSimplePtr )spContainer )->m_IsStanderdPositionDraw = m_IsStanderdPositionDraw;
	( ( CFontSimplePtr )spContainer )->m_IsStanderdPosition = m_IsStanderdPosition;
	return spContainer;
}

serialize::CSerializeInterfacePtr CFontSimple::Clone()
{
	return Copy( Create( hash::CHashStringPtr() ) );
}

void CFontSimple::Activate()
{
	ACTIVATE_COMMON();

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();
}

CTextureMappingShaderObjectPtr CFontSimple::GetShader( skyBool isRequired )
{
	CTextureMappingShaderObjectPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CTextureMappingShaderObject , CTextureMappingShaderObjectPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "TextureMappingShaderObject is not found." ) );
	}
#endif
	return spResult;
}

CMeshPtr CFontSimple::GetMesh( skyBool isRequired )
{
	CMeshPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CMesh , CMeshPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Mesh is not found." ) );
	}
#endif
	return spResult;
}

CVertexBufferPtr CFontSimple::GetVertexBuffer( skyBool isRequired )
{
	CVertexBufferPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CVertexBuffer , CVertexBufferPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Material is not found." ) );
	}
#endif
	return spResult;
}

} } }