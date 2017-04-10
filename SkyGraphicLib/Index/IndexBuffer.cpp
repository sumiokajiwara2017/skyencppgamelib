#include "StdAfx.h"
#include "SkyGraphicLib/Index.h"

#include "SkyGraphicLib/Controller.h"

SkyImplementRTTI( sky::lib::graphic::CIndexBuffer , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

void CIndexBuffer::DeleteGpuMemory()
{
	if( m_IsCreateGpuMemory == skyTrue )
	{
		m_pMember->m_pGpuMemoryIndexBuffer.Release();
	    m_IsCreateGpuMemory = skyFalse;
	}
}

void CIndexBuffer::CreateGpuMemory()
{
	if( m_IsCreateGpuMemory == skyFalse )
	{
		SKY_ASSERT_MSG( m_IndexNum > 0 , _T( "�ݒ�C���f�b�N�X�����O�̏�ԂŃC���f�b�N�X�o�b�t�@���쐬���悤�Ƃ��܂����B���_����ݒ肵�ĉ������B\n" ) );

		//�f�o�C�X���擾
		ID3D11Device	*pD3DDevice	= CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst();

		//���łɊm�ۍς݂̏ꍇ��x�J������
		if ( !m_pMember->m_pGpuMemoryIndexBuffer.IsEmpty() )
		{
			m_pMember->m_pGpuMemoryIndexBuffer.Release();
		}

		// �C���f�b�N�X�o�b�t�@�p�̏�񐶐�
		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.Usage          = D3D11_USAGE_DYNAMIC;        // �f�t�H���g�̔z�u
		BufferDesc.ByteWidth      = sizeof( u16 ) * m_IndexNum; // �T�C�Y
		BufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;    // �C���f�b�N�X�o�b�t�@�Ƃ��ăo�C���h
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	    // CPU����̓ǂݏ����͂��Ȃ�
		BufferDesc.MiscFlags      = 0;

		// �o�b�t�@�������p
		D3D11_SUBRESOURCE_DATA SubData;
		SubData.pSysMem				= m_pCpuMemoryIndexBuffer;
		SubData.SysMemPitch			= 0;
		SubData.SysMemSlicePitch	= 0;

		// �������ɃC���f�b�N�X�o�b�t�@�𐶐�
		ID3D11Buffer *pBuf;
		HRESULT hr = pD3DDevice->CreateBuffer( &BufferDesc, &SubData, &pBuf );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "�C���f�b�N�X�̍쐬�Ɏ��s���܂����B\n" ) );
		}

		//�C���f�b�N�X�o�b�t�@���X�}�[�g�|�C���^��
		m_pMember->m_pGpuMemoryIndexBuffer.SetPtr( pBuf );

		m_IsCreateGpuMemory = skyTrue;
	}
}

void CIndexBuffer::WirteGpuMemory()
{
	//�f�o�C�X���擾
	ID3D11DeviceContext	*pD3DDeviceContext = CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//�f�o�t���̎Q�Ƃ�ێ�����\���̐錾
	D3D11_MAPPED_SUBRESOURCE	resource;

	//�f�o�t�̃��������}�b�v
	HRESULT hr = pD3DDeviceContext->Map( m_pMember->m_pGpuMemoryIndexBuffer.GetPtrNoConst() , 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "It failed in the mapping of the index buffer" ) ); //�C���f�b�N�X�o�b�t�@�̃}�b�s���O�Ɏ��s���܂����B
	}


	//�f�o�t���̃|�C���^�擾
	USHORT *pGPUMem = reinterpret_cast< USHORT * >( resource.pData );

	//�f�o�t���֏�������
	for ( u16 i = 0 ; i < m_UseIndexNum ; i++ )
	{
		pGPUMem[ i ] = ( u16 )m_pCpuMemoryIndexBuffer[ i ];
	}

	//�}�b�v����
	pD3DDeviceContext->Unmap( m_pMember->m_pGpuMemoryIndexBuffer.GetPtrNoConst() , 0 );
}

void CIndexBuffer::SetGpuMemoryStream()
{
	//�C���f�b�N�X�o�b�t�@��ݒ肷��
	CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst()->IASetIndexBuffer( m_pMember->m_pGpuMemoryIndexBuffer.GetPtrNoConst(), DXGI_FORMAT_R16_UINT, 0 );
}

#endif
//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

void CIndexBuffer::DeleteGpuMemory()
{
    if( m_IsCreateGpuMemory == skyTrue )
    {
        glDeleteBuffers( 1 , &m_pMember->m_hBuffer );
        m_IsCreateGpuMemory = skyFalse;
    }
}

void CIndexBuffer::CreateGpuMemory()
{
	if( m_IsCreateGpuMemory == skyFalse )
	{
		//�C���f�b�N�X
		glGenBuffers( 1 , &m_pMember->m_hBuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , m_pMember->m_hBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER , sizeof( GLushort ) * m_IndexNum ,
						NULL , GL_STATIC_DRAW );//�f�[�^�ύX�Ȃ�

		m_IsCreateGpuMemory = skyTrue;
	}
}

void CIndexBuffer::WirteGpuMemory()
{
	glBindBuffer( GL_ARRAY_BUFFER , m_pMember->m_hBuffer );

	//�f�o�t���̃|�C���^�擾
	GLushort *pBuffer = ( GLushort * )glMapBuffer( GL_ARRAY_BUFFER , GL_READ_WRITE );
	if( pBuffer == NULL )
	{
        SKY_PANIC_MSG( _T( "It failed in the mapping of the top buffer" ) ); //���_�o�b�t�@�̃}�b�s���O�Ɏ��s���܂����B
	}

	//�f�o�t���֏�������
	for ( u16 i = 0 ; i < m_IndexNum ; i++ )
	{
		pBuffer[ i ] = m_pCpuMemoryIndexBuffer[ i ];
//		SKY_PRINTF( _T( "%d," ) , pBuffer[ i ] );
	}

	glUnmapBuffer( GL_ARRAY_BUFFER );
}

void CIndexBuffer::SetGpuMemoryStream()
{
	//�C���f�b�N�X���o�C���h
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , m_pMember->m_hBuffer );
}

#endif

//===================================�yCommon�z====================================================>

void CIndexBuffer::SetIndex( vrtxtype indexNo , vrtxtype value )
{
    SKY_ASSERT_MSG( indexNo >= 0 && indexNo < m_IndexNum , _T( "The size of the index buffer is exceeded." ) );

    if ( m_UseIndexNum < indexNo + 1 ) m_UseIndexNum = indexNo + 1;

    m_pCpuMemoryIndexBuffer[ indexNo ] = value;
}

vrtxtype CIndexBuffer::Index( vrtxtype indexNo ) const
{
    SKY_ASSERT_MSG( indexNo >= 0 && indexNo < m_IndexNum , _T( "The size of the index buffer is exceeded." ) ); //�C���f�b�N�X�o�b�t�@�̃T�C�Y�𒴂��Ă��܂��B

    return m_pCpuMemoryIndexBuffer[ indexNo ];
}

void CIndexBuffer::Dispose()
{
    if ( m_IndexNum > 0 )
    {
        SAFE_DELETE_ARR( m_pCpuMemoryIndexBuffer );
        m_IndexNum = 0;
    }
}

CIndexBuffer::CIndexBuffer() :
    m_IndexNum( 0 ) ,
    m_UseIndexNum( 0 ) ,
    m_pCpuMemoryIndexBuffer( skyNull ) ,
    m_IsCreateGpuMemory( skyFalse ) ,
	m_pMember( NEW__( CIndexBufferMember , CIndexBufferMember ) )
{
	DeleteGpuMemory();
}

CIndexBuffer::~CIndexBuffer()
{
    Dispose();
}

void CIndexBuffer::Initialize( vrtxtype indexNum )
{
    Dispose();

    m_IndexNum = indexNum;
    m_pCpuMemoryIndexBuffer = NEW_ARRAY__( u16 , m_IndexNum );
    m_UseIndexNum = 0;
}

void CIndexBuffer::Copy( CIndexBufferPtr &spIndexBuffer )
{
    spIndexBuffer->Initialize( m_IndexNum );

    for ( vrtxtype i = 0 ; i < m_IndexNum ; i++ )
    {
        spIndexBuffer->SetIndex( i , Index( i ) );
    }
}

void CIndexBuffer::DeleteCpuMemory()
{
    SAFE_DELETE_ARR( m_pCpuMemoryIndexBuffer );
}

const CIndexBufferMemberPtr &CIndexBuffer::GetMember()
{
    return m_pMember;
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IndexNum , sizeof( u16 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_UseIndexNum , sizeof( u16 ) , isWrite ); \

skyBool CIndexBuffer::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "indexbuffer bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	stream->vWriteReadStream( ( u8 * )m_pCpuMemoryIndexBuffer , sizeof( u16 ) * m_IndexNum , skyTrue );

	return result;
}

skyBool CIndexBuffer::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "indexbuffer bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	m_pCpuMemoryIndexBuffer = NEW_ARRAY__( u16 , m_IndexNum );
	stream->vWriteReadStream( ( u8 * )m_pCpuMemoryIndexBuffer , sizeof( u16 ) * m_IndexNum , skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CIndexBuffer::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CIndexBufferPtr )spContainer )->m_IndexNum = m_IndexNum;
	( ( CIndexBufferPtr )spContainer )->m_UseIndexNum = m_UseIndexNum;
    ( ( CIndexBufferPtr )spContainer )->m_pCpuMemoryIndexBuffer = NEW_ARRAY__( u16 , m_IndexNum );
	CMemoryUtil_::Copy( ( ( CIndexBufferPtr )spContainer )->m_pCpuMemoryIndexBuffer , m_pCpuMemoryIndexBuffer , sizeof( u16 ) * m_IndexNum );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CIndexBuffer::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CIndexBufferPtr )spContainer )->m_pMember = m_pMember;
	( ( CIndexBufferPtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CIndexBuffer::Clone()
{
	return Copy( CIndexBufferPtr( NEW__( CIndexBuffer , CIndexBuffer() ) ) );
}

serialize::CSerializeInterfacePtr CIndexBuffer::CloneSharing()
{
	return CopySharing( CIndexBufferPtr( NEW__( CIndexBuffer , CIndexBuffer() ) ) );
}

void CIndexBuffer::Activate()
{
	ACTIVATE_COMMON();

	CreateGpuMemory();
	WirteGpuMemory();
}

void CIndexBuffer::Deactivate()
{
	DEACTIVATE_COMMON();

	DeleteGpuMemory();
}

void CIndexBuffer::InfoPrint()
{
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
	SKY_PRINTF( _T( "name=[ %s ] [ %d ] [ " ) , Name.GetString() , m_IndexNum );
	for ( vrtxtype i = 0 ; i < m_IndexNum ; i++ )
	{
		SKY_PRINTF( _T( "%d " ) , Index( i ) );
	}
	SKY_PRINTF( _T( "]\n" ) );
}

} } }
