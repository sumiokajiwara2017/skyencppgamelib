#pragma once

namespace sky { namespace lib { namespace graphic {

//===================================�yCommon�z====================================================>

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)

struct CVertexBufferMember;
typedef SmartPointer2< CVertexBufferMember > CVertexBufferMemberPtr;		//�X�}�[�g�|�C���^�Œ�`

//�e���v���[�gTypedef�S

class CVertexBuffer;
typedef SmartPointer< CVertexBuffer >				CVertexBufferPtr;			//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CVertexBufferPtr >				CVertexBufferPtrList;		//���X�g��`
typedef CHash< CVertexBufferPtr >					CVertexBufferPtrHash;		//Hash��`
typedef CHashMT< CVertexBufferPtr >					CVertexBufferPtrHashMT;		//Hash��`

class CVertexBuffer : public CSceneGraphNode
{
    SkyTypeDefRTTI;

friend class CFont;
friend class CFontSimple;
friend class CDecalMember;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual                ~CVertexBuffer();

	/// <summary>
	/// ���_�̐����擾
	/// </summary>
	inline vrtxtype			GetVertexNum() const { return m_VertexNum; };

	/// <summary>
	/// �ʂ̖@���Z�o
	/// </summary>
	void					ComputePrimNormalVector();
	void					ComputePrimNormalVectorFromIndexBuffer( const CIndexBufferPtr &spIndexBuffer );

	/// <summary>
	/// ���_�̖@���Z�o
	/// </summary>
	void					ComputeVertexNormalVector( const CIndexBufferPtr &spIndexBuffer = CIndexBufferPtr() );

	/// <summary>
	/// �e��Z�o�����ׂĂ��B
	/// </summary>
	void					ComputeAll();
	void					Initialize( vrtxtype vertexNum , vertex::ePrimitiveType ePrimType = vertex::ePrimitiveType_Trianglelist );
	void					Dispose();

	/// <summary>
	/// CPU���̃�������ɒ��_�o�b�t�@���폜����
	/// </summary>
	void                   DeleteCpuMemory();

//---Leaf

	/// <summary>
	/// ���W�X�g���[���̎擾( eVertexBufferStreamType_Position )
	/// </summary>
	void                   AttachPositionStream( const CVertexBufferStreamPtr &position );
	CVertexBufferStreamPtr GetPositionStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );

	/// <summary>
	/// �@���X�g���[���ieVertexBufferStreamType_Normal:���C�g�v�Z�Ɏg�p�j
	/// </summary>
	void                   AttachNormalStream( const CVertexBufferStreamPtr &normal );
	CVertexBufferStreamPtr GetNormalStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateNormalStream( vrtxtype num );

	/// <summary>
	/// UV�X�g���[���ieVertexBufferStreamType_Uv:�e�N�X�`���}�b�s���O�Ɏg�p�j
	/// </summary>
	void                   AttachUvStream( const CVertexBufferStreamPtr &uv );
	CVertexBufferStreamPtr GetUvStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateUvStream( vrtxtype num );

	/// <summary>
	/// �J���[�X�g���[���ieVertexBufferStreamType_Color:���_�J���[���Ɏg�p�j
	/// </summary>
	void                   AttachColorStream( const CVertexBufferStreamPtr &color );
	CVertexBufferStreamPtr GetColorStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateColorStream( vrtxtype num , CColor initColor = CColor_White );

	/// <summary>
	/// �X�L���C���f�b�N�X
	/// </summary>
	void                   AttachIndexStream( const CVertexBufferStreamPtr &uv );
	CVertexBufferStreamPtr GetIndexStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateIndexStream( vrtxtype num );

	/// <summary>
	/// �E�F�C�g
	/// </summary>
	void                   AttachWeightsStream( const CVertexBufferStreamPtr &uv );
	CVertexBufferStreamPtr GetWeightsStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateWeightsStream( vrtxtype num );

	/// <summary>
	/// �w���ID�Œ��_�X�g���[�����֘A�t������
	/// </summary>
	void AttachStream( eVertexBufferStreamType eType , const CVertexBufferStreamPtr& stream );

	/// <summary>
	/// �w���ID�Œ��_�X�g���[�����֘A�t����������
	/// </summary>
	void DetachStream( eVertexBufferStreamType eType , const hash::CHashStringPtr &name = hash::CHashStringPtr() );

	/// <summary>
	/// �w���ID�Œ��_�X�g���[�����擾����
	/// </summary>
	CVertexBufferStreamPtr GetStream( eVertexBufferStreamType eType , const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );

	/// <summary>
	/// �v���~�e�B�u���̖@���X�g���[���̎擾
	/// </summary>
	CVertexBufferStreamPtr GetPrimitiveNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse );
	CVertexBufferStreamPtr DetachPrimitiveNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse ); //���_�o�b�t�@����͏����X�g���[�����擾�B�擾�����X�g���[���̓}�e���A���Ɗ֘A�t���Ďg�p����B

	/// <summary>
	/// ���_���̖@���X�g���[���̎擾
	/// </summary>
	CVertexBufferStreamPtr GetVertexNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse );
	CVertexBufferStreamPtr DetachVertexNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse ); //���_�o�b�t�@����͏����X�g���[�����擾�B�擾�����X�g���[���̓}�e���A���Ɗ֘A�t���Ďg�p����B

//---Leaf

	/// <summary>
	/// �v���~�e�B�u��ʂ̐ݒ�^�擾
	/// </summary>
	void						SetPrimitiveType			( vertex::ePrimitiveType ePrimType ) { m_ePrimType = ePrimType; }
	vertex::ePrimitiveType      GetPrimitiveType            (){ return m_ePrimType; }

	/// <summary>
	/// ���W�ɂS�~�R�s���������
	/// </summary>
	void                        ComputePosMatrix      ( const math::CMatrix4x3 &matrix );

	/// <summary>
	/// ���_�Q�̒��S��( 0 , 0 , 0 )�ɂ���悤�ɒ��_���W�����ړ�������B
	/// </summary>
	void                        TransPosCenter        ();

	/// <summary>
	/// GPU���̃�������ɒ��_�o�b�t�@���쐬����
	/// </summary>
	void                        CreateGpuMemory();

	/// <summary>
	/// GPU���̃�������ɒ��_�o�b�t�@���폜����
	/// </summary>
	void                        DeleteGpuMemory();

	/// <summary>
	/// GPU���̃�������̒��_�o�b�t�@�ɏ�������
	/// </summary>
	void                        WirteGpuMemory  ();

	/// <summary>
	/// ���_��`�悷��
	/// </summary>
	void                     Draw                        ( vrtxtype startIndex = 0 , vrtxtype drawIndexNum = 0 , const CIndexBufferPtr &spIndexBuffer = CIndexBufferPtr() );

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	const CVertexBufferMemberPtr& GetMember(){ return m_pMember; }

	/// <summary>
	/// �N���A�i�g�p���_����Index�����O�ɂ���B�������͊m�ۂ����܂܂ł��B��������j���������ꍇ��Dispose���Ăԁj
	/// </summary>
	void Clear()
	{ 
		m_UseVertexNum = 0;
		DetachBranch();
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
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CVertexBufferPtr  Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CVertexBufferPtr spVertexBuffer( NEW__( CVertexBuffer , CVertexBuffer() ) );
		if ( name.IsEmpty() == skyFalse ) spVertexBuffer->Name.SetName( name );
		return spVertexBuffer;
	}

protected:

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// GPU�������m�ۃt���O
	/// </summary>
	skyBool                      m_IsCreateGpuMemory;

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CVertexBufferMemberPtr       m_pMember;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

//---Member

	/// <summary>
	/// �v���~�e�B�u���
	/// </summary>
	vertex::ePrimitiveType       m_ePrimType;

	/// <summary>
	/// ���_��
	/// </summary>
	vrtxtype			         m_VertexNum;

	/// <summary>
	/// �g�p���_��
	/// </summary>
	vrtxtype				     m_UseVertexNum;

//---Member

	/// <summary>
	/// ���_�g�p���̍X�V
	/// </summary>
	void UpdateUseVertexNum( vrtxtype vertexNo )
	{
		m_UseVertexNum = vertexNo;

		FOREACH( CSceneGraphNodePtrList , it , LeafList() )
		{
			if( SkyIsKindOf( CVertexBufferStream , ( *it ).GetPtrNoConst() ) )
			{
				CVertexBufferStream *pStream = SkyDynamicCast( CVertexBufferStream , ( *it ).GetPtrNoConst() );
				switch( pStream->ID )
				{
				case eVertexBufferStreamType_Position :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_Pos ) * vertexNo );
					break;
				case eVertexBufferStreamType_Normal :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_Normal ) * vertexNo );
					break;
				case eVertexBufferStreamType_Uv :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_UV ) * vertexNo );
					break;
				case eVertexBufferStreamType_Color :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_Color ) * vertexNo );
					break;
				}
			}
		}
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CVertexBuffer();
};

} } }

#define CVertexBufferCreate_()				sky::lib::graphic::CVertexBuffer::Create()
#define CVertexBufferCreateName_( name )	sky::lib::graphic::CVertexBuffer::Create( name )

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �c�R�c�̒��_�o�b�t�@�̃����o
/// </summary>
struct CVertexBufferMember
{
	/// <summary>
	/// ���_���C�A�E�g���
	/// </summary>
	D3DInputVertexLayoutPtr m_spVertexDec;
};

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// ���_�o�b�t�@�̃����o
/// </summary>
struct CVertexBufferMember
{

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

	/// <summary>
	/// Cg�̃V�F�[�_�[ID
	/// </summary>
    CGprogram m_CgProgram;

#endif

};

} } }

#endif
