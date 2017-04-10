#pragma once

namespace sky { namespace lib { namespace graphic {

namespace font {

static const u32 ONE_LINE_NUM_MAX       = 32;  //�P�s������
static const u32 ONE_LINE_FONT_NUM_MAX  = 256; //�P�s������
static const u32 LINE_FONT_POOL_NUM_MAX = 256; //�P�s�������v�[����

}

class CFont;
typedef SmartPointer< CFont >   CFontPtr;
typedef CList< u32 , CFontPtr > CFontPtrList;

/// <summary>
/// �����N���X
/// �ȈՕ����\���ƕ�������A�^�O�ɂ��F�ς��A�C�x���g�����ȂǂɑΉ����镶���I�u�W�F�N�g�Ƃ��Ă̈���
/// �Ƃ����Q��ނ̕����̏o����������B
/// </summary>
class CFont : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CFont(){}

	/// <summary>
	/// �g�p����
	/// </summary>
	void SetUp();

	/// <summary>
	/// �����\��
	/// �����񕶎����o�͂����ꍇ�A�Ō�̕����o�͂Ō��݂̕����o�͂��㏑�������
	/// </summary>
	void PrintfSentence      ( dectype posX , dectype posY , const CColor &color , dectype sc , u32 outIntvl , const skyString *format , ... );

	/// <summary>
	/// �F�ݒ�
	/// </summary>
	void SetColor            ( const CColor &color ){ m_SentenceData.Color = color; };

	/// <summary>
	/// ���W�ݒ�
	/// </summary>
	void SetPosition         ( math::CBasicVector2 &pos ){ m_SentenceData.Pos = pos; };

	/// <summary>
	/// ���W�擾
	/// </summary>
	math::CBasicVector2 &GetPosition(){ return m_SentenceData.Pos; };

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	void vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// ������T�C�Y�擾
	/// </summary>
	math::CBasicSize &GetSentenceSize(){ return m_SentenceSize; }

	/// <summary>
	/// �����T�C�Y�擾
	/// </summary>
	math::CBasicSize &GetFontSize(){ return m_FontSize; }

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
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	      Clone();
	serialize::CSerializeInterfacePtr 		  CloneSharing();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	void Activate();

//---Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CFontPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() , font::eFontStyleType eStyle = font::eFontStyleType_Gosic13 , dectype prio = RENDERING_2D_PRIO_FAST )
	{
		
		CFontPtr spFont( NEW__( CFont , CFont( eStyle , prio ) ) );
		if ( name.IsEmpty() == skyFalse ) spFont->Name.SetName( name );
		return spFont;
	}


private:

//--Branch

	/// <summary>
	/// �o�͗p�V�F�[�_�[
	/// </summary>
	CTextureMappingShaderObjectPtr GetShader( skyBool isRequired = skyTrue );

//--Branch

//---Member

	/// <summary>
	/// �P���f�[�^
	/// </summary>
	struct CSentenceData_
	{
		CColor               Color;
		dectype              Sc;
		skyString            Sentence[ font::ONE_LINE_FONT_NUM_MAX ];
		u32                  OutIntvl;                          //�������肷��Ԋu�i�t���[���j�O�ň�C�ɏo�͂����B
		math::CBasicVector2  Pos;

		/// <summary>
		/// ������Z�q�I�[�o�[���C�h
		/// </summary>
		virtual CSentenceData_& operator =( const CSentenceData_& src )
		{
			Color    = src.Color;
			Sc       = src.Sc;
			CStringUtil_::Copy( Sentence , font::ONE_LINE_FONT_NUM_MAX , src.Sentence );
			OutIntvl = src.OutIntvl;
			Pos      = src.Pos;
			return *this;
		}
	};
	CSentenceData_  m_SentenceData;

	/// <summary>
	/// �X�^�C��
	/// </summary>
    font::eFontStyleType m_Style;

	/// <summary>
	/// ������
	/// </summary>
	vrtxtype  m_printFontNum;

	/// <summary>
	/// ������̕��ƍ������擾
	/// </summary>
	math::CBasicSize m_SentenceSize;

	/// <summary>
	/// �����̕��ƍ������擾
	/// </summary>
	math::CBasicSize m_FontSize;

	/// <summary>
	/// �D�揇��
	/// </summary>
	dectype m_Prio;

//---Member

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// ���b�V��
	/// </summary>
    CMeshContenaPtr m_MeshContena;

	/// <summary>
	/// �}�e���A��
	/// </summary>
	CMaterialPtr m_spMaterial;

	/// <summary>
	/// �������������t���O
	/// </summary>
    skyBool  m_IsReWrite;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CFont( font::eFontStyleType eStyle , dectype prio  );
};

#define CFontCreate_()				sky::lib::graphic::CFont::Create()
#define CFontCreateName_( name )	sky::lib::graphic::CFont::Create( name )

class CFontSimple;
typedef SmartPointer< CFontSimple > CFontSimplePtr;

/// <summary>
/// �ȈՕ����o�̓N���X
/// </summary>
class CFontSimple : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CFontSimple(){}

	/// <summary>
	/// �g�p����
	/// </summary>
	void SetUp();

    /// <summary>
	///  ����W�A���`��I��
	///  ����W�`����s���ƁA�w��̊���W�ɍŏ��̕������o�͂���A�ȍ~�̕����͂��̉��ɕ`�悳��Ă����B
    ///  �e���͂ɐݒ肳�ꂽ���W�̓I�t�Z�b�g���W�Ƃ��Ďg�p�����B
    /// </summary>
	void SetStanderdPositionDraw( skyBool isOn ){ m_IsStanderdPositionDraw = isOn; }

    /// <summary>
	///  ����W�̐ݒ�
    /// </summary>
	void SetStanderdPosition( const math::CBasicVector2 &pos ){ m_IsStanderdPosition = pos; };

	/// <summary>
	/// �����\��
	/// ���̃I�u�W�F�N�g���畡���̕����o�͉\�B���̂����o�͂��������𕶎�����ł��Ȃ��B�㏑���ł��Ȃ��B�f�o�b�O�\�����ɕ֗�
	/// </summary>
	void PrintfSentence  ( dectype posX , dectype posY , const CColor &color , dectype sc , const skyString *format , ... );

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	virtual void  vRendering ( CRenderingParam &param );

	/// <summary>
	/// �`��I�����R�[���C�x���g�n���h��
	/// </summary>
	delegate::CDelegateHandler m_RenderingEnd;

	/// <summary>
	/// �`��s���擾
	/// </summary>
	u32 GetPrintNum(){ return m_SentenceList.GetNum(); }

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

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	void Activate();

//---Static

	/// <summary>
	/// �쐬�i�`��̊֌W�ŃX�^�C���͍쐬���Ɍ��肷��j
	/// </summary>
	static CFontSimplePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() , font::eFontStyleType eStyle = font::eFontStyleType_Gosic13 , u32 lineNum = font::LINE_FONT_POOL_NUM_MAX , dectype prio = RENDERING_2D_PRIO_FAST )
	{
		
		CFontSimplePtr spFont( NEW__( CFontSimple , CFontSimple( eStyle , lineNum , prio ) ) );
		if ( name.IsEmpty() == skyFalse ) spFont->Name.SetName( name );
		return spFont;
	}

private:

	/// <summary>
	/// �����o�̓Z�b�g�A�b�v�f�[�^
	/// </summary>
	struct CSentenceData_
	{
		CColor               Color;
		dectype              Sc;
		skyString            Sentence[ font::ONE_LINE_FONT_NUM_MAX ];
		math::CBasicVector2  Pos;
	};
	typedef CSentenceData_*                   CSentenceDataPtr_;
    typedef CList< u32 , CSentenceDataPtr_ >  CSentenceDataPtrList_;			//���X�g��`
	typedef CMemoryPool< CSentenceData_ >     SentencePool;
	SentencePool           m_SentencePool;
	CSentenceDataPtrList_  m_SentenceList;

//--Branch

	/// <summary>
	/// �o�͗p�V�F�[�_�[�̎擾
	/// </summary>
	CTextureMappingShaderObjectPtr GetShader( skyBool isRequired = skyTrue );

	/// <summary>
	/// �􉽏��̎擾
	/// </summary>
	CMeshPtr GetMesh( skyBool isRequired = skyTrue );

	/// <summary>
	/// ���_�o�b�t�@�̎擾
	/// </summary>
	CVertexBufferPtr GetVertexBuffer( skyBool isRequired = skyTrue );

//--Branch

//---Member

	/// <summary>
	/// �X�^�C��
	/// </summary>
    font::eFontStyleType   m_Style;

	/// <summary>
	/// ����W�`��t���O
	/// </summary>
	skyBool m_IsStanderdPositionDraw;

	/// <summary>
	/// ����W
	/// </summary>
	math::CBasicVector2 m_IsStanderdPosition;

	/// <summary>
	/// �D�揇��
	/// </summary>
	dectype m_Prio;

//---Member

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CFontSimple( font::eFontStyleType eStyle , u32 lineNum , dectype prio );
};

} } }

#define CFontSimpleCreate_()                       sky::lib::graphic::CFontSimple::Create()
#define CFontSimpleCreateNum_( style , lineNum )   sky::lib::graphic::CFontSimple::Create( hash::CHashStringPtr() , style , lineNum )
#define CFontSimpleCreateName_( name )	           sky::lib::graphic::CFontSimple::Create( name );
