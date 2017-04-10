#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/DataBase.h"

SkyImplementRTTI( sky::lib::graphic::CDecal       , sky::lib::graphic::CEffectObject );

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

//最大デカール頂点数
#define		MAX_DECAL_VERT		( 256 )
#define		DECAL_EPSILON		( 0.25f )

/// <summary>
/// デカール頂点データ
/// </summary>
struct CDecalVertexData
{
	vertex::CVertexFmt_Pos    m_Pos;
	vertex::CVertexFmt_Normal m_Normal;
	vertex::CVertexFmt_Color  m_Color;
	vertex::CVertexFmt_UV     m_Tex1;
};

/// <summary>
/// メンバ変数
/// </summary>
struct CDecalMember
{
	/// <summary>
	/// 破棄タイム
	/// </summary>
	updateTime m_DestoryTime;

	/// <summary>
	/// テクスチャ
	/// </summary>
	CTexturePtr m_spTexture;

	/// <summary>
	/// モデル
	/// </summary>
	CModelPtr m_spModel;

	/// <summary>
	/// 平面の方程式
	/// </summary>
	math::CBasicVector4   m_avPlane[ 6 ];

	/// <summary>
	/// 中心位置
	/// </summary>
	math::CBasicVector3   m_vPos;

	/// <summary>
	/// 法線
	/// </summary>
	math::CBasicVector3   m_vNormal;

	/// <summary>
	/// 接線
	/// </summary>
	math::CBasicVector3   m_vTangent;

	/// <summary>
	/// 従法線
	/// </summary>
	math::CBasicVector3   m_vBinormal;

	/// <summary>
	/// 頂点数
	/// </summary>
	vrtxtype			  m_dVertNum;

	/// <summary>
	/// インデックス数
	/// </summary>
	vrtxtype			  m_dIndexNum;

	/// <summary>
	/// 頂点データ
	/// </summary>
	CVertexBufferPtr      m_spVertexBuffer;
	CDecalVertexData      m_asVert[ MAX_DECAL_VERT ];

	/// <summary>
	/// インデックスデータ
	/// </summary>
	u16				      m_awIndex[ MAX_DECAL_VERT * 3 ];

	/// <summary>
	/// 初期化
	/// </summary>
	void CleanUp();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDecalMember();

	/// <summary>
	/// ポリゴンをシザリングする
	/// </summary>
	vrtxtype ScissorPolygon( math::CBasicVector4 &plane , vrtxtype num , CDecalVertexData *vert );

	/// <summary>
	/// モデルからデカール部分を切り出す
	/// </summary>
	skyBool ScissorDecal( f32 w , f32 h , s32 vnum , s32 inum , CDecalVertexData *vert , u16 *index );

	/// <summary>
	/// デカールからモデルにコピー
	/// </summary>
	void CopyDecalVertexDataToModel();
};

/// <summary>
/// コンストラクタ
/// </summary>
CDecalMember::CDecalMember()
{
	math::CBasicVector3Vector posArray;
	math::CBasicVector2Vector uvArray;
	graphic::CVector_CColor   colorArray;
	CVector_u16               indexArray;

	posArray.Reset( MAX_DECAL_VERT );
	uvArray.Reset( MAX_DECAL_VERT );
	colorArray.Reset( MAX_DECAL_VERT );
	indexArray.Reset( MAX_DECAL_VERT * 3 );

	m_spModel = CModelUtil::Create( posArray , indexArray , uvArray , colorArray , m_spTexture );

	m_spModel->SetAlpha( skyTrue );
}

vrtxtype CDecalMember::ScissorPolygon( math::CBasicVector4 &plane , vrtxtype num , CDecalVertexData *vert )
{
	CDecalVertexData	newvert[ 9 ];
	s32				    csw[ 9 ];
	vrtxtype		    cnum, vnum;
	s32				    i, j;

	// 頂点のクリッピングチェックを行う
	for( i = cnum = 0 ; i < num ; i++ )
	{
		f32	dot = plane.GetVector3().Dot( math::CBasicVector3(vert[i].m_Pos.x ,vert[i].m_Pos.y,vert[i].m_Pos.z) ) + plane.W;

		// 内積が負ならクリップされる
		csw[ i ] = ( dot < 0.0f );
		cnum += ( ( dot < 0.0f ) ? 1 : 0 );
	}

	// 全ての頂点がクリップされている、もしくは全ての頂点がクリップされていない
	if( 0 == cnum )   { return num; }
	if( num == cnum ) { return 0;   }

	// シザリングを行う
	for( i = vnum = 0 ; i < num ; i++ )
	{
		j = i + 1;
		if( j >= num ){ j = 0; }
		// この頂点はクリップされていない
		if ( !csw[ i ] ) {

			// この頂点を追加する
			newvert[ vnum ].m_Pos.x = vert[ i ].m_Pos.x;
			newvert[ vnum ].m_Pos.y = vert[ i ].m_Pos.y;
			newvert[ vnum ].m_Pos.z = vert[ i ].m_Pos.z;

			newvert[ vnum ].m_Normal.nx = vert[ i ].m_Normal.nx;
			newvert[ vnum ].m_Normal.ny = vert[ i ].m_Normal.ny;
			newvert[ vnum ].m_Normal.nz = vert[ i ].m_Normal.nz;

			vnum++;

			// 次の頂点がクリップされているなら
			if( csw[ j ] ){
				f32	t = ( plane.GetVector3().Dot( math::CBasicVector3( vert[i].m_Pos.x,vert[i].m_Pos.y,vert[i].m_Pos.z ) ) + plane.W )
					  / ( plane.GetVector3().Dot( math::CBasicVector3(vert[i].m_Pos.x,vert[i].m_Pos.y,vert[i].m_Pos.z) - math::CBasicVector3(vert[j].m_Pos.x,vert[j].m_Pos.y,vert[j].m_Pos.z ) ) );

				newvert[ vnum ].m_Pos.x = vert[ i ].m_Pos.x * (1.0f - t) + vert[ j ].m_Pos.x * t;
				newvert[ vnum ].m_Pos.y = vert[ i ].m_Pos.y * (1.0f - t) + vert[ j ].m_Pos.y * t;
				newvert[ vnum ].m_Pos.z = vert[ i ].m_Pos.z * (1.0f - t) + vert[ j ].m_Pos.z * t;

				newvert[ vnum ].m_Normal.nx = vert[ i ].m_Normal.nx * ( 1.0f - t ) + vert[ j ].m_Normal.nx * t;
				newvert[ vnum ].m_Normal.ny = vert[ i ].m_Normal.ny * ( 1.0f - t ) + vert[ j ].m_Normal.ny * t;
				newvert[ vnum ].m_Normal.nz = vert[ i ].m_Normal.nz * ( 1.0f - t ) + vert[ j ].m_Normal.nz * t;

				math::CBasicVector3 nml( newvert[ vnum ].m_Normal.nx , newvert[ vnum ].m_Normal.ny , newvert[ vnum ].m_Normal.nz );
				nml.Normalize();
				newvert[vnum].m_Normal.nx = nml.X;
				newvert[vnum].m_Normal.ny = nml.Y;
				newvert[vnum].m_Normal.nz = nml.Z;

				vnum++;
			}
		}
		// この頂点はクリップされていて、次の頂点はクリップされていない
		else if( !csw[ j ] )
		{
			f32	t = ( plane.GetVector3().Dot( math::CBasicVector3( vert[ i ].m_Pos.x , vert[ i ].m_Pos.y , vert[ i ].m_Pos.z ) ) + plane.W )
				  / ( plane.GetVector3().Dot( math::CBasicVector3( vert[ i ].m_Pos.x,vert[ i ].m_Pos.y , vert[ i ].m_Pos.z ) - math::CBasicVector3( vert[ j ].m_Pos.x , vert[ j ].m_Pos.y , vert[ j ].m_Pos.z ) ) );

			newvert[ vnum ].m_Pos.x = vert[ i ].m_Pos.x * ( 1.0f - t ) + vert[ j ].m_Pos.x * t;
			newvert[ vnum ].m_Pos.y = vert[ i ].m_Pos.y * ( 1.0f - t ) + vert[ j ].m_Pos.y * t;
			newvert[ vnum ].m_Pos.z = vert[ i ].m_Pos.z * ( 1.0f - t ) + vert[ j ].m_Pos.z * t;

			newvert[ vnum ].m_Normal.nx = vert[ i ].m_Normal.nx * ( 1.0f - t ) + vert[ j ].m_Normal.nx * t;
			newvert[ vnum ].m_Normal.ny = vert[ i ].m_Normal.ny * ( 1.0f - t ) + vert[ j ].m_Normal.ny * t;
			newvert[ vnum ].m_Normal.nz = vert[ i ].m_Normal.nz * ( 1.0f - t ) + vert[ j ].m_Normal.nz * t;

			math::CBasicVector3 nml( newvert[ vnum ].m_Normal.nx , newvert[ vnum ].m_Normal.ny , newvert[ vnum ].m_Normal.nz );
			nml.Normalize();
			newvert[ vnum ].m_Normal.nx = nml.X;
			newvert[ vnum ].m_Normal.ny = nml.Y;
			newvert[ vnum ].m_Normal.nz = nml.Z;

			vnum++;
		}
	}

	// データをコピーする
	memcpy(vert , newvert , sizeof( CDecalVertexData ) * vnum );

	return vnum;
}

skyBool CDecalMember::ScissorDecal( f32 w , f32 h , s32 vnum , s32 inum , CDecalVertexData *vert , u16 *index )
{
	SKY_UNUSED_ARG( vnum );

	math::CBasicVector3	v0, v1, v2;
	CDecalVertexData	newvert[ 9 ];
	vrtxtype		    i, j, num;
	f32	                iw, ih;

	// ポリゴン数分だけループする
	iw = 1.0f / w;
	ih = 1.0f / h;
	for( i = 0; i < inum ; i += 3 )
	{
		s32		in0 = index[ i ];
		s32		in1 = index[ i + 1 ];
		s32		in2 = index[ i + 2 ];

		// ポリゴンの面法線をチェックする
		v0 = ( math::CBasicVector3( vert[ in1 ].m_Pos.x , vert[ in1 ].m_Pos.y , vert[ in1 ].m_Pos.z ) - math::CBasicVector3( vert[ in0 ].m_Pos.x , vert[ in0 ].m_Pos.y , vert[ in0 ].m_Pos.z ) ).Cross(
						  ( math::CBasicVector3( vert[ in2 ].m_Pos.x , vert[ in2 ].m_Pos.y , vert[ in2 ].m_Pos.z ) - math::CBasicVector3( vert[ in0 ].m_Pos.x , vert[ in0 ].m_Pos.y , vert[ in0 ].m_Pos.z ) ) );
		if( v0.Dot( m_vNormal ) < 0.0f){ continue; }

		// このポリゴンをクリップ面でシザリングする
		num = 3;
		newvert[ 0 ].m_Pos.x= vert[ in0 ].m_Pos.x;
		newvert[ 0 ].m_Pos.y= vert[ in0 ].m_Pos.y;
		newvert[ 0 ].m_Pos.z= vert[ in0 ].m_Pos.z;
		newvert[ 0 ].m_Normal.nx = vert[ in0 ].m_Normal.nx;
		newvert[ 0 ].m_Normal.ny = vert[ in0 ].m_Normal.ny;
		newvert[ 0 ].m_Normal.nz = vert[ in0 ].m_Normal.nz;

		newvert[ 1 ].m_Pos.x= vert[ in1 ].m_Pos.x;
		newvert[ 1 ].m_Pos.y= vert[ in1 ].m_Pos.y;
		newvert[ 1 ].m_Pos.z= vert[ in1 ].m_Pos.z;
		newvert[ 1 ].m_Normal.nx = vert[ in1 ].m_Normal.nx;
		newvert[ 1 ].m_Normal.ny = vert[ in1 ].m_Normal.ny;
		newvert[ 1 ].m_Normal.nz = vert[ in1 ].m_Normal.nz;

		newvert[ 2 ].m_Pos.x= vert[ in2 ].m_Pos.x;
		newvert[ 2 ].m_Pos.y= vert[ in2 ].m_Pos.y;
		newvert[ 2 ].m_Pos.z= vert[ in2 ].m_Pos.z;
		newvert[ 2 ].m_Normal.nx = vert[ in2 ].m_Normal.nx;
		newvert[ 2 ].m_Normal.ny = vert[ in2 ].m_Normal.ny;
		newvert[ 2 ].m_Normal.nz = vert[ in2 ].m_Normal.nz;

		for( j = 0 ; j < 6 ; j++ )
		{
			num = ScissorPolygon( m_avPlane[ j ] , num , newvert );
			if( 0 == num ){ break; }
		}
		if( 0 == num ){ continue; }

		// UV座標とα値を求める
		for( j = 0 ; j < num ; j++ )
		{
			f32	alpha = ( math::CBasicVector3( newvert[ j ].m_Normal.nx, newvert[ j ].m_Normal.ny,newvert[ j ].m_Normal.nz ).Dot( m_vNormal ) - DECAL_EPSILON )
										/ ( 1 - DECAL_EPSILON ) ;

			newvert[ j ].m_Color = CColor(1.0f, 1.0f, 1.0f, alpha);
			newvert[ j ].m_Tex1.tu = m_vTangent.Dot( math::CBasicVector3( newvert[ j ].m_Pos.x , newvert[ j ].m_Pos.y , newvert[ j ].m_Pos.z )- m_vPos ) * iw + 0.5f;
			newvert[ j ].m_Tex1.tv = m_vBinormal.Dot( math::CBasicVector3(newvert[ j ].m_Pos.x , newvert[ j ].m_Pos.y , newvert[ j ].m_Pos.z ) - m_vPos ) * ih + 0.5f;

			// 法線方向に座標を少しだけ移動する（Zバッファ誤差対策）
			newvert[ j ].m_Pos.x += ( m_vNormal.X * 0.01f );
			newvert[ j ].m_Pos.y += ( m_vNormal.Y * 0.01f );
			newvert[ j ].m_Pos.z += ( m_vNormal.Z * 0.01f );
		}

		// この頂点を加え、インデックスを三角形リスト形式に展開する
		if( m_dVertNum + num >= MAX_DECAL_VERT ){ continue; }
		if( m_dIndexNum + ( num - 2 ) * 3 >= MAX_DECAL_VERT * 3 ){ continue; }

		memcpy( &m_asVert[ m_dVertNum ], newvert, sizeof( CDecalVertexData ) * num );

		for( j = 2 ; j < num ; j++ )
		{
			m_awIndex[ m_dIndexNum++ ] = ( WORD )m_dVertNum;
			m_awIndex[ m_dIndexNum++ ] = ( WORD )( m_dVertNum + j - 1 );
			m_awIndex[ m_dIndexNum++ ] = ( WORD )( m_dVertNum + j );
		}

		m_dVertNum += num;
	}

	//m_asVertとm_awIndexを使用してm_spModelを更新する
	CopyDecalVertexDataToModel();

	return ( m_dVertNum != 0 );
}

void CDecalMember::CopyDecalVertexDataToModel()
{
	m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->UpdateUseVertexNum( m_dVertNum );
	m_spModel->GetMeshContena()->GetMesh()->GetIndexBuffer()->UpdateUseIndexNum( m_dIndexNum );

	for ( vrtxtype i = 0 ; i < m_dVertNum ; i )
	{
		m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->GetPositionStream()->WritePosition( i , m_asVert[ i ].m_Pos );
		m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->GetNormalStream()->WriteNormal( i , m_asVert[ i ].m_Normal );
		m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->GetNormalStream()->WriteColor( i , m_asVert[ i ].m_Color );
		m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->GetNormalStream()->WriteUv( i , m_asVert[ i ].m_Tex1 );
	}
}

void CDecalMember::CleanUp()
{
	memset( m_avPlane , 0 , sizeof( m_avPlane ) );
	m_vPos = m_vNormal = math::CBasicVector3( 0.0f , 0.0f , 0.0f );
	m_dVertNum = m_dIndexNum = 0;
	memset( m_asVert , 0 , sizeof( m_asVert ) );
	memset( m_awIndex , 0 , sizeof( m_awIndex ) );
}

CDecal::~CDecal()
{
	DEL m_pMember;
}

CDecal::CDecal()
{
	m_pMember = NEW CDecalMember();
}

void CDecal::AttachTexture( const CTexturePtr &spTexture )
{
	m_pMember->m_spTexture = spTexture;
}

void CDecal::SetDestoryTime( updateTime destoryTime )
{
	m_pMember->m_DestoryTime = destoryTime;
}

skyBool CDecal::IsDestroy()
{
	return m_pMember->m_DestoryTime <= 0;
}

void CDecal::Update( const fps::CFpsController &fps )
{
	m_pMember->m_DestoryTime -= fps.GetPassageTime_Second();
}

static void CreateDecalVertexDataFromMesh( CDecalVertexData **ppVertex , u16 **ppIndex , const CMeshPtr &spMesh , vrtxtype *pVertexNum , u16 *pIndexNum )
{
	*pVertexNum = spMesh->GetVertexBuffer()->GetVertexNum();
	*pIndexNum = spMesh->GetIndexBuffer()->GetIndexNum();

	*ppVertex = NEW CDecalVertexData[ *pVertexNum ];
	*ppIndex  = NEW u16[ *pIndexNum ];

	for ( u16 i = 0 ; i < *pVertexNum ; i++ )
	{
		( *ppVertex )[ i ].m_Pos    = *spMesh->GetVertexBuffer()->GetPositionStream()->GetPosition( i );
		( *ppVertex )[ i ].m_Normal = *spMesh->GetVertexBuffer()->GetNormalStream()->GetNormal( i );
		( *ppVertex )[ i ].m_Color  = *spMesh->GetVertexBuffer()->GetColorStream()->GetColor( i );
		( *ppVertex )[ i ].m_Tex1   = *spMesh->GetVertexBuffer()->GetUvStream()->GetUv( i );
	}

	for ( u16 i = 0 ; i < *pIndexNum ; i++ )
	{
		( *ppIndex )[ i ] = spMesh->GetIndexBuffer()->Index( i );
	}
}

void CDecal::Create( const math::CBasicVector3 &pos , const math::CBasicVector3 &norm , const math::CBasicVector3 &tang , f32 w , f32 h , f32 d , const CMeshPtr &spMesh )
{
	f32			dot;

	// 初期化する
	m_pMember->CleanUp();

	// 位置と法線を格納する
	m_pMember->m_vPos = pos;
	m_pMember->m_vNormal = norm;

	// 従法線を求める
	m_pMember->m_vBinormal = Cross( norm , tang );
	m_pMember->m_vBinormal.Normalize();

	m_pMember->m_vTangent = Cross( m_pMember->m_vBinormal , norm );
	m_pMember->m_vTangent.Normalize();

	// クリップ面を求める
	// +X
	dot = Dot( pos , m_pMember->m_vTangent );
	m_pMember->m_avPlane[ 0 ] = math::CBasicVector4( m_pMember->m_vTangent.X , m_pMember->m_vTangent.Y , m_pMember->m_vTangent.Z , w * 0.5f - dot );

	// -X
	m_pMember->m_avPlane[ 1 ] = math::CBasicVector4( -m_pMember->m_vTangent.X , -m_pMember->m_vTangent.Y , -m_pMember->m_vTangent.Z , w * 0.5f + dot );

	// +Y
	dot = Dot( pos , m_pMember->m_vBinormal );
	m_pMember->m_avPlane[ 2 ] = math::CBasicVector4( m_pMember->m_vBinormal.X , m_pMember->m_vBinormal.Y , m_pMember->m_vBinormal.Z , h * 0.5f - dot );

	// -Y
	m_pMember->m_avPlane[ 3 ] = math::CBasicVector4( -m_pMember->m_vBinormal.X , -m_pMember->m_vBinormal.Y , -m_pMember->m_vBinormal.Z , h * 0.5f + dot );

	// +Z
	dot = Dot( pos , norm );
	m_pMember->m_avPlane[ 4 ] = math::CBasicVector4( norm.X , norm.Y , norm.Z , d - dot );

	// -Z
	m_pMember->m_avPlane[ 5 ] = math::CBasicVector4( -norm.X , -norm.Y , -norm.Z , d + dot );

	CDecalVertexData* pVertex = NULL;
	u16*              pIndex  = NULL;
	vrtxtype          vertexNum = 0;
	u16               indexNum = 0;
	CreateDecalVertexDataFromMesh( &pVertex , &pIndex , spMesh , &vertexNum , &indexNum );

	// モデルからデカール部分を切り取る
	m_pMember->ScissorDecal( w , h , vertexNum , indexNum , pVertex , pIndex );

	DEL pVertex;
	DEL pIndex;
}

void CDecal::vDraw()
{
	//シェーダーIDの構築（BlinnPhongShaderObjectではライトの状態に応じてシェーダーを作る）
	u32 shaderId = 0;

	//シェーダーIDが変化していたらシェーダーを取得
	if ( m_spShaderInfo.IsEmpty() || m_ShaderId != shaderId )
	{
		//Hashが無ければつくる
		if ( s_pShaderHash == skyNull )
		{
			s_pShaderHash = NEW__( CShaderInfoPtrHash , CShaderInfoPtrHash( SHADER_HASH_SIZE ) );
		}

		m_spShaderInfo = GetShaderInfo( shaderId );
		m_ShaderId = shaderId; //現在のシェーダーを変更
	}

	if ( m_RenderingPassNo == 0 )
	{
		//描画
		DrawPass1();
	}
}

} } }

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//頂点シェーダー入力値構造体
struct BillboardBeamShaderConst
{
	//頂点変換行列
    XMFLOAT4X4 WorldViewProjectionMatrix;
	FLOAT      Alpha;	// アルファ値
	XMFLOAT3   dummy;	// アライメント用ダミー
};

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //シェーダークリエーターが初期化されていません。

	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//(未実装）IDに関連づくシェーダーファイルを探す。ファイルであったらそれをロードしてシェーダーとする

		//(未実装）shaderIdに応じてシェーダーのソースを文字列操作で変化させる。

		//デバイスを取得
        ID3D11Device	*pD3DDevice	= CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst();

		ID3DBlob            *pShader      = NULL;
		ID3DBlob            *pError       = NULL;
		ID3D11InputLayout   *layout       = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader  = NULL;

		// 頂点シェーダ作成
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/Decal.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( (const DWORD*)pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		spShaderInfo->m_Pass1.m_spVertexShader.SetPtr( vertexShader );

		// 頂点レイアウトの作成
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT   , 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "COLOR"      , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "TEXCOORD"   , 0 , DXGI_FORMAT_R32G32_FLOAT,       2 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
		};
		hr =CDevice_->GetMember()->m_spD3DDevice->CreateInputLayout(
										ElementDesc ,
										array_sizeof( ElementDesc ) ,
										pShader->GetBufferPointer() ,
										pShader->GetBufferSize() ,
										&layout );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_Pass1.m_spVertexLayout.SetPtr( layout );
		pShader->Release();

        // グローバル変数バッファ作成
		D3D11_BUFFER_DESC ConstDesc;
		ID3D11Buffer  *pConstantBuffer;
		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( BillboardBeamShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/Decal.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "ピクセルシェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreatePixelShader( ( const DWORD* )pShader->GetBufferPointer() , pShader->GetBufferSize() , NULL , &pixelShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass1.m_spPixelShader.SetPtr( pixelShader );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CDecal::DrawPass1()
{
	//定数バッファ受け渡し用の変数
	BillboardBeamShaderConst shaderConst;

	//ワールド行列 * ビュー行列 * プロジェクション行列の設定
	XMMATRIX	mat;
	D3DUtil::Convert4x3Matrix( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

//-----描画する-----▽

	//デバイスを取得
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IAに入力レイアウト・オブジェクトを設定
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass1.m_spVertexLayout.GetPtrNoConst() );

	//VSに頂点シェーダを設定
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass1.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0, 1, m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PSにピクセル・シェーダを設定
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass1.m_spPixelShader.GetPtrNoConst() , NULL , 0 );  // ピクセルシェーダー

	// PSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	CMesh *pMesh = ( CMesh * )m_pMember->m_spModel->GetMeshContena()->GetMesh().GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		//アルファ値
		shaderConst.Alpha = spMatDrawData->GetMaterial()->Alpha;

		//定数パラメーターを受け渡す
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//ディビューズマップを使用する
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			spMatDrawData->GetMaterial()->GetDiffuseMap()->SetShader();
		}

		//ストリームを設定する
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetColorStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

		//頂点の描画
		pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
	}

	//各種シェーダーのクリア
	pD3DDeviceContext->VSSetShader( NULL , NULL , 0 ); // 頂点シェーダー
	pD3DDeviceContext->PSSetShader( NULL , NULL , 0 ); // ピクセルシェーダー
}

} } }

#endif

#ifdef SKYLIB_RENDER_OPENGL4

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

} } }

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

} } }

#endif

#endif
