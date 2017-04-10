//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

inline CVertexBufferStream::~CVertexBufferStream()
{
	Dispose();
}

inline void CVertexBufferStream::Initialize( u32 size )
{
	if ( m_pCpuMemoryBuffer == skyNull )
	{
		m_pCpuMemoryBuffer = NEW_ARRAY__( u8 , size );
		m_Size = size;
		m_UseSize = m_Size; //最初はすべて使用しているとみなす。
	}
}

inline void CVertexBufferStream::InitializePosition( u32 num )
{
	Initialize( num * sizeof( vertex::CVertexFmt_Pos ) );
}

inline void CVertexBufferStream::InitializeNormal( u32 num )
{
	Initialize( num * sizeof( vertex::CVertexFmt_Normal ) );
}

inline void CVertexBufferStream::InitializeUv( u32 num )
{
	Initialize( num * sizeof( vertex::CVertexFmt_UV ) );
}

inline void CVertexBufferStream::InitializeColor( u32 num )
{
	Initialize( num * sizeof( vertex::CVertexFmt_Color ) );
}

inline void CVertexBufferStream::InitializeIndex( u32 num )
{
	Initialize( num * sizeof( vertex::CVertexFmt_Index ) );
}

inline void CVertexBufferStream::InitializeWeights( u32 num )
{
	Initialize( num * sizeof( vertex::CVertexFmt_Weights ) );
}

inline u32 CVertexBufferStream::GetNumPosition()
{
	return m_UseSize / sizeof( vertex::CVertexFmt_Pos );
}
inline u32 CVertexBufferStream::GetNumNormal()
{
	return m_UseSize / sizeof( vertex::CVertexFmt_Normal );
}
inline u32 CVertexBufferStream::GetNumUv()
{
	return m_UseSize / sizeof( vertex::CVertexFmt_UV );
}
inline u32 CVertexBufferStream::GetNumColor()
{
	return m_UseSize / sizeof( vertex::CVertexFmt_Color );
}
inline u32 CVertexBufferStream::GetNumIndex()
{
	return m_UseSize / sizeof( vertex::CVertexFmt_Index );
}
inline u32 CVertexBufferStream::GetNumWeights()
{
	return m_UseSize / sizeof( vertex::CVertexFmt_Weights );
}
inline void CVertexBufferStream::Dispose()
{
	DeleteGpuMemory();
	DeleteCpuMemory();
}

inline void CVertexBufferStream::DeleteCpuMemory()
{
	if ( m_pCpuMemoryBuffer != skyNull )
	{
		SAFE_DELETE_ARR( m_pCpuMemoryBuffer );
		m_Size = 0;
	}
}

inline void CVertexBufferStream::WriteCpuMemory( vrtxtype index , f32 *p , u32 num  )
{
	SKY_ASSERT_MSG( m_pCpuMemoryBuffer != skyNull , _T( "Memory none." ) );
	SKY_ASSERT_MSG( m_Size > 0 , _T( "Memory none." ) );

	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( f32 ) * num ] , p , sizeof( f32 ) * num );
}

inline void CVertexBufferStream::WriteCpuMemory( vrtxtype index , f32 p1  )
{
	f32 p[ 1 ] = { p1 };
	WriteCpuMemory( index , p , 1  );
}

inline void CVertexBufferStream::WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2  )
{
	f32 p[ 2 ] = { p1 , p2 };
	WriteCpuMemory( index , p , 2  );
}

inline void CVertexBufferStream::WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2 , f32 p3  )
{
	f32 p[ 3 ] = { p1 , p2 , p3 };
	WriteCpuMemory( index , p , 3  );
}

inline void CVertexBufferStream::WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2 , f32 p3 , f32 p4  )
{
	f32 p[ 4 ] = { p1 , p2 , p3 , p4 };
	WriteCpuMemory( index , p , 4  );
}

inline void CVertexBufferStream::WriteMemoryValueAll( u8 *pData , u32 size )
{
	SKY_ASSERT_MSG( m_Size % size == 0 , _T( "size is invalid." ) );

	u32 count = m_Size / size;

	for ( u32 i = 0 ; i < count ; i++ )
	{
		CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ i * size ] , pData , size );
	}
}

inline void* CVertexBufferStream::GetCpuMemory( vrtxtype index , u32 size )
{
	return &m_pCpuMemoryBuffer[ index * size ];
}

#ifdef SKYLIB_RENDER_DIRECTX11

void CVertexBufferStream::CreateGpuMemory()
{
	if ( m_IsCreateGpuMemory == skyTrue ) return;
//	SKY_ASSERT_MSG( m_IsCreateGpuMemory == skyFalse , _T( "The GPU memory has already been secured." ) ); //すでにGPUメモリは確保されています。
	SKY_ASSERT_MSG( m_pCpuMemoryBuffer != skyNull, _T( "Cpu memory no buffer." ) ); //CPUメモリにバッファがありません。

	ID3D11Buffer *pBuffer = skyNull;
    HRESULT hr;

	// 頂点バッファ用の情報生成
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage          = D3D11_USAGE_DYNAMIC;		        // デフォルトの配置
	BufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;	        // 頂点バッファとしてバインド
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		        // CPUからの読み書きはしない
	BufferDesc.MiscFlags      = 0;
	BufferDesc.StructureByteStride = 0;

	// 頂点バッファ用の情報生成
	BufferDesc.ByteWidth      = m_Size;	// サイズ

	// 情報を元に頂点バッファを生成
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateBuffer( &BufferDesc , NULL , &pBuffer );
	if ( FAILED( hr ) )
	{
        D3DUtil::PrintD3D11Result( hr );
        SKY_PANIC_MSG( _T( "It failed in securing the GPU memory." ) ); //It failed in securing the GPU memory.
	}

	//スマートポンタ管理にする
	m_pMember->m_spGpuMemoryBuffer.SetPtr( pBuffer );

	m_IsCreateGpuMemory = skyTrue;
}

inline void CVertexBufferStream::DeleteGpuMemory()
{
	if ( !m_pMember->m_spGpuMemoryBuffer.IsEmpty() )     m_pMember->m_spGpuMemoryBuffer.Delete();
}

inline void CVertexBufferStream::WirteGpuMemory  ()
{
	//デバイスを取得
	ID3D11DeviceContext	*pD3DDeviceContext = CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//ＧＰＵ側の参照を保持する構造体宣言
	D3D11_MAPPED_SUBRESOURCE	resource;

	//ＧＰＵのメモリをマップ
	HRESULT hr = pD3DDeviceContext->Map( m_pMember->m_spGpuMemoryBuffer.GetPtrNoConst() , 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
	if ( FAILED( hr ) )
	{
        D3DUtil::PrintD3D11Result( hr );
        SKY_PANIC_MSG( _T( "It failed in the mapping of the vertex buffer." ) ); //It failed in the mapping of the vertex buffer.
	}

	//ＧＰＵ側のポインタ取得
	vertex::CVertexFmt_Pos *pGPUMem = reinterpret_cast< vertex::CVertexFmt_Pos * >( resource.pData );

	//ＧＰＵ側へ書き込む
	CMemoryUtil_::Copy( pGPUMem , m_pCpuMemoryBuffer , m_Size );

	//マップ解除
	pD3DDeviceContext->Unmap( m_pMember->m_spGpuMemoryBuffer.GetPtrNoConst() , 0 );
}

#endif

#ifdef SKYLIB_RENDER_OPENGL4

inline void CVertexBufferStream::CreateGpuMemory()
{
	if ( m_IsCreateGpuMemory == skyTrue ) return;
//	SKY_ASSERT_MSG( m_IsCreateGpuMemory == skyFalse , _T( "The GPU memory has already been secured." ) ); //すでにGPUメモリは確保されています。
	SKY_ASSERT_MSG( m_pCpuMemoryBuffer != skyNull, _T( "Cpu memory no buffer." ) ); //CPUメモリにバッファがありません。

	//頂点バッファハンドルの取得
	glGenBuffers( 1 , &m_pMember->m_hBuffer );

	glBindBuffer( GL_ARRAY_BUFFER , m_pMember->m_hBuffer );
	glBufferData( GL_ARRAY_BUFFER , m_Size , NULL , GL_STATIC_DRAW );

	m_IsCreateGpuMemory = skyTrue;
}

inline void CVertexBufferStream::DeleteGpuMemory()
{
	if ( m_IsCreateGpuMemory == skyFalse ) return;

    glDeleteBuffers( 1 , &m_pMember->m_hBuffer );

	m_IsCreateGpuMemory = skyFalse;
}

inline void CVertexBufferStream::WirteGpuMemory()
{
    GLfloat *pBuffer;

	//GPUメモリをマッピング
	glBindBuffer( GL_ARRAY_BUFFER , m_pMember->m_hBuffer );

	//ＧＰＵ側のポインタ取得
	pBuffer = ( GLfloat * )glMapBuffer( GL_ARRAY_BUFFER , GL_READ_WRITE );
	if( pBuffer == NULL)
	{
        SKY_PANIC_MSG( _T( "It failed in the mapping of the vertex buffer." ) );
	}

	SKY_ASSERT( m_UseSize > 0 ); //使ってない

	//ＧＰＵ側へ書き込む
	CMemoryUtil_::Copy( pBuffer , m_pCpuMemoryBuffer , m_UseSize ); //使用している分だけ更新する

	//GPUメモリのマッピング解除
	glUnmapBuffer( GL_ARRAY_BUFFER );
}

#endif

inline CVertexBufferStream::CVertexBufferStream() :
    m_pMember( NEW__( CVertexBufferStreamMember , CVertexBufferStreamMember ) ) ,
	m_pCpuMemoryBuffer( skyNull ) ,
	m_IsCreateGpuMemory( skyFalse )
{ 
}

inline void CVertexBufferStream::WritePosition( vrtxtype index , const vertex::CVertexFmt_Pos &pos )
{
	SKY_ASSERT_MSG( index * sizeof( vertex::CVertexFmt_Pos ) <= m_Size , _T( "size is over." ) );
	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( vertex::CVertexFmt_Pos ) ] , &pos , sizeof( vertex::CVertexFmt_Pos ) );
}

inline void CVertexBufferStream::WriteNormal( vrtxtype index , const vertex::CVertexFmt_Normal &normal )
{
	SKY_ASSERT_MSG( index * sizeof( vertex::CVertexFmt_Normal ) <= m_Size , _T( "size is over." ) );
	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( vertex::CVertexFmt_Normal ) ] , &normal , sizeof( vertex::CVertexFmt_Normal ) );
}

inline void CVertexBufferStream::WriteUv( vrtxtype index , const vertex::CVertexFmt_UV &uv )
{
	SKY_ASSERT_MSG( index * sizeof( vertex::CVertexFmt_UV ) <= m_Size , _T( "size is over." ) );
	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( vertex::CVertexFmt_UV ) ] , &uv , sizeof( vertex::CVertexFmt_UV ) );
}

inline void CVertexBufferStream::WriteColor( vrtxtype index , const vertex::CVertexFmt_Color &color )
{
	SKY_ASSERT_MSG( index * sizeof( vertex::CVertexFmt_Color ) <= m_Size , _T( "size is over." ) );
	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( vertex::CVertexFmt_Color ) ] , &color , sizeof( vertex::CVertexFmt_Color ) );
}

inline void CVertexBufferStream::WriteIndex( vrtxtype index , const vertex::CVertexFmt_Index &indexs )
{
	SKY_ASSERT_MSG( index * sizeof( vertex::CVertexFmt_Index ) <= m_Size , _T( "size is over." ) );
	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( vertex::CVertexFmt_Index ) ] , &indexs , sizeof( vertex::CVertexFmt_Index ) );
}

inline void CVertexBufferStream::WriteWeights( vrtxtype index , const vertex::CVertexFmt_Weights &weights )
{
	SKY_ASSERT_MSG( index * sizeof( vertex::CVertexFmt_Weights ) <= m_Size , _T( "size is over." ) );
	CMemoryUtil_::Copy( &m_pCpuMemoryBuffer[ index * sizeof( vertex::CVertexFmt_Weights ) ] , &weights , sizeof( vertex::CVertexFmt_Weights ) );
}

inline void CVertexBufferStream::WritePositionAll( const vertex::CVertexFmt_Pos &pos )
{
	SKY_ASSERT_MSG( m_Size % sizeof( vertex::CVertexFmt_Pos ) == 0 , _T( "size is invalid." ) );
	
	u32 num = m_Size / sizeof( vertex::CVertexFmt_Pos );
	for( vrtxtype i = 0 ; i < num ; i++ )
	{
		WritePosition( i , pos );
	}
}

inline void CVertexBufferStream::WriteNormalAll  ( const vertex::CVertexFmt_Normal &normal )
{
	SKY_ASSERT_MSG( m_Size % sizeof( vertex::CVertexFmt_Normal ) == 0 , _T( "size is invalid." ) );

	u32 num = m_Size / sizeof( vertex::CVertexFmt_Normal );
	for( vrtxtype i = 0 ; i < num ; i++ )
	{
		WriteNormal( i , normal );
	}
}

inline void CVertexBufferStream::WriteUvAll      ( const vertex::CVertexFmt_UV &uv )
{
	SKY_ASSERT_MSG( m_Size % sizeof( vertex::CVertexFmt_UV ) == 0 , _T( "size is invalid." ) );

	u32 num = m_Size / sizeof( vertex::CVertexFmt_UV );
	for( vrtxtype i = 0 ; i < num ; i++ )
	{
		WriteUv( i , uv );
	}
}

inline void CVertexBufferStream::WriteColorAll   ( const vertex::CVertexFmt_Color &color )
{
	SKY_ASSERT_MSG( m_Size % sizeof( vertex::CVertexFmt_Color ) == 0 , _T( "size is invalid." ) );

	u32 num = m_Size / sizeof( vertex::CVertexFmt_Color );
	for( vrtxtype i = 0 ; i < num ; i++ )
	{
		WriteColor( i , color );
	}
}

inline void CVertexBufferStream::WriteIndexAll   ( const vertex::CVertexFmt_Index &indexs )
{
	SKY_ASSERT_MSG( m_Size % sizeof( vertex::CVertexFmt_Index ) == 0 , _T( "size is invalid." ) );

	u32 num = m_Size / sizeof( vertex::CVertexFmt_Index );
	for( vrtxtype i = 0 ; i < num ; i++ )
	{
		WriteIndex( i , indexs );
	}
}

inline void CVertexBufferStream::WriteWeightsAll   ( const vertex::CVertexFmt_Weights &weights )
{
	SKY_ASSERT_MSG( m_Size % sizeof( vertex::CVertexFmt_Weights ) == 0 , _T( "size is invalid." ) );

	u32 num = m_Size / sizeof( vertex::CVertexFmt_Weights );
	for( vrtxtype i = 0 ; i < num ; i++ )
	{
		WriteWeights( i , weights );
	}
}

inline vertex::CVertexFmt_Pos* CVertexBufferStream::GetPosition( vrtxtype index )
{
	return ( vertex::CVertexFmt_Pos * )GetCpuMemory( index , sizeof( vertex::CVertexFmt_Pos ) );
}
inline vertex::CVertexFmt_Normal*  CVertexBufferStream::GetNormal( vrtxtype index )
{
	return ( vertex::CVertexFmt_Normal * )GetCpuMemory( index , sizeof( vertex::CVertexFmt_Normal ) );
}
inline vertex::CVertexFmt_UV* CVertexBufferStream::GetUv( vrtxtype index )
{
	return ( vertex::CVertexFmt_UV * )GetCpuMemory( index , sizeof( vertex::CVertexFmt_UV ) );
}
inline vertex::CVertexFmt_Color* CVertexBufferStream::GetColor( vrtxtype index )
{
	return ( vertex::CVertexFmt_Color * )GetCpuMemory( index , sizeof( vertex::CVertexFmt_Color ) );
}
inline vertex::CVertexFmt_Index* CVertexBufferStream::GetIndex( vrtxtype index )
{
	return ( vertex::CVertexFmt_Index * )GetCpuMemory( index , sizeof( vertex::CVertexFmt_Index ) );
}
inline vertex::CVertexFmt_Weights* CVertexBufferStream::GetWeights( vrtxtype index )
{
	return ( vertex::CVertexFmt_Weights * )GetCpuMemory( index , sizeof( vertex::CVertexFmt_Weights ) );
}

inline void CVertexBufferStream::ComputePosMatrixPositionAll( const math::CMatrix4x3 &matrix )
{
	for ( vrtxtype i = 0 ; i < GetNumPosition() ; i++ )
	{
		*GetPosition( i ) = GetPosition( i )->Get() * matrix;
	}
}

inline void CVertexBufferStream::ComputePosMatrixNormalAll( const math::CMatrix4x3 &matrix )
{
	for ( vrtxtype i = 0 ; i < GetNumNormal() ; i++ )
	{
		*GetNormal( i ) = GetNormal( i )->Get() * matrix;
	}
}

inline void CVertexBufferStream::AddPositionAll( vrtxtype index , const vertex::CVertexFmt_Pos &pos )
{
	GetPosition( index )->x += pos.x;
	GetPosition( index )->y += pos.y;
	GetPosition( index )->z += pos.z;
}

inline void CVertexBufferStream::AddNormalAll( vrtxtype index , const vertex::CVertexFmt_Normal &normal )
{
	GetNormal( index )->nx += normal.nx;
	GetNormal( index )->ny += normal.ny;
	GetNormal( index )->nz += normal.nz;
}

inline void CVertexBufferStream::AddUvAll( vrtxtype index , const vertex::CVertexFmt_UV &uv )
{
	GetUv( index )->tu += uv.tu;
	GetUv( index )->tv += uv.tv;
}

inline void CVertexBufferStream::AddColorAll( vrtxtype index , const vertex::CVertexFmt_Color &color )
{
	GetColor( index )->color[ 0 ] += color.color[ 0 ];
	GetColor( index )->color[ 1 ] += color.color[ 1 ];
	GetColor( index )->color[ 2 ] += color.color[ 2 ];
	GetColor( index )->color[ 3 ] += color.color[ 3 ];
}

inline const CVertexBufferStreamMemberPtr& CVertexBufferStream::GetMember()
{
	return m_pMember;
}

inline void CVertexBufferStream::SwapPositionYAndZ()
{
	u32 num = GetNumPosition();
	vertex::CVertexFmt_Pos swapedPos;
	for ( vrtxtype i = 0 ; i < num ; i++ )
	{
		vertex::CVertexFmt_Pos* pPos = GetPosition( i );
		swapedPos.x = pPos->x;
		swapedPos.z = pPos->y;
		swapedPos.y = pPos->z;
		WritePosition( i , swapedPos );
	}
}

} } }
