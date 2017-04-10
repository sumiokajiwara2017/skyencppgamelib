#include "StdAfx.h"
#include "SkyGraphicLib/Vertex.h"

//===================================�yCommon�z====================================================>

SkyImplementRTTI( sky::lib::graphic::CVertexBuffer , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

u8 GetPrimNum( vertex::ePrimitiveType eType )
{
	switch ( eType )
	{
	case vertex::ePrimitiveType_Pointlist:
	case vertex::ePrimitiveType_Linelist:
	case vertex::ePrimitiveType_Linestrip:
		return 2;
	case vertex::ePrimitiveType_Trianglelist:
	case vertex::ePrimitiveType_Meshtrip:
	case vertex::ePrimitiveType_Trianglefan:
		return 3;
	default:
        SKY_PANIC_MSG( _T( "This type is no supported. \n" ) );
	}
	return 0;

}	

CVertexBuffer::~CVertexBuffer()
{
	DeleteGpuMemory();
	Dispose();
}

void CVertexBuffer::Initialize( vrtxtype vertexNum , vertex::ePrimitiveType ePrimType )
{
	//�܂�����������
	Dispose();

	m_ePrimType = ePrimType;
	m_VertexNum	= vertexNum;
	m_UseVertexNum = 0;

	AttachPositionStream( CVertexBufferStream::Create( hash::CHashStringPtr() ) );
	GetPositionStream()->InitializePosition( m_VertexNum );
}

void CVertexBuffer::Dispose()
{
	if ( m_VertexNum > 0 )
	{
		GetPositionStream()->Dispose();

		m_VertexNum = 0;
	}
}

void CVertexBuffer::ComputeAll()
{
	//���_���̖@���̌v�Z
	ComputeVertexNormalVector();
}

void CVertexBuffer::ComputePrimNormalVector()
{
	//�v���~�e�B�u���\�����钸�_�̍��W���\������ʂƒ��s������i���Ȃ킿�@���j���v�Z����
	vrtxtype primNum =  m_VertexNum / GetPrimNum( m_ePrimType );
	vrtxtype onePrimVertexNum = GetPrimNum( m_ePrimType );

	//�����̃o�b�t�@������
	if ( GetPrimitiveNormalStream().IsEmpty() == skyFalse )
	{
		GetPrimitiveNormalStream()->Dispose();
	}

	//�V�����o�b�t�@�����
	AttachStream( eVertexBufferStreamType_Normal , CVertexBufferStream::Create() );
	GetPrimitiveNormalStream()->InitializeNormal( m_VertexNum );

	//�v�Z�p�o�b�t�@�쐬
	math::CBasicVector3 *pBuf = NEW_ARRAY__( math::CBasicVector3 , onePrimVertexNum );
	for ( vrtxtype i = 0 ; i < primNum ;  i++ )
	{
		pBuf[ 0 ] = GetPositionStream()->GetPosition( i + 0 )->Get();
		pBuf[ 1 ] = GetPositionStream()->GetPosition( i + 1 )->Get();
		pBuf[ 2 ] = GetPositionStream()->GetPosition( i + 2 )->Get();

		math::CBasicVector3 vec;
		math::VectorUtil::ComputeBestFitNormal< math::CBasicVector3 >( pBuf , onePrimVertexNum , vec );
		*GetPrimitiveNormalStream()->GetNormal( i ) = vec;
	}
	SAFE_DELETE_ARR( pBuf );
}

void CVertexBuffer::ComputePrimNormalVectorFromIndexBuffer( const CIndexBufferPtr &spIndexBuffer )
{
	SKY_ASSERT_MSG( !spIndexBuffer.IsEmpty() , _T("The index buffer is necessary." ) ); //�v���~�e�B�u���̖@�����v�Z����ɂ̓C���f�b�N�X�o�b�t�@�̃n���h�����Z�b�g���ĉ������B

	//�v���~�e�B�u���\�����钸�_�̍��W���\������ʂƒ��s������i���Ȃ킿�@���j���v�Z����
	vrtxtype primNum =  spIndexBuffer->GetIndexNum() / GetPrimNum( m_ePrimType );
	vrtxtype onePrimVertexNum = GetPrimNum( m_ePrimType );

	//�����̃o�b�t�@������
	if ( GetPrimitiveNormalStream().IsEmpty() == skyFalse )
	{
		GetPrimitiveNormalStream()->Dispose();
	}

	//�V�����o�b�t�@�����
	AttachStream( eVertexBufferStreamType_Normal , CVertexBufferStream::Create() );
	GetPrimitiveNormalStream()->InitializeNormal( m_VertexNum );

	//�v�Z�p�o�b�t�@�쐬
	math::CBasicVector3 *pBuf = NEW_ARRAY__( math::CBasicVector3 , onePrimVertexNum );
	for ( vrtxtype i = 0 ; i < primNum ;  i++ )
	{
		pBuf[ 0 ] = GetPositionStream()->GetPosition( spIndexBuffer->Index( onePrimVertexNum * i + 0 ) )->Get();
		pBuf[ 1 ] = GetPositionStream()->GetPosition( spIndexBuffer->Index( onePrimVertexNum * i + 1 ) )->Get();
		pBuf[ 2 ] = GetPositionStream()->GetPosition( spIndexBuffer->Index( onePrimVertexNum * i + 2 ) )->Get();

		//�C���f�b�N�X�o�b�t�@����C���f�b�N�X��onePrimVertexNum���B
		//���̃C���f�b�N�X�̍��W��onePrimVertexNum���B���̂R�̍��W���g���Ė@��������o��
		math::CBasicVector3 vec;
		math::VectorUtil::ComputeBestFitNormal< math::CBasicVector3 >(  pBuf , onePrimVertexNum , vec );
		*GetPrimitiveNormalStream()->GetNormal( i ) = vec;
	}
	SAFE_DELETE_ARR( pBuf );
}

void CVertexBuffer::ComputeVertexNormalVector( const CIndexBufferPtr &spIndexBuffer )
{
	//�����̃o�b�t�@������
	if ( GetVertexNormalStream().IsEmpty() == skyFalse )
	{
		GetVertexNormalStream()->Dispose();
	}

	//�V�����o�b�t�@�����
	AttachStream( eVertexBufferStreamType_Normal , CVertexBufferStream::Create() );
	GetVertexNormalStream()->InitializeNormal( m_VertexNum );

	vrtxtype onePrimVertexNum = 0;
	vrtxtype primNum = 0;
	if ( spIndexBuffer.IsEmpty() )
	{//�C���f�b�N�X�o�b�t�@��������

		// �v���~�e�B�u�̖@�����v�Z����
		ComputePrimNormalVector();
	}
	else
	{//�C���f�b�N�X�o�b�t�@������B��

		// �v���~�e�B�u�̖@�����v�Z����
		ComputePrimNormalVectorFromIndexBuffer( spIndexBuffer );
	}


	// �@�����O�ɏ�����
	math::CBasicVector3 zero;
	zero.Zero();
	for ( vrtxtype i = 0 ; i < m_VertexNum ; i++ )
	{
		*GetVertexNormalStream()->GetNormal( i ) = zero;
	}

	// �v���~�e�B�u�̖@���𒸓_�̖@���ɑ�������
	for ( vrtxtype i = 0 ; i < primNum ;  i++ )
	{
		for ( vrtxtype j = 0 ; j < onePrimVertexNum ; j++ )
		{
			vrtxtype indexNo = i * onePrimVertexNum + j;
			if ( spIndexBuffer.IsEmpty() )
			{//�C���f�b�N�X�o�b�t�@��������

				*GetVertexNormalStream()->GetNormal( indexNo ) += *GetPrimitiveNormalStream()->GetNormal( i );
			}
			else
			{//�C���f�b�N�X�o�b�t�@������B��

				//�ʂɊ֌W���钸�_�ɖʂ̖@���𑫂�����
				vrtxtype index   = spIndexBuffer->Index( indexNo );
				*GetVertexNormalStream()->GetNormal( index ) += *GetPrimitiveNormalStream()->GetNormal( i );
			}

		}
	}

	// ���_�̖@���𐳋K������
	for ( vrtxtype i = 0 ; i < m_VertexNum ; i++ )
	{
		*GetVertexNormalStream()->GetNormal( i ) = GetVertexNormalStream()->GetNormal( i )->Get().Normalize();
	}
}

void CVertexBuffer::ComputePosMatrix( const math::CMatrix4x3 &matrix )
{
	GetPositionStream()->ComputePosMatrixPositionAll( matrix );
}

void  CVertexBuffer::TransPosCenter()
{
	dectype mX = 0 , hX = 0 , mY = 0 , hY = 0 , mZ = 0 , hZ = 0;
	math::CBasicVector3 vec3( 0.0f , 0.0f , 0.0f );
	for ( vrtxtype i = 0 ; i < m_VertexNum ; i++ )
	{
		//X�̍ŏ������߂�
		//X�̍ő�����߂�
		if ( mX > GetPositionStream()->GetPosition( i )->x ){ mX = GetPositionStream()->GetPosition( i )->x; }
		if ( hX < GetPositionStream()->GetPosition( i )->x ){ hX = GetPositionStream()->GetPosition( i )->x; }

		//Y�̍ŏ������߂�
		//Y�̍ő�����߂�
		if ( mY > GetPositionStream()->GetPosition( i )->y ){ mY = GetPositionStream()->GetPosition( i )->y; }
		if ( hY < GetPositionStream()->GetPosition( i )->y ){ hY = GetPositionStream()->GetPosition( i )->y; }

		//Z�̍ŏ������߂�
		//Z�̍ő�����߂�
		if ( mZ > GetPositionStream()->GetPosition( i )->z ){ mZ = GetPositionStream()->GetPosition( i )->z; }
		if ( hZ < GetPositionStream()->GetPosition( i )->z ){ hZ = GetPositionStream()->GetPosition( i )->z; }
	}

	//���݂̒��S�ʒu�̕������t�ɂ�����
	vec3.X = ( hX - mX ) / 2;
	vec3.Y = ( hY - mY ) / 2;
	vec3.Z = ( hZ - mZ ) / 2;

	//�S���̍��W���璆�S���W������
	for ( vrtxtype i = 0 ; i < m_VertexNum ; i++ )
	{
		GetPositionStream()->GetPosition( i )->x -= ( f32 )vec3.X;
		GetPositionStream()->GetPosition( i )->y -= ( f32 )vec3.Y;
		GetPositionStream()->GetPosition( i )->z -= ( f32 )vec3.Z;
	}
}
//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

void CVertexBuffer::Draw( vrtxtype startIndex , vrtxtype drawIndexNum , const CIndexBufferPtr &spIndexBuffer  )
{
	SKY_UNUSED_ARG( spIndexBuffer );

	if ( drawIndexNum == 0 )
	{
		//���_�݂̂ŕ`��
		CDevice_->GetMember()->m_spD3DDeviceContext->Draw( m_UseVertexNum , 0 );
	}
	else
	{
		//�C���f�b�N�X�o�b�t�@�g�p�`��
		CDevice_->GetMember()->m_spD3DDeviceContext->DrawIndexed( drawIndexNum , startIndex , 0 );
	}
}

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

/// <summary>
/// �v���~�e�B�u�^�e�[�u��
/// </summary>
static GLenum s_TypeTable[] =
{
    GL_POINTS         , // vertex::ePrimitiveType_Pointlist
    GL_LINES          , // vertex::ePrimitiveType_Linelist
    GL_LINE_STRIP     , // vertex::ePrimitiveType_Linestrip
    GL_TRIANGLES      , // vertex::ePrimitiveType_Trianglelist
    GL_TRIANGLE_STRIP , // vertex::ePrimitiveType_Meshtrip
    GL_TRIANGLE_FAN   , // vertex::ePrimitiveType_Trianglefan //D3D11�ł͌�������Ȃ��B
};
static s32 s_TypeTable_size = array_sizeof( s_TypeTable );

static GLenum GetPrimitiveType_( vertex::ePrimitiveType eType )
{
	SKY_ASSERT_MSG( s_TypeTable_size <= ( s32 )s_TypeTable || 0 > ( s32 )s_TypeTable , _T( "The value of the primitive type is illegal." ) ); //�v���~�e�B�u�^�̒l���s���ł��B

	return s_TypeTable[ eType ];
}

void CVertexBuffer::Draw( vrtxtype startIndex , vrtxtype indexNum , const CIndexBufferPtr &spIndexBuffer )
{
	SKY_UNUSED_ARG( startIndex );

	if ( indexNum == 0 )
	{
		if( spIndexBuffer.IsEmpty() )
		{
			//���_�����`��
            indexNum = m_UseVertexNum;
		}
		else
		{
			//�C���f�b�N�X�����`��
			indexNum = spIndexBuffer->GetUseIndexNum();
		}
	}

	glDrawElements( GetPrimitiveType_( m_ePrimType ) , indexNum , GL_UNSIGNED_SHORT , 0 );
}

#endif

//===================================�yCommon�z====================================================>

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_ePrimType , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_VertexNum , sizeof( u16 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_UseVertexNum , sizeof( u16 ) , isWrite ); \

skyBool CVertexBuffer::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "vertexbuffer bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CVertexBuffer::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "vertexbuffer bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CVertexBuffer::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CVertexBufferPtr )spContainer )->m_ePrimType = m_ePrimType;
	( ( CVertexBufferPtr )spContainer )->m_VertexNum = m_VertexNum;
	( ( CVertexBufferPtr )spContainer )->m_UseVertexNum = m_UseVertexNum;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CVertexBuffer::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CVertexBufferPtr )spContainer )->m_pMember = m_pMember;
	( ( CVertexBufferPtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CVertexBuffer::Clone()
{
	return Copy( CVertexBufferPtr( NEW__( CVertexBuffer , CVertexBuffer() ) ) );
}

serialize::CSerializeInterfacePtr CVertexBuffer::CloneSharing()
{
	return CopySharing( Create( hash::CHashStringPtr() ) );
}

void CVertexBuffer::DeleteCpuMemory()
{
	if( GetPositionStream( hash::CHashStringPtr() , skyFalse ).IsEmpty() == skyFalse ) GetPositionStream()->DeleteCpuMemory();
}

void CVertexBuffer::CreateGpuMemory()
{
	SKY_ASSERT_MSG( m_IsCreateGpuMemory == skyFalse , _T( "The GPU memory has already been secured." ) ); //���ł�GPU�������͊m�ۂ���Ă��܂��B

	FOREACH( CSceneGraphNodePtrList , it , LeafList() )
	{
		( ( CVertexBufferStreamPtr )*it )->CreateGpuMemory();
	}

	m_IsCreateGpuMemory = skyTrue;
}

void CVertexBuffer::WirteGpuMemory()
{
	FOREACH( CSceneGraphNodePtrList , it , LeafList() )
	{
		( ( CVertexBufferStreamPtr )*it )->WirteGpuMemory();
	}
}

void CVertexBuffer::DeleteGpuMemory()
{
	if( m_IsCreateGpuMemory == skyTrue )
	{
		FOREACH( CSceneGraphNodePtrList , it , LeafList() )
		{
			( ( CVertexBufferStreamPtr )*it )->DeleteGpuMemory();
		}
	}
}

CVertexBuffer::CVertexBuffer() :
    m_pMember( NEW__( CVertexBufferMember , CVertexBufferMember ) ) ,
	m_VertexNum( 0 ) , 
	m_UseVertexNum( 0 ) ,
	m_IsCreateGpuMemory( skyFalse )
{ 
	DeleteGpuMemory();
}

CVertexBufferStreamPtr CVertexBuffer::GetPositionStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Position , name , isRequired );
}
void CVertexBuffer::AttachPositionStream( const CVertexBufferStreamPtr &position )
{
	AttachStream( eVertexBufferStreamType_Position , position );
}

CVertexBufferStreamPtr CVertexBuffer::GetNormalStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Normal , name , isRequired );
}
void CVertexBuffer::AttachNormalStream( const CVertexBufferStreamPtr &normal )
{
	AttachStream( eVertexBufferStreamType_Normal , normal );
}
void CVertexBuffer::CreateNormalStream( vrtxtype num )
{
	CVertexBufferStreamPtr spBuffer = CVertexBufferStream::Create();
	AttachStream( eVertexBufferStreamType_Normal , spBuffer );
	spBuffer->InitializeNormal( num );
}

CVertexBufferStreamPtr CVertexBuffer::GetUvStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Uv , name , isRequired );
}
void CVertexBuffer::AttachUvStream( const CVertexBufferStreamPtr &uv )
{
	AttachStream( eVertexBufferStreamType_Uv , uv );
}
void CVertexBuffer::CreateUvStream( vrtxtype num )
{
	CVertexBufferStreamPtr spBuffer = CVertexBufferStream::Create();
	AttachStream( eVertexBufferStreamType_Uv , spBuffer );
	spBuffer->InitializeUv( num );
}

CVertexBufferStreamPtr CVertexBuffer::GetColorStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Color , name , isRequired );
}
void CVertexBuffer::AttachColorStream( const CVertexBufferStreamPtr &color )
{
	AttachStream( eVertexBufferStreamType_Color , color );
}
void CVertexBuffer::CreateColorStream( vrtxtype num , CColor initColor  )
{
	CVertexBufferStreamPtr spBuffer = CVertexBufferStream::Create();
	AttachStream( eVertexBufferStreamType_Color , spBuffer );
	spBuffer->InitializeColor( num );
	spBuffer->WriteColorAll( initColor );
}

CVertexBufferStreamPtr CVertexBuffer::GetIndexStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Index , name , isRequired );
}
void CVertexBuffer::AttachIndexStream( const CVertexBufferStreamPtr &uv )
{
	AttachStream( eVertexBufferStreamType_Index , uv );
}
void CVertexBuffer::CreateIndexStream( vrtxtype num )
{
	CVertexBufferStreamPtr spBuffer = CVertexBufferStream::Create();
	AttachStream( eVertexBufferStreamType_Index , spBuffer );
	spBuffer->InitializeIndex( num );
}

CVertexBufferStreamPtr CVertexBuffer::GetWeightsStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Weights , name , isRequired );
}
void CVertexBuffer::AttachWeightsStream( const CVertexBufferStreamPtr &uv )
{
	AttachStream( eVertexBufferStreamType_Weights , uv );
}
void CVertexBuffer::CreateWeightsStream( vrtxtype num )
{
	CVertexBufferStreamPtr spBuffer = CVertexBufferStream::Create();
	AttachStream( eVertexBufferStreamType_Weights , spBuffer );
	spBuffer->InitializeWeights( num );
}

void CVertexBuffer::AttachStream( eVertexBufferStreamType eType , const CVertexBufferStreamPtr& stream )
{
	if ( stream.IsEmpty() ) return;

	stream->ID = ( s32 )eType;
	this->AttachLeaf( stream );
}

CVertexBufferStreamPtr CVertexBuffer::GetStream( eVertexBufferStreamType eType , const hash::CHashStringPtr &name , skyBool isRequired )
{
	CVertexBufferStreamPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeafIDAndName< CVertexBufferStream , CVertexBufferStreamPtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eType , spResult , name );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "buffer is not found." ) );
	}
#endif
	return spResult;
}

void CVertexBuffer::DetachStream( eVertexBufferStreamType eType , const hash::CHashStringPtr &name )
{
	SceneGraphNodeUtil::DetachLeafIDAndName< CVertexBufferStream >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eType , name );
}

CVertexBufferStreamPtr CVertexBuffer::GetPrimitiveNormalStream(  const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Normal , name , isRequired );
}

CVertexBufferStreamPtr CVertexBuffer::DetachPrimitiveNormalStream( const hash::CHashStringPtr &name ,  skyBool isRequired )
{
	CVertexBufferStreamPtr result = GetStream( eVertexBufferStreamType_Normal , name , isRequired );
	DetachStream( eVertexBufferStreamType_Normal );
	return result;
}

CVertexBufferStreamPtr CVertexBuffer::GetVertexNormalStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	return GetStream( eVertexBufferStreamType_Normal , name , isRequired );
}

CVertexBufferStreamPtr CVertexBuffer::DetachVertexNormalStream( const hash::CHashStringPtr &name , skyBool isRequired )
{
	CVertexBufferStreamPtr result = GetStream( eVertexBufferStreamType_Normal , name , isRequired );
	DetachStream( eVertexBufferStreamType_Normal );
	return result;
}

} } }
