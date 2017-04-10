#include "StdAfx.h"
#include "SkyGraphicLib/Sprite.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Loader.h"

namespace sky { namespace lib { namespace graphic {

CSpritePtr CSpriteUtil::CreateLine2D( const math::CBasicVector2Vector &vector , const CColor &diffuseColor , dectype prio , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	SKY_ASSERT_MSG( vector.GetNum() > 0 , _T( "num is over 0. \n" ) );

	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateLine2D( spVertex , vector , spMesh , diffuseColor );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//�X�v���C�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );

	return sprite;

}

CSpritePtr CSpriteUtil::CreateSquareLine2D( dectype width , dectype height , const CColor &diffuseColor ,  dectype prio , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateSquareLine2D( spVertex , spMesh , width , height , diffuseColor );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//�e�X�g�p��z�肵�Ă�̂ł�����GPU���������m�ۂ��Ă��܂��BCPU����������GPU�������̏������݂��s���B
	spMeshContena->ActivateTraverse();

	//�X�v���C�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}

CSpritePtr CSpriteUtil::CreateSquare(  const math::CBasicVector2 &scale , const CColor &diffuseColor , dectype prio , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@���g�p���ė����̂̒��_�����쐬����B
	CVertexUtil::CreateSquare( spVertex , spIndex , spMesh , scale , diffuseColor );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//�e�X�g�p��z�肵�Ă�̂ł�����GPU���������m�ۂ��Ă��܂��BCPU����������GPU�������̏������݂��s���B
	spMeshContena->ActivateTraverse();

	//�X�v���C�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}


CSpritePtr CSpriteUtil::CreateSquare( const CTexturePtr &texPtr , dectype prio , const math::CBasicVector2 &scale , const CColor &diffuseColor , const math::CBasicVector2 &offset , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateSquare( texPtr , math::CBasicVector2( ( dectype )texPtr->GetInfo().Width , ( dectype )texPtr->GetInfo().Height ) , prio , math::CBasicVector2( scale.X , scale.Y ) , diffuseColor , offset , spVertex , spIndex , effectPtr );
}

CSpritePtr CSpriteUtil::CreateSquare( const CTexturePtr &texPtr , const math::CBasicVector2 &size , dectype prio , const math::CBasicVector2 &scale , const CColor &diffuseColor , const math::CBasicVector2 &offset , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	mat->AttachDiffuseMap( texPtr );
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@���g�p���Ďl�p�`�̒��_�����쐬����B
	CVertexUtil::CreateSquare( spVertex , spIndex , spMesh , math::CBasicVector2( size.X * scale.X , size.Y * scale.Y ) , diffuseColor );

	//�����ʒu��ύX
	CVertexUtil::ComputeEntity3D( spVertex , math::CEntity3D::Create( math::CBasicVector3( offset.X , offset.Y , 0.0f ) , math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) ,  math::CBasicVector3( 1.0f , 1.0f , 1.0f ) ) );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//�e�X�g�p��z�肵�Ă�̂ł�����GPU���������m�ۂ��Ă��܂��BCPU����������GPU�������̏������݂��s���B
	spMeshContena->ActivateTraverse();

	//�X�v���C�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}

CSpritePtr CSpriteUtil::CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , dectype prio , CColor color , math::CBasicVector2 pos , dectype rot , math::CBasicVector2 sc , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//���b�V���̍쐬
	CMeshContenaPtr spMeshContena = CWavefrontOBJLoader_::Load( pPath , spVertex , spIndex );

	//�����ʒu�A��]�A�X�P�[���𔽉f
	CVertexUtil::ComputeEntity2D( spMeshContena->GetMesh()->GetVertexBuffer() , math::CEntity2D::Create( pos , rot , sc ) );

	//�e�X�g�p��z�肵�Ă�̂ł�����GPU���������m�ۂ��Ă��܂��BCPU����������GPU�������̏������݂��s���B
	spMeshContena->ActivateTraverse();

	//���f���̍쐬�^�p�����[�^�[�ݒ�^������
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );

	//�}�e���A���J���[��ݒ肷��
	sprite->SetDiffuse( color );

	return sprite;
}

CSpritePtr CSpriteUtil::CreateStandardLine2( dectype length , dectype prio )
{
	//�e�m�[�h���쐬
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );

	//X���̍쐬
	math::CBasicVector2Vector vectorX;
	vectorX.Add( math::CBasicVector2( 0 , 0 ) );
	vectorX.Add( math::CBasicVector2( length , 0 ) );
	sprite->AttachChild( CreateLine2D( vectorX ) );

	//Y���̍쐬
	math::CBasicVector2Vector vectorY;
	vectorY.Add( math::CBasicVector2( 0 , 0 ) );
	vectorY.Add( math::CBasicVector2( 0 , length ) );
	sprite->AttachChild( CreateLine2D( vectorY , CColor_Blue , prio ) );

	return sprite;
}

#define _CIRCLE_PATH _T( "testcircle.obj" )
CSpritePtr CSpriteUtil::CreateTestCircle( CColor color , math::CBasicVector2 pos , dectype rot , math::CBasicVector2 sc , dectype prio , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _CIRCLE_PATH ) , prio , color , pos , rot , sc , spVertex , spIndex , effectPtr );
}

CSpritePtr CSpriteUtil::Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , dectype prio , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//�}�e���A���̍쐬
	CMaterialPtr mat = CMaterialCreate_();
	mat->AttachDiffuseMap( texPtr );
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//���_�o�b�t�@�쐬
	CVertexUtil::Create( pos , index , uv , color ,  spVertex , spIndex , spMesh );

	//�`�撸�_����ʒm
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//���b�V���̍쐬
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//�X�v���C�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}

} } }