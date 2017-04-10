#include "StdAfx.h"
#include "SkyGraphicLib/Vertex.h"

#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

void CVertexUtil::CreatePoint( const math::CBasicVector3 &point , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	spVertexBuffer->Initialize( 1 , vertex::ePrimitiveType_Pointlist );
	spVertexBuffer->CreateColorStream( 1 );

	spVertexBuffer->GetPositionStream()->WritePosition( 0 , point );

	spVertexBuffer->GetColorStream()->WriteColor( 0 , color );

	//�C���f�b�N�X�o�b�t�@�̍쐬
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();
	spIndexBuffer->Initialize( 1 );

	spIndexBuffer->SetIndex( 0 , 0 );

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector2Vector &vector , const CMeshPtr &spMesh , const CColor &color )
{
	SKY_ASSERT_MSG( vector.GetNum() > 0 , _T( "num is over 0. \n" ) );

	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linestrip );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		pos3 = vector[ i ];
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos3 );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();
	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );

	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &start , const math::CBasicVector2 &end , const CColor &color )
{
	math::CBasicVector2Vector vector;
	vector.Add( start );
	vector.Add( end );

	CreateLine2D( spVertexBuffer , vector , spMesh , color );
}

void CVertexUtil::CreateSquareLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , dectype width , dectype height , const CColor &color )
{
	dectype oneX = width / 2;
	dectype oneY = height / 2;

	math::CBasicVector2Vector vector;
	vector.Add( math::CBasicVector2( -oneX ,  oneY ) );
	vector.Add( math::CBasicVector2(  oneX ,  oneY ) );
	vector.Add( math::CBasicVector2( -oneX , -oneY ) );
	vector.Add( math::CBasicVector2(  oneX , -oneY ) );

	CreateLine2D( spVertexBuffer , vector , spMesh , color );

	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���Q�ŏ�����
	spIndexBuffer->Initialize( 5 );

	vrtxtype idx = 0; 

	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 0 );

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CMeshPtr &spMesh , const CColor &color )
{
	//���_�͂Q�ŏ�����
	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linestrip );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , vector[ i ] );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );
	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicLine3List &lineList , const CMeshPtr &spMesh , const CColor &color )
{
	vrtxtype vertexNum = ( vrtxtype )lineList.GetNum() * 2;

	//���_�͂Q�ŏ�����
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( vertexNum );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < ( vrtxtype )lineList.GetNum() ;  i++ )
	{
		pos3 = lineList[ i ].m_Pos1;
		spVertexBuffer->GetPositionStream()->WritePosition( i * 2 , pos3 );
		pos3 = lineList[ i ].m_Pos2;
		spVertexBuffer->GetPositionStream()->WritePosition( i * 2 + 1 , pos3 );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( vertexNum );
	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &start , const math::CBasicVector3 &end , const CColor &color )
{
	math::CBasicVector3Vector vector;
	vector.Add( start );
	vector.Add( end );
	CreateLine3D( spVertexBuffer , vector , spMesh , color );
}


struct Vertex
{
	dectype x , y , z , w;
	dectype u , v;
};

void CVertexUtil::CreateSphereTexture( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype radius , vrtxtype xnum , vrtxtype ynum , const CColor &color )
{
	vrtxtype vertexNum = ( xnum + 1 ) * ( ynum + 1 ); //���_��
	vrtxtype indexNum  = 6 * xnum * ynum;             //�C���f�b�N�X��
	vrtxtype idx = 0; 

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

	//���_���
	for ( vrtxtype j = 0 ; j <= ynum ; j++ )
	{
		 for ( vrtxtype i = 0 ; i <= xnum ; i++ )
		 {
			 Vertex vertex;

			 dectype theta = ( ( dectype ) i ) * 2 * math::util::KPI / xnum;
			 dectype phi   = ( ( dectype ) j ) * math::util::KPI / ynum - math::util::KPI / 2;
			 vertex.x  = radius * ( dectype )math::CMathUtil::Cos( phi ) * ( dectype )math::CMathUtil::Cos( theta );
			 vertex.z  = radius * ( dectype )math::CMathUtil::Cos( phi ) * ( dectype )math::CMathUtil::Sin( theta );
			 vertex.y  = radius * ( dectype )math::CMathUtil::Sin( phi );
			 vertex.w  = 1.0f;
			 vertex.u  = ( dectype )i / xnum;
			 vertex.v  = 1.0f - ( dectype )j / ynum;
			 spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( vertex.x ,  vertex.y , vertex.z ) );
			 spVertexBuffer->GetUvStream()->WriteUv( idx , math::CBasicVector2( vertex.u , vertex.v ) );
			 idx++;
		 }
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���Q�A�v���~�e�B�u���P�ŏ�����
	spIndexBuffer->Initialize( indexNum );

	idx = 0;

	//�C���f�b�N�X���
	for ( vrtxtype j = 0 ; j < ynum ; j++ )
	{
		for ( vrtxtype i = 0 ; i < xnum ; i++)
		{
			spIndexBuffer->SetIndex( idx++ , ( j + 0 ) * ( xnum + 1 ) + i + 0 );
			spIndexBuffer->SetIndex( idx++ , ( j + 1 ) * ( xnum + 1 ) + i + 0 );
			spIndexBuffer->SetIndex( idx++ , ( j + 0 ) * ( xnum + 1 ) + i + 1 );
			spIndexBuffer->SetIndex( idx++ , ( j + 0 ) * ( xnum + 1 ) + i + 1 );
			spIndexBuffer->SetIndex( idx++ , ( j + 1 ) * ( xnum + 1 ) + i + 0 );
			spIndexBuffer->SetIndex( idx++ , ( j + 1 ) * ( xnum + 1 ) + i + 1 );
		}
	}

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype size , dectype uvrelnum , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		Vertex vertex;

		vertex.x  = ( i == 0 || i == 1 )? ( -size ) : ( +size );
		vertex.z  = ( i == 0 || i == 2 )? ( -size ) : ( +size );
		vertex.y  = 0.0f;
		vertex.w  = 1.0f;
		vertex.u  = ( i == 0 || i == 1 )? 0 : uvrelnum;
		vertex.v  = ( i == 0 || i == 2 )? 0 : uvrelnum;
		spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( vertex.x ,  vertex.y , vertex.z ) );
		spVertexBuffer->GetUvStream()->WriteUv( idx , math::CBasicVector2( vertex.u , vertex.v ) );
		idx++;
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���Q�ŏ�����
	spIndexBuffer->Initialize( 6 );

	idx = 0; 

	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx   , 2 );

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateCylinderRevers( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype radius , dectype height , vrtxtype ypolnum , vrtxtype circlepolnum , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = ( ypolnum + 1 ) * ( circlepolnum + 1 ); //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

    // ���_���Z�b�g�A�b�v
    for ( vrtxtype i = 0; i <= circlepolnum; i++)
	{
		Vertex vertex;

        f32 theta = ( 2 * ( f32 )math::util::KPI * ( f32 ) i ) / ( f32 ) circlepolnum;
        for ( u32 j = 0; j <= ypolnum; j++)
		{
            vertex.x = radius * ( dectype )math::CMathUtil::Cos( theta );
            vertex.z = radius * ( dectype )math::CMathUtil::Sin( theta );
            vertex.y = height * ( ( dectype )j / ( dectype )ypolnum - 0.0f );
            vertex.w   = 1.0f;
            vertex.u = ( dectype )i / ( dectype )circlepolnum;
            vertex.v = 1.0f-( dectype )j / ( dectype )ypolnum;
			spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( vertex.x ,  vertex.y , vertex.z ) );
			spVertexBuffer->GetUvStream()->WriteUv( idx , math::CBasicVector2( vertex.u , vertex.v ) );
			idx++;
        }
    }

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���Q�A�v���~�e�B�u���P�ŏ�����
	spIndexBuffer->Initialize( circlepolnum * ypolnum );

	idx = 0;

    for ( vrtxtype i = 0; i < circlepolnum; i++)
	{
        for ( vrtxtype j = 0; j < ypolnum; j++)
		{
			spIndexBuffer->SetIndex( idx++ , j + 0 + ( i + 0 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 0 + ( i + 1 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 1 + ( i + 0 ) * ( ypolnum + 1 ) );

			spIndexBuffer->SetIndex( idx++ , j + 1 + ( i + 0 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 0 + ( i + 1 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 1 + ( i + 1 ) * ( ypolnum + 1 ) );
        }
    }

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateCube( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &scale , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //���_��

	//���_�͂W�ŏ�����
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );

	dectype oneX = 0.5f * scale.X;
	dectype oneY = 0.5f * scale.Y;
	dectype oneZ = 0.5f * scale.Z;

	//���_�̍��W��ݒ�
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX , -oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY ,  oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY ,  oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY ,  oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx   , math::CBasicVector3( -oneX , -oneY ,  oneZ ) );

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���P�Q�ŏ�����
	spIndexBuffer->Initialize( 36 );

	idx = 0; 

	//�C���f�b�N�X�o�b�t�@��ݒ肷��
	spIndexBuffer->SetIndex( idx++ , 0 ); //1
	spIndexBuffer->SetIndex( idx++ , 1 ); //2
	spIndexBuffer->SetIndex( idx++ , 2 ); //3
	spIndexBuffer->SetIndex( idx++ , 0 ); //4
	spIndexBuffer->SetIndex( idx++ , 2 ); //5
	spIndexBuffer->SetIndex( idx++ , 3 ); //6
	spIndexBuffer->SetIndex( idx++ , 1 ); //7
	spIndexBuffer->SetIndex( idx++ , 5 ); //8
	spIndexBuffer->SetIndex( idx++ , 6 ); //9
	spIndexBuffer->SetIndex( idx++ , 1 ); //10
	spIndexBuffer->SetIndex( idx++ , 6 ); //11
	spIndexBuffer->SetIndex( idx++ , 2 ); //12
	spIndexBuffer->SetIndex( idx++ , 5 ); //13
	spIndexBuffer->SetIndex( idx++ , 4 ); //14
	spIndexBuffer->SetIndex( idx++ , 7 ); //15
	spIndexBuffer->SetIndex( idx++ , 5 ); //16
	spIndexBuffer->SetIndex( idx++ , 7 ); //17
	spIndexBuffer->SetIndex( idx++ , 6 ); //18
	spIndexBuffer->SetIndex( idx++ , 4 ); //19
	spIndexBuffer->SetIndex( idx++ , 0 ); //20
	spIndexBuffer->SetIndex( idx++ , 3 ); //21
	spIndexBuffer->SetIndex( idx++ , 4 ); //22
	spIndexBuffer->SetIndex( idx++ , 3 ); //23
	spIndexBuffer->SetIndex( idx++ , 7 ); //24
	spIndexBuffer->SetIndex( idx++ , 4 ); //25
	spIndexBuffer->SetIndex( idx++ , 5 ); //26
	spIndexBuffer->SetIndex( idx++ , 1 ); //27
	spIndexBuffer->SetIndex( idx++ , 4 ); //28
	spIndexBuffer->SetIndex( idx++ , 1 ); //29
	spIndexBuffer->SetIndex( idx++ , 0 ); //30
	spIndexBuffer->SetIndex( idx++ , 3 ); //31
	spIndexBuffer->SetIndex( idx++ , 2 ); //32
	spIndexBuffer->SetIndex( idx++ , 6 ); //33
	spIndexBuffer->SetIndex( idx++ , 3 ); //34
	spIndexBuffer->SetIndex( idx++ , 6 ); //35
	spIndexBuffer->SetIndex( idx   , 7 ); //36

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

	dectype oneX = 0.5f * scale.X;
	dectype oneY = 0.5f * scale.Y;

	//���_�̍��W��ݒ�
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY , 0 ) ); //����
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY , 0 ) ); //�E��
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX , -oneY , 0 ) ); //����
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY , 0 ) ); //�E��

	//�t�u���W��ݒ�
	idx = 0; 
#ifdef SKYLIB_RENDER_OPENGL4
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  1.0f ) ); //����
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  1.0f ) ); //�E��
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  0.0f ) ); //����
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  0.0f ) ); //�E��
#else
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  0.0f ) ); //����
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  0.0f ) ); //�E��
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  1.0f ) ); //����
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  1.0f ) ); //�E��
#endif

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���Q�ŏ�����
	spIndexBuffer->Initialize( 6 );

	idx = 0; 

	//�C���f�b�N�X�o�b�t�@��ݒ肷��
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype width , dectype height , const math::CBasicVector2 &uvLt , const math::CBasicVector2 &uvRt , const math::CBasicVector2 &uvLb , const math::CBasicVector2 &uvRb )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateUvStream( vertexNum );

	dectype oneX = width / 2;
	dectype oneY = height / 2;

	//���_�̍��W��ݒ�
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY , 0 ) ); //����
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY , 0 ) ); //�E��
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX , -oneY , 0 ) ); //����
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY , 0 ) ); //�E��

	//�t�u���W��ݒ�
	idx = 0; 
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvLt ); //����
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvRt ); //�E��
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvLb ); //����
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvRb ); //�E��

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���Q�ŏ�����
	spIndexBuffer->Initialize( 6 );

	idx = 0; 

	//�C���f�b�N�X�o�b�t�@��ݒ肷��
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateSphere( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , dectype sc , vrtxtype divNum, const CColor &color )
{
	SKY_UNUSED_ARG( spIndexBuffer );

	vrtxtype idx = 0; 
	vrtxtype vertexNum = divNum * 3 * 2; //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( vertexNum );

	dectype		fStep = math::util::KPI / divNum * 2.0f;
		
	// XY����
	for ( vrtxtype i = 0; i < divNum; ++i )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep ), math::CMathUtil::Cos( i * fStep ), 0.0f ) * 0.5f * sc );
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep + fStep ), math::CMathUtil::Cos( i * fStep + fStep ), 0.0f ) * 0.5f * sc );
	}
	// XZ����
	for ( vrtxtype i = 0; i < divNum; ++i ) {
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep ), 0.0f, math::CMathUtil::Cos( i * fStep ) ) * 0.5f * sc );
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep + fStep ), 0.0f, math::CMathUtil::Cos( i * fStep + fStep ) ) * 0.5f * sc );
	}
	// YZ����
	for ( vrtxtype i = 0; i < divNum; ++i ) {
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( 0.0f, math::CMathUtil::Cos( i * fStep ), math::CMathUtil::Sin( i * fStep ) ) * 0.5f * sc );
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( 0.0f, math::CMathUtil::Cos( i * fStep + fStep ), math::CMathUtil::Sin( i * fStep + fStep ) ) * 0.5f * sc );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::ComputeEntity3D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity3DPtr &entity3d )
{
	math::CMatrix4x3 trans , rot , sc , w;

	//�g��k��
	sc.SetupScale( entity3d->GetSc() );

	//��]
	entity3d->GetRot().ToMatrixYXZ( rot );

	//�ړ�
	trans.SetupTranslation( entity3d->GetPos() );

	//�s�����������
	w = sc * rot * trans;

	//���_�Ƃ������킹��
	spVertexBuffer->ComputePosMatrix( w );
}

void CVertexUtil::ComputeEntity2D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity2DPtr &entity )
{
	ComputeEntity3D( spVertexBuffer , entity->Create3D() );
}

void CVertexUtil::CreateGrid_XZ( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	math::CBasicVector3Vector vector;

	dectype startIndex = -( dectype )large;

	//�c���̒��_��ݒ�
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex + i , 0 , large ) );
		vector.Add( math::CBasicVector3( startIndex + i , 0 , -( dectype )large ) );
	}

	//�����̒��_��ݒ�
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex , 0 , ( dectype )large - i ) );
		vector.Add( math::CBasicVector3( large , 0 , ( dectype )large - i ) );
	}

	//���_�͂Q�ŏ�����
	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		pos3 = vector[ i ];
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos3 );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );
	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateGrid_YX( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	math::CBasicVector3Vector vector;

	dectype startIndex = -( dectype )large;

	//�c���̒��_��ݒ�
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex + i , large , 0 ) );
		vector.Add( math::CBasicVector3( startIndex + i , -( dectype )large , 0 ) );
	}

	//�����̒��_��ݒ�
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex , ( dectype )large - i , 0 ) );
		vector.Add( math::CBasicVector3( large , ( dectype )large - i , 0 ) );
	}

	//���_�͂Q�ŏ�����
	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		pos3 = vector[ i ];
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos3 );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );
	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateGridFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale , const CColor &color )
{
	vrtxtype maxLevel = 6;
	vrtxtype bufSize = ( vrtxtype )math::CMathUtil::Pow( 2, maxLevel ) + 1;
	vrtxtype vertexNum = bufSize * bufSize; //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateNormalStream( vertexNum );
	spVertexBuffer->CreateColorStream( vertexNum );

    // ���_���Z�b�g�A�b�v
	for ( vrtxtype indexX = 0; indexX < bufSize; indexX++ )
	{
		for ( vrtxtype indexY = 0; indexY < bufSize; indexY++ )
		{
			//�ΏۃC���f�b�N�X
			vrtxtype idx =  indexY + ( indexX * bufSize );

			// ���W�Ɩ@���ƐF�Z�b�g�A�b�v
			spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( indexX * scale.X , 0 , indexY * scale.Y ) );
			spVertexBuffer->GetNormalStream()->WriteNormal( idx , math::CBasicVector3( indexY , 1.0f , indexY ) );
			spVertexBuffer->GetColorStream()->WriteColor( idx , color );
		}
	}

	//���_�Q�̒��S�����_�ɂȂ�悤�ɂ���
	spVertexBuffer->TransPosCenter();

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���Q�A�v���~�e�B�u���P�ŏ�����
	spIndexBuffer->Initialize( bufSize * bufSize );

    // �C���f�b�N�X���Z�b�g�A�b�v
	for ( vrtxtype indexY = 1; indexY < bufSize - 1; indexY++ )
	{
		for ( vrtxtype indexX = 1; indexX < bufSize - 1; indexX++)
		{
			vrtxtype idx = 6 * ( ( indexX - 1 ) + ( ( indexY - 1 ) *	( bufSize ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 1 ) * bufSize + ( indexX - 1 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 0 ) * bufSize + ( indexX - 1 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 1 ) * bufSize + ( indexX - 0 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 1 ) * bufSize + ( indexX - 0 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 0 ) * bufSize + ( indexX - 1 ) ) );
			spIndexBuffer->SetIndex( idx   , ( ( indexY - 0 ) * bufSize + ( indexX - 0 ) ) );
		}
	}

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );
}

CVertexBufferPtr CVertexUtil::CreateVertexBuffer( vrtxtype vertexNum , vrtxtype indexNum , vertex::ePrimitiveType ePrimType , const CIndexBufferPtr &spIndexBuffer )
{
	//���_�o�b�t�@�̍쐬
	graphic::CVertexBufferPtr spVertex = CVertexBufferCreate_();

	//���_������
	spVertex->Initialize( vertexNum , ePrimType );

	if ( spIndexBuffer.IsEmpty() == skyFalse )
	{
		//�C���f�b�N�X������
		spIndexBuffer->Initialize( indexNum );
	}

	//�ԋp
	return spVertex;
}

#ifdef SKYLIB_RENDER_DIRECTX11

/// <summary>
/// �v���~�e�B�u�^�e�[�u��
/// </summary>
static D3D11_PRIMITIVE_TOPOLOGY s_TypeTable[] =
{
	{ D3D11_PRIMITIVE_TOPOLOGY_POINTLIST     } , // vertex::ePrimitiveType_Pointlist
	{ D3D11_PRIMITIVE_TOPOLOGY_LINELIST      } , // vertex::ePrimitiveType_Linelist
	{ D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP     } , // vertex::ePrimitiveType_Linestrip
	{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST  } , // vertex::ePrimitiveType_Trianglelist
	{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } , // vertex::ePrimitiveType_Meshtrip
	{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } , // vertex::ePrimitiveType_Trianglefan //D3D11�ł͌�������Ȃ��B
};
static s32 s_TypeTable_size = array_sizeof( s_TypeTable );

static D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType( vertex::ePrimitiveType eType )
{
	SKY_ASSERT_MSG( s_TypeTable_size <= ( s32 )s_TypeTable || 0 > ( s32 )s_TypeTable , _T( "The value of the primitive type is illegal." ) ); //�v���~�e�B�u�^�̒l���s���ł��B

	return s_TypeTable[ eType ];
}

void CVertexUtil::SetRenderBufferStream( const CVertexBufferStreamPtrList &list , const CIndexBufferPtr &spIndexBuffer , vertex::ePrimitiveType ePrimType )
{
	ID3D11Buffer* pBufferTbl [ eVertexBufferStreamType_Max ] = { 0 };
	UINT SizeTbl             [ eVertexBufferStreamType_Max ] = { 0 };
	UINT OffsetTbl           [ eVertexBufferStreamType_Max ] = { 0 };
	u32 setNum = 0;
	FOREACH( CVertexBufferStreamPtrList , it , list )
	{//�����Ńn���O����͕̂K�v�ȃX�g���[�����p�ӂ���Ă��Ȃ����f�����A�p�ӂ���Ă��Ă�GPU���ɂ͂܂��R�s�[���Ă��Ȃ����B�܂��̓X�v���C�g���`�悳��悤�Ƃ����Ƃ�
		pBufferTbl[ setNum ] = ( *it )->GetMember()->m_spGpuMemoryBuffer.GetPtrNoConst();
		SizeTbl   [ setNum ] = GetVertexBufferStreamTypeSize( ( eVertexBufferStreamType )( *it )->ID );
		OffsetTbl [ setNum ] = 0; //�擪���炿���Ǝg��
		setNum++;
	}

	//���_�o�b�t�@�̐ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->IASetVertexBuffers( 0 , setNum , pBufferTbl , SizeTbl , OffsetTbl );

	if ( spIndexBuffer.IsEmpty() == skyFalse )
	{
		//�C���f�b�N�X�o�b�t�@�̐ݒ�
		spIndexBuffer->SetGpuMemoryStream();
	}

	//�v���~�e�B�u�̃^�C�v������
	CDevice_->GetMember()->m_spD3DDeviceContext->IASetPrimitiveTopology( GetPrimitiveType( ePrimType ) );
}

#endif

#ifdef SKYLIB_RENDER_OPENGL4

void CVertexUtil::SetRenderBufferStream( const CVertexBufferStreamPtrList &list , const CIndexBufferPtr &spIndexBuffer , vertex::ePrimitiveType ePrimType )
{
	SKY_UNUSED_ARG( ePrimType );
	FOREACH( CVertexBufferStreamPtrList , it , list )
	{//�����Ńn���O����͕̂K�v�ȃX�g���[�����p�ӂ���Ă��Ȃ����f���܂��̓X�v���C�g���`�悳��悤�Ƃ����Ƃ�
		glBindBuffer( GL_ARRAY_BUFFER, ( *it )->GetMember()->m_hBuffer );
		glVertexAttribPointer( ( *it )->GetMember()->m_Location , GetVertexBufferStreamTypeSize( ( eVertexBufferStreamType )( *it )->ID ) / sizeof( f32 ) , GL_FLOAT , GL_FALSE , 0 , 0 );
		glEnableVertexAttribArray( ( *it )->GetMember()->m_Location );
	}

	if ( spIndexBuffer.IsEmpty() == skyFalse )
	{
		//�C���f�b�N�X�o�b�t�@�̐ݒ�
		spIndexBuffer->SetGpuMemoryStream();
	}
}

#endif

void CVertexUtil::CreateCubeLine( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	SKY_ASSERT_MSG( vector.GetNum() == 8 , _T( "Please set 8 position. \n" ) );

	vrtxtype vertexNum = ( vrtxtype )vector.GetNum(); //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );

	//���_�ɍ��W����������
	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , vector[ i ] );
	}

	math::CMatrix4x3 scMat;

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���Q�ŏ�����
	spIndexBuffer->Initialize( 24 );

	vrtxtype idx = 0; 

	//�C���f�b�N�X�o�b�t�@��ݒ肷��
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 4 );
	spIndexBuffer->SetIndex( idx++ , 5 );
	spIndexBuffer->SetIndex( idx++ , 5 );
	spIndexBuffer->SetIndex( idx++ , 6 );
	spIndexBuffer->SetIndex( idx++ , 6 );
	spIndexBuffer->SetIndex( idx++ , 7 );
	spIndexBuffer->SetIndex( idx++ , 7 );
	spIndexBuffer->SetIndex( idx++ , 4 );
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 4 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 5 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 6 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 7 );

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}
}

void CVertexUtil::CreatePyramid( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	SKY_ASSERT_MSG( vector.GetNum() == 5 , _T( "Please set 5 position. \n" ) );

	vrtxtype vertexNum = ( vrtxtype )vector.GetNum(); //���_��

	//���_������
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );

	//���_�ɍ��W����������
	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , vector[ i ] );
	}

	math::CMatrix4x3 scMat;

	//�C���f�b�N�X���P�v���~�e�B�u�C���f�b�N�X���R�A�v���~�e�B�u���Q�ŏ�����
	spIndexBuffer->Initialize( 24 );

	vrtxtype idx = 0; 

	//�C���f�b�N�X�o�b�t�@��ݒ肷��
	spIndexBuffer->SetIndex( idx++ , 0 ); 
	spIndexBuffer->SetIndex( idx++ , 1 ); 
	spIndexBuffer->SetIndex( idx++ , 1 ); 
	spIndexBuffer->SetIndex( idx++ , 2 ); 
	spIndexBuffer->SetIndex( idx++ , 2 ); 
	spIndexBuffer->SetIndex( idx++ , 3 ); 
	spIndexBuffer->SetIndex( idx++ , 3 ); 
	spIndexBuffer->SetIndex( idx++ , 0 ); 
	spIndexBuffer->SetIndex( idx++ , 0 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 1 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 2 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 3 ); 

	//�O�p�`�W�����ɃC���f�b�N�X�o�b�t�@��ݒ肷��
	spMesh->AttachIndexBuffer( spIndexBuffer );

	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}
}

void CVertexUtil::Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color , const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh )
{
	SKY_ASSERT( pos.GetNum() == color.GetNum() && pos.GetNum() == uv.GetNum() );

	//���_������
	spVertexBuffer->Initialize( ( vrtxtype )pos.GetNum() , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( ( vrtxtype )color.GetNum() );

	//���_�ɍ��W����������
	for ( vrtxtype i = 0 ; i < pos.GetNum() ; i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos[ i ] );
	}

	//�C���f�b�N�X�o�b�t�@�쐬
	spIndexBuffer->Initialize( ( vrtxtype )index.GetNum() );

	for ( vrtxtype i = 0 ; i < index.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , index[ i ] ); 
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );

	//UV�̐ݒ�
	spVertexBuffer->CreateUvStream( ( vrtxtype )pos.GetNum() );

	for ( vrtxtype i = 0 ; i < uv.GetNum() ; i++ )
	{
		spVertexBuffer->GetUvStream()->WriteUv( i , uv[ i ] );
	}

	//���_�̃J���[�l��ݒ�
	for ( vrtxtype i = 0 ; i < color.GetNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color[ i ] );
	}

	//���_�̖@�����v�Z����
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

} } }