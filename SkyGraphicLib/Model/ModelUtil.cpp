#include "StdAfx.h"
#include "SkyGraphicLib/Model.h"

#include "SkyGraphicLib/Loader.h"
#include "SkyMathLib/Graph.h"

namespace sky { namespace lib { namespace graphic {

CModelPtr CModelUtil::CreateModelFromMesh( const CMeshContenaPtr &spMesh , CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CShaderObjectPtr &effectPtr )
{
	//���_�J���[��ݒ肷��
	spMesh->GetMesh()->GetVertexBuffer()->CreateColorStream( spMesh->GetMesh()->GetVertexBuffer()->GetVertexNum() );
	spMesh->GetMesh()->GetVertexBuffer()->GetColorStream()->WriteColorAll( color );

	//�����ʒu�A��]�A�X�P�[���𔽉f
	CVertexUtil::ComputeEntity3D( spMesh->GetMesh()->GetVertexBuffer() , math::CEntity3D::Create( pos , rot , sc ) );

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr model = CModelCreate_();
	model->AttachMeshContena( spMesh );
	model->AttachShader( effectPtr );

	//�Z�b�g�A�b�v
	model->ActivateTraverse();

	return model;
}

CModelPtr CModelUtil::CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//���b�V���̍쐬
	CMeshContenaPtr spMeshContena = CWavefrontOBJLoader_::Load( pPath , spVertex , spIndex );

	//�����ʒu�A��]�A�X�P�[���𔽉f
	CVertexUtil::ComputeEntity3D( spMeshContena->GetMesh()->GetVertexBuffer() , math::CEntity3D::Create( pos , rot , sc ) );

	//�e�X�g�p��z�肵�Ă�̂ł�����GPU���������m�ۂ��Ă��܂��BCPU����������GPU�������̏������݂��s���B
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr model = CModelCreate_();
	model->AttachMeshContena( spMeshContena );
	model->AttachShader( effectPtr );

	//�Z�b�g�A�b�v
	model->ActivateTraverse();

	//�}�e���A���ɐF����ݒ肷��
	model->SetDiffuse( color );

	return model;
}

#define _CUBE_PATH _T( "testcube.obj" )
CModelPtr  CModelUtil::CreateTestCube( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _CUBE_PATH ) , color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

#define _CONE_PATH _T( "testcone.obj" )
CModelPtr  CModelUtil::CreateTestCone( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _CONE_PATH ), color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

#define _PLANE_PATH _T( "testplane10x10.obj" )
CModelPtr  CModelUtil::CreateTestPlane( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _PLANE_PATH ), color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

#define _SPHERE_PATH _T( "testsphere.obj" )
CModelPtr  CModelUtil::CreateTestSphere( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _SPHERE_PATH ), color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

CModelPtr CModelUtil::CreatePoint( const math::CBasicVector3 &point , CColor color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreatePoint( point , spVertex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	return spModel;
}

CModelPtr CModelUtil::CreateLine3D( const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr  )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateLine3D( spVertex , vector , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateLine3D( const math::CBasicLine3List &lineList , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateLine3D( spVertex , lineList , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateStandardLine3( dectype length )
{
	//�e�m�[�h���쐬
	CModelPtr spResult = CModelCreate_(); //�q���������̎��g�͕`�悵�Ȃ��󃂃f��
	spResult->ActivateTraverse();

	//X���̍쐬
	math::CBasicVector3Vector vectorX;
	vectorX.Add( math::CBasicVector3( 0 , 0 , 0 ) );
	vectorX.Add( math::CBasicVector3( length , 0 , 0 ) );
	spResult->AttachChild( CreateLine3D( vectorX ) );
	spResult->AttachChild( graphic::CModelUtil::CreateTestCone( CColor_Red , math::CBasicVector3( 0.3f , 0.3f , 0.3f ) , math::CBasicAngleVector3( 2 , 0.0f , 0.0f , 90.0f ) , math::CBasicVector3( length , 0.0f , 0.0f ) , CVertexBuffer::Create() , CIndexBuffer::Create() , CMaterialDiffuseColorShaderObject::Create() ) );

	//Y���̍쐬
	math::CBasicVector3Vector vectorY;
	vectorY.Add( math::CBasicVector3( 0 , 0 , 0 ) );
	vectorY.Add( math::CBasicVector3( 0 , length , 0 ) );
	spResult->AttachChild( CreateLine3D( vectorY , CColor_Blue ) );
	spResult->AttachChild( graphic::CModelUtil::CreateTestCone( CColor_Blue , math::CBasicVector3( 0.3f , 0.3f , 0.3f ) , math::CBasicAngleVector3( 2 , 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , length , 0.0f ) , CVertexBuffer::Create() , CIndexBuffer::Create() , CMaterialDiffuseColorShaderObject::Create() ) );

	//Z���̍쐬
	math::CBasicVector3Vector vectorZ;
	vectorZ.Add( math::CBasicVector3( 0 , 0 , 0 ) );
	vectorZ.Add( math::CBasicVector3( 0 , 0 , length ) );
	spResult->AttachChild( CreateLine3D( vectorZ , CColor_Green ) );
	spResult->AttachChild( graphic::CModelUtil::CreateTestCone( CColor_Green , math::CBasicVector3( 0.3f , 0.3f , 0.3f ) , math::CBasicAngleVector3( 2 , 90.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 0.0f , length ) , CVertexBuffer::Create() , CIndexBuffer::Create() , CMaterialDiffuseColorShaderObject::Create() ) );

	return spResult;
}

CModelPtr CModelUtil::CreateGrid_XZ( dectype large , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateGrid_XZ( large , spVertex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateGrid_YX( dectype large , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateGrid_YX( large , spVertex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateSquare( const math::CBasicVector2 &scale , const CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@���g�p���ė����̂̒��_�����쐬����B
	CVertexUtil::CreateSquare( spVertex , spIndex , spMesh , scale , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//�e�X�g�p��z�肵�Ă�̂ł�����GPU���������m�ۂ��Ă��܂��BCPU����������GPU�������̏������݂��s���B
	spMeshContena->ActivateTraverse();

	//�X�v���C�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateCircle_XZ( const math::CBasicCircle& circle , const CColor &color , u32 granularity )
{
	math::CBasicVector3 polar;
	math::CBasicVector3 rectangular;

	//���s�ړ��̍s����쐬
	math::CMatrix4x3 mat;
	mat.SetupTranslation( math::CBasicVector3( circle.X() , 0 , circle.Y() ) );

	//XZ����
	math::CBasicVector3Vector vectorXZ;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = circle.GetRadius();
		polar.Y = 0;
		polar.Z = ( dectype )( math::CMathUtil::DegToRad( ( ( dectype )( 360 / granularity ) ) * index ) );
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorXZ.Add( rectangular * mat );
	}
	return CreateLine3D( vectorXZ , color );
}

CModelPtr CModelUtil::CreateLine3DCube( const math::CMatrix4x3 &trans , const CColor &color )
{
	math::CBasicVector3Vector vec;
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, +1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, -1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, -1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, +1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, +1.0f, +1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, -1.0f, +1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, -1.0f, +1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, +1.0f, +1.0f ) , trans ) );

	return CreateLine3DCube( vec , color );
}

CModelPtr CModelUtil::CreateFrustum( const math::CBasicVector3 &pos , const CameraPtr &spCamera , const CColor &color )
{
	math::CMatrix4x3 trans;
	trans.SetupTranslation( pos );
	trans = MulMatrix4x4( trans , Invert4x4( spCamera->GetCameraMatrix() ) );
	return CreateLine3DCube( trans , color );
}

CModelPtr CModelUtil::CreateLine3DCube( const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateCubeLine( spVertex , vector , spIndex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	//�}�e���A���ɐF����ݒ肷��
	spModel->SetDiffuse( color );

	return spModel;
}

CModelPtr CModelUtil::CreateLinePyramid( const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreatePyramid( spVertex , vector , spIndex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	//�}�e���A���ɐF����ݒ肷��
	spModel->SetDiffuse( color );

	return spModel;
}

CModelPtr CModelUtil::CreateLinePyramid( const math::CMatrix4x3 &trans , const CColor &color )
{
	math::CBasicVector3Vector vec;
	vec.Add( MulMatrix4x4( math::CBasicVector3( -0.5f, -0.0f, +0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +0.5f, -0.0f, +0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +0.5f, -0.0f, -0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -0.5f, -0.0f, -0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -0.0f, +1.0f, +0.0f ) , trans ) );

	return CreateLinePyramid( vec , color );
}

//�ɍ��W (r,��,��) ���ar�AXY���ʉ�]�p�x�ƁA�p��
CModelPtr CModelUtil::CreateLine3DSphere( const math::CBasicSphere &sphere , const CColor &color , u32 granularity )
{
	//�e�m�[�h���쐬
	CModelPtr spResult = CModelCreate_(); //�q���������̎��g�͕`�悵�Ȃ��󃂃f��
	spResult->ActivateTraverse();

	math::CBasicVector3 polar;
	math::CBasicVector3 rectangular;

	//���s�ړ��̍s����쐬
	math::CMatrix4x3 mat;
	mat.SetupTranslation( sphere.GetPos() );

	//XY����
	math::CBasicVector3Vector vectorXY;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = sphere.GetRadius();
		polar.Y = ( dectype )( math::CMathUtil::DegToRad( ( dectype )( ( 360 / granularity ) * index ) ) );
		polar.Z = 0;
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorXY.Add( rectangular * mat );
	}
	spResult->AttachChild( CreateLine3D( vectorXY , color ) );

	//XZ����
	math::CBasicVector3Vector vectorXZ;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = sphere.GetRadius();
		polar.Y = 0;
		polar.Z = ( dectype )( math::CMathUtil::DegToRad( ( dectype )( ( 360 / granularity ) * index ) ) );
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorXZ.Add( rectangular * mat );
	}
	spResult->AttachChild( CreateLine3D( vectorXZ , color ) );

	//YZ����
	math::CBasicVector3Vector vectorYZ;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = sphere.GetRadius();
		polar.Y = math::CMathUtil::DegToRad( 90 );
		polar.Z = ( dectype )( math::CMathUtil::DegToRad( ( dectype )( ( 360 / granularity ) * index ) ) );
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorYZ.Add( rectangular * mat );
	}
	spResult->AttachChild( CreateLine3D( vectorYZ , color ) );

	return spResult;
}

CModelPtr CModelUtil::CreateAABBDisp( const math::CAABB3 &aabb , const CColor &color )
{
	SKY_ASSERT( aabb.IsZero() == skyFalse );

	math::CBasicVector3Vector vec;
	vec.Add( aabb.Corner( 2 ) );
	vec.Add( aabb.Corner( 3 ) );
	vec.Add( aabb.Corner( 1 ) );
	vec.Add( aabb.Corner( 0 ) );
	vec.Add( aabb.Corner( 6 ) );
	vec.Add( aabb.Corner( 7 ) );
	vec.Add( aabb.Corner( 5 ) );
	vec.Add( aabb.Corner( 4 ) );

	CModelPtr spResult = CreateLine3DCube( vec , color );
	spResult->SetLockParentRotAndScale( skyTrue );

	return spResult;
}

CModelPtr CModelUtil::CreateOBBDisp( const math::COBB3 &obb , const CColor &color )
{
	SKY_ASSERT( obb.IsZero() == skyFalse );

	math::CBasicVector3Vector vec;
	vec.Add( obb.GetCorner( 5 ) );
	vec.Add( obb.GetCorner( 4 ) );
	vec.Add( obb.GetCorner( 7 ) );
	vec.Add( obb.GetCorner( 6 ) );
	vec.Add( obb.GetCorner( 1 ) );
	vec.Add( obb.GetCorner( 2 ) );
	vec.Add( obb.GetCorner( 3 ) );
	vec.Add( obb.GetCorner( 0 ) );

	return CreateLine3DCube( vec , color );
}

CModelPtr CModelUtil::Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	mat->AttachDiffuseMap( texPtr );
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//���_�o�b�t�@�쐬
	CVertexUtil::Create( pos , index , uv , color ,  spVertex , spIndex , spMesh );

	//�}�e���A���ɕ`�撸�_����ʒm
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	return spModel;
}

} } }