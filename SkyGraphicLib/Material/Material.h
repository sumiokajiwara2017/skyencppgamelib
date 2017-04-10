#pragma once

namespace sky { namespace lib { namespace graphic {

class CMaterial;
typedef SmartPointer< CMaterial >		CMaterialPtr;	  //スマートポインタ版定義
typedef CHash< CMaterialPtr >			CMaterialPtrHash; //ハッシュ定義
typedef kfm::CFCurve< CMaterial >       CMaterialFCurve;
typedef kfm::CFCurvePlayer< CMaterial > CMaterialFCurvePlayer;

/*
【物体が見えるとは】
アクエリアスのペットボトルが青く見えるのは、アクエリアスのペットボトルが青いのではなく、光が当たった結果、
青く見える要素が強く反射したからだ。と考える。物体にはそれぞれ属性があり、同じ光を受けても違う反射をし、
結果様々な色で見える。物体がどんな属性を持つのかはとても重要だ。それがないと物体が光にたいして同反応し
結果どうみえるべきかを決定できない。モデルやスプライトはマテリアル属性を必ず持つべきだ。
*/

/// <summary>
/// 影の付き方の種類
/// </summary>
enum eShadeKind
{
	eShadeKind_Lambert = 0 ,
	eShadeKind_Fresnel ,
	eShadeKind_Toon ,
	eShadeKind_Minnaert ,
	eShadeKind_Oren_Nayar ,
};

/// <summary>
/// マテリアルクラス
/// 光に対して物体がどう反応するか、物体自体がどういう色かのパラメーターの定義群である。
/// </summary>
class CMaterial : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

    /// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMaterial(){}

//---Member

	/// <summary>
	/// 環境光
	/// </summary>
	CColor                  Ambient;

	/// <summary>
	/// 拡散反射光
	/// </summary>
	CColor                  Diffuse;

	/// <summary>
	/// 鏡面反射光
	/// </summary>
	CColor		            Specular;

	/// <summary>
	/// 鏡面反射光の強さ
	/// </summary>
	f32  					Power;

	/// <summary>
	/// エミッション		（物体自体の発光色）
	/// </summary>
	CColor                  Emissive;

	/// <summary>
	/// 光沢(0.0（つやなし）～ ）
	/// </summary>
	f32 					Shininess;

	/// <summary>
	/// 屈折率(1（屈折なし）～ ）
	/// </summary>
	f32 					Refractive;

	/// <summary>
	/// 半透明用アルファ値(透明にする場合はシーングラフノードのSetAlphaをskyTrueにして、この値を調整してください）
	/// </summary>
	f32                     Alpha;

	/// <summary>
	/// 影の付き方（これによってシェーダーの計算の仕方が変わる）
	/// </summary>
	eShadeKind				eShade;

//---Member

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// =演算子オーバーライド
	/// </summary>
	virtual CMaterial& operator =( const CMaterial& src )
	{
		Ambient    = src.Ambient;
		Diffuse    = src.Diffuse;
		Specular   = src.Specular;
		Power      = src.Power;
		Emissive   = src.Emissive;
		Shininess  = src.Shininess;
		Refractive = src.Refractive;
		Alpha      = src.Alpha;
	
		return *this;
	}

	/// <summary>
	/// ==演算子オーバーライド
	/// </summary>
	skyBool operator ==( const CMaterial& src ) const
	{
		return ( 
		Ambient    == src.Ambient &&
		Diffuse    == src.Diffuse &&
		Specular   == src.Specular &&
		Power      == src.Power &&
		Emissive   == src.Emissive &&
		Shininess  == src.Shininess &&
		Refractive == src.Refractive &&
		Alpha      == src.Alpha );
	}

	/// <summary>
	/// *演算子オーバーライド
	/// </summary>
	CMaterial operator *( colortype src ) const
	{
		CMaterial ret;

		ret.Ambient    = Ambient * src;
		ret.Diffuse    = Diffuse * src;
		ret.Specular   = Specular * src;
		ret.Power      = Power * src;
		ret.Emissive   = Emissive * src;
		ret.Shininess  = Shininess * src;
		ret.Refractive = Refractive * src;
		ret.Alpha      = Alpha * src;

		return ret;
	}
	CMaterial operator *( const CMaterial &src ) const
	{
		CMaterial ret;

		ret.Ambient    = Ambient * src.Ambient;
		ret.Diffuse    = Diffuse * src.Diffuse;
		ret.Specular   = Specular * src.Specular;
		ret.Power      = Power * src.Power;
		ret.Emissive   = Emissive * src.Emissive;
		ret.Shininess  = Shininess * src.Shininess;
		ret.Refractive = Refractive * src.Refractive;
		ret.Alpha      = Alpha * src.Alpha;

		return ret;
	}

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	CMaterial operator -( const CMaterial& src ) const
	{
		CMaterial ret;

		ret.Ambient    = Ambient - src.Ambient;
		ret.Diffuse    = Diffuse - src.Diffuse;
		ret.Specular   = Specular - src.Specular;
		ret.Power      = Power - src.Power;
		ret.Emissive   = Emissive - src.Emissive;
		ret.Shininess  = Shininess - src.Shininess;
		ret.Refractive = Refractive - src.Refractive;
		ret.Alpha      = Alpha - src.Alpha;

		return ret;
	}

	/// <summary>
	/// +演算子オーバーライド
	/// </summary>
	CMaterial operator +( const CMaterial& src ) const
	{
		CMaterial ret;

		ret.Ambient    = Ambient + src.Ambient;
		ret.Diffuse    = Diffuse + src.Diffuse;
		ret.Specular   = Specular + src.Specular;
		ret.Power      = Power + src.Power;
		ret.Emissive   = Emissive + src.Emissive;
		ret.Shininess  = Shininess + src.Shininess;
		ret.Refractive = Refractive + src.Refractive;
		ret.Alpha      = Alpha + src.Alpha;

		return ret;
	}

//---Leaf

	/// <summary>
	/// ディフューズ・マップ。一般的なテクスチャ。
	/// </summary>
	CTexturePtr             m_ColorStreamMapCash;
	CTexturePtr             GetDiffuseMap( skyBool isRequired = skyFalse );
	void                    AttachDiffuseMap( const CTexturePtr &pTex );
	void                    DetachDiffuseMap();

	/// <summary>
	/// スペキュラ・マップ
	/// </summary>
	CTexturePtr             m_SpecularMapCash;
	CTexturePtr             GetSpecularMap( skyBool isRequired = skyFalse );
	void                    AttachSpecularMap( const CTexturePtr &pTex );

	/// <summary>
	/// 環境マップ。
	/// </summary>
	CTexturePtr             m_AmbientMapCash;
	CTexturePtr             GetAmbientMap( skyBool isRequired = skyFalse );
	void                    AttachAmbientMap( const CTexturePtr &pTex );

	/// <summary>
	/// バンプマップ。
	/// </summary>
	CTexturePtr             m_BumpMapCash;
	CTexturePtr             GetBumpMap( skyBool isRequired = skyFalse );
	void                    AttachBumpMap( const CTexturePtr &pTex );

	/// <summary>
	/// 透明マップ。
	/// </summary>
	CTexturePtr             m_TransparenceMapCash;
	CTexturePtr             GetTransparenceMap( skyBool isRequired = skyFalse );
	void                    AttachTransparenceMap( const CTexturePtr &pTex );

	/// <summary>
	/// 反射マップ。
	/// </summary>
	CTexturePtr             m_RefractiveMapCash;
	CTexturePtr             GetRefractiveMap( skyBool isRequired = skyFalse );
	void                    AttachRefractiveMap( const CTexturePtr &pTex );

	/// <summary>
	/// ディスプレイステイトメントマップ。
	/// </summary>
	CTexturePtr             m_DisplayStateMapCash;
	CTexturePtr             GetDisplayStateMap( skyBool isRequired = skyFalse );
	void                    AttachDisplayStateMap( const CTexturePtr &pTex );

	/// <summary>
	/// 法線マップ。
	/// </summary>
	CTexturePtr             m_NomalMapCash;
	CTexturePtr             GetNomalMap( skyBool isRequired = skyFalse );
	void                    AttachNomalMap( const CTexturePtr &pTex );

	/// <summary>
	/// 高さマップ。
	/// </summary>
	CTexturePtr             m_HeightMapCash;
	CTexturePtr             GetHeightMap( skyBool isRequired = skyFalse );
	void                    AttachHeightMap( const CTexturePtr &pTex );

	/// <summary>
	/// 速度マップ。
	/// </summary>
	CTexturePtr             m_SpeedMapCash;
	CTexturePtr             GetSpeedMap( skyBool isRequired = skyFalse );
	void                    AttachSpeedMap( const CTexturePtr &pTex );

	/// <summary>
	/// 視差マップ。
	/// </summary>
	CTexturePtr             m_ParallaxMapCash;
	CTexturePtr             GetParallaxMap( skyBool isRequired = skyFalse );
	void                    AttachParallaxMap( const CTexturePtr &pTex );

	/// <summary>
	/// シャドウマップ。
	/// </summary>
	CTexturePtr             m_ShadowMapCash;
	CTexturePtr             GetShadowMap( skyBool isRequired = skyFalse );
	void                    AttachShadowMap( const CTexturePtr &pTex );

//---追加予定
	/// ライトマップ
	/// オクルージョンマップ

//---Leaf

	/// <summary>
	/// プログラムキャッシュのクリア
	/// </summary>
	void CashReset()
	{
		if ( m_ColorStreamMapCash.IsEmpty() == skyFalse )  m_ColorStreamMapCash.Delete();
		if ( m_SpecularMapCash.IsEmpty() == skyFalse )     m_SpecularMapCash.Delete();
		if ( m_AmbientMapCash.IsEmpty() == skyFalse )      m_AmbientMapCash.Delete();
		if ( m_BumpMapCash.IsEmpty() == skyFalse )         m_BumpMapCash.Delete();
		if ( m_TransparenceMapCash.IsEmpty() == skyFalse ) m_TransparenceMapCash.Delete();
		if ( m_RefractiveMapCash.IsEmpty() == skyFalse )   m_RefractiveMapCash.Delete();
		if ( m_DisplayStateMapCash.IsEmpty() == skyFalse ) m_DisplayStateMapCash.Delete();
		if ( m_NomalMapCash.IsEmpty() == skyFalse )        m_NomalMapCash.Delete();
		if ( m_HeightMapCash.IsEmpty() == skyFalse )       m_HeightMapCash.Delete();
		if ( m_SpeedMapCash.IsEmpty() == skyFalse )        m_SpeedMapCash.Delete();
		if ( m_ParallaxMapCash.IsEmpty() == skyFalse )     m_ParallaxMapCash.Delete();
		if ( m_ShadowMapCash.IsEmpty() == skyFalse )       m_ShadowMapCash.Delete();
	}

//---Serialize

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

//---Static

	/// <summary>
	/// インスタンス生成
	/// </summary>
	static CMaterialPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMaterialPtr spMaterial( NEW__( CMaterial , CMaterial() ) );
		if ( name.IsEmpty() == skyFalse ) spMaterial->Name.SetName( name );
		return spMaterial;
	}

//private: //アニメーションの関係でコンストラクタを公開しているが、シーングラフで使用するマテリアルは必ずCreate関数を通したものを使用すること。

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMaterial(){ Reset(); }
	CMaterial( f32 alpha , 
               const CColor &diffuse = CColor( 0.5f , 0.5f , 0.5f ) , 
               const CColor &ambient = CColor( 0.5f , 0.5f , 0.5f ) ,
               const CColor &specular = CColor( 0.5f , 0.5f , 0.5f ) ,
               f32 power = 0.5f ,
               const CColor &emissive = CColor( 0.5f , 0.5f , 0.5f ) ,
               eShadeKind eshade = eShadeKind_Lambert ,
               f32 shininess = 0 ,
               f32 refractive = 1
               ) : Alpha( alpha ) ,
	              Diffuse( diffuse ) , 
		          Ambient( ambient ) , 
				  Specular( specular ) , 
				  Power( power ) , 
				  Emissive( emissive ) , 
				  eShade( eshade ) ,
	              Shininess( shininess ) ,
	              Refractive( refractive ){}

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	CMaterial( const CMaterial& src )
	{
		*this = src;
	};
};

} } }

#define CMaterialCreate_()				sky::lib::graphic::CMaterial::Create()
#define CMaterialCreateName_( name )	sky::lib::graphic::CMaterial::Create( name )
