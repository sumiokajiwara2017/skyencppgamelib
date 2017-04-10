#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"
#include "CgLightShaderObject.h"

#ifdef SKY_GRAPHIC_LIB_OPENGL

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

struct CCgLightShaderObjectMember
{
	CCgCombineProgram Shader;

    CGparameter World;                  // ワールド変換行列
    CGparameter View;                   // ビュー変換行列
    CGparameter Projection;             // 透視変換行列
    CGparameter Diffuse;                // ディフューズ色
    CGparameter Light;                  // 光源座標(ビュー座標系)

    CGparameter texCoord0Location;
};

static CCgCombineProgram s_Shader;
static skyBool    s_IsLoad = skyFalse;

void CCgLightShaderObject::Load( CCgLightShaderObjectMember *dest )
{
	if ( s_IsLoad == skyFalse )
	{
	    CCg::CreateVertexFragmentCombineShaderFromFile( s_Shader , 
		                                            "data/test/cg/CCgLight_vertex.cg"   , "Cg_Main" ,
		                                            "data/test/cg/CCgLight_fragment.cg" , "Cg_Fragment_Main"  );

        dest->World = cgGetNamedParameter ( s_Shader.Program, "uniforms.World" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        dest->View = cgGetNamedParameter ( s_Shader.Program, "uniforms.View" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        dest->Projection = cgGetNamedParameter ( s_Shader.Program, "uniforms.Projection" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        dest->Diffuse = cgGetNamedParameter ( s_Shader.Program, "uniforms.Diffuse" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        dest->Light = cgGetNamedParameter ( s_Shader.Program, "uniforms.Light" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

        dest->texCoord0Location = cgGetNamedParameter( s_Shader.Program , "texCoord0Location" );//名前
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        
		s_IsLoad = skyTrue;
	}

	dest->Shader = s_Shader;
}

void CCgLightShaderObject::Destroy()
{
    cgDestroyProgram( s_Shader.Program );
}

CCgLightShaderObject::CCgLightShaderObject()
{
	m_pMember = NEW CCgLightShaderObjectMember;
}

CCgLightShaderObject::~CCgLightShaderObject()
{
	DEL m_pMember;
}

//グローバルパラメーター
static GLfloat World     [ 16 ];
static GLfloat View      [ 16 ];
static GLfloat Projection[ 16 ];
static GLfloat Diffuse   [ 4 ];
static GLfloat Light     [ 3 ]; 

void CCgLightShaderObject::vDraw()
{
	//シェーダーロード
	Load( m_pMember );
        
	//プロジェクション行列の設定
	m_spCamera->ComputeProjectionMatrix();
	sl_CopyMatrixf( Projection , m_spCamera->GetMember()->m_ProjectionMatrix );

	//視点座標変換
	m_spCamera->ComputeViewMatrix();
	sl_CopyMatrixf( View , m_spCamera->GetMember()->m_ViewMatrix );

	//ワールド行列設定
	sl_CopyMatrixf( World , m_WorldMatrix );

	//行列設定
	cgSetMatrixParameterfr( m_pMember->Projection , Projection );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
	cgSetMatrixParameterfr( m_pMember->View , View );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
	cgSetMatrixParameterfr( m_pMember->World , World );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

	// 点光源ベクトル計算設定
	GLfloat lightPos[ 3 ] = { 3.0f, 3.0f, 3.0f };
	sl_MultVectorMatrixf( Light , lightPos , m_spCamera->GetMember()->m_ViewMatrix );
    cgSetParameter3fv( m_pMember->Light , Light );

	const CGprofile profile0 = cgGetProgramDomainProfile( m_pMember->Shader.Program , 0);
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
	const CGprofile profile1 = cgGetProgramDomainProfile( m_pMember->Shader.Program , 1);
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );

	//頂点シェーダーを設定
	cgGLBindProgram( m_pMember->Shader.Program );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
    cgGLEnableProfile( profile0 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
    cgGLEnableProfile( profile1 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );

	// 頂点バッファ/インデックスバッファの設定
	m_spMesh->m_spVertexBuffer->GetMember()->m_VertexLocation = 0;
	m_spMesh->m_spVertexBuffer->GetMember()->m_NormalLocation = 1;
	m_spMesh->m_spVertexBuffer->GetMember()->m_TexCoord0Location = 2;
	m_spMesh->m_spVertexBuffer->SetGpuMemoryStream( vertex::VERTEX_FORMAT_BIT_XYZ | vertex::VERTEX_FORMAT_BIT_NORMAL | vertex::VERTEX_FORMAT_BIT_TEX0 );

	FOREACH( CTrianglesPtrList , it , m_spMesh->m_spGroupList )
	{
		//マテリアル情報をシェーダーに引き継ぎ
		Diffuse[ 0 ] = ( GLfloat )( *it )->Material->Diffuse.X;
		Diffuse[ 1 ] = ( GLfloat )( *it )->Material->Diffuse.Y;
		Diffuse[ 2 ] = ( GLfloat )( *it )->Material->Diffuse.Z;
		Diffuse[ 3 ] = ( GLfloat )( *it )->Material->ZTransp;
        cgSetParameter4fv( m_pMember->Diffuse , Diffuse );

		//ディビューズマップテクスチャを使用する
		if ( ( *it )->Material->DiffuseMap.IsEmpty() == skyFalse )
		{
            cgGLSetTextureParameter( m_pMember->texCoord0Location , ( *it )->Material->DiffuseMap->GetMember()->texId[ 0 ] ); //テクスチャとの関連付け
            CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
            cgGLEnableTextureParameter( m_pMember->texCoord0Location );
            CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
		}

		//メッシュ毎に描画
		m_spMesh->m_spVertexBuffer->Draw();
	}

	//シェーダーを設定解除
    cgGLDisableProfile( profile0 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
    cgGLDisableProfile( profile1 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
	cgGLUnbindProgram( profile0 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
	cgGLUnbindProgram( profile1 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );

}

CShaderObjectPtr  CCgLightShaderObject::vCloneNode()
{
	CCgLightShaderObjectPtr spResult = Create();
	return spResult;
}

} } }

#endif

#endif