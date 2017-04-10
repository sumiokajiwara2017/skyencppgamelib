#pragma once

namespace sky { namespace lib { namespace graphic {

//�e���v���[�gTypedef�S
class CLight;
typedef SmartPointer< CLight >		CLightPtr;			//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CLightPtr >	CLightPtrList;		//���X�g��`
typedef CHash< CLightPtr >			CLightPtrHash;		//Hash��`
typedef CHashMT< CLightPtr >		CLightPtrHashMT;	//Hash��`

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �������
/// </summary>
struct CAmbientLightData
{
	CColor				m_AmbientColor;  //���̐F
	CColor				m_DiffuseColor;  //�g�U���̐F
	CColor				m_SpecularColor; //���ˌ��̐F

	CAmbientLightData() : m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
						  m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
	                      m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ){}
};

/// <summary>
/// ���s�����
/// </summary>
struct CDirectionalLightData
{
	CColor				m_AmbientColor;  //���̐F
	CColor				m_DiffuseColor;	 //�g�U���̐F
	CColor				m_SpecularColor; //���ˌ��̐F
	math::CBasicVector3	m_Vec;

	CDirectionalLightData() : m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
							  m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
							  m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
							  m_Vec( 1.0f , 1.0f , 1.0f ){ m_Vec.Normalize(); }
};

/// <summary>
/// �_�������
/// </summary>
struct CPointLightData
{
	math::CBasicVector3 m_Pos;
	dectype				m_Range;
	CColor				m_AmbientColor; //���̐F
	CColor				m_DiffuseColor;	//�g�U���̐F
	CColor				m_SpecularColor;//���ˌ��̐F
	dectype				m_ConsAtt;

	CPointLightData() : m_Pos( 2.0f , 2.0f , 2.0f ) ,
		                m_Range( 10.0f ) ,
	                    m_AmbientColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
						m_DiffuseColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
	                    m_SpecularColor( CColor( 1.0f , 1.0f , 1.0f , 1.0f ) ) ,
						m_ConsAtt( 1.0f ){}
};

/// <summary>
/// �X�|�b�g���C�g���
/// </summary>
struct CSpotLightData
{
	math::CBasicVector3	m_Pos;
	math::CBasicVector3	m_Vec;
	CColor				m_AmbientColor; //���̐F
	CColor				m_DiffuseColor;	//�g�U���̐F
	CColor				m_SpecularColor;//���ˌ��̐F
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
/// ���C�g
/// �V�[���ɂ͕K�����C�g������A�������Ȃ���΃f�t�H���g�̃p�����[�^�[�ł���B
/// </summary>
class CLight : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// ���������擾
	/// </summary>
	inline CAmbientLightData &GetAmbientLightData(){ return m_AmbientLightData; }

	/// <summary>
	/// ���s�����
	/// </summary>
	inline CDirectionalLightData &GetDirectionalLightData(){ return m_DirectionalLightData; }

	/// <summary>
	/// �_�������
	/// </summary>
	inline CPointLightData &GetPointLightData(){ return m_PointLightData; }

	/// <summary>
	/// �_�������
	/// </summary>
	inline CSpotLightData &GetSpotLightData(){ return m_SpotLightData; }

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	void vRendering( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	void vPostRendering( CRenderingParam &param );

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CLightPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CLightPtr spLight( NEW__( CLight , CLight() ) );
		if ( name.IsEmpty() == skyFalse ) spLight->Name.SetName( name );
		return spLight;
	}

private:

	/// <summary>
	/// �������
	/// </summary>
	CAmbientLightData m_AmbientLightData;

	/// <summary>
	/// ���s�����
	/// </summary>
	CDirectionalLightData m_DirectionalLightData;

	/// <summary>
	/// �_�������
	/// </summary>
	CPointLightData m_PointLightData;

	/// <summary>
	/// �X�|�b�g���C�g���
	/// </summary>
	CSpotLightData m_SpotLightData;
};

} } }

#define CLightCreate_()	          sky::lib::graphic::CLight::Create()
#define CLightCreateName_( name ) sky::lib::graphic::CLight::Create( name )
