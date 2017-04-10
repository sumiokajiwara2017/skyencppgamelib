#pragma once

namespace sky { namespace lib { namespace graphic {

enum eVertexBufferStreamType
{
	//CVertexBuffer���f�t�H���g�ň����Ă���v�f
	eVertexBufferStreamType_Position = 0 ,
	eVertexBufferStreamType_Normal ,      
	eVertexBufferStreamType_Uv ,          
	eVertexBufferStreamType_Color ,       
	eVertexBufferStreamType_Index ,
	eVertexBufferStreamType_Weights ,

	eVertexBufferStreamType_Max ,
};

inline u32 GetVertexBufferStreamTypeSize( eVertexBufferStreamType eType )
{
	u32 result = 0;
	switch( eType )
	{
	case eVertexBufferStreamType_Position:     
		result = sizeof( vertex::CVertexFmt_Pos );
		break;
	case eVertexBufferStreamType_Normal:       
		result = sizeof( vertex::CVertexFmt_Normal );
		break;
	case eVertexBufferStreamType_Uv:           
		result = sizeof( vertex::CVertexFmt_UV );
		break;
	case eVertexBufferStreamType_Color:        
		result = sizeof( vertex::CVertexFmt_Color );
		break;
	case eVertexBufferStreamType_Index:
		result = sizeof( vertex::CVertexFmt_Index );
		break;
	case eVertexBufferStreamType_Weights:
		result = sizeof( vertex::CVertexFmt_Weights );
		break;
	}
	return result;
}

//===================================�yCommon�z====================================================>

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)

struct CVertexBufferStreamMember;
typedef SmartPointer2< CVertexBufferStreamMember > CVertexBufferStreamMemberPtr;		//�X�}�[�g�|�C���^�Œ�`

//�e���v���[�gTypedef�S

class CVertexBufferStream;
typedef SmartPointer< CVertexBufferStream >				CVertexBufferStreamPtr;			//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CVertexBufferStreamPtr >			CVertexBufferStreamPtrList;		//���X�g��`
typedef CHash< CVertexBufferStreamPtr >					CVertexBufferStreamPtrHash;		//Hash��`
typedef CHashMT< CVertexBufferStreamPtr >				CVertexBufferStreamPtrHashMT;	//Hash��`

class CVertexBufferStream : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual                    ~CVertexBufferStream();

	/// <summary>
	/// ������
	/// </summary>
	inline void						Initialize         ( u32 size );
	inline void						InitializePosition ( u32 num );
	inline void						InitializeNormal   ( u32 num );
	inline void						InitializeUv       ( u32 num );
	inline void						InitializeColor    ( u32 num );
	inline void						InitializeIndex    ( u32 num );
	inline void						InitializeWeights  ( u32 num );

	/// <summary>
	/// �e�l�Ɋ��Z�������̌���Ԃ�
	/// </summary>
	inline u32                      GetNumPosition();
	inline u32                      GetNumNormal();
	inline u32                      GetNumUv();
	inline u32                      GetNumColor();
	inline u32                      GetNumIndex();
	inline u32                      GetNumWeights();

	/// <summary>
	/// ������
	/// </summary>
	inline void						Dispose					    ();

	/// <summary>
	/// CPU���̃�������ɒ��_�o�b�t�@���폜����
	/// </summary>
	inline void                     DeleteCpuMemory();

	/// <summary>
	/// CPU���̃�������ɏ�������
	/// </summary>
	inline void                        WriteCpuMemory( vrtxtype index , f32 *p , u32 num  );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2  );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2 , f32 p3  );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2 , f32 p3 , f32 p4  );
	inline void                        WritePosition( vrtxtype index , const vertex::CVertexFmt_Pos &pos );
	inline void                        WriteNormal  ( vrtxtype index , const vertex::CVertexFmt_Normal &normal );
	inline void                        WriteUv      ( vrtxtype index , const vertex::CVertexFmt_UV &uv );
	inline void                        WriteColor   ( vrtxtype index , const vertex::CVertexFmt_Color &color );
	inline void                        WriteIndex( vrtxtype index , const vertex::CVertexFmt_Index &indexs );
	inline void                        WriteWeights( vrtxtype index , const vertex::CVertexFmt_Weights &weights );

	/// <summary>
	/// CPU���̃��������w��̒l�œh��Ԃ�
	/// </summary>
	inline void                        WriteMemoryValueAll( u8 *pData , u32 size );
	inline void                        WritePositionAll( const vertex::CVertexFmt_Pos &pos );
	inline void                        WriteNormalAll  ( const vertex::CVertexFmt_Normal &normal );
	inline void                        WriteUvAll      ( const vertex::CVertexFmt_UV &uv );
	inline void                        WriteColorAll   ( const vertex::CVertexFmt_Color &color );
	inline void                        WriteIndexAll( const vertex::CVertexFmt_Index &indexs );
	inline void                        WriteWeightsAll( const vertex::CVertexFmt_Weights &weights );

	/// <summary>
	/// CPU���̃��������w��̒l�ŉ��Z����
	/// </summary>
	inline void                        AddPositionAll( vrtxtype index , const vertex::CVertexFmt_Pos &pos );
	inline void                        AddNormalAll  ( vrtxtype index , const vertex::CVertexFmt_Normal &normal );
	inline void                        AddUvAll      ( vrtxtype index , const vertex::CVertexFmt_UV &uv );
	inline void                        AddColorAll   ( vrtxtype index , const vertex::CVertexFmt_Color &color );
	inline void                        AddIndexAll   ( vrtxtype index , const vertex::CVertexFmt_Index &indexs );
	inline void                        AddWeightsAll ( vrtxtype index , const vertex::CVertexFmt_Weights &weights );

	/// <summary>
	/// �l�ɍs��������ĕϊ�����
	/// </summary>
	inline void                        ComputePosMatrixPositionAll( const math::CMatrix4x3 &matrix );
	inline void                        ComputePosMatrixNormalAll( const math::CMatrix4x3 &matrix );

	/// <summary>
	/// CPU���̃������̎Q�Ƃ̎擾
	/// </summary>
	inline void*                        GetCpuMemory( vrtxtype index , u32 size );
	inline vertex::CVertexFmt_Pos*      GetPosition( vrtxtype index );
	inline vertex::CVertexFmt_Normal*   GetNormal( vrtxtype index );
	inline vertex::CVertexFmt_UV*       GetUv( vrtxtype index );
	inline vertex::CVertexFmt_Color*    GetColor( vrtxtype index );
	inline vertex::CVertexFmt_Index*    GetIndex( vrtxtype index );
	inline vertex::CVertexFmt_Weights*  GetWeights( vrtxtype index );

	/// <summary>
	/// �o�b�t�@�����W���Ɖ��肵��Y���W��Z���W�����ւ���
	/// </summary>
	inline void                        SwapPositionYAndZ();

	/// <summary>
	/// GPU���̃�������ɒ��_�o�b�t�@���쐬����
	/// </summary>
	inline void                        CreateGpuMemory();

	/// <summary>
	/// GPU���̃�������ɒ��_�o�b�t�@���폜����
	/// </summary>
	inline void                        DeleteGpuMemory();

	/// <summary>
	/// GPU���̃�������̒��_�o�b�t�@�ɏ�������
	/// </summary>
	inline void                        WirteGpuMemory  ();

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	inline const CVertexBufferStreamMemberPtr& GetMember();

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

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	virtual void Deactivate();

	/// <summary>
	/// ���_�g�p���̍X�V
	/// </summary>
	inline void UpdateUseSize( u32 size )
	{
		m_UseSize = size;
	}

	/// <summary>
	/// ���v�����g�i�f�o�b�O�v�����g���ڍׂȏ����o��,�C���f���g�͂��Ȃ��B�j
	/// </summary>
	virtual void InfoPrint();
	void InfoPrintPosition();
	void InfoPrintNormal();
	void InfoPrintUv();
	void InfoPrintColor();
	void InfoPrintIndex();
	void InfoPrintWeights();

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CVertexBufferStreamPtr  Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CVertexBufferStreamPtr spBuffer( NEW__( CVertexBufferStream , CVertexBufferStream() ) );
		if ( name.IsEmpty() == skyFalse ) spBuffer->Name.SetName( name );
		return spBuffer;
	}

protected:

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CVertexBufferStreamMemberPtr m_pMember;

	/// <summary>
	/// �b�o�t��������o�b�t�@
	/// </summary>
	u8*                        m_pCpuMemoryBuffer;

	/// <summary>
	/// �f�o�t�������쐬�ς݃t���O
	/// </summary>
	skyBool                    m_IsCreateGpuMemory;

	/// <summary>
	/// �T�C�Y
	/// </summary>
	u32                        m_Size;

	/// <summary>
	/// �g�p�T�C�Y
	/// </summary>
	u32                        m_UseSize;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CVertexBufferStream();
};

} } }

#define CVertexBufferStreamCreate_()			sky::lib::graphic::CVertexBufferStream::Create()
#define CVertexBufferStreamCreateName_( name )	sky::lib::graphic::CVertexBufferStream::Create( name )

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//�^�̍Ē�`
typedef SmartPointerCom< ID3D11Buffer >      D3VertexBufferPtr;
typedef SmartPointerCom< ID3D11InputLayout > D3DInputVertexLayoutPtr;

/// <summary>
/// �c�R�c�̒��_�o�b�t�@�̃����o
/// </summary>
struct CVertexBufferStreamMember
{
	/// <summary>
	/// �f�o�t����������W�o�b�t�@
	/// </summary>
	D3VertexBufferPtr   m_spGpuMemoryBuffer;
};

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// ���_�o�b�t�@�̃����o
/// </summary>
struct CVertexBufferStreamMember
{
	/// <summary>
	/// �o�b�t�@�n���h��
	/// </summary>
	GLuint  m_hBuffer;

	/// <summary>
	/// ���P�[�V�����i�V�F�[�_�[�̕ϐ��Ƃ̊֘A�t���j
	/// </summary>
	GLint    m_Location;
};

} } }

#endif

#include "VertexBufferStream.inl"