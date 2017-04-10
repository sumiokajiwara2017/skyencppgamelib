#pragma once

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct  CIndexBufferMember;
typedef SmartPointer2< CIndexBufferMember >	CIndexBufferMemberPtr;		//�X�}�[�g�|�C���^�Œ�`


//�e���v���[�gTypedef�S
class CIndexBuffer;
typedef SmartPointer< CIndexBuffer >		CIndexBufferPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CIndexBufferPtr >		CIndexBufferPtrList;	//���X�g��`
typedef CHash< CIndexBufferPtr >			CIndexBufferPtrHash;	//Hash��`
typedef CHash< CIndexBufferPtr >			CIndexBufferPtrHashMT;	//Hash��`

class CIndexBuffer : public CSceneGraphNode
{
    SkyTypeDefRTTI;

friend class GLCgVertexBuffer;
friend class CFont;
friend class CFontSimple;
friend class CDecalMember;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CIndexBuffer();

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̃T�C�Y���擾����
	/// </summary>
	inline u16				GetIndexNum           () const { return m_IndexNum; };

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̎g�p��
	/// </summary>
	inline u16				GetUseIndexNum        () const { return m_UseIndexNum; };

	/// <summary>
	/// ���e���R�s�[����
	/// </summary>
	void			Copy				( CIndexBufferPtr &spIndexBuffer );

	/// <summary>
	/// �w��̓Y�����̃C���f�b�N�X���擾����
	/// </summary>
	vrtxtype		Index				( vrtxtype indexNo ) const;

	/// <summary>
	/// �w��̓Y�����ɃC���f�b�N�X��ݒ肷��
	/// </summary>
	void			SetIndex			( vrtxtype indexNo , u16 value );

	/// <summary>
	/// ���e��������
	/// </summary>
	void			Initialize			( vrtxtype indexNum );

	/// <summary>
	/// ���e��������
	/// </summary>
	void			Dispose		         ();

	/// <summary>
	/// CPU���̃��������N���A����
	/// </summary>
	virtual void    DeleteCpuMemory();

	/// <summary>
	/// GPU���̃�������ɃC���f�b�N�X�o�b�t�@���쐬����
	/// </summary>
	virtual void CreateGpuMemory();

	/// <summary>
	/// GPU���̃�������̃C���f�b�N�X�o�b�t�@���폜����
	/// </summary>
	virtual void DeleteGpuMemory();

	/// <summary>
	/// GPU���̃�������̃C���f�b�N�X�o�b�t�@�ɏ�������
	/// </summary>
	virtual void WirteGpuMemory();

	/// <summary>
	/// GPU���̃������̃X�g���[����ݒ肷��
	/// </summary>
	virtual void SetGpuMemoryStream();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
    inline const CIndexBufferMemberPtr &GetMember();

	/// <summary>
	/// �N���A�i�g�p�C���f�b�N�X�����O�ɂ���B�������͔j�����܂���B��������j���������ꍇ��Dispose()���ĂԂ��ƁB
	/// </summary>
	inline void Clear()
	{ 
        m_UseIndexNum = 0;
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
	serialize::CSerializeInterfacePtr         Clone();
	serialize::CSerializeInterfacePtr 		  CloneSharing();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	virtual void Deactivate();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CIndexBufferPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CIndexBufferPtr spIndexBuffer( NEW__( CIndexBuffer , CIndexBuffer() ) );
		if ( name.IsEmpty() == skyFalse ) spIndexBuffer->Name.SetName( name );
		return spIndexBuffer;
	}

	/// <summary>
	/// ���v�����g
	/// </summary>
	virtual void InfoPrint();

protected:

//---Member

	/// <summary>
	/// �C���f�b�N�X���i�P�v���~�e�B�u�̒��_���~�v���~�e�B�u���j
	/// </summary>
	vrtxtype	m_IndexNum;

	/// <summary>
	/// �g�p�C���f�b�N�X��
	/// </summary>
	vrtxtype     m_UseIndexNum;

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@
	/// </summary>
	vrtxtype	*m_pCpuMemoryIndexBuffer;

//---Member

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// GPU�������m�ۃt���O
	/// </summary>
	skyBool m_IsCreateGpuMemory;

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CIndexBufferMemberPtr  m_pMember;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// �C���f�b�N�X�g�p���̍X�V
	/// </summary>
	inline void UpdateUseIndexNum( vrtxtype indexNo )
	{
		m_UseIndexNum = indexNo;
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CIndexBuffer();
};

} } }

#define CIndexBufferCreate_()			sky::lib::graphic::CIndexBuffer::Create()
#define CIndexBufferCreateName_( name )	sky::lib::graphic::CIndexBuffer::Create( name )

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< ID3D11Buffer > D3DIndexBufferPtr;

/// <summary>
/// �c�R�c�̃C���f�b�N�X�o�b�t�@�̃����o
/// </summary>
struct CIndexBufferMember
{
	/// <summary>
	/// �O���t�B�b�N�n�[�h��̃�������̃C���f�b�N�X�o�b�t�@
	/// </summary>
	D3DIndexBufferPtr	m_pGpuMemoryIndexBuffer;
};

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �C���f�b�N�X�o�b�t�@�̃����o
/// </summary>
struct CIndexBufferMember
{
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�n���h��
	/// </summary>
	GLuint                 m_hBuffer;

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

	/// <summary>
	/// Cg�̃V�F�[�_�[ID
	/// </summary>
    CGprogram m_CgProgram;

#endif

};

} } }

#endif
