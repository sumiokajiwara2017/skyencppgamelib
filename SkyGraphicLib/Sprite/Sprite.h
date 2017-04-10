#pragma once

namespace sky { namespace lib { namespace graphic {

//�e���v���[�gTypedef�S
class CSprite;
typedef SmartPointer< CSprite >				CSpritePtr;				//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CSpritePtr >			CSpritePtrList;			//���X�g��`
typedef CHash< CSpritePtr >					CSpritePtrHash;			//Hash��`
typedef CHashMT< CSpritePtr >				CSpritePtrHashMT;		//Hash��`

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �X�v���C�g�N���X
/// �X�v���C�g�Ƃ͂Q�c�̋�`�ŕ\�������`��v�f�ł��B
/// �i���Ӂj�Q�c�J������ԁi���ˉe�j�Ŏg�p���Ă��������B�X�v���C�g�̂y���W�̈������Q�c�J������Ԑ�p�ɂȂ��Ă��܂��B
/// </summary>
class CSprite : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual                        ~CSprite(){};

	/// <summary>
	/// �g�p����
	/// </summary>
	void                            SetUp();

	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	CMaterialPtr       GetMaterial( u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// �􉽊w�`�����DifuseMap���w��̐F�ɐݒ肷��
	/// </summary>
	void               AttachDiffuseMap( const graphic::CTexturePtr &spTexture , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// �􉽊w�`�����Difuse�J���[���w��̐F�ɐݒ肷��
	/// </summary>
	void               SetDiffuse( const CColor &color , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// �A���t�@�l�̐ݒ�
	/// </summary>
	void               SetAlphaValue( f32 alpha , u32 meshNo = 0 , u32 materialNo = 0 );

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	virtual void				    vRendering			( CRenderingParam &param );

    /// <summary>
	///  �����_�����O���������֐�
    /// </summary>
	void                            RenderingImple      ( CRenderingParam &param , const CShaderObjectPtr &shader );

//----Branch

    /// <summary>
	///  Mesh�̊֘A�t���^�擾
    /// </summary>
	void            AttachMeshContena( const CMeshContenaPtr &spMesh );
	CMeshContenaPtr GetMeshContena( skyBool isRequired = skyTrue );

//----Branch

//----Leaf

    /// <summary>
	///  �V�F�[�_�[���֘A�t���^�֘A�t������������
    /// </summary>
	void            AttachShader( const CShaderObjectPtr &effectPtr );
	void            ClearAndAttachShader( const CShaderObjectPtr &effectPtr ); //�V�F�[�_�[����U�N���A���A�^�b�`����
	void            DetachShader();

//----Leaf

#ifdef SW_SKYLIB_DEBUG_ON

    /// <summary>
	///  �f�o�b�O�v�����g
    /// </summary>
	virtual void NodeTreePrint( u32 indent = 0 , eRelationKind eKind = eRelationKind_Root  );

#endif

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
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CSpritePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CSpritePtr spSprite( NEW__( CSprite , CSprite() ) );
		if ( name.IsEmpty() == skyFalse ) spSprite->Name.SetName( name );
		return spSprite;
	}

protected:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSprite();
	CSprite( CSceneGraphNode &spNode );

    /// <summary>
	/// ��΍��W��Ԃ��i�����j
    /// </summary>
	static void GetAbsoluteCoordinatesImple( CSceneGraphNode *pNode , u32 &count , math::CBasicVector2 &dest );
};

} } }

#define CSpriteCreate_()			    sky::lib::graphic::CSprite::Create()
#define CSpriteCreateName_( name )	    sky::lib::graphic::CSprite::Create( name )
