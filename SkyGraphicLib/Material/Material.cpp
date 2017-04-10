#include "StdAfx.h"
#include "SkyGraphicLib/Material.h"

SkyImplementRTTI( sky::lib::graphic::CMaterial , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

enum eMapType
{
	eMapType_Diffuse = 0 ,
	eMapType_Specular ,
	eMapType_Ambient ,
	eMapType_Bump ,
	eMapType_Transparence ,
	eMapType_Refractive ,
	eMapType_DisplayState ,
	eMapType_Nomal ,
	eMapType_Height ,
	eMapType_Speed ,
	eMapType_Parallax ,
	eMapType_Shadow ,
};

void CMaterial::Reset()
{
	//初期値のマテリアルは赤茶色の銅のような質感になる。テスト用
	Ambient    = CColor( 0.25f, 0.20725f, 0.20725f , 1.0f );
	Diffuse    = CColor( 1.0f, 0.829f, 0.829f , 1.0f );
	Specular   = CColor( 0.296648f, 0.296648f, 0.296648f );
	Power      = 0.5f;
	Emissive   = CColor_White;
	Shininess  = 0.5f;
	Refractive = 0.5;
	Alpha      = 1.0f;
    eShade     = eShadeKind_Lambert;
}


CTexturePtr CMaterial::GetDiffuseMap( skyBool isRequired )
{
	if( m_ColorStreamMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Diffuse , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "Diffuse is not found." ) );
		}
#endif

		m_ColorStreamMapCash = spResult;
	}
	return m_ColorStreamMapCash;
}
void CMaterial::AttachDiffuseMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Diffuse;
	AttachLeaf( pTex );
	m_ColorStreamMapCash = pTex;
}

void CMaterial::DetachDiffuseMap()
{
	if ( m_ColorStreamMapCash.IsEmpty() == skyFalse )
	{
		DetachLeaf( m_ColorStreamMapCash );
		m_ColorStreamMapCash.Delete();
	}
}
	
CTexturePtr CMaterial::GetSpecularMap( skyBool isRequired )
{
	if( m_SpecularMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Specular , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "Diffuse is not found." ) );
		}
#endif
		m_SpecularMapCash = spResult;
	}
	return m_SpecularMapCash;
}
void CMaterial::AttachSpecularMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Specular;
	this->AttachLeaf( pTex );
	m_SpecularMapCash = pTex;
}

CTexturePtr CMaterial::GetAmbientMap( skyBool isRequired )
{
	if( m_AmbientMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Ambient , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "Diffuse is not found." ) );
		}
#endif
		m_AmbientMapCash = spResult;
	}
	return m_AmbientMapCash;
}
void CMaterial::AttachAmbientMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Ambient;
	this->AttachLeaf( pTex );
	m_AmbientMapCash = pTex;
}

CTexturePtr CMaterial::GetBumpMap( skyBool isRequired )
{
	if( m_BumpMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Bump , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_BumpMapCash = spResult;
	}
	return m_BumpMapCash;
}

void CMaterial::AttachBumpMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Bump;
	this->AttachLeaf( pTex );
	m_BumpMapCash = pTex;
}

CTexturePtr CMaterial::GetTransparenceMap( skyBool isRequired )
{
	if( m_TransparenceMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Transparence , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_TransparenceMapCash = spResult;
	}
	return m_TransparenceMapCash;
}

void CMaterial::AttachTransparenceMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Transparence;
	this->AttachLeaf( pTex );
	m_TransparenceMapCash = pTex;
}

CTexturePtr CMaterial::GetRefractiveMap( skyBool isRequired )
{
	if( m_RefractiveMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Refractive , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_RefractiveMapCash = spResult;
	}
	return m_RefractiveMapCash;
}

void CMaterial::AttachRefractiveMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Refractive;
	this->AttachLeaf( pTex );
	m_RefractiveMapCash = pTex;
}

CTexturePtr CMaterial::GetDisplayStateMap( skyBool isRequired )
{
	if( m_DisplayStateMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_DisplayState , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_DisplayStateMapCash = spResult;
	}
	return m_DisplayStateMapCash;
}

void CMaterial::AttachDisplayStateMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_DisplayState;
	this->AttachLeaf( pTex );
	m_DisplayStateMapCash = pTex;
}

CTexturePtr CMaterial::GetNomalMap( skyBool isRequired )
{
	if( m_NomalMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Nomal , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_NomalMapCash = spResult;
	}
	return m_NomalMapCash;
}

void CMaterial::AttachNomalMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Nomal;
	this->AttachLeaf( pTex );
	m_NomalMapCash = pTex;
}

CTexturePtr CMaterial::GetHeightMap( skyBool isRequired )
{
	if( m_HeightMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Height , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_HeightMapCash = spResult;
	}
	return m_HeightMapCash;
}

void CMaterial::AttachHeightMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Height;
	this->AttachLeaf( pTex );
	m_HeightMapCash = pTex;
}

CTexturePtr CMaterial::GetSpeedMap( skyBool isRequired )
{
	if( m_SpeedMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Speed , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_SpeedMapCash = spResult;
	}
	return m_SpeedMapCash;
}

void CMaterial::AttachSpeedMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Speed;
	this->AttachLeaf( pTex );
	m_SpeedMapCash = pTex;
}

CTexturePtr CMaterial::GetParallaxMap( skyBool isRequired )
{
	if( m_ParallaxMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Parallax , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_ParallaxMapCash = spResult;
	}
	return m_ParallaxMapCash;
}

void CMaterial::AttachParallaxMap( const CTexturePtr &pTex )
{
	pTex->ID = ( s32 )eMapType_Parallax;
	this->AttachLeaf( pTex );
	m_ParallaxMapCash = pTex;
}

CTexturePtr CMaterial::GetShadowMap( skyBool isRequired )
{
	if( m_ShadowMapCash.IsEmpty() == skyTrue )
	{
		CTexturePtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeafID< CTexture , CTexturePtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eMapType_Shadow , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "map is not found." ) );
		}
#endif
		m_ShadowMapCash = spResult;
	}
	return m_ShadowMapCash;
}

void CMaterial::AttachShadowMap( const CTexturePtr &pTex )
{
	if ( pTex.IsEmpty() ) return;

	pTex->ID = ( s32 )eMapType_Shadow;
	this->AttachLeaf( pTex );
	m_ShadowMapCash = pTex;
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&Ambient.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Ambient.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Ambient.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Ambient.a , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Diffuse.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Diffuse.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Diffuse.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Diffuse.a , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Specular.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Specular.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Specular.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Specular.a , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Emissive.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Emissive.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Emissive.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Emissive.a , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Alpha , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Power , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Shininess , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&Refractive , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&eShade , sizeof( s32 ) , isWrite ); \

skyBool CMaterial::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "material bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}
			
skyBool CMaterial::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "material bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CMaterial::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CMaterialPtr )spContainer )->Ambient = Ambient;
	( ( CMaterialPtr )spContainer )->Diffuse = Diffuse;
	( ( CMaterialPtr )spContainer )->Specular = Specular;
	( ( CMaterialPtr )spContainer )->Emissive = Emissive;
	( ( CMaterialPtr )spContainer )->Alpha = Alpha;
	( ( CMaterialPtr )spContainer )->Power = Power;
	( ( CMaterialPtr )spContainer )->Shininess = Shininess;
	( ( CMaterialPtr )spContainer )->Refractive = Refractive;
	( ( CMaterialPtr )spContainer )->eShade = eShade;
	return spContainer;
}
const serialize::CSerializeInterfacePtr& CMaterial::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CMaterial::Clone()
{
	return Copy( Create( hash::CHashStringPtr() ) );
}

serialize::CSerializeInterfacePtr CMaterial::CloneSharing()
{
//	return Clone();
	return thisPtr( CMaterialPtr ); //自信のポインタをスマートポインタで保護して返せばいいはず
}

} } }
