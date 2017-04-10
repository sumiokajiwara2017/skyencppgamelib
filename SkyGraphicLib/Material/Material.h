#pragma once

namespace sky { namespace lib { namespace graphic {

class CMaterial;
typedef SmartPointer< CMaterial >		CMaterialPtr;	  //�X�}�[�g�|�C���^�Œ�`
typedef CHash< CMaterialPtr >			CMaterialPtrHash; //�n�b�V����`
typedef kfm::CFCurve< CMaterial >       CMaterialFCurve;
typedef kfm::CFCurvePlayer< CMaterial > CMaterialFCurvePlayer;

/*
�y���̂�������Ƃ́z
�A�N�G���A�X�̃y�b�g�{�g������������̂́A�A�N�G���A�X�̃y�b�g�{�g�������̂ł͂Ȃ��A���������������ʁA
��������v�f���������˂������炾�B�ƍl����B���̂ɂ͂��ꂼ�ꑮ��������A���������󂯂Ă��Ⴄ���˂����A
���ʗl�X�ȐF�Ō�����B���̂��ǂ�ȑ��������̂��͂ƂĂ��d�v���B���ꂪ�Ȃ��ƕ��̂����ɂ������ē�������
���ʂǂ��݂���ׂ���������ł��Ȃ��B���f����X�v���C�g�̓}�e���A��������K�����ׂ����B
*/

/// <summary>
/// �e�̕t�����̎��
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
/// �}�e���A���N���X
/// ���ɑ΂��ĕ��̂��ǂ��������邩�A���̎��̂��ǂ������F���̃p�����[�^�[�̒�`�Q�ł���B
/// </summary>
class CMaterial : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

    /// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMaterial(){}

//---Member

	/// <summary>
	/// ����
	/// </summary>
	CColor                  Ambient;

	/// <summary>
	/// �g�U���ˌ�
	/// </summary>
	CColor                  Diffuse;

	/// <summary>
	/// ���ʔ��ˌ�
	/// </summary>
	CColor		            Specular;

	/// <summary>
	/// ���ʔ��ˌ��̋���
	/// </summary>
	f32  					Power;

	/// <summary>
	/// �G�~�b�V����		�i���̎��̂̔����F�j
	/// </summary>
	CColor                  Emissive;

	/// <summary>
	/// ����(0.0�i��Ȃ��j�` �j
	/// </summary>
	f32 					Shininess;

	/// <summary>
	/// ���ܗ�(1�i���܂Ȃ��j�` �j
	/// </summary>
	f32 					Refractive;

	/// <summary>
	/// �������p�A���t�@�l(�����ɂ���ꍇ�̓V�[���O���t�m�[�h��SetAlpha��skyTrue�ɂ��āA���̒l�𒲐����Ă��������j
	/// </summary>
	f32                     Alpha;

	/// <summary>
	/// �e�̕t�����i����ɂ���ăV�F�[�_�[�̌v�Z�̎d�����ς��j
	/// </summary>
	eShadeKind				eShade;

//---Member

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h
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
	/// ==���Z�q�I�[�o�[���C�h
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
	/// *���Z�q�I�[�o�[���C�h
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
	/// -���Z�q�I�[�o�[���C�h
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
	/// +���Z�q�I�[�o�[���C�h
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
	/// �f�B�t���[�Y�E�}�b�v�B��ʓI�ȃe�N�X�`���B
	/// </summary>
	CTexturePtr             m_ColorStreamMapCash;
	CTexturePtr             GetDiffuseMap( skyBool isRequired = skyFalse );
	void                    AttachDiffuseMap( const CTexturePtr &pTex );
	void                    DetachDiffuseMap();

	/// <summary>
	/// �X�y�L�����E�}�b�v
	/// </summary>
	CTexturePtr             m_SpecularMapCash;
	CTexturePtr             GetSpecularMap( skyBool isRequired = skyFalse );
	void                    AttachSpecularMap( const CTexturePtr &pTex );

	/// <summary>
	/// ���}�b�v�B
	/// </summary>
	CTexturePtr             m_AmbientMapCash;
	CTexturePtr             GetAmbientMap( skyBool isRequired = skyFalse );
	void                    AttachAmbientMap( const CTexturePtr &pTex );

	/// <summary>
	/// �o���v�}�b�v�B
	/// </summary>
	CTexturePtr             m_BumpMapCash;
	CTexturePtr             GetBumpMap( skyBool isRequired = skyFalse );
	void                    AttachBumpMap( const CTexturePtr &pTex );

	/// <summary>
	/// �����}�b�v�B
	/// </summary>
	CTexturePtr             m_TransparenceMapCash;
	CTexturePtr             GetTransparenceMap( skyBool isRequired = skyFalse );
	void                    AttachTransparenceMap( const CTexturePtr &pTex );

	/// <summary>
	/// ���˃}�b�v�B
	/// </summary>
	CTexturePtr             m_RefractiveMapCash;
	CTexturePtr             GetRefractiveMap( skyBool isRequired = skyFalse );
	void                    AttachRefractiveMap( const CTexturePtr &pTex );

	/// <summary>
	/// �f�B�X�v���C�X�e�C�g�����g�}�b�v�B
	/// </summary>
	CTexturePtr             m_DisplayStateMapCash;
	CTexturePtr             GetDisplayStateMap( skyBool isRequired = skyFalse );
	void                    AttachDisplayStateMap( const CTexturePtr &pTex );

	/// <summary>
	/// �@���}�b�v�B
	/// </summary>
	CTexturePtr             m_NomalMapCash;
	CTexturePtr             GetNomalMap( skyBool isRequired = skyFalse );
	void                    AttachNomalMap( const CTexturePtr &pTex );

	/// <summary>
	/// �����}�b�v�B
	/// </summary>
	CTexturePtr             m_HeightMapCash;
	CTexturePtr             GetHeightMap( skyBool isRequired = skyFalse );
	void                    AttachHeightMap( const CTexturePtr &pTex );

	/// <summary>
	/// ���x�}�b�v�B
	/// </summary>
	CTexturePtr             m_SpeedMapCash;
	CTexturePtr             GetSpeedMap( skyBool isRequired = skyFalse );
	void                    AttachSpeedMap( const CTexturePtr &pTex );

	/// <summary>
	/// �����}�b�v�B
	/// </summary>
	CTexturePtr             m_ParallaxMapCash;
	CTexturePtr             GetParallaxMap( skyBool isRequired = skyFalse );
	void                    AttachParallaxMap( const CTexturePtr &pTex );

	/// <summary>
	/// �V���h�E�}�b�v�B
	/// </summary>
	CTexturePtr             m_ShadowMapCash;
	CTexturePtr             GetShadowMap( skyBool isRequired = skyFalse );
	void                    AttachShadowMap( const CTexturePtr &pTex );

//---�ǉ��\��
	/// ���C�g�}�b�v
	/// �I�N���[�W�����}�b�v

//---Leaf

	/// <summary>
	/// �v���O�����L���b�V���̃N���A
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
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static CMaterialPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMaterialPtr spMaterial( NEW__( CMaterial , CMaterial() ) );
		if ( name.IsEmpty() == skyFalse ) spMaterial->Name.SetName( name );
		return spMaterial;
	}

//private: //�A�j���[�V�����̊֌W�ŃR���X�g���N�^�����J���Ă��邪�A�V�[���O���t�Ŏg�p����}�e���A���͕K��Create�֐���ʂ������̂��g�p���邱�ƁB

	/// <summary>
	/// �R���X�g���N�^
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
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	CMaterial( const CMaterial& src )
	{
		*this = src;
	};
};

} } }

#define CMaterialCreate_()				sky::lib::graphic::CMaterial::Create()
#define CMaterialCreateName_( name )	sky::lib::graphic::CMaterial::Create( name )
