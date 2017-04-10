#pragma once

namespace sky { namespace lib { namespace graphic {

//テンプレートTypedef郡
class CLight;
typedef SmartPointer< CLight >		CLightPtr;			//スマートポインタ版定義
typedef CList< u32 , CLightPtr >	CLightPtrList;		//リスト定義
typedef CHash< CLightPtr >			CLightPtrHash;		//Hash定義
typedef CHashMT< CLightPtr >		CLightPtrHashMT;	//Hash定義

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// 環境光情報
/// </summary>
struct CAmbientLightData
{
	CColor				m_AmbientColor;  //光の色
	CColor				m_DiffuseColor;  //拡散光の色
	CColor				m_SpecularColor; //反射光の色

	CAmbientLightData() : m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
						  m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
	                      m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ){}
};

/// <summary>
/// 並行光情報
/// </summary>
struct CDirectionalLightData
{
	CColor				m_AmbientColor;  //光の色
	CColor				m_DiffuseColor;	 //拡散光の色
	CColor				m_SpecularColor; //反射光の色
	math::CBasicVector3	m_Vec;

	CDirectionalLightData() : m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
							  m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
							  m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
							  m_Vec( 1.0f , 1.0f , 1.0f ){ m_Vec.Normalize(); }
};

/// <summary>
/// 点光源情報
/// </summary>
struct CPointLightData
{
	math::CBasicVector3 m_Pos;
	dectype				m_Range;
	CColor				m_AmbientColor; //光の色
	CColor				m_DiffuseColor;	//拡散光の色
	CColor				m_SpecularColor;//反射光の色
	dectype				m_ConsAtt;

	CPointLightData() : m_Pos( 2.0f , 2.0f , 2.0f ) ,
		                m_Range( 10.0f ) ,
	                    m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
						m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
	                    m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
						m_ConsAtt( 1.0f ){}
};

/// <summary>
/// スポットライト情報
/// </summary>
struct CSpotLightData
{
	math::CBasicVector3	m_Pos;
	math::CBasicVector3	m_Vec;
	CColor				m_AmbientColor; //光の色
	CColor				m_DiffuseColor;	//拡散光の色
	CColor				m_SpecularColor;//反射光の色
	dectype				m_Range;
	dectype				m_ConsAtt;

	CSpotLightData() : m_Pos( 2.0f , 2.0f , 2.0f ) , 
		               m_Vec( 1.0f , 1.0f , 1.0f ) ,
					   m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
					   m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
					   m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
					   m_Range( 10.0f ) ,
					   m_ConsAtt( 1.0f ){ m_Vec.Normalize(); }
};

/// <summary>
/// ライト
/// シーンには必ずライトがあり、何もしなければデフォルトのパラメーターである。
/// </summary>
class CLight : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 環境光情報を取得
	/// </summary>
	inline CAmbientLightData &GetAmbientLightData(){ return m_AmbientLightData; }

	/// <summary>
	/// 並行光情報
	/// </summary>
	inline CDirectionalLightData &GetDirectionalLightData(){ return m_DirectionalLightData; }

	/// <summary>
	/// 点光源情報
	/// </summary>
	inline CPointLightData &GetPointLightData(){ return m_PointLightData; }

	/// <summary>
	/// 点光源情報
	/// </summary>
	inline CSpotLightData &GetSpotLightData(){ return m_SpotLightData; }

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	void vRendering( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	void vPostRendering( CRenderingParam &param );

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CLightPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CLightPtr spLight( NEW__( CLight , CLight() ) );
		if ( name.IsEmpty() == skyFalse ) spLight->Name.SetName( name );
		return spLight;
	}

private:

	/// <summary>
	/// 環境光情報
	/// </summary>
	CAmbientLightData m_AmbientLightData;

	/// <summary>
	/// 並行光情報
	/// </summary>
	CDirectionalLightData m_DirectionalLightData;

	/// <summary>
	/// 点光源情報
	/// </summary>
	CPointLightData m_PointLightData;

	/// <summary>
	/// スポットライト情報
	/// </summary>
	CSpotLightData m_SpotLightData;
};

} } }

#define CLightCreate_()	          sky::lib::graphic::CLight::Create()
#define CLightCreateName_( name ) sky::lib::graphic::CLight::Create( name )
